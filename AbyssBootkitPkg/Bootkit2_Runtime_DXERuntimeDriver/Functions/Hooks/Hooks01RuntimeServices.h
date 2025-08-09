// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks Runtime Services
**/



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Structures/Structures01Execution.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Track boot process state, TRUE after SetVirtualAddressMap() is called
extern BOOLEAN                         Global_HooksRuntimeServices_VirtualMemoryActivated;

// Track boot process state, TRUE after ExitBootServices() is called
extern BOOLEAN                         Global_HooksRuntimeServices_ExitBootServicesReached;

// Event triggered when SetVirtualAddressMap() is called (used to convert physical to virtual pointers)
extern EFI_EVENT                       Global_HooksRuntimeServices_VirtualAddressChangeEvent;

// Event triggered when ExitBootServices() is called (useful for cleanup or debugging before OS takes over)
extern EFI_EVENT                       Global_HooksRuntimeServices_ExitBootServicesEvent;

// Runtime Services
extern EFI_GET_TIME                    Global_HooksRuntimeServices_OriginalGetTime;
extern EFI_SET_TIME                    Global_HooksRuntimeServices_OriginalSetTime;
extern EFI_GET_WAKEUP_TIME             Global_HooksRuntimeServices_OriginalGetWakeupTime;
extern EFI_SET_WAKEUP_TIME             Global_HooksRuntimeServices_OriginalSetWakeupTime;
extern EFI_GET_VARIABLE                Global_HooksRuntimeServices_OriginalGetVariable;
extern EFI_GET_NEXT_VARIABLE_NAME      Global_HooksRuntimeServices_OriginalGetNextVariableName;
extern EFI_SET_VARIABLE                Global_HooksRuntimeServices_OriginalSetVariable;
extern EFI_RESET_SYSTEM                Global_HooksRuntimeServices_OriginalResetSystem;
extern EFI_CONVERT_POINTER             Global_HooksRuntimeServices_OriginalConvertPointer;
extern EFI_SET_VIRTUAL_ADDRESS_MAP     Global_HooksRuntimeServices_OriginalSetVirtualAddressMap;
extern EFI_GET_NEXT_HIGH_MONO_COUNT    Global_HooksRuntimeServices_OriginalGetNextHighMonotonicCount;
extern EFI_UPDATE_CAPSULE              Global_HooksRuntimeServices_OriginalUpdateCapsule;
extern EFI_QUERY_CAPSULE_CAPABILITIES  Global_HooksRuntimeServices_OriginalQueryCapsuleCapabilities;
extern EFI_QUERY_VARIABLE_INFO         Global_HooksRuntimeServices_OriginalQueryVariableInfo;



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
);



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
);



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
);



/**
	@brief          Hooked version of the UEFI SetTime() runtime service.


	@param[in]      Time                    Pointer to the time structure to set.


	@retval         EFI_STATUS              Status returned by the original SetTime().
**/
EFI_STATUS
HooksRuntimeServices_HookedSetTime(
	IN              EFI_TIME                *Time
);



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
);



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
);



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
);



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
);



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
);



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
);



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
);



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
);



/**
	@brief          Hooked version of the UEFI GetNextHighMonotonicCount() runtime service.


	@param[out]     HighCount              Pointer that will receive the high 32 bits of the monotonic counter.


	@retval         EFI_STATUS             Status returned by the original GetNextHighMonotonicCount().
**/
EFI_STATUS
HooksRuntimeServices_HookedGetNextHighMonotonicCount(
	OUT             UINT32                 *HighCount
);



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
);



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
);



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
);



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
