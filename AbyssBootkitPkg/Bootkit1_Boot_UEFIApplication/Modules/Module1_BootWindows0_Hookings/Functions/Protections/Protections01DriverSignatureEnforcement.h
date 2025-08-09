// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Protections Driver Signature Enforcement
**/



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Disables the Driver Signature Enforcement protection for the duration of the boot by preventing it from initializing.
	@details        This function takes a PE image represented by its ImageBase, NtHeaders, and PageSection, and disables the Driver Signature Enforcement protection. The Driver Signature Enforcement is a security feature that restricts the loading of unsigned or improperly signed drivers in Windows.
	@details        The actual heart of Code Integrity lies inside a single executable image, called CI.dll (you can find it inside your \Windows\system32 directory). If we take a look at the list of imported symbols, we will most likely see the following names: CiCheckSignedFile, CiFindPageHashesInCatalog, CiFindPageHashesInSignedFile, CiFreePolicyInfo, CiGetPEInformation, CiInitialize, CiVerifyHashInCatalog. The first function within our interest is the initialization routine, CI!CiInitialize. This routine is imported by the NT core (ntoskrnl.exe), and called during system initialization


	@param[in]      ImageBase                       The base address of the PE image.
	@param[in]      NtHeaders                       A pointer to the PE image's NT headers.
	@param[in]      PageSection                     A pointer to the section header of the PE image.
	@param[in]      BuildNumber                     The build number of the operating system.


	@retval         EFI_SUCCESS	                    The Driver Signature Enforcement protection was successfully disabled.
	@retval         EFI_UNSUPPORTED                 The function is not supported on the provided PE image.
	@retval         EFI_INVALID_PARAMETER           One or more of the input parameters are invalid.
	@retval         EFI_NOT_FOUND                   The required data or structures were not found in the PE image.
**/
EFI_STATUS
Protections_DisableDriverSignatureEnforcement(
	IN              UINT8*                          ImageBase,
	IN              PEFI_IMAGE_NT_HEADERS           NtHeaders,
	IN              PEFI_IMAGE_SECTION_HEADER       PageSection,
	IN              UINT16                          BuildNumber
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
