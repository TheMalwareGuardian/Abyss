// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Banner
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Displays an ASCII banner.
	@details        This function is responsible for displaying an ASCII banner when called.


	@retval         None
**/
VOID
PreBootSetup_UtilsBanner_PrintBannerAscii();



/**
	@brief          Displays a BMP image on the screen based on the selected image.
	@details        This function selects a BMP image based on the ImageSelect parameter and uses the Graphics Output Protocol to display it on the screen.


	@param[in]      GraphicsOutput                      The Graphics Output Protocol instance to use for drawing the image.
	@param[in]      ImageSelect                         The index of the image to display (1 or 2).


	@retval         EFI_SUCCESS                         The image was successfully displayed.
	@retval         EFI_INVALID_PARAMETER               The ImageSelect parameter is invalid.
	@retval         EFI_OUT_OF_RESOURCES                Failed to allocate memory for the image buffer.
	@retval         EFI_UNSUPPORTED                     The image format is not supported.
**/
EFI_STATUS
PreBootSetup_UtilsBanner_PrintBannerBmp(
	IN              EFI_GRAPHICS_OUTPUT_PROTOCOL        *GraphicsOutput,
	IN              UINTN                               ImageSelect
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
