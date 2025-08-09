// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks Runtime Services
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
// Provides a service to retrieve a pointer to the EFI Runtime Services Table. This library does not contain any functions or macros. It simply exports the global variable gRT that is a pointer to the EFI Runtime Services Table as defined in the UEFI Specification. The global variable gRT must be preinitialized to NULL. The library constructor must set gRT to point at the EFI Runtime Services Table so it is available at the module's entry point. Since there is overhead in initializing this global variable, only those modules that actually require access to the EFI Runtime Services Table should use this library. Only available to DXE and UEFI module types.
#include <Library/UefiRuntimeServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Functions/Hooks/Hooks01RuntimeServices.h>
#include <Structures/Structures01Execution.h>
#include <Functions/Utils/Utils02Execution.h>
#include <Globals/Globals01Operations.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Track boot process state, TRUE after SetVirtualAddressMap() is called
BOOLEAN Global_HooksRuntimeServices_VirtualMemoryActivated = FALSE;

// Track boot process state, TRUE after ExitBootServices() is called
BOOLEAN Global_HooksRuntimeServices_ExitBootServicesReached = FALSE;

// Event triggered when SetVirtualAddressMap() is called (used to convert physical to virtual pointers)
EFI_EVENT Global_HooksRuntimeServices_VirtualAddressChangeEvent = NULL;

// Event triggered when ExitBootServices() is called (useful for cleanup or debugging before OS takes over)
EFI_EVENT Global_HooksRuntimeServices_ExitBootServicesEvent = NULL;

// Pointer to the original GetTime() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_GET_TIME Global_HooksRuntimeServices_OriginalGetTime;

// Pointer to the original SetTime() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_SET_TIME Global_HooksRuntimeServices_OriginalSetTime;

// Pointer to the original GetWakeupTime() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_GET_WAKEUP_TIME Global_HooksRuntimeServices_OriginalGetWakeupTime;

// Pointer to the original SetWakeupTime() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_SET_WAKEUP_TIME Global_HooksRuntimeServices_OriginalSetWakeupTime;

// Pointer to the original GetVariable() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_GET_VARIABLE Global_HooksRuntimeServices_OriginalGetVariable;

// Pointer to the original GetNextVariableName() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_GET_NEXT_VARIABLE_NAME Global_HooksRuntimeServices_OriginalGetNextVariableName;

// Pointer to the original SetVariable() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_SET_VARIABLE Global_HooksRuntimeServices_OriginalSetVariable;

// Pointer to the original ResetSystem() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_RESET_SYSTEM Global_HooksRuntimeServices_OriginalResetSystem;

// Pointer to the original ConvertPointer() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_CONVERT_POINTER Global_HooksRuntimeServices_OriginalConvertPointer;

// Pointer to the original SetVirtualAddressMap() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_SET_VIRTUAL_ADDRESS_MAP Global_HooksRuntimeServices_OriginalSetVirtualAddressMap;

// Pointer to the original GetNextHighMonotonicCount() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_GET_NEXT_HIGH_MONO_COUNT Global_HooksRuntimeServices_OriginalGetNextHighMonotonicCount;

// Pointer to the original UpdateCapsule() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_UPDATE_CAPSULE Global_HooksRuntimeServices_OriginalUpdateCapsule;

// Pointer to the original QueryCapsuleCapabilities() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_QUERY_CAPSULE_CAPABILITIES Global_HooksRuntimeServices_OriginalQueryCapsuleCapabilities;

// Pointer to the original QueryVariableInfo() service. Saved before hooking in order to forward calls or restore functionality if needed.
EFI_QUERY_VARIABLE_INFO Global_HooksRuntimeServices_OriginalQueryVariableInfo;



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Callback function triggered when SetVirtualAddressMap() is called by the OS. This signals the transition from physical to virtual addressing.
	@details        In UEFI, the OS calls SetVirtualAddressMap() to inform firmware that it's switching to virtual memory. Any pointers used by runtime code (like Runtime Services) must now be converted from physical to virtual addresses using ConvertPointer(). This function updates all global function pointers originally hooked in physical space, so they continue to work correctly in virtual memory.


	@param[in]      Event                   The event that triggered this callback (unused).
	@param[in]      Context                 Optional context (unused).
