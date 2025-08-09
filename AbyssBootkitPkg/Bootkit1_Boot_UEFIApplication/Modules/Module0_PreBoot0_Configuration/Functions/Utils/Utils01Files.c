// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Files
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Boot Services Table. Only available to DXE and UEFI module types.
#include <Library/UefiBootServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
// Provides copy memory, fill memory, zero memory, and GUID functions. The Base Memory Library provides optimized implementations for common memory-based operations. These functions should be used in place of coding your own loops to do equivalent common functions. This allows optimized library implementations to help increase performance.
#include <Library/BaseMemoryLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/MemoryAllocationLib.h
// Provides services to allocate and free memory buffers of various memory types and alignments. The Memory Allocation Library abstracts various common memory allocation operations. This library allows code to be written in a phase-independent manner because the allocation of memory in PEI, DXE, and SMM (for example) is done via a different mechanism. Using a common library interface makes it much easier to port algorithms from phase to phase.
#include <Library/MemoryAllocationLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DevicePathLib.h
// Provides library functions to construct and parse UEFI Device Paths. This library provides defines, macros, and functions to help create and parse EFI_DEVICE_PATH_PROTOCOL structures.
#include <Library/DevicePathLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/FileHandleLib.h
// Provides interface to EFI_FILE_HANDLE functionality.
#include <Library/FileHandleLib.h>



// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------
// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h
// UEFI 2.0 Loaded image protocol definition. Every EFI driver and application is passed an image handle when it is loaded. This image handle will contain a Loaded Image Protocol.
#include <Protocol/LoadedImage.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/SimpleFileSystem.h
// SimpleFileSystem protocol as defined in the UEFI 2.0 specification. The SimpleFileSystem protocol is the programmatic access to the FAT (12,16,32) file system specified in UEFI 2.0. It can also be used to abstract a file system other than FAT. UEFI 2.0 can boot from any valid EFI image contained in a SimpleFileSystem.
#include <Protocol/SimpleFileSystem.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Creates a log file at the specified path.


	@param[in]      FilePath                    The path where the file will be created.

	@retval         EFI_SUCCESS                 The file was created successfully.
	@retval         EFI_ABORTED                 Failed to create the file.
