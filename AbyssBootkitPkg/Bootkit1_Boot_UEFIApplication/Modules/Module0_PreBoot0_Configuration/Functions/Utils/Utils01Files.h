// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Files
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Creates a log file at the specified path.


	@param[in]      FilePath                    The path where the file will be created.


	@retval         EFI_SUCCESS                 The file was created successfully.
	@retval         EFI_ABORTED                 Failed to create the file.
**/
EFI_STATUS
PreBootConfiguration_UtilsFiles_CreateLogFile(
	IN              CHAR16                      *FilePath
);



/**
	@brief          Writes a message to the log file.


	@param[in]      FilePath                    The path to the log file.
	@param[in]      Message                     The message to write to the log file.


	@retval         EFI_SUCCESS                 The message was written to the log file successfully.
	@retval         EFI_ABORTED                 Failed to write the message to the log file.
	@retval         EFI_OUT_OF_RESOURCES        Not enough resources to allocate memory.
**/
EFI_STATUS
PreBootConfiguration_UtilsFiles_AppendToLogFile(
	IN              CHAR16                      *FilePath,
	IN              CONST CHAR16                *Message
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
