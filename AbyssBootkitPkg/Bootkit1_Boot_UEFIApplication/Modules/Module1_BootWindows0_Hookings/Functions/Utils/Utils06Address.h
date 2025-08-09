// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Address
**/



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
);



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