**/
VOID
HooksRuntimeServices_SetVirtualAddressMapEvent(
	IN              EFI_EVENT               Event,
	IN              VOID*                   Context
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Convert original SetVariable address
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalSetVariable);


	// ---------------------------------------------------------------------------------------------------------------------
	// Convert all other addresses
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalGetTime);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalSetTime);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalGetWakeupTime);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalSetWakeupTime);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalGetVariable);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalGetNextVariableName);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalResetSystem);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalConvertPointer);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalSetVirtualAddressMap);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalGetNextHighMonotonicCount);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalUpdateCapsule);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalQueryVariableInfo);
	gRT->ConvertPointer(0, (VOID **)&Global_HooksRuntimeServices_OriginalQueryCapsuleCapabilities);


	// ---------------------------------------------------------------------------------------------------------------------
	// Null and close the event so it does not get called again
	Global_HooksRuntimeServices_VirtualAddressChangeEvent = NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// We are now working in virtual address-space
	Global_HooksRuntimeServices_VirtualMemoryActivated = TRUE;


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
}



/**
	@brief          Callback function triggered just before control is handed over to the OS (ExitBootServices).
	@details        This event marks the end of boot services. After this point: 1. Boot Services (gBS) are no longer usable, 2. Only Runtime Services (gRT) remain valid, 3. It's the final chance to clean up or perform setup before the OS fully takes over. This is commonly used by DXE runtime drivers to invalidate or nullify Boot Services pointers.


	@param[in]      Event                   The event that triggered this callback (unused).
	@param[in]      Context                 Optional context (unused).
**/
VOID
HooksRuntimeServices_ExitBootServicesEvent(
	IN              EFI_EVENT               Event,
	IN              VOID*                   Context
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// This event is called only once so close it
	gBS->CloseEvent(Global_HooksRuntimeServices_ExitBootServicesEvent);
	Global_HooksRuntimeServices_ExitBootServicesEvent = NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// Boot services are now not avaible
	//gBS = NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// We are booting the OS now
	Global_HooksRuntimeServices_ExitBootServicesReached = TRUE;


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
}



/**
	@brief          Hooked version of the UEFI GetTime() runtime service.


	@param[in,out]  Time                    Pointer to the returned current time.
	@param[out]     Capabilities            Pointer to the capabilities of the Real-Time Clock.


	@retval         EFI_STATUS              Status returned by the original GetTime().
**/
EFI_STATUS
HooksRuntimeServices_HookedGetTime(
	IN OUT          EFI_TIME                *Time,
	OUT             EFI_TIME_CAPABILITIES   *Capabilities
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Returns the current time and date information, and the time-keeping capabilities of the hardware platform.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#gettime
	return Global_HooksRuntimeServices_OriginalGetTime(Time, Capabilities);
}



/**
	@brief          Hooked version of the UEFI SetTime() runtime service.


	@param[in]      Time                    Pointer to the time structure to set.


	@retval         EFI_STATUS              Status returned by the original SetTime().
**/
EFI_STATUS
HooksRuntimeServices_HookedSetTime(
	IN              EFI_TIME                *Time
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Sets the current local time and date information.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#settime
	return Global_HooksRuntimeServices_OriginalSetTime(Time);
}



/**
	@brief          Hooked version of the UEFI GetWakeupTime() runtime service.


	@param[out]     Enabled                 Indicates whether the wakeup timer is currently enabled.
	@param[out]     Pending                 Indicates whether the wakeup timer is pending and not triggered.
	@param[out]     Time                    Pointer to the time at which the system is set to wake up.


	@retval         EFI_STATUS              Status returned by the original GetWakeupTime().
**/
EFI_STATUS
HooksRuntimeServices_HookedGetWakeupTime(
	OUT             BOOLEAN                 *Enabled,
	OUT             BOOLEAN                 *Pending,
	OUT             EFI_TIME                *Time
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Returns the current wakeup alarm clock setting.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#getwakeuptime
	return Global_HooksRuntimeServices_OriginalGetWakeupTime(Enabled, Pending, Time);
}