**/
EFI_STATUS
PreBootConfiguration_UtilsFiles_CreateLogFile(
	IN              CHAR16                      *FilePath
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
	EFI_FILE_HANDLE Root;
	EFI_FILE_HANDLE File;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the LoadedImage protocol for the current image

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h#L79
	Status = gBS->HandleProtocol(gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (CreateLogFile) - gBS->HandleProtocol [gEfiLoadedImageProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return EFI_ABORTED;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the Simple File System Protocol of the device we're on

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/SimpleFileSystem.h#L551
	Status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&SimpleFileSystem);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (CreateLogFile) - gBS->HandleProtocol [gEfiSimpleFileSystemProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return EFI_ABORTED;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Open the volume to get the root directory of the ESP

	// Opens the root directory on a volume.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html?highlight=openvolume#efi-simple-file-system-protocol-openvolume
	Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &Root);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (CreateLogFile) - SimpleFileSystem->OpenVolume failed: %r (%llx).\r\n", Status, Status);
		return EFI_ABORTED;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Create the file (if it already exists, it will be overwritten)

	// Opens a new file relative to the source file’s location.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-open
	Status = Root->Open(Root, &File, FilePath, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		Root->Close(Root);

		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (CreateLogFile) - Root->Open failed: %r (%llx).\r\n", Status, Status);
		return EFI_ABORTED;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye

	// Closes a specified file handle.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
	File->Close(File);
	Root->Close(Root);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Writes a message to the log file.


	@param[in]      FilePath                    The path to the log file.
	@param[in]      Message                     The message to write to the log file.


	@retval         EFI_SUCCESS                 The message was written to the log file successfully.
	@retval         EFI_ABORTED                 Failed to write the message to the log file.
	@retval         EFI_OUT_OF_RESOURCES        Not enough resources to allocate memory.
**/
EFI_STATUS
PreBootConfiguration_UtilsFiles_AppendToLogFile(
	IN              CHAR16                      *FilePath,
	IN              CONST CHAR16                *Message
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
	EFI_FILE_PROTOCOL *Root;
	EFI_FILE_PROTOCOL *File;
	CHAR8 *AsciiMessage;
	UINTN AsciiSize;
	UINTN Size;
	UINT64 Position;
	EFI_FILE_INFO *FileInfo;
	UINTN FileInfoSize;


	// ---------------------------------------------------------------------------------------------------------------------
	// Calculate the maximum size for the ASCII buffer

	// Returns the length of a Null-terminated Unicode string.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L30
	AsciiSize = StrLen(Message) + 1;

	// Allocates pool memory.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
	AsciiMessage = AllocatePool(AsciiSize);

	// Failed
	if (AsciiMessage == NULL)
	{
		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (AppendToLogFile) - AllocatePool [AsciiSize] failed.\r\n");
		return EFI_OUT_OF_RESOURCES;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Attempt to convert the CHAR16 message to CHAR8

	// Convert a Null-terminated Unicode string to a Null-terminated ASCII string.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/SafeString.c#L2650
	Status = UnicodeStrToAsciiStrS(Message, AsciiMessage, AsciiSize);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(AsciiMessage);

		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (AppendToLogFile) - UnicodeStrToAsciiStrS failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}

	// Size of the message in bytes, excluding the null terminator
	Size = AsciiSize - 1;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the LoadedImage protocol for the current image

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h#L79
	Status = gBS->HandleProtocol(gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(AsciiMessage);

		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (AppendToLogFile) - gBS->HandleProtocol [gEfiLoadedImageProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the Simple File System Protocol of the device we're on

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/SimpleFileSystem.h#L551
	Status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&SimpleFileSystem);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(AsciiMessage);

		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (AppendToLogFile) - gBS->HandleProtocol [gEfiSimpleFileSystemProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Open the volume to get the root directory of the ESP

	// Opens the root directory on a volume.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html?highlight=openvolume#efi-simple-file-system-protocol-openvolume
	Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &Root);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(AsciiMessage);

		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (AppendToLogFile) - SimpleFileSystem->OpenVolume failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Attempt to open the file in read/write mode

	// Opens a new file relative to the source file’s location.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-open
	Status = Root->Open(Root, &File, FilePath, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		Root->Close(Root);

		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(AsciiMessage);

		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (AppendToLogFile) - Root->Open failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the current file size to move the file pointer to the end

	// Buffer size for FileInfo
	FileInfoSize = sizeof(EFI_FILE_INFO) + 1024;

	// Allocates pool memory.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
	FileInfo = AllocatePool(FileInfoSize);

	// Failed
	if (FileInfo == NULL)
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		File->Close(File);
		Root->Close(Root);

		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(AsciiMessage);

		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (AppendToLogFile) - AllocatePool [FileInfoSize] failed.\r\n");
		return EFI_OUT_OF_RESOURCES;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the size of the file

	// Returns information about a file.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-getinfo
	Status = File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(FileInfo);

		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		File->Close(File);
		Root->Close(Root);

		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(AsciiMessage);

		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (AppendToLogFile) - File->GetInfo failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}

	// Retrieve the current file size to determine the position to append new data
	Position = FileInfo->FileSize;

	// Returns pool memory to the system.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
	FreePool(FileInfo);


	// ---------------------------------------------------------------------------------------------------------------------
	// Set the file pointer to the end of the file

	// Sets a file’s current position.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-setposition
	Status = File->SetPosition(File, Position);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		File->Close(File);
		Root->Close(Root);

		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(AsciiMessage);

		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (AppendToLogFile) - File->SetPosition failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Write the ASCII message to the log file

	// Writes data to a file.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-write
	Status = File->Write(File, &Size, AsciiMessage);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		File->Close(File);
		Root->Close(Root);

		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(AsciiMessage);

		PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Files.c] (AppendToLogFile) - File->Write failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye

	// Closes a specified file handle.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
	File->Close(File);
	Root->Close(Root);

	// Returns pool memory to the system.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
	FreePool(AsciiMessage);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
