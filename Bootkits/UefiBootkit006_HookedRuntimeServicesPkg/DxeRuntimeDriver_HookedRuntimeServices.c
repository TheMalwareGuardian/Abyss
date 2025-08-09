// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	DxeRuntimeDriver_HookedRuntimeServices.c

	A DXE Runtime Driver that hooks all major UEFI Runtime Services (gRT), including GetVariable, SetVariable, GetTime, SetTime, ResetSystem, ConvertPointer, and SetVirtualAddressMap.

	This example serves as a foundation for understanding DXE runtime drivers, their persistent execution after ExitBootServices, and how they can be leveraged for developing bootkits or enabling low-level runtime interaction with UEFI services.


	@note

	Instructions to Compile and Run this DXE Runtime Driver (in a Virtual Machine using VMware Workstation):

	1. Open the file 'Conf/target.txt' located in the root of your EDK2 directory. Modify the following line to point to this DSC file:
		ACTIVE_PLATFORM = UefiBootkit006_HookedRuntimeServicesPkg/DxeRuntimeDriver_HookedRuntimeServices.dsc

	2. Open a command prompt and set up the EDK2 environment:
		> edksetup.bat Rebuild

	3. Build the driver:
		> build

	4. Once built successfully, locate the generated .EFI binary at:
		C:\edk2\Build\UefiBootkit006_HookedRuntimeServicesPkg\DEBUG_VS2019\X64\UefiBootkit006_HookedRuntimeServices.efi

	5. To test the driver:

		a. If you're already inside a Windows virtual machine (VM):

			- Mount the EFI System Partition (ESP):
				> mountvol U: /S

			- Copy the generated .efi binary to the EFI\Boot\Bootkits directory:
				> mkdir U:\EFI\Boot\Bootkits
				> copy C:\edk2\Build\UefiBootkit006_HookedRuntimeServicesPkg\DEBUG_VS2019\X64\UefiBootkit006_HookedRuntimeServices.efi U:\EFI\Boot\Bootkits\UefiBootkit006_HookedRuntimeServices.efi

			- Dismount the ESP:
				> mountvol U: /D

			- Shut down the VM

			- Power on the VM directly to firmware:
				Right-click the VM -> Power -> Power On to Firmware

			- From the firmware menu, select 'EFI Internal Shell', then press 'Enter'.

			- Run the driver in the shell:
				Shell> map -r
				Shell> fs0:
				Shell> dir
				Shell> cd EFI\Boot\Bootkits
				Shell> dir
				Shell> date
				Shell> time
				Shell> load UefiBootkit006_HookedRuntimeServices.efi
				Shell> date
				Shell> time

		b. If testing from outside the VM:

			- Create a shared folder between the host and guest, and place the .efi file inside it.

			- Power on the VM directly to firmware:
				Right-click the VM -> Power -> Power On to Firmware

			- From the firmware menu, select 'EFI Internal Shell', then press 'Enter'.

			- Run the driver in the shell:
				Shell> map -r
				Shell> fs0:
				Shell> dir
				Shell> cd EFI\Boot\Bootkits
				Shell> cp fsnt0:\UefiBootkit006_HookedRuntimeServices.efi .
				Shell> dir
				Shell> date
				Shell> time
				Shell> load UefiBootkit006_HookedRuntimeServices.efi
				Shell> date
				Shell> time
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
// Provides a service to retrieve a pointer to the EFI Runtime Services Table. This library does not contain any functions or macros.  It simply exports the global variable gRT that is a pointer to the EFI Runtime Services Table as defined in the UEFI Specification.  The global variable gRT must be preinitialized to NULL. The library constructor must set gRT to point at the EFI Runtime Services Table so it is available at the module's entry point. Since there is overhead in initializing this global variable, only those modules that actually require access to the EFI Runtime Services Table should use this library. Only available to DXE and UEFI module types.
// Provides a service to retrieve a pointer to the EFI Boot Services Table. Only available to DXE and UEFI module types.
#include <Library/UefiRuntimeServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>


// START -> GUIDS -----------------------------------------------------------------------------------------------------------------------------
// START -> GUIDS -----------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Guid/EventGroup.h
// GUIDs for gBS->CreateEventEx Event Groups. Defined in UEFI spec 2.0 and PI 1.2.1.
#include <Guid/EventGroup.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Event triggered when SetVirtualAddressMap() is called. Used to notify the driver to convert physical pointers to virtual ones.
STATIC EFI_EVENT Global_VirtualEvent = NULL;

