// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Debug
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------


// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
// Provides copy memory, fill memory, zero memory, and GUID functions. The Base Memory Library provides optimized implementations for common memory-based operations. These functions should be used in place of coding your own loops to do equivalent common functions. This allows optimized library implementations to help increase performance.
#include <Library/BaseMemoryLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/MemoryAllocationLib.h
// Provides services to allocate and free memory buffers of various memory types and alignments. The Memory Allocation Library abstracts various common memory allocation operations. This library allows code to be written in a phase-independent manner because the allocation of memory in PEI, DXE, and SMM (for example) is done via a different mechanism. Using a common library interface makes it much easier to port algorithms from phase to phase.
#include <Library/MemoryAllocationLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>
#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils01Files.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Global variables to enable specific debug messages
BOOLEAN Global_PreBootConfiguration_UtilsDebug_DebugInfo = FALSE;
BOOLEAN Global_PreBootConfiguration_UtilsDebug_DebugWarn = FALSE;
BOOLEAN Global_PreBootConfiguration_UtilsDebug_DebugError = FALSE;
BOOLEAN Global_PreBootConfiguration_UtilsDebug_DebugCritical = FALSE;

// Global variables to enable logging
BOOLEAN Global_PreBootConfiguration_UtilsDebug_LoggingEnabledDebug = FALSE;
BOOLEAN Global_PreBootConfiguration_UtilsDebug_LoggingEnabledConsole = FALSE;
BOOLEAN Global_PreBootConfiguration_UtilsDebug_LoggingEnabledFile = FALSE;
CHAR16 *Global_PreBootConfiguration_UtilsDebug_LoggingFilePath = NULL;



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Prints a formatted message to the console if console logging is enabled.
	@details        This function formats a message string using a variable argument list and prints it to the console. The function first checks if console logging is enabled via the 'Global_PreBootConfiguration_UtilsDebug_LoggingEnabledConsole' flag. If console logging is enabled, the function processes the provided format string and arguments to produce a formatted message, which it then prints to the console.


	@param[in]      Format                          A null-terminated Unicode format string.
	@param          ...                             A variable argument list whose contents correspond to the format string.


	@retval         None
**/
VOID
PreBootConfiguration_UtilsDebug_Print(
	IN              CONST CHAR16*                   Format,
	...
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Logging to console
	if (Global_PreBootConfiguration_UtilsDebug_LoggingEnabledConsole)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Variables
		CHAR16 Message[1024];
		VA_LIST Marker;


		// -----------------------------------------------------------------------------------------------------------------
		// Retrieves a pointer to the beginning of a variable argument list, based on the name of the parameter that immediately precedes the variable argument list.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Base.h#L661
		VA_START(Marker, Format);


		// -----------------------------------------------------------------------------------------------------------------
		// Produces a Null-terminated Unicode string in an output buffer based on a Null-terminated Unicode format string and a VA_LIST argument list.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h#L242
		UnicodeVSPrint(Message, sizeof(Message), Format, Marker);


		// -----------------------------------------------------------------------------------------------------------------
		// Terminates the use of a variable argument list.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Base.h#L691
		VA_END(Marker);


		// -----------------------------------------------------------------------------------------------------------------
		// Print message
		Print(L"%s", Message);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
}



