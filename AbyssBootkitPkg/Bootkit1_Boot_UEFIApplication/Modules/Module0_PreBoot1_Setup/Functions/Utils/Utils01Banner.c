// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Banner
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

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/MemoryAllocationLib.h
// Provides services to allocate and free memory buffers of various memory types and alignments. The Memory Allocation Library abstracts various common memory allocation operations. This library allows code to be written in a phase-independent manner because the allocation of memory in PEI, DXE, and SMM (for example) is done via a different mechanism. Using a common library interface makes it much easier to port algorithms from phase to phase.
#include <Library/MemoryAllocationLib.h>

// https://github.com/tianocore/edk2/blob/master/MdeModulePkg/Include/Library/BmpSupportLib.h
// Provides services to convert a BMP graphics image to a GOP BLT buffer and to convert a GOP BLT buffer to a BMP graphics image.
#include <Library/BmpSupportLib.h>



// START -> STANDARDS -------------------------------------------------------------------------------------------------------------------------
// START -> STANDARDS -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/IndustryStandard/Bmp.h
// This file defines BMP file header data structures.
#include <IndustryStandard/Bmp.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>



// START -> PAYLOADS --------------------------------------------------------------------------------------------------------------------------
// START -> PAYLOADS --------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot1_Setup/Payloads/Payloads00DefaultBmpLogos.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Displays an ASCII banner.
	@details        This function is responsible for displaying an ASCII banner when called.


	@retval         None
