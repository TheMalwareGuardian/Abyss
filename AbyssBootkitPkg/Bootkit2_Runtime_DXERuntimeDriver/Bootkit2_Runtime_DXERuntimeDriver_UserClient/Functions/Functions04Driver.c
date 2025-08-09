// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



#include <windows.h>
#include <stdio.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include "Utils/Utils02Kernel.h"
#include "Utils/Utils03Communication.h"
#include "../Globals/Globals01Operations.h"
#include "../Structures/Structures01Driver.h"
#include "../Structures/Structures00Execution.h"



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



// Load a file from disk into memory
BOOL FunctionsDriver_LoadFile(const wchar_t* path, BYTE** outBuf, DWORD* outSize)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Open file for reading

	// Creates or opens a file or I/O device. The most commonly used I/O devices are as follows: file, file stream, directory, physical disk, volume, console buffer, tape drive, communications resource, mailslot, and pipe. The function returns a handle that can be used to access the file or device for various types of I/O depending on the file or device and the flags and attributes specified. To perform this operation as a transacted operation, which results in a handle that can be used for transacted I/O, use the CreateFileTransacted function.
	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
	HANDLE hFile = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// Failed
	if (hFile == INVALID_HANDLE_VALUE)
	{
		wprintf(L"[-] Cannot open %s (err=%lu)\n", path, GetLastError());
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get file size

	// Retrieves the size of the specified file, in bytes.
	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfilesize
	DWORD size = GetFileSize(hFile, NULL);


	// ---------------------------------------------------------------------------------------------------------------------
	// Allocate memory to store the file's raw contents
	BYTE* buf = (BYTE*)malloc(size);
	if (!buf)
	{
		wprintf(L"[-] Out of memory\n");
		CloseHandle(hFile);
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Read file contents into memory

	// Reads data from the specified file or input/output (I/O) device. Reads occur at the position specified by the file pointer if supported by the device.
	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
	DWORD read = 0;
	ReadFile(hFile, buf, size, &read, NULL);


	// ---------------------------------------------------------------------------------------------------------------------
	// Close the file handle after reading

	// Closes an open object handle.
	// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
	CloseHandle(hFile);


	// ---------------------------------------------------------------------------------------------------------------------
	// Check for short reads
	if (read != size)
	{
		wprintf(L"[-] Failed to read file completely\n");
		free(buf);
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	*outBuf = buf;
	*outSize = size;
	return TRUE;
}



// Rebuild a complete in-memory PE image from raw disk contents
BYTE* FunctionsDriver_RebuildImage(BYTE* rawImage, DWORD imageSize)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Validate input parameters
	if (!rawImage || !imageSize)
	{
		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Parse PE headers
	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)rawImage;
	IMAGE_NT_HEADERS64* nt = (IMAGE_NT_HEADERS64*)(rawImage + dos->e_lfanew);


	// ---------------------------------------------------------------------------------------------------------------------
	// Allocate memory for the fully rebuilt PE image
	BYTE* image = (BYTE*)calloc(1, imageSize);
	if (!image)
	{
		wprintf(L"[-] Out of memory while rebuilding PE image\n");
		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Copy PE headers
	memcpy(image, rawImage, nt->OptionalHeader.SizeOfHeaders);


	// ---------------------------------------------------------------------------------------------------------------------
	// Copy each section to its virtual offset
	PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(nt);
	for (int i = 0; i < nt->FileHeader.NumberOfSections; ++i)
	{
		memcpy(image + sec[i].VirtualAddress, rawImage + sec[i].PointerToRawData, sec[i].SizeOfRawData);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Print section layout for debugging
	UtilsKernel_PrintSectionInfo(sec, nt->FileHeader.NumberOfSections);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return image;
}



// Upload a .sys and execute its DriverEntry (reads a PE-style .SYS from disk, relocates/resolves it entirely in user-land, copies the final image into the DXE runtime buffer, and invokes DriverEntry inside the kernel
int FunctionsDriver_CommandLoadDriver(const wchar_t* driverPath)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Request get buffer bounds
	ULONGLONG bufferStart = 0;
	ULONGLONG bufferEnd = 0;
	if (!UtilsCommunication_RequestGetBufferBounds(&bufferStart, &bufferEnd))
	{
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Load .sys file and copy to memory
	BYTE* imageBuf = NULL;
	DWORD  fileSize = 0;
	if (!FunctionsDriver_LoadFile(driverPath, &imageBuf, &fileSize))
	{
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Parse PE headers
	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)imageBuf;
	IMAGE_NT_HEADERS64* nt = (IMAGE_NT_HEADERS64*)(imageBuf + dos->e_lfanew);
	DWORD imageSize = nt->OptionalHeader.SizeOfImage;
	DWORD entryRVA = nt->OptionalHeader.AddressOfEntryPoint;
	ULONGLONG origBase = nt->OptionalHeader.ImageBase;
	ULONGLONG kernelImageBase = bufferStart;
	wprintf(L"[+] Driver image size: 0x%x bytes\n", imageSize);


	// ---------------------------------------------------------------------------------------------------------------------
	// Ensure the entire driver image will fit inside the allocated DXE Runtime Driver buffer
	if ((kernelImageBase + imageSize) > bufferEnd)
	{
		wprintf(L"[-] Driver image (0x%x) won't fit inside kernel buffer range!\n", imageSize);
		free(imageBuf);
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Re-build full in-memory PE image

	// Rebuild PE image in memory from raw file
	BYTE* image = FunctionsDriver_RebuildImage(imageBuf, imageSize);

	// Raw buffer no longer needed
	free(imageBuf);

	// Rebuild failed
	if (!image)
	{
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Apply relocations for new base
	UtilsKernel_ApplyRelocations(image, kernelImageBase, origBase);


	// ---------------------------------------------------------------------------------------------------------------------
	// Fix /GS security cookie if present
	if (!UtilsKernel_FixSecurityCookie(image, kernelImageBase, origBase))
	{
		free(image);
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Resolve imported functions
	if (!UtilsKernel_ResolveImports(image))
	{
		wprintf(L"[-] Import resolution failed\n");
		free(image);
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Upload the rebuilt PE image into the DXE Runtime Driver buffer at the target address

	// Request write in buffer
	if (!UtilsCommunication_RequestWriteInBuffer(kernelImageBase, image, imageSize))
	{
		free(image);
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Upload RegistryPath string to buffer just after driver image

	// Craft fake RegistryPath UNICODE_STRING
	static const wchar_t regPathW[] = L"\\Registry\\Machine\\System\\Temp";
	DWORD regByteLen = sizeof(regPathW);
	ULONGLONG regPathKernel = kernelImageBase + imageSize;

	// Request write in buffer
	if (!UtilsCommunication_RequestWriteInBuffer(regPathKernel, (BYTE*)regPathW, regByteLen))
	{
		free(image);
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Build the UNICODE_STRING structure that DriverEntry expects
	STRUCTURES_DRIVER_FAKEUNICODESTRING unicodeStructVA = {
		(USHORT)(regByteLen - sizeof(wchar_t)),
		(USHORT)regByteLen,
		regPathKernel
	};


	// ---------------------------------------------------------------------------------------------------------------------
	// Upload the structure 0x100 bytes past the string
	ULONGLONG uniKernel = regPathKernel + 0x100;


	// ---------------------------------------------------------------------------------------------------------------------
	// Request write in buffer
	if (!UtilsCommunication_RequestWriteInBuffer(uniKernel, (BYTE*)&unicodeStructVA, sizeof(unicodeStructVA)))
	{
		free(image);
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Calculate virtual address of DriverEntry (kernelImageBase + RVA from PE header)
	ULONGLONG driverEntryVA = kernelImageBase + entryRVA;
	ULONGLONG ntStatus = 0;

	// Request DXE Runtime Driver to jump into DriverEntry, passing the crafted RegistryPath
	if (!UtilsCommunication_RequestDriverEntry(driverEntryVA, uniKernel, &ntStatus))
	{
		wprintf(L"[-] Failed to invoke DriverEntry via DXE Runtime Driver\n");
		free(image);
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Report result
	wprintf(L"[+] DriverEntry returned: 0x%llx\n", ntStatus);


	// ---------------------------------------------------------------------------------------------------------------------
	// Check NTSTATUS result and show appropriate status message
	if (ntStatus == 0x0)
	{
		wprintf(L"[+] Driver loaded and executed successfully!\n");
	}
	else
	{
		wprintf(L"[-] DriverEntry returned a failure status. The driver may not have initialized correctly.\n");
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Clean up user-mode image buffer after execution
	free(image);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return 0;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