// Flag indicating whether the system is now operating in virtual memory space. Set to TRUE after the VirtualAddressMap event fires.
STATIC BOOLEAN Global_IsVirtual = FALSE;

// Pointer to the original GetTime() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_GET_TIME Global_OriginalGetTime;

// Pointer to the original SetTime() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_SET_TIME Global_OriginalSetTime;

// Pointer to the original GetVariable() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_GET_VARIABLE Global_OriginalGetVariable;

// Pointer to the original SetVariable() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_SET_VARIABLE Global_OriginalSetVariable;

// Pointer to the original ResetSystem() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_RESET_SYSTEM Global_OriginalResetSystem;

// Pointer to the original ConvertPointer() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_CONVERT_POINTER Global_OriginalConvertPointer;

// Pointer to the original SetVirtualAddressMap() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_SET_VIRTUAL_ADDRESS_MAP Global_OriginalSetVirtualAddressMap;



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Hooked version of the UEFI GetTime() runtime service.


	@param[in,out]  Time                    Pointer to the returned current time.
	@param[out]     Capabilities            Pointer to the capabilities of the Real-Time Clock.


	@retval         EFI_STATUS              Status returned by the original GetTime().
**/
EFI_STATUS
HookedGetTime(
	IN OUT          EFI_TIME                *Time,
	OUT             EFI_TIME_CAPABILITIES   *Capabilities
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Returns the current time and date information, and the time-keeping capabilities of the hardware platform.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#gettime
	return Global_OriginalGetTime(Time, Capabilities);
}



/**
	@brief          Hooked version of the UEFI SetTime() runtime service.


	@param[in]      Time                    Pointer to the time structure to set.


	@retval         EFI_STATUS              Status returned by the original SetTime().
**/
EFI_STATUS
HookedSetTime(
	IN              EFI_TIME                *Time
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Sets the current local time and date information.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#settime
	return Global_OriginalSetTime(Time);
}



/**
	@brief          Hooked version of the UEFI GetVariable() runtime service.


	@param[in]      Name                    Name of the variable.
	@param[in]      VendorGuid              Vendor GUID of the variable.
	@param[out]     Attributes              Attributes of the variable.
	@param[in,out]  DataSize                Size of the data buffer.
	@param[out]     Data                    Pointer to the buffer that will receive the variable data.


	@retval         EFI_STATUS              Status returned by the original GetVariable().
**/
EFI_STATUS
HookedGetVariable(
	IN              CHAR16                  *Name,
	IN              EFI_GUID                *VendorGuid,
	OUT             UINT32                  *Attributes,
	IN OUT          UINTN                   *DataSize,
	OUT             VOID                    *Data
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Returns the value of a variable.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#getvariable
	return Global_OriginalGetVariable(Name, VendorGuid, Attributes, DataSize, Data);
}



/**
	@brief          Hooked version of the UEFI SetVariable() runtime service.


	@param[in]      Name                    Name of the variable.
	@param[in]      VendorGuid              Vendor GUID of the variable.
	@param[in]      Attributes              Variable attributes.
	@param[in]      DataSize                Size of the data buffer.
	@param[in]      Data                    Pointer to the data to be stored.


	@retval        EFI_STATUS               Status returned by the original SetVariable().
**/
EFI_STATUS
HookedSetVariable(
	IN              CHAR16                  *Name,
	IN              EFI_GUID                *VendorGuid,
	IN              UINT32                  Attributes,
	IN              UINTN                   DataSize,
	IN              VOID                    *Data
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Sets the value of a variable. This service can be used to create a new variable, modify the value of an existing variable, or to delete an existing variable.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#setvariable
	return Global_OriginalSetVariable(Name, VendorGuid, Attributes, DataSize, Data);
}



/**
	@brief          Hooked version of the UEFI ResetSystem() runtime service.


	@param[in]      Type                    Type of reset to perform.
	@param[in]      Status	                Status code for the reset.
	@param[in]      DataSize                Size of ResetData in bytes.
	@param[in]      ResetData               Optional reset-specific data.
**/
VOID
HookedResetSystem(
	IN              EFI_RESET_TYPE          Type,
	IN              EFI_STATUS              Status,
	IN              UINTN                   DataSize,
	IN              VOID                    *ResetData
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Resets the entire platform. If the platform supports See ref:EFI_RESET_NOTIFICATION_PROTOCOL, then prior to completing the reset of the platform, all of the pending notifications must be called.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#resetsystem
	Global_OriginalResetSystem(Type, Status, DataSize, ResetData);
}



