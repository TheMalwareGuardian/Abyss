// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Ntfs
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Remaps partitions by executing the "map -r" command.
	@details        This function utilizes the UEFI Shell to remap all available partitions, ensuring they are accessible for subsequent operations.


	@retval         EFI_SUCCESS         The partitions were successfully remapped.
	@retval         EFI_ABORTED         An error occurred while executing the remap command.
**/
EFI_STATUS
PreBootSetup_UtilsNtfs_RemapPartitions();



/**
	@brief          Lists the contents of the first 5 mapped partitions (fs0: to fs4:).
	@details        This function iterates over the initial fs-mapped partitions (fs0: to fs4:), executing the "ls" command on each to retrieve a list of files and directories.


	@retval         EFI_SUCCESS         The contents of all partitions were successfully listed.
	@retval         EFI_ABORTED         An error occurred while listing the contents of a partition.
**/
EFI_STATUS
PreBootSetup_UtilsNtfs_ListPartitions();



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
