// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Decrypt
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



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Decrypts an encrypted data buffer using AES in CBC mode.
	@details        This function decrypts the provided encrypted data buffer using the AES algorithm in CBC mode. The decryption process uses the provided key and initialization vector (IV) to produce the decrypted data.


	@param[in]      encrypted_data              Pointer to the buffer containing the encrypted data.
	@param[in]      encrypted_data_size         Size of the encrypted data buffer in bytes.
	@param[in]      key                         Pointer to the buffer containing the AES decryption key.
	@param[in]      iv                          Pointer to the buffer containing the initialization vector.
	@param[out]     decrypted_data              Pointer to the buffer that will receive the decrypted data.
	@param[out]     decrypted_data_size         Pointer to the variable that will receive the size of the decrypted data buffer.


	@retval         EFI_SUCCESS                 The data was successfully decrypted.
	@retval         EFI_OUT_OF_RESOURCES        There was not enough memory to allocate the decrypted data buffer.
	@retval         EFI_ABORTED                 The decryption process failed.
**/
EFI_STATUS
PreBootConfiguration_UtilsDecrypt_DecryptConfiguration(
	IN              UINT8                       *encrypted_data,
	IN              UINTN                       encrypted_data_size,
	IN              UINT8                       *key,
	IN              UINT8                       *iv,
	OUT             UINT8                       **decrypted_data,
	OUT             UINTN                       *decrypted_data_size
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	VOID *AesContext;
	UINTN BlockSize;
	UINT8 *Buffer;
	UINTN BufferSize;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the size of the AES context structure
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - Get the size of the AES context structure\r\n");

	// Retrieves the size, in bytes, of the context buffer required for AES operations.
	// https://github.com/tianocore/edk2/blob/master/CryptoPkg/Library/BaseCryptLib/Cipher/CryptAes.c#L20
	BlockSize = AesGetContextSize();


	// ---------------------------------------------------------------------------------------------------------------------
	// Allocate memory for the AES context
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - Allocate memory for the AES context\r\n");

	// Allocates pool memory.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
	AesContext = AllocatePool(BlockSize);

	// Failed
	if (AesContext == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - AllocatePool [BlockSize] failed.\r\n");
		return EFI_OUT_OF_RESOURCES;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Initialize the AES context with the provided key and a key size of 256 bits
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - Allocate memory for the AES context\r\n");

	// Initializes user-supplied memory as AES context for subsequent use.
	// https://github.com/tianocore/edk2/blob/master/CryptoPkg/Library/BaseCryptLib/Cipher/CryptAes.c#L53
	if (!AesInit(AesContext, key, 256))
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(AesContext);

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - AesInit failed.\r\n");
		return EFI_ABORTED;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Allocate memory for the buffer to hold the decrypted data
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - Allocate memory for the buffer to hold the decrypted data\r\n");

	// Allocate memory for the buffer to hold the decrypted data
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - Allocate memory for the buffer to hold the decrypted data\r\n");

	// Allocates pool memory.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
	BufferSize = encrypted_data_size;
	Buffer = AllocatePool(BufferSize + 1);

	// Failed
	if (Buffer == NULL)
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(AesContext);

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - AllocatePool [BufferSize] failed.\r\n");
		return EFI_OUT_OF_RESOURCES;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Perform AES decryption on the encrypted data using CBC mode
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - Perform AES decryption on the encrypted data using CBC mode\r\n");

	// Performs AES decryption on a data buffer of the specified size in CBC mode.
	// https://github.com/tianocore/edk2/blob/master/CryptoPkg/Library/BaseCryptLib/Cipher/CryptAes.c#L174
	if (!AesCbcDecrypt(AesContext, encrypted_data, encrypted_data_size, iv, Buffer))
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(Buffer);
		FreePool(AesContext);

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - AesCbcDecrypt failed.\r\n");
		return EFI_ABORTED;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Calculate the actual size of the decrypted data
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - Calculate the actual size of the decrypted data\r\n");

	// Get the padding value (last byte of the decrypted data)
	UINT8 padding_value = Buffer[BufferSize - 1];

	// Check if padding is valid
	if (padding_value > 0 && padding_value <= 16)
	{

		// Verify padding
		BOOLEAN valid_padding = TRUE;
		for (UINTN i = 0; i < padding_value; i++)
		{
			if (Buffer[BufferSize - 1 - i] != padding_value)
			{
				valid_padding = FALSE;
				break;
			}
		}

		// If padding is valid, adjust the decrypted data size
		if (valid_padding)
		{
			*decrypted_data_size = BufferSize - padding_value;
		}
		// If padding is not valid, use the full buffer size
		else
		{
			*decrypted_data_size = BufferSize;
		}
	}
	// If padding value is not valid, use the full buffer size
	else
	{
		*decrypted_data_size = BufferSize;
	}

	// Ensure null termination
	Buffer[*decrypted_data_size] = '\0';

	// Set the output parameter to the decrypted data
	*decrypted_data = Buffer;


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Decrypt.c] (DecryptConfiguration) - Bye\r\n");

	// Returns pool memory to the system.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
	FreePool(AesContext);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
