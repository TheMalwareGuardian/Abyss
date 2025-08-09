// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Debug
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h
// Provides services to print a formatted string to a buffer. All combinations of Unicode and ASCII strings are supported.
#include <Library/PrintLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DebugLib.h
// Provides services to print debug and assert messages to a debug output device.
#include <Library/DebugLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils01Files.h>



// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------
// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------



// Define different debug levels
#define MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO			L"INFO"
#define MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN			L"WARN"
#define MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR		L"ERROR"
#define MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELCRITICAL		L"CRITICAL"



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Global variables to enable specific debug messages
extern BOOLEAN Global_PreBootConfiguration_UtilsDebug_DebugInfo;
extern BOOLEAN Global_PreBootConfiguration_UtilsDebug_DebugWarn;
extern BOOLEAN Global_PreBootConfiguration_UtilsDebug_DebugError;
extern BOOLEAN Global_PreBootConfiguration_UtilsDebug_DebugCritical;

// Global variables to enable logging
extern BOOLEAN Global_PreBootConfiguration_UtilsDebug_LoggingEnabledDebug;
extern BOOLEAN Global_PreBootConfiguration_UtilsDebug_LoggingEnabledConsole;
extern BOOLEAN Global_PreBootConfiguration_UtilsDebug_LoggingEnabledFile;
extern CHAR16 *Global_PreBootConfiguration_UtilsDebug_LoggingFilePath;



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
);



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
);



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
