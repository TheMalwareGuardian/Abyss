// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Functions Booting
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Locates, loads and starts an image from a specified file path.
	@details        This function locates, loads, and starts an image using the provided path. It first locates the file in the EFI System Partition, then loads it into memory, and finally starts the image.


	@param[in]      ImagePath                   The path of the image to load, specified as a wide string.


	@retval         EFI_SUCCESS                 The image was loaded and started successfully.
	@retval         other                       An error occurred during one of the steps (locating, loading, or starting).
**/
EFI_STATUS
BootOsBooting_FunctionsBooting_LocatesLoadsStartsImage(
	IN              CHAR16*                     ImagePath
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