/**
	@brief          Hooked version of the UEFI SetWakeupTime() runtime service.


	@param[in]      Enable                  TRUE to enable the wakeup timer, FALSE to disable it.
	@param[in]      Time                    Pointer to the time at which the system should wake up.


	@retval         EFI_STATUS              Status returned by the original SetWakeupTime().
**/
EFI_STATUS
HooksRuntimeServices_HookedSetWakeupTime(
	IN              BOOLEAN                 Enable,
	IN              EFI_TIME                *Time
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Sets the system wakeup alarm clock time.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#setwakeuptime
	return Global_HooksRuntimeServices_OriginalSetWakeupTime(Enable, Time);
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
HooksRuntimeServices_HookedGetVariable(
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
	return Global_HooksRuntimeServices_OriginalGetVariable(Name, VendorGuid, Attributes, DataSize, Data);
}



/**
	@brief          Hooked version of the UEFI GetNextVariableName() runtime service.


	@param[in,out]  VariableNameSize        Size of the variable name buffer.
	@param[in,out]  VariableName            Pointer to the buffer holding the previous or next variable name.
	@param[in,out]  VendorGuid              Vendor GUID of the variable.



	@retval         EFI_STATUS              Status returned by the original GetNextVariableName().
**/
EFI_STATUS
HooksRuntimeServices_HookedGetNextVariableName(
	IN OUT          UINTN                   *VariableNameSize,
	IN OUT          CHAR16                  *VariableName,
	IN OUT          EFI_GUID                *VendorGuid
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Enumerates the current variable names.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#getnextvariablename
	return Global_HooksRuntimeServices_OriginalGetNextVariableName(VariableNameSize, VariableName, VendorGuid);
}



/**
	@brief          Hooked version of the UEFI SetVariable() runtime service.
	@details        Logs the name of the variable being set and delegates the call to the original SetVariable().


	@param[in]      VariableName            Name of the variable.
	@param[in]      VendorGuid              Vendor GUID of the variable.
	@param[in]      Attributes              Variable attributes.
	@param[in]      DataSize                Size of the data buffer.
	@param[in]      Data                    Pointer to the data to be stored.


	@retval        EFI_STATUS               Status returned by the original SetVariable().
**/
EFI_STATUS
HooksRuntimeServices_HookedSetVariable(
	IN              CHAR16                  *VariableName,
	IN              EFI_GUID                *VendorGuid,
	IN              UINT32                  Attributes,
	IN              UINTN                   DataSize,
	IN              VOID                    *Data
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Use our hook only after we are in virtual address-space
	if (Global_HooksRuntimeServices_VirtualMemoryActivated && Global_HooksRuntimeServices_ExitBootServicesReached) 
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Check of input is not null
		if (VariableName != NULL && VariableName[0] != CHAR_NULL && VendorGuid != NULL) 
		{
			// -------------------------------------------------------------------------------------------------------------
			// Check if variable name is same as our declared one. This is used to check if call is really from Bootkit2_Runtime_DXERuntimeDriver_UserClient running in the OS
			if (StrnCmp(VariableName, MACRO_GLOBALSOPERATIONS_VARIABLE_NAME, (sizeof(MACRO_GLOBALSOPERATIONS_VARIABLE_NAME) / sizeof(CHAR16)) - 1) == 0) 
			{
				if (DataSize == 0 && Data == NULL)
				{
					// Skip no data
					return EFI_SUCCESS;
				}
				// ---------------------------------------------------------------------------------------------------------
				// Check if the data size is correct
				if (DataSize == sizeof(STRUCTURES_EXECUTION_MEMORYCOMMAND)) 
				{
					// Valid command received from user-mode client
					// Cast the data buffer to our command structure and pass it to the internal dispatcher responsible for executing kernel-level operations.
					return UtilsExecution_RunCommand((STRUCTURES_EXECUTION_MEMORYCOMMAND*)Data);
				}
			}
		}
	}



	// ---------------------------------------------------------------------------------------------------------------------
	// Call the original SetVariable() function

	// Sets the value of a variable. This service can be used to create a new variable, modify the value of an existing variable, or to delete an existing variable.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#setvariable
	return Global_HooksRuntimeServices_OriginalSetVariable(VariableName, VendorGuid, Attributes, DataSize, Data);
}



/**
	@brief          Hooked version of the UEFI ResetSystem() runtime service.


	@param[in]      Type                    Type of reset to perform.
	@param[in]      Status	                Status code for the reset.
	@param[in]      DataSize                Size of ResetData in bytes.
	@param[in]      ResetData               Optional reset-specific data.
**/
VOID
HooksRuntimeServices_HookedResetSystem(
	IN              EFI_RESET_TYPE          Type,
	IN              EFI_STATUS              Status,
	IN              UINTN                   DataSize,
	IN              VOID                    *ResetData
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Resets the entire platform. If the platform supports See ref:EFI_RESET_NOTIFICATION_PROTOCOL, then prior to completing the reset of the platform, all of the pending notifications must be called.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#resetsystem
	Global_HooksRuntimeServices_OriginalResetSystem(Type, Status, DataSize, ResetData);
}



