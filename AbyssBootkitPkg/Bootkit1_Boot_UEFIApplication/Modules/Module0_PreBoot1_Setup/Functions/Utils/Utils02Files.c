// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Files
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Boot Services Table. Only available to DXE and UEFI module types.
#include <Library/UefiBootServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/ShellPkg/Include/Library/ShellLib.h
// Provides interface to shell functionality for shell commands and applications.
#include <Library/ShellLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>



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
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
	EFI_FILE_HANDLE Root;
	EFI_FILE_HANDLE File;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the LoadedImage protocol for the current image
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - Get the LoadedImage protocol to obtain the device path\r\n");

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h#L79
	Status = gBS->HandleProtocol(gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - gBS->HandleProtocol [gEfiLoadedImageProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the Simple File System Protocol of the device we're on
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - Get the Simple File System Protocol\r\n");

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/SimpleFileSystem.h#L551
	Status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&SimpleFileSystem);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - gBS->HandleProtocol [gEfiSimpleFileSystemProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Open the volume to get the root directory of the ESP
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - Open the volume to get the root directory\r\n");

	// Opens the root directory on a volume.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html?highlight=openvolume#efi-simple-file-system-protocol-openvolume
	Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &Root);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - SimpleFileSystem->OpenVolume failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Create the file (if it already exists, it will be overwritten)
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - Create the file\r\n");

	// Opens a new file relative to the source file’s location.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-open
	Status = Root->Open(Root, &File, FilePath, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		Root->Close(Root);

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - Root->Open failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Set the file pointer to the beginning of the file

	// Sets a file’s current position.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-setposition
	Status = File->SetPosition(File, 0);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		File->Close(File);
		Root->Close(Root);

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - File->SetPosition failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Write binary data to the file
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - Write binary data to the file\r\n");

	// Writes data to a file.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-write
	Status = File->Write(File, &DataSize, (VOID*)Data);
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		File->Close(File);
		Root->Close(Root);

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - File->Write failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (WriteBinaryFile) - Bye\r\n");

	// Closes a specified file handle.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
	File->Close(File);
	Root->Close(Root);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



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
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (LoadImageFileIntoMemory) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_STATUS CommandStatus = EFI_SUCCESS;
	CHAR16 *EnvironmentVars[] = { NULL };
	CHAR16 CommandLine[256];


	// ---------------------------------------------------------------------------------------------------------------------
	// Build the load FilePath command line

	// Produces a Null-terminated Unicode string in an output buffer based on a Null-terminated Unicode format string and variable argument list.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h#L338
	UnicodeSPrint(CommandLine, sizeof(CommandLine), L"load %s", FilePath);


	// ---------------------------------------------------------------------------------------------------------------------
	// Execute the load command via the UEFI Shell
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (LoadImageFileIntoMemory) - Execute shell command: %s\r\n", CommandLine);

	// Cause the shell to parse and execute a command line.
	// https://github.com/tianocore/edk2/blob/master/ShellPkg/Library/UefiShellLib/UefiShellLib.c#L1313
	Status = ShellExecute(&gImageHandle, CommandLine, TRUE, EnvironmentVars, &CommandStatus);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (LoadImageFileIntoMemory) - ShellExecute failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}

	// Command execution failed
	if (EFI_ERROR(CommandStatus))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (LoadImageFileIntoMemory) - load command failed: %r (%llx).\r\n", CommandStatus, CommandStatus);
		return CommandStatus;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Files.c] (LoadImageFileIntoMemory) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
