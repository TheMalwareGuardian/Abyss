// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



#include <windows.h>
#include <stdio.h>
#include <psapi.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include "Utils02Kernel.h"
#include "../../Globals/Globals01Operations.h"
#include "../../Globals/Globals00Communication.h"
#include "../../Structures/Structures00Execution.h"



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



// Resolves the kernel-mode address of an export (e.g. DbgPrint, by locating ntoskrnl.exe, loading it in user mode, getting the RVA via GetProcAddress, and adding it to the real kernel base)
FARPROC UtilsKernel_GetKernelExport(const char* name)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Variables

	// Array to receive the base addresses of loaded kernel modules
	HMODULE loadedKernelModules[1024];

	// Receives the total number of bytes written, used to calculate how many modules were returned
	DWORD bytesReturned;


	// ---------------------------------------------------------------------------------------------------------------------
	// Enumerate all loaded kernel-mode modules (typically drivers + ntoskrnl.exe)

	// Retrieves the load address for each device driver in the system.
	// https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-enumdevicedrivers
	if (!EnumDeviceDrivers((LPVOID*)loadedKernelModules, sizeof(loadedKernelModules), &bytesReturned))
	{
		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate through the list of loaded modules
	for (DWORD i = 0; i < bytesReturned / sizeof(LPVOID); i++)
	{


		// -----------------------------------------------------------------------------------------------------------------
		// ASCII buffer that will receive the driver's base name
		char modName[MAX_PATH] = {0};


		// -----------------------------------------------------------------------------------------------------------------
		// Retrieve the base name of the module (e.g., ntoskrnl.exe)

		// Retrieves the base name of the specified device driver.
		// https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getdevicedriverbasenamea
		GetDeviceDriverBaseNameA(loadedKernelModules[i], modName, sizeof(modName));


		// -----------------------------------------------------------------------------------------------------------------
		// Check if it's ntoskrnl.exe, where the exports we want live
		if (_stricmp(modName, "ntoskrnl.exe") == 0)
		{


			// -------------------------------------------------------------------------------------------------------------
			// Load it in user mode (but do not resolve dependencies)

			// Loads the specified module into the address space of the calling process. The specified module may cause other modules to be loaded.
			// https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibraryexa
			HMODULE userModeKernelImage = LoadLibraryExA(modName, NULL, DONT_RESOLVE_DLL_REFERENCES);
			if (!userModeKernelImage)
			{
				return NULL;
			}


			// -------------------------------------------------------------------------------------------------------------
			// Get the RVA of the export from the user-mode image

			// Retrieves the address of an exported function (also known as a procedure) or variable from the specified dynamic-link library (DLL).
			// https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress
			FARPROC userExportAddress = GetProcAddress(userModeKernelImage, name);
			if (!userExportAddress)
			{
				return NULL;
			}


			// -------------------------------------------------------------------------------------------------------------
			// Convert the user-mode RVA to a kernel-mode VA
			return (FARPROC)((ULONG_PTR)loadedKernelModules[i] + ((ULONG_PTR)userExportAddress - (ULONG_PTR)userModeKernelImage));
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Export not found
	return NULL;
}



// Dump PE section table in a human-readable format
void UtilsKernel_PrintSectionInfo(PIMAGE_SECTION_HEADER sec, int count)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Print index, name (padded/trimmed to 8 chars), virtual address and raw size.
	wprintf(L"[+] PE Section Table\n");
	for (int i = 0; i < count; i++)
	{
		wprintf(L"    [+] Section %d: %.*hs, VA: 0x%x, RawSize: %d\n", i, 8, sec[i].Name, sec[i].VirtualAddress, sec[i].SizeOfRawData);
	}
}



// Apply relocations manually using the relocation table, adapting image base
void UtilsKernel_ApplyRelocations(BYTE* image, ULONGLONG newBase, ULONGLONG originalBase)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	wprintf(L"[+] Applying relocations...\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Locate relocation directory via PE headers
	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)image;
	IMAGE_NT_HEADERS64* nt = (IMAGE_NT_HEADERS64*)(image + dos->e_lfanew);
	DWORD relocVA = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;


	// ---------------------------------------------------------------------------------------------------------------------
	// If there's no relocation table, nothing to do
	if (!relocVA)
	{
		wprintf(L"    [!] No relocation table found. Skipping base relocations.\n");
		return;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Calculate the delta between original and new base addresses
	ULONGLONG delta = newBase - originalBase;


	// ---------------------------------------------------------------------------------------------------------------------
	// Process each relocation block
	PIMAGE_BASE_RELOCATION reloc = (PIMAGE_BASE_RELOCATION)(image + relocVA);
	while (reloc->VirtualAddress && reloc->SizeOfBlock)
	{
		DWORD count = (reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
		WORD* entries = (WORD*)((BYTE*)reloc + sizeof(IMAGE_BASE_RELOCATION));


		// -----------------------------------------------------------------------------------------------------------------
		// Iterate every relocation entry
		for (DWORD i = 0; i < count; i++)
		{


			// -------------------------------------------------------------------------------------------------------------
			// Only process 64-bit absolute relocations
			if ((entries[i] >> 12) == IMAGE_REL_BASED_DIR64)
			{


				// ---------------------------------------------------------------------------------------------------------
				// Compute address to patch and apply delta
				ULONGLONG* patch = (ULONGLONG*)(image + reloc->VirtualAddress + (entries[i] & 0xFFF));
				*patch += delta;
			}
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Advance to next block
		reloc = (PIMAGE_BASE_RELOCATION)((BYTE*)reloc + reloc->SizeOfBlock);
	}


	// -----------------------------------------------------------------------------------------------------------------
	// Bye
	wprintf(L"    [+] Relocations applied successfully.\n");
}



// Patch the /GS security cookie in a driver image so that the runtime value matches the one baked into the binary
BOOL UtilsKernel_FixSecurityCookie(BYTE* image, ULONGLONG kernelBase, ULONGLONG originalImageBase)
{
	// -----------------------------------------------------------------------------------------------------------------
	// Hello
	wprintf(L"[+] Attempting to fix /GS security cookie if present...\n");


	// -----------------------------------------------------------------------------------------------------------------
	// Variables
	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)image;
	IMAGE_NT_HEADERS64* nt = (IMAGE_NT_HEADERS64*)(image + dos->e_lfanew);


	// -----------------------------------------------------------------------------------------------------------------
	// Get load config directory (contains address of /GS cookie)
	DWORD configRVA = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress;
	if (!configRVA)
	{
		wprintf(L"    [!] No Load Config Directory found. Skipping cookie fix.\n");
		return TRUE;
	}
	PIMAGE_LOAD_CONFIG_DIRECTORY64 config = (PIMAGE_LOAD_CONFIG_DIRECTORY64)(image + configRVA);
	if (!config->SecurityCookie)
	{
		wprintf(L"    [!] No Security Cookie present. Skipping fix.\n");
		return TRUE;
	}


	// -----------------------------------------------------------------------------------------------------------------
	// Calculate the RVA and offset of the cookie
	ULONGLONG cookieRVA = config->SecurityCookie;


	// -----------------------------------------------------------------------------------------------------------------
	// Sanity-check that the RVA is inside the image buffer
	DWORD imageSize = nt->OptionalHeader.SizeOfImage;
	if (cookieRVA > imageSize - sizeof(ULONGLONG)) {
		wprintf(L"    [!] Cookie RVA (0x%llx) is outside image (0x%x). Skipping.\n", cookieRVA, imageSize);
		return TRUE;
	}


	// -----------------------------------------------------------------------------------------------------------------
	// Compute the pointer to the cookie inside the image using its RVA
	ULONGLONG* cookiePtr = (ULONGLONG*)(image + cookieRVA);
	wprintf(L"    [+] Located Security Cookie at RVA: 0x%llx\n", cookieRVA);


	// -----------------------------------------------------------------------------------------------------------------
	// Generate a random cookie (based on process and thread IDs)
	ULONGLONG new_cookie = 0x2B992DDFA232 ^ GetCurrentProcessId() ^ GetCurrentThreadId();
	// Avoid default
	if (new_cookie == 0x2B992DDFA232)
	{
		new_cookie++;
	}


	// -----------------------------------------------------------------------------------------------------------------
	// Patch the image
	*cookiePtr = new_cookie;
	wprintf(L"    [+] Security cookie patched with: 0x%llx\n", new_cookie);


	// -----------------------------------------------------------------------------------------------------------------
	// Return
	return TRUE;
}



// Walk every imported module & function, patch each IAT entry with the real kernel-space address obtained via UtilsKernel_GetKernelExport()
BOOL UtilsKernel_ResolveImports(BYTE* image)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Resolve imports
	wprintf(L"[+] Resolving imports...\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Get DOS and NT headers to locate the import directory
	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)image;
	IMAGE_NT_HEADERS64* nt = (IMAGE_NT_HEADERS64*)(image + dos->e_lfanew);
	DWORD importVA = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;


	// ---------------------------------------------------------------------------------------------------------------------
	// No imports to resolve
	if (!importVA)
	{
		return TRUE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Walk the import descriptor table
	PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR)(image + importVA);
	while (importDesc->Name)
	{
		char* moduleName = (char*)(image + importDesc->Name);
		wprintf(L"    [+] Module: %hs\n", moduleName);


		// -----------------------------------------------------------------------------------------------------------------
		// ILT (Import Lookup Table) points to names; IAT (Import Address Table) will hold resolved addresses
		PIMAGE_THUNK_DATA64 thunkILT = (PIMAGE_THUNK_DATA64)(image + importDesc->OriginalFirstThunk);
		PIMAGE_THUNK_DATA64 thunkIAT = (PIMAGE_THUNK_DATA64)(image + importDesc->FirstThunk);


		// -----------------------------------------------------------------------------------------------------------------
		// Iterate over each imported function
		while (thunkILT->u1.AddressOfData)
		{

			// -------------------------------------------------------------------------------------------------------------
			// Get function name from IMAGE_IMPORT_BY_NAME
			PIMAGE_IMPORT_BY_NAME import = (PIMAGE_IMPORT_BY_NAME)(image + thunkILT->u1.AddressOfData);
			wprintf(L"        [+] Resolving: %hs\n", import->Name);


			// -------------------------------------------------------------------------------------------------------------
			// Resolve kernel address of the function
			FARPROC addr = UtilsKernel_GetKernelExport((const char*)import->Name);
			if (!addr)
			{
				wprintf(L"        [!] Failed to resolve %hs\n", import->Name);
				return FALSE;
			}


			// -------------------------------------------------------------------------------------------------------------
			// Patch the function pointer in the IAT
			thunkIAT->u1.Function = (ULONGLONG)addr;


			// -------------------------------------------------------------------------------------------------------------
			// Move to next thunk
			++thunkILT;
			++thunkIAT;
		}

		++importDesc;
	}


	// -----------------------------------------------------------------------------------------------------------------
	// Bye
	wprintf(L"    [+] Imports resolved successfully.\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return TRUE;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