/**
	@brief          Hooked version of the UEFI ConvertPointer() runtime service.


	@param[in]      DebugDisposition        Reserved. Set to 0.
	@param[in,out]  Address                 Pointer to the pointer to be converted.


	@retval         EFI_STATUS              Status returned by the original ConvertPointer().
**/
EFI_STATUS
HooksRuntimeServices_HookedConvertPointer(
	IN              UINTN                   DebugDisposition,
	IN OUT          VOID                    **Address
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Determines the new virtual address that is to be used on subsequent memory accesses.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#convertpointer
	return Global_HooksRuntimeServices_OriginalConvertPointer(DebugDisposition, Address);
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
HooksRuntimeServices_HookedSetVirtualAddressMap(
	IN              UINTN                   MemoryMapSize,
	IN              UINTN                   DescriptorSize,
	IN              UINT32                  DescriptorVersion,
	IN              EFI_MEMORY_DESCRIPTOR   *VirtualMap
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Changes the runtime addressing mode of EFI firmware from physical to virtual.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#setvirtualaddressmap
	return Global_HooksRuntimeServices_OriginalSetVirtualAddressMap(MemoryMapSize, DescriptorSize, DescriptorVersion, VirtualMap);
}



/**
	@brief          Hooked version of the UEFI GetNextHighMonotonicCount() runtime service.


	@param[out]     HighCount              Pointer that will receive the high 32 bits of the monotonic counter.


	@retval         EFI_STATUS             Status returned by the original GetNextHighMonotonicCount().
**/
EFI_STATUS
HooksRuntimeServices_HookedGetNextHighMonotonicCount(
	OUT             UINT32                 *HighCount
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Resets the entire platform. If the platform supports See ref:EFI_RESET_NOTIFICATION_PROTOCOL, then prior to completing the reset of the platform, all of the pending notifications must be called.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#resetsytem
	return Global_HooksRuntimeServices_OriginalGetNextHighMonotonicCount(HighCount);
}



/**
	@brief          Hooked version of the UEFI UpdateCapsule() runtime service.


	@param[in]      CapsuleHeaderArray     Array of capsule headers to be passed to the firmware.
	@param[in]      CapsuleCount           Number of capsule headers in the array.
	@param[in]      ScatterGatherList      Optional physical address of a scatter-gather list for data buffers.


	@retval         EFI_STATUS             Status returned by the original UpdateCapsule().
**/
EFI_STATUS
HooksRuntimeServices_HookedUpdateCapsule(
	IN              EFI_CAPSULE_HEADER     **CapsuleHeaderArray,
	IN              UINTN                  CapsuleCount,
	IN              EFI_PHYSICAL_ADDRESS   ScatterGatherList
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Passes capsules to the firmware with both virtual and physical mapping. Depending on the intended consumption, the firmware may process the capsule immediately. If the payload should persist across a system reset, the reset value returned from EFI_QueryCapsuleCapabilities must be passed into Reset System and will cause the capsule to be processed by the firmware as part of the reset process.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#updatecapsule
	return Global_HooksRuntimeServices_OriginalUpdateCapsule(CapsuleHeaderArray, CapsuleCount, ScatterGatherList);
}



/**
	@brief          Hooked version of the UEFI QueryCapsuleCapabilities() runtime service.


	@param[in]      CapsuleHeaderArray      Array of capsule headers to evaluate.
	@param[in]      CapsuleCount            Number of entries in the CapsuleHeaderArray.
	@param[out]     MaximumCapsuleSize      Maximum size of a capsule supported in one boot.
	@param[out]     ResetType               Type of reset required after capsule processing.


	@retval         EFI_STATUS              Status returned by the original QueryCapsuleCapabilities().
**/
EFI_STATUS
HooksRuntimeServices_HookedQueryCapsuleCapabilities(
	IN              EFI_CAPSULE_HEADER      **CapsuleHeaderArray,
	IN              UINTN                   CapsuleCount,
	OUT             UINT64                  *MaximumCapsuleSize,
	OUT             EFI_RESET_TYPE          *ResetType
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Returns if the capsule can be supported via UpdateCapsule().
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#querycapsulecapabilities
	return Global_HooksRuntimeServices_OriginalQueryCapsuleCapabilities(CapsuleHeaderArray, CapsuleCount, MaximumCapsuleSize, ResetType);
}



/**
	@brief          Hooked version of the UEFI QueryVariableInfo() runtime service.


	@param[in]      Attributes                      Variable store attributes being queried.
	@param[out]     MaximumVariableStorageSize      Maximum size of the variable storage.
	@param[out]     RemainingVariableStorageSize    Remaining available space for variable storage.
	@param[out]     MaximumVariableSize             Maximum size allowed for a single variable.


	@retval         EFI_STATUS                      Status returned by the original QueryVariableInfo().
**/
EFI_STATUS
HooksRuntimeServices_HookedQueryVariableInfo(
	IN              UINT32                          Attributes,
	OUT             UINT64                          *MaximumVariableStorageSize,
	OUT             UINT64                          *RemainingVariableStorageSize,
	OUT             UINT64                          *MaximumVariableSize
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Returns information about the EFI variables.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#queryvariableinfo
	return Global_HooksRuntimeServices_OriginalQueryVariableInfo(Attributes, MaximumVariableStorageSize, RemainingVariableStorageSize, MaximumVariableSize);
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
HooksRuntimeServices_SwapService(
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



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