/**
	@brief          Hooked version of the UEFI ConvertPointer() runtime service.


	@param[in]      DebugDisposition        Reserved. Set to 0.
	@param[in,out]  Address                 Pointer to the pointer to be converted.


	@retval         EFI_STATUS              Status returned by the original ConvertPointer().
**/
EFI_STATUS
HookedConvertPointer(
	IN              UINTN                   DebugDisposition,
	IN OUT          VOID                    **Address
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Determines the new virtual address that is to be used on subsequent memory accesses.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#convertpointer
	return Global_OriginalConvertPointer(DebugDisposition, Address);
}



/**
	@brief          Hooked version of the UEFI SetVirtualAddressMap() runtime service.


	@param[in]      MemoryMapSize           Size in bytes of the VirtualMap buffer.
	@param[in]      DescriptorSize          Size in bytes of an individual EFI_MEMORY_DESCRIPTOR.
	@param[in]      DescriptorVersion       Version of the EFI_MEMORY_DESCRIPTOR.
	@param[in]      VirtualMap              Pointer to the virtual memory map.


	@retval         EFI_STATUS              Status returned by the original SetVirtualAddressMap().
**/
EFI_STATUS
HookedSetVirtualAddressMap(
	IN              UINTN                   MemoryMapSize,
	IN              UINTN                   DescriptorSize,
	IN              UINT32                  DescriptorVersion,
	IN              EFI_MEMORY_DESCRIPTOR   *VirtualMap
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Changes the runtime addressing mode of EFI firmware from physical to virtual.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#setvirtualaddressmap
	return Global_OriginalSetVirtualAddressMap(MemoryMapSize, DescriptorSize, DescriptorVersion, VirtualMap);
}



/**
	@brief          Replaces a pointer to a UEFI Runtime Service with a custom hook.
	@details        Swaps the specified service function pointer with a new one (hook), recalculates the CRC32 of the table header, and returns the original pointer so it can be restored or used later if needed. The operation is performed with interrupts disabled (TPL_HIGH_LEVEL) to avoid race conditions.


	@param[in,out]  Header                  Pointer to the EFI_TABLE_HEADER of the service table (typically gRT->Hdr).
	@param[in,out]  Target                  Address of the pointer to the service function to replace.
	@param[in]      Hook                    Pointer to the new function to hook into the service.


	@retval         VOID*                   Original function pointer before the hook was installed. NULL if inputs are invalid.
**/
VOID*
SwapService(
	IN OUT          EFI_TABLE_HEADER        *Header,
	IN OUT          VOID                    **Target,
	IN              VOID                    *Hook
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Validate input
	if (!Target || !*Target || !Hook) return NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// Raise task priority level to TPL_HIGH_LEVEL to prevent race conditions while modifying function pointers

	// Raises a task’s priority level and returns its previous level.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-raisetpl
	EFI_TPL OldTpl = gBS->RaiseTPL(TPL_HIGH_LEVEL);


	// ---------------------------------------------------------------------------------------------------------------------
	// Save the original function pointer to restore it later or call it from the hook
	VOID *Original = *Target;


	// ---------------------------------------------------------------------------------------------------------------------
	// Overwrite the function pointer with the hook
	*Target = Hook;


	// ---------------------------------------------------------------------------------------------------------------------
	// Invalidate current CRC32 in the table header to update it after changes
	Header->CRC32 = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Recalculate the new CRC32 checksum of the table, now that a pointer has been modified in it

	// Computes and returns a 32-bit CRC for a data buffer.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-calculatecrc32
	gBS->CalculateCrc32(Header, Header->HeaderSize, &Header->CRC32);


	// ---------------------------------------------------------------------------------------------------------------------
	// Restore the original task priority level

	// Restores a task’s priority level to its previous value.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-restoretpl
	gBS->RestoreTPL(OldTpl);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return Original;
}



