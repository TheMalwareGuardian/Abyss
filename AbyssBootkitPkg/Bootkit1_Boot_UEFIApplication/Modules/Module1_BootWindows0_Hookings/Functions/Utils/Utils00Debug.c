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

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h
// Provides services to print a formatted string to a buffer. All combinations of Unicode and ASCII strings are supported.
#include <Library/PrintLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>
#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils01Files.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils00Debug.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Boot limit
BOOLEAN Global_BootWindowsHookings_UtilsDebug_BootFinishesRuntimeStarts = FALSE;

// Buffer
CHAR16 *Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBuffer;
UINTN Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferIndex;
UINTN Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferSize;



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Logs and debugs messages during boot and runtime phases in shared functions.
	@details        As the UEFI Application has been developed to include functions that operate across both the boot and runtime phases (Memory, Pattern, ...), this function manages the logging and debugging of messages accordingly.During the runtime phase, where only debug logging is available, the function converts the message to ASCII and logs it using the debug mechanism. During the boot phase, the function checks the provided debug level against globally configured levels and logs the message using the appropriate method, whether it be debug output, console printing, or file logging.


	@param[in]      Level                       The debug level of the message (INFO, WARN, ERROR, CRITICAL).
	@param[in]      Message                     A message string to log.


	@retval         None
**/
VOID
PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(
	IN              CONST CHAR16*               Level,
	IN              CONST CHAR16*               Message
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Runtime
	if (Global_BootWindowsHookings_UtilsDebug_BootFinishesRuntimeStarts)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Only debug logging is available in runtime phase
		if (Global_PreBootConfiguration_UtilsDebug_LoggingEnabledDebug)
		{
			// Variables
			CHAR8 MessageAscii[1024];

			// Convert CHAR16 to CHAR8 for debugging

			// Convert a Null-terminated Unicode string to a Null-terminated ASCII string.
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/SafeString.c#L2650
			UnicodeStrToAsciiStrS(Message, MessageAscii, sizeof(MessageAscii));

			// Output debug message
			// https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Debugging
			DEBUG((DEBUG_INFO, "%a", MessageAscii));
		}
	}
	// ---------------------------------------------------------------------------------------------------------------------
	// Boot
	else
	{
		// Check if the Level matches any of the globally configured debug levels
		if ((StrCmp(Level, MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO) == 0 && Global_PreBootConfiguration_UtilsDebug_DebugInfo) ||
			(StrCmp(Level, MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN) == 0 && Global_PreBootConfiguration_UtilsDebug_DebugWarn) ||
			(StrCmp(Level, MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR) == 0 && Global_PreBootConfiguration_UtilsDebug_DebugError) ||
			(StrCmp(Level, MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELCRITICAL) == 0 && Global_PreBootConfiguration_UtilsDebug_DebugCritical))
		{
			// -------------------------------------------------------------------------------------------------------------
			// Debug output
			if (Global_PreBootConfiguration_UtilsDebug_LoggingEnabledDebug)
			{
				// Variables
				CHAR8 MessageAscii[1024];

				// Convert CHAR16 to CHAR8 for debugging

				// Convert a Null-terminated Unicode string to a Null-terminated ASCII string.
				// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/SafeString.c#L2650
				UnicodeStrToAsciiStrS(Message, MessageAscii, sizeof(MessageAscii));

				// Output debug message
				// https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Debugging
				DEBUG((DEBUG_INFO, "%a", MessageAscii));
			}

			// -------------------------------------------------------------------------------------------------------------
			// Console printing
			if (Global_PreBootConfiguration_UtilsDebug_LoggingEnabledConsole)
			{
				Print(L"%s", Message);
			}

			// -------------------------------------------------------------------------------------------------------------
			// File logging
			if (Global_PreBootConfiguration_UtilsDebug_LoggingEnabledFile && Global_PreBootConfiguration_UtilsDebug_LoggingFilePath != NULL)
			{
				PreBootConfiguration_UtilsFiles_AppendToLogFile(Global_PreBootConfiguration_UtilsDebug_LoggingFilePath, Message);
			}
		}
	}
}



/**
	@brief          Initializes a buffer used for deferred debug logging.
	@details        Allocates a zero-initialized CHAR16 buffer in memory to store log messages when Print() or other Boot Services are unavailable. The buffer contents are printed later from the ExitBootServices event handler for debugging purposes.


	@param[in]      BufferSize                  Size of the buffer in CHAR16 characters, including null terminators.


	@retval         EFI_SUCCESS                 The buffer was successfully allocated and initialized.
	@retval         EFI_OUT_OF_RESOURCES        Memory allocation failed.
**/
EFI_STATUS
BootWindowsHookings_UtilsDebug_InitializeEdgeBuffer(
	IN              UINTN                       BufferSize
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Allocate memory for the buffer
	Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBuffer = AllocateZeroPool(BufferSize * sizeof(CHAR16));

	if (Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBuffer == NULL)
	{
		return EFI_OUT_OF_RESOURCES;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Initialize buffer index and size
	Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferIndex = 0;
	Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferSize = BufferSize;


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Appends a debug message to the edge buffer.
	@details        Copies a null-terminated CHAR16 string to the internal debug buffer used to collect messages when standard output is unavailable (e.g., late boot hook execution).


	@param[in]      Message                     Null-terminated CHAR16 string to append to the debug buffer.


	@retval         EFI_SUCCESS                 The message was successfully appended.
	@retval         EFI_BUFFER_TOO_SMALL        Not enough space left in the buffer to store the message.
**/
EFI_STATUS
BootWindowsHookings_UtilsDebug_AppendToEdgeBuffer(
	IN              CONST CHAR16*               Message
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	UINTN MessageLength = StrLen(Message);


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if there is enough space in the buffer
	if (Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferIndex + MessageLength >= Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferSize)
	{
		return EFI_BUFFER_TOO_SMALL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Copy the message to the buffer

	// Copies the string pointed to by Source (including the terminating null char) to the array pointed to by Destination.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/SafeString.c#L226
	StrCpyS(&Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBuffer[Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferIndex], Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferSize - Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferIndex, Message);


	// ---------------------------------------------------------------------------------------------------------------------
	// Update the buffer index
	Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferIndex += MessageLength;


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