/**
	@brief          Set the logging configuration and create a log file if logging is enabled.
	@details       This function sets the global logging flags for different debug levels (Info, Warn, Error, Critical) and enables or disables logging to debug output, console, and file. If file logging is enabled and a valid file path is provided, the function also creates the log file.


	@param[in]      EnableInfo                      TRUE to enable INFO level, FALSE to disable.
	@param[in]      EnableWarn                      TRUE to enable WARN level, FALSE to disable.
	@param[in]      EnableError                     TRUE to enable ERROR level, FALSE to disable.
	@param[in]      EnableCritical                  TRUE to enable CRITICAL level, FALSE to disable.
	@param[in]      LoggingEnabledDebug             TRUE to enable Debug logging, FALSE to disable.
	@param[in]      LoggingEnabledConsole           TRUE to enable Console logging, FALSE to disable.
	@param[in]      LoggingEnabledFile              TRUE to enable File logging, FALSE to disable.
	@param[in]      LoggingFilePath                 The path to the log file in CHAR16 format. Set to NULL to clear the file path.


	@retval         EFI_SUCCESS                     The configuration was set successfully, and the log file was created if enabled.
	@retval         EFI_ABORTED                     Failed to create the log file or invalid parameters.
**/
EFI_STATUS
PreBootConfiguration_UtilsDebug_SetLogging(
	IN              BOOLEAN                         EnableInfo,
	IN              BOOLEAN                         EnableWarn,
	IN              BOOLEAN                         EnableError,
	IN              BOOLEAN                         EnableCritical,
	IN              BOOLEAN                         LoggingEnabledDebug,
	IN              BOOLEAN                         LoggingEnabledConsole,
	IN              BOOLEAN                         LoggingEnabledFile,
	IN              CHAR16                          *LoggingFilePath
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Set the global logging enabled flags
	Global_PreBootConfiguration_UtilsDebug_DebugInfo = EnableInfo;
	Global_PreBootConfiguration_UtilsDebug_DebugWarn = EnableWarn;
	Global_PreBootConfiguration_UtilsDebug_DebugError = EnableError;
	Global_PreBootConfiguration_UtilsDebug_DebugCritical = EnableCritical;
	Global_PreBootConfiguration_UtilsDebug_LoggingEnabledDebug = LoggingEnabledDebug;
	Global_PreBootConfiguration_UtilsDebug_LoggingEnabledConsole = LoggingEnabledConsole;
	Global_PreBootConfiguration_UtilsDebug_LoggingEnabledFile = LoggingEnabledFile;


	// ---------------------------------------------------------------------------------------------------------------------
	// Update the global file path if a valid path is provided
	if (LoggingFilePath != NULL)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Allocate and copy the new file path

		// Copies a buffer to an allocated buffer of type EfiBootServicesData.s
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L616
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L72
		Global_PreBootConfiguration_UtilsDebug_LoggingFilePath = AllocateCopyPool(StrSize(LoggingFilePath), LoggingFilePath);
		if (Global_PreBootConfiguration_UtilsDebug_LoggingFilePath == NULL)
		{
			// Failed to allocate memory for file path
			PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Debug.c] (SetLogging) - AllocateCopyPool failed.\r\n");
			return EFI_ABORTED;
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// If logging is enabled and a valid file path is set, create the log file
	if (Global_PreBootConfiguration_UtilsDebug_LoggingEnabledFile && Global_PreBootConfiguration_UtilsDebug_LoggingFilePath != NULL)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Create log file
		EFI_STATUS Status = PreBootConfiguration_UtilsFiles_CreateLogFile(Global_PreBootConfiguration_UtilsDebug_LoggingFilePath);
		if (EFI_ERROR(Status))
		{
			PreBootConfiguration_UtilsDebug_Print(L"[ERROR]	AbyssBootkit1 -> PreBootConfiguration [Utils/Debug.c] (SetLogging) - PreBootConfiguration_UtilsFiles_CreateLogFile failed: %r (%llx).\r\n", Status, Status);

			// File creation operation status
			return Status;
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Logs, prints, and debugs messages based on the provided debug level.
	@details        This function checks the provided `Level` against globally configured debug levels and, if there's a match, it formats the message and optionally outputs it to the debug console, standard console, or a log file depending on the global settings.


	@param[in]      Level                           The debug level of the message (INFO, WARN, ERROR, CRITICAL).
	@param[in]      Format                          A format string followed by arguments (similar to printf).


	@retval         None
**/
VOID
PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(
	IN              CONST CHAR16*                   Level,
	IN              CONST CHAR16*                   Format,
	...
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the Level matches any of the globally configured debug levels
	if ((StrCmp(Level, MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO) == 0 && Global_PreBootConfiguration_UtilsDebug_DebugInfo) ||
		(StrCmp(Level, MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN) == 0 && Global_PreBootConfiguration_UtilsDebug_DebugWarn) ||
		(StrCmp(Level, MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR) == 0 && Global_PreBootConfiguration_UtilsDebug_DebugError) ||
		(StrCmp(Level, MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELCRITICAL) == 0 && Global_PreBootConfiguration_UtilsDebug_DebugCritical))
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Variables
		CHAR16 MessageUser[1024];
		CHAR16 MessageFinal[1048];
		CHAR8 MessageFinalAscii[1048];
		VA_LIST Marker;


		// -----------------------------------------------------------------------------------------------------------------
		// Retrieves a pointer to the beginning of a variable argument list, based on the name of the parameter that immediately precedes the variable argument list.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Base.h#L661
		VA_START(Marker, Format);


		// -----------------------------------------------------------------------------------------------------------------
		// Produces a Null-terminated Unicode string in an output buffer based on a Null-terminated Unicode format string and a VA_LIST argument list.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h#L242
		UnicodeVSPrint(MessageUser, sizeof(MessageUser), Format, Marker);


		// -----------------------------------------------------------------------------------------------------------------
		// Terminates the use of a variable argument list.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Base.h#L691
		VA_END(Marker);


		// -----------------------------------------------------------------------------------------------------------------
		// Produces a Null-terminated Unicode string in an output buffer based on a Null-terminated Unicode format string and variable argument list.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h#L338
		UnicodeSPrint (MessageFinal, sizeof(MessageFinal), L"[%s] %s", Level, MessageUser);


		// -----------------------------------------------------------------------------------------------------------------
		// Debug output
		if (Global_PreBootConfiguration_UtilsDebug_LoggingEnabledDebug)
		{
			// -------------------------------------------------------------------------------------------------------------
			// Convert CHAR16 to CHAR8 for debugging

			// Convert a Null-terminated Unicode string to a Null-terminated ASCII string.
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/SafeString.c#L2650
			UnicodeStrToAsciiStrS(MessageFinal, MessageFinalAscii, sizeof(MessageFinalAscii));

			// Output debug message
			// https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Debugging
			DEBUG((DEBUG_INFO, "%a", MessageFinalAscii));
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Console printing
		if (Global_PreBootConfiguration_UtilsDebug_LoggingEnabledConsole)
		{
			Print(L"%s", MessageFinal);
		}


		// -----------------------------------------------------------------------------------------------------------------
		// File logging
		if (Global_PreBootConfiguration_UtilsDebug_LoggingEnabledFile && Global_PreBootConfiguration_UtilsDebug_LoggingFilePath != NULL)
		{
			PreBootConfiguration_UtilsFiles_AppendToLogFile(Global_PreBootConfiguration_UtilsDebug_LoggingFilePath, MessageFinal);
		}
	}
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