/**
	@brief          Callback for the SetVirtualAddressMap event.
	@details        This function is triggered when the UEFI firmware transitions to virtual addressing mode. It updates the saved original function pointers by converting them to their virtual addresses using ConvertPointer(), allowing the runtime driver to continue operating correctly after ExitBootServices.


	@param[in]      Event                   The event that triggered this callback (unused).
	@param[in]      Context                 Optional context pointer (unused).
**/
VOID
OnVirtualAddressChange(
	IN              EFI_EVENT               Event,
	IN              VOID                    *Context
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Once the OS takes over, UEFI switches from physical to virtual addresses. Any pointers saved before that (like the original runtime functions) are now invalid. We must convert them to their virtual equivalents using ConvertPointer().

	// Convert saved physical address of GetTime() to its virtual address

	// Determines the new virtual address that is to be used on subsequent memory accesses.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#convertpointer
	gRT->ConvertPointer(0, (VOID**)&Global_OriginalGetTime);

	// Convert saved physical address of SetTime() to its virtual address

	// Determines the new virtual address that is to be used on subsequent memory accesses.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#convertpointer
	gRT->ConvertPointer(0, (VOID**)&Global_OriginalSetTime);

	// Convert saved physical address of GetVariable() to its virtual address

	// Determines the new virtual address that is to be used on subsequent memory accesses.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#convertpointer
	gRT->ConvertPointer(0, (VOID**)&Global_OriginalGetVariable);

	// Convert saved physical address of SetVariable() to its virtual address

	// Determines the new virtual address that is to be used on subsequent memory accesses.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#convertpointer
	gRT->ConvertPointer(0, (VOID**)&Global_OriginalSetVariable);

	// Convert saved physical address of ResetSystem() to its virtual address

	// Determines the new virtual address that is to be used on subsequent memory accesses.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#convertpointer
	gRT->ConvertPointer(0, (VOID**)&Global_OriginalResetSystem);

	// Convert saved physical address of ConvertPointer() to its virtual address

	// Determines the new virtual address that is to be used on subsequent memory accesses.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#convertpointer
	gRT->ConvertPointer(0, (VOID**)&Global_OriginalConvertPointer);

	// Convert saved physical address of SetVirtualAddressMap() to its virtual address

	// Determines the new virtual address that is to be used on subsequent memory accesses.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#convertpointer
	gRT->ConvertPointer(0, (VOID**)&Global_OriginalSetVirtualAddressMap);


	// ---------------------------------------------------------------------------------------------------------------------
	// Flag to indicate that we are now in virtual memory mode
	Global_IsVirtual = TRUE;
}



/**
	@brief          The entry point for the DXE Runtime Driver.
	@details        This function is invoked by the UEFI firmware when the driver is loaded. It prints a message to the screen to indicate its execution and remains resident in memory even after the ExitBootServices call has been made. This persistence allows the driver to continue operating at runtime, after the operating system has taken control.


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


	// ---------------------------------------------------------------------------------------------------------------------
	// Hook all runtime services
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#rules-for-reentry-into-runtime-services

	// Returns the current time and date information, and the time-keeping capabilities of the hardware platform.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#gettime
	Global_OriginalGetTime              = (EFI_GET_TIME)SwapService(&gRT->Hdr, (VOID**)&gRT->GetTime, (VOID*)HookedGetTime);

	// Sets the current local time and date information.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#settime
	Global_OriginalSetTime              = (EFI_SET_TIME)SwapService(&gRT->Hdr, (VOID**)&gRT->SetTime, (VOID*)HookedSetTime);

	// Returns the value of a variable.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#getvariable
	Global_OriginalGetVariable          = (EFI_GET_VARIABLE)SwapService(&gRT->Hdr, (VOID**)&gRT->GetVariable, (VOID*)HookedGetVariable);

	// Sets the value of a variable. This service can be used to create a new variable, modify the value of an existing variable, or to delete an existing variable.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#setvariable
	Global_OriginalSetVariable          = (EFI_SET_VARIABLE)SwapService(&gRT->Hdr, (VOID**)&gRT->SetVariable, (VOID*)HookedSetVariable);

	// Resets the entire platform. If the platform supports See ref:EFI_RESET_NOTIFICATION_PROTOCOL, then prior to completing the reset of the platform, all of the pending notifications must be called.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#resetsystem
	Global_OriginalResetSystem          = (EFI_RESET_SYSTEM)SwapService(&gRT->Hdr, (VOID**)&gRT->ResetSystem, (VOID*)HookedResetSystem);

	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#convertpointer
	Global_OriginalConvertPointer       = (EFI_CONVERT_POINTER)SwapService(&gRT->Hdr, (VOID**)&gRT->ConvertPointer, (VOID*)HookedConvertPointer);

	// Changes the runtime addressing mode of EFI firmware from physical to virtual.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#setvirtualaddressmap
	Global_OriginalSetVirtualAddressMap = (EFI_SET_VIRTUAL_ADDRESS_MAP)SwapService(&gRT->Hdr, (VOID**)&gRT->SetVirtualAddressMap, (VOID*)HookedSetVirtualAddressMap);


	// ---------------------------------------------------------------------------------------------------------------------
	// Register event for virtual address change
	EFI_STATUS Status = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_NOTIFY, OnVirtualAddressChange, NULL, &gEfiEventVirtualAddressChangeGuid, &Global_VirtualEvent);

	// Failed
	if (EFI_ERROR(Status))
	{
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------