// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Files
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Writes binary data to a file.
	@details        This function creates or overwrites a file specified by the FilePath and writes the provided binary data to it. The file is created if it does not exist or replaced if it already exists.


	@param[in]      FilePath            The full path to the file where the binary data will be written.
	@param[in]      Data                A pointer to the binary data to be written to the file.
	@param[in]      DataSize            The size of the binary data to be written, in bytes.


	@retval         EFI_SUCCESS         The data was successfully written to the file.
	@retval         EFI_ABORTED         The operation was aborted due to an error.
**/
EFI_STATUS
PreBootSetup_UtilsFiles_WriteBinaryFile(
	IN              CHAR16              *FilePath,
	IN              CONST VOID          *Data,
	IN              UINTN               DataSize
);



/**
	@brief          Loads an EFI binary image file (UEFI Driver, DXE Runtime Driver, ...) into memory and prepares it for execution.


	@param[in]      FilePath                    The path to the image file that should be loaded into memory.
	@param[out]     FileImageHandle             A pointer to an EFI_HANDLE variable that will receive the handle of the newly loaded image (this handle can be used to start the image if necessary).


	@retval         EFI_SUCCESS                 The image was successfully loaded into memory.
	@retval         EFI_ABORTED                 The operation was aborted due to an error.
	@retval         EFI_OUT_OF_RESOURCES        Failed to create a device path due to insufficient resources.
**/
EFI_STATUS
PreBootSetup_UtilsFiles_LoadImageFileIntoMemory(
	IN              CHAR16              *FilePath,
	OUT             EFI_HANDLE          *FileImageHandle
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