**/
VOID
PreBootSetup_UtilsBanner_PrintBannerAscii()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerAscii) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// ASCII
	Print(L"########******************************#####*==##*##************#*#####********************\r\n");
	Print(L"+==*#***+++++++++++++++***+===+++*****#**+=---=+*****+++++++++***************+++==++******\r\n");
	Print(L"+==++****+++++++*****+++**++++++++****+++====--=-+*+++++++++++++***********+++*===++++++++\r\n");
	Print(L"+++++***++++***+******++++++++++++#**==+========---++*++++++=+++******+++++=++++++++++++*+\r\n");
	Print(L"+++****++***+++++++++++++++++++++*%%*===++++=======--=-+*+++====++++***++++++++++++++*++*++\r\n");
	Print(L"+++***++*+++++++==+++++++++++++***++===++======--=---=-+*+=====++++++*++++++++++++**+*++++\r\n");
	Print(L"++***++++++++++==+++++++++++++#**===+===+++========--==-+*======+=====++++++++++++***++*+=\r\n");
	Print(L"++*+==++++==++++++++++++++===#*++++++++++++++===++=======+*========-=++++++++++++++**++**+\r\n");
	Print(L"++=--::-=+==++*++++++++=----+#*+++++++*****++++++++++==+=++*========++++++++++++++++**++++\r\n");
	Print(L"+=====---+++++*++++++++=-:-*#********************+++++++++*#*=====-:=+============++++++++\r\n");
	Print(L"++======+++++***+++++++=-+#****++++++===============+++++*****=====--+=============+==++++\r\n");
	Print(L"+++++++++++++++++++++++****+**#########%%%%@@@@@@@@%%#***++******#+===-=============++++=====\r\n");
	Print(L"*++***++++====++++++++#*#%%@@@%%*+*#*--##*+**+**+*++*##*::==+#%%%%#*#===+============+====+===\r\n");
	Print(L"**+*+++++===++++++++==*#%%@@@@@*###   =*##+*++++*+*#*-    ++@@@@@*-==========++=+++++++****\r\n");
	Print(L"**+++++++++++++++++====+*#@@@@**#%%+#*%%*#**********#+#++-=**@@@@#--=========***+++==++*****\r\n");
	Print(L"**++**++++++++**++++=====**%%@@#**%%%%##*#*+*#+=+*#++*#******#@@%%%%---=========****++++++*****\r\n");
	Print(L"++++++=+*+=++**+=++=======**@@@#**##**++*#++==+*#*+**##***@@##=--==========****+++*****+++\r\n");
	Print(L"*****+++******++===******==#*@%%**##***##*+++=++++###*###**#%%*+-----========+**=======+*===\r\n");
	Print(L"*###******+===+*++***=*+**-+*%%@%%##**#*+***+*+++++++****#%%%%@##----+++++=====+**++====++**++\r\n");
	Print(L"####****++====+*+++++*++#+--#*@@@@%%***+*+*+*+++*+*=#**%%@@@%%*#---***=+**+==++*+++++++++++++\r\n");
	Print(L"##***+==---=======++++**+=++#*%%@@@%%#**=***+**++#**=#*+%%%%@@%%#*=--#**==*+*=+++*++=====+++++*\r\n");
	Print(L"#+==-====--=++++++=+**++***#%%*#%%@#%%*#=+#%%++**++%%#*=+#+*#%%@@#*#**#*+*+=+***++*++=====++++**\r\n");
	Print(L"*+=--======+*+==++**+#*#**+*#*###**#=+*##++*#*+#%%*+=**+*##%%#*#***#*+++++===+*++===-=++++*+\r\n");
	Print(L"*+=--==+=*+==+****+=#+****#****++*#=+*#@+++##*+*%%#*+=##++*###*++*****+++++++*++=====++++++\r\n");
	Print(L"***++++*+=+*#**+====#****+++++*##*=+*#@%%++*%%%%*++#%%#*++%%#*+++***#******#**+++***++++++++++*\r\n");
	Print(L"+*==+**=+****+++++++#*++=++**#%%%%+++*%%%%%%#++#@##++#%%%%#*++*%%%%#*++++++*****++***+++**+++++++++\r\n");
	Print(L"++++**+****+++++++*+=+++**%%##%%*+**#%%##%%*++%%%%##++*%%%%@%%#*+*##%%##**+++++**===++***++**+++++++\r\n");
	Print(L"++++#++**++++++++*++++*##%%@##*+*#%%%%##@@++*%%%%#%%*++@@##%%#*+*#%%@@%%##*+++++*=====+***++*++++++\r\n");
	Print(L"====#+***=======*++++*#%%##%%%%**#%%##*##%%%%++#@%%@@#+=%%%%%%%%##%%#**@%%%%%%%%%%%%%%*++++*+====+***++*==+==\r\n");
	Print(L"***+****#+====+*#+++*##@@%%%%#*%%@###***%%#+*#%%%%%%%%%%*+*%%##*###**%%%%@@@@@@@#*+++**=====***+#+*=+*\r\n");
	Print(L"#**++*****++*####++*#%%%%%%%%@@%%*@#============================+@@%%%%%%%%##%%#+++##*+==+##**##*++*\r\n");
	Print(L"#**+++++*##*#####++*%%%%%%@@@#*###--------------=-==--==+-----=@@#%%%%%%%%%%#%%*++#####*##*#%%#**+=+\r\n");
	Print(L"**++==+*%%%%##*#**#***#@@%%*===#%%%%-------------=--++=*#*+-----+@@+=+*%%%%##*+*%%###*######*+++==\r\n");
	Print(L"*++++==%%#*++*#####*+*#======@@%%-----------==++#*#**=-------+@@+--==#@***#****##*+*%%***++++\r\n");
	Print(L"*++++=+###***+***###**#***++@@@-------=+-==-=*+**+++=------*@@*++**#***##******+*#%%***+++=\r\n");
	Print(L"*++++=+#**####*****###********#=-----------==+====---------**********#####****##%%#%%**++++=\r\n");
	Print(L"**+++=+###**###################==-------===-=-=------------*########********#*#***%%**+++==\r\n");
	Print(L"**+++=+*******#####%%%%%%%%%%%%%%%%%%%%%%%%=====----::::::::::---------#%%%%%%%%%%%%%%%%%%%%###*********#***++++\r\n");
	Print(L"*****+*=--------=+===+****+*++++===========================*++++++==----------=---****++++\r\n");
	Print(L"#####**===---==++=-=*******+==+*****************************+==*+-------::::::::--+###****\r\n");
	Print(L"===--***++++++**+=--#######*+#********************************-=++++=---------=====*******\r\n");
	Print(L"-===+********+=---==******++=================================------=+++====+++*=====++==--\r\n");
	Print(L"******************************************************************************************\r\n");
	Print(L"Abyss Windows UEFI Bootkit - By TheMalwareGuardian\r\n");
	Print(L"\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerAscii) - Bye\r\n");
}



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
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	BMP_IMAGE_HEADER *BmpHeader;
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
	UINTN BltSize;
	UINTN Index;
	UINTN X;
	UINTN Y;
	UINTN ImageX, ImageY;
	UINTN ScreenWidth, ScreenHeight;
	UINTN ImageWidth, ImageHeight;
	UINT8* SelectedImage;


	// ---------------------------------------------------------------------------------------------------------------------
	// Select the image based on ImageSelect
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - Select the image based on ImageSelect\r\n");

	// Abyss Squid
	if (ImageSelect == 1)
	{
		SelectedImage = Global_PreBootSetup_PayloadsBmpLogos_BmpAbyssSquid;
	}
	// Squid
	else if (ImageSelect == 2)
	{
		SelectedImage = Global_PreBootSetup_PayloadsBmpLogos_BmpSquid;
	}
	// Invalid image selection
	else
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - Operation failed: Invalid image selection.\r\n");
		return EFI_INVALID_PARAMETER;
	}

	// Use the selected BMP data
	BmpHeader = (BMP_IMAGE_HEADER*)SelectedImage;


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the file is a BMP file
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - Check if the file is a BMP file\r\n");

	if (BmpHeader->CharB != 'B' || BmpHeader->CharM != 'M')
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - Operation failed: Embedded data is not a BMP file.\r\n");
		return EFI_UNSUPPORTED;
	}

	// ---------------------------------------------------------------------------------------------------------------------
	// Get the dimensions
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - Get the dimensions\r\n");

	// Get the dimensions of the BMP image
	ImageWidth = BmpHeader->PixelWidth;
	ImageHeight = BmpHeader->PixelHeight;

	// Get the dimensions of the screen
	ScreenWidth = GraphicsOutput->Mode->Info->HorizontalResolution;
	ScreenHeight = GraphicsOutput->Mode->Info->VerticalResolution;

	// Calculate the position to center the image
	ImageX = (ScreenWidth - ImageWidth) / 2;
	ImageY = (ScreenHeight - ImageHeight) / 2;


	// ---------------------------------------------------------------------------------------------------------------------
	// Allocate buffer for the Blt operation
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - Allocate buffer for the Blt operation\r\n");

	BltSize = ImageWidth * ImageHeight * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL);

	// Allocates pool memory.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
	BltBuffer = AllocatePool(BltSize);

	// Failed
	if (BltBuffer == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - AllocatePool [BltSize] failed.\r\n");
		return EFI_OUT_OF_RESOURCES;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Copy BMP data to Blt buffer, accounting for the bottom-up order
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - Copy BMP data to Blt buffer, accounting for the bottom-up order\r\n");

	// Iterate
	for (Y = 0; Y < ImageHeight; Y++)
	{
		for (X = 0; X < ImageWidth; X++)
		{
			Index = (ImageHeight - 1 - Y) * ImageWidth + X;
			BltBuffer[Y * ImageWidth + X].Blue = SelectedImage[BmpHeader->ImageOffset + Index * 3 + 0];
			BltBuffer[Y * ImageWidth + X].Green = SelectedImage[BmpHeader->ImageOffset + Index * 3 + 1];
			BltBuffer[Y * ImageWidth + X].Red = SelectedImage[BmpHeader->ImageOffset + Index * 3 + 2];
			BltBuffer[Y * ImageWidth + X].Reserved = 0;
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Draw the image at the calculated position
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - Draw the image at the calculated position\r\n");

	// Draw
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/GraphicsOutput.h#L261
	Status = GraphicsOutput->Blt(GraphicsOutput, BltBuffer, EfiBltBufferToVideo, 0, 0, ImageX, ImageY, ImageWidth, ImageHeight, 0);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(BltBuffer);

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - GraphicsOutput->Blt failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Banner.c] (PrintBannerBmp) - Bye\r\n");

	// Returns pool memory to the system.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
	FreePool(BltBuffer);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
