// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Tables
**/



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Searches for the Import Address Table (IAT) address of a specified function in a PE image.
	@details        This function analyze a PE image to find and return the address of the Import Address Table (IAT) entry for a specified function. The function starts by searching the Import Descriptor Table of the PE image for the specified import DLL name. If the DLL is found, it then iterates through the function imports to locate the target function by name.


	@param[in]      ImageBase                   The base address of the PE image.
	@param[in]      NtHeaders                   A pointer to the PE image's NT headers.
	@param[in]      ImportDllName               The name of the import DLL to search for.
	@param[in]      FunctionName                The name of the function to find in the import DLL.
	@param[out]     FunctionIATAddress          A pointer to the address of the function's IAT entry if found.


	@retval         EFI_SUCCESS                 The function's IAT address was successfully found.
	@retval         EFI_NOT_FOUND               The specified import DLL or function was not found in the PE image.
**/
EFI_STATUS
BootWindowsHookings_UtilsTables_FindImportAddressTable(
	IN              CONST VOID*                 ImageBase,
	IN              PEFI_IMAGE_NT_HEADERS       NtHeaders,
	IN              CONST CHAR8*                ImportDllName,
	IN              CONST CHAR8*                FunctionName,
	OUT             VOID                        **FunctionIATAddress
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
