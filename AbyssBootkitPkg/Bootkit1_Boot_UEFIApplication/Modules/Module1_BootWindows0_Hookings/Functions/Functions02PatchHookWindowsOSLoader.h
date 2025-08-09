// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Functions Patch/Hook Windows OS Manager
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Patch to Windows OS Loader (winload.efi) image.
	@details        This function applies a custom patch to modify the behavior of the Windows OS Loader.


	@param[in]      ImageBase                   A pointer to the base address of the loaded image.
	@param[in]      NtHeaders                   A pointer to the NT headers of the Winload EFI image.


	@retval         EFI_SUCCESS                 The patch was successfully applied.
	@retval         other                       An error occurs during the patching process.
**/
EFI_STATUS
BootWindowsHookings_FunctionsWindowsOSLoader_PatchWinloadEfi(
	IN              VOID*                       ImageBase,
	IN              PEFI_IMAGE_NT_HEADERS       NtHeaders
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
