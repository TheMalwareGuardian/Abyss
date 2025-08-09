// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Debug
**/



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Boot limit
extern BOOLEAN Global_BootWindowsHookings_UtilsDebug_BootFinishesRuntimeStarts;

// Buffer
extern CHAR16 *Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBuffer;
extern UINTN Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferIndex;
extern UINTN Global_BootWindowsHookings_UtilsDebug_SaveHookingInfoBufferSize;



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
);



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
);



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
