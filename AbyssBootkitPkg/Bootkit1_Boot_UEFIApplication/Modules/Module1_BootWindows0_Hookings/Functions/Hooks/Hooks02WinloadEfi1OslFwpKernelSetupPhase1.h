// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks winload.efi!OslFwpKernelSetupPhase1
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Hooks the OslFwpKernelSetupPhase1 function.
	@details        This function replaces the original OslFwpKernelSetupPhase1 function with a custom hook that allows for additional functionality.


	@param[in]      LoaderBlock                 A pointer to the Loader Parameter Block.


	@retval         EFI_SUCCESS                 The OslFwpKernelSetupPhase1 function was successfully hooked.
	@retval         other                       An error occurred during the hooking process.
**/
EFI_STATUS
BootWindowsHookings_HooksWinloadEfi_OslFwpKernelSetupPhase1(
	IN              PLOADER_PARAMETER_BLOCK     LoaderBlock
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
