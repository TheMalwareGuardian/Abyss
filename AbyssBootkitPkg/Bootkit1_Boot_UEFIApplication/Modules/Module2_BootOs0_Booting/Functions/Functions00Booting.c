// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Booting
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
#include <Modules/Module2_BootOs0_Booting/Functions/Utils/Utils00EFISystemPartition.h>



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
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootOsBooting [Functions/Booting.c] (LocatesLoadsStartsImage) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_DEVICE_PATH *DevicePath = NULL;
	EFI_HANDLE ImageHandle = NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// Locate file in EFI partition
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootOsBooting [Functions/Booting.c] (LocatesLoadsStartsImage) - Locate file in EFI partition\r\n");

	Status = BootOsBooting_UtilsEFISystemPartition_LocateFileInEFIPartition(ImagePath, &DevicePath);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootOsBooting [Functions/Booting.c] (LocatesLoadsStartsImage) - BootOsBooting_UtilsEFISystemPartition_LocateFileInEFIPartition failed: %r (%llx)\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Load the image
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootOsBooting [Functions/Booting.c] (LocatesLoadsStartsImage) - Load the image\r\n");

	// Loads an EFI image into memory.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-loadimage
	Status = gBS->LoadImage(FALSE, gImageHandle, DevicePath, NULL, 0, &ImageHandle);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootOsBooting [Functions/Booting.c] (LocatesLoadsStartsImage) - gBS->LoadImage failed: %r (%llx)\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Start the image
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootOsBooting [Functions/Booting.c] (LocatesLoadsStartsImage) - Start the image\r\n");

	// Transfers control to a loaded image’s entry point.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-startimage
	Status = gBS->StartImage(ImageHandle, NULL, NULL);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootOsBooting [Functions/Booting.c] (LocatesLoadsStartsImage) - gBS->StartImage failed: %r (%llx)\r\n", Status, Status);
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
