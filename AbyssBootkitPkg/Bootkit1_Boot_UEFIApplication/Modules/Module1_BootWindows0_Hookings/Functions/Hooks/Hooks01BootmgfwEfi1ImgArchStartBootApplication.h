// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks bootmgfw.efi!ImgArchStartBootApplication
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Hooks the ImgArchStartBootApplication function.
	@details        This function replaces the original ImgArchStartBootApplication function with a custom hook that allows for additional functionality.


	@param[in]      AppEntry                    A pointer to the BL_APPLICATION_ENTRY structure.
	@param[in]      ImageBase                   A pointer to the base address of the loaded image.
	@param[in]      ImageSize                   The size, in bytes, of the loaded image.
	@param[in]      BootOption                  The boot option identifier.
	@param[out]     ReturnArguments             A pointer to the BL_RETURN_ARGUMENTS structure.


	@retval         EFI_SUCCESS                 The ArchStartBootApplication function was successfully hooked.
	@retval         other                       An error occurred during the hooking process.
**/
EFI_STATUS
BootWindowsHookings_HooksBootmgfwEfi_ImgArchStartBootApplication(
	IN              PBL_APPLICATION_ENTRY       AppEntry,
	IN              VOID*                       ImageBase,
	IN              UINT32                      ImageSize,
	IN              UINT32                      BootOption,
	OUT             PBL_RETURN_ARGUMENTS        ReturnArguments
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
