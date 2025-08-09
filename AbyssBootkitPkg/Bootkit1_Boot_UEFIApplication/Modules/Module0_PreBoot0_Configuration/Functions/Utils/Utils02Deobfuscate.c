// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Deobfuscate
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



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Reverse the XOR and permutation obfuscation on a key.


	@param[in]      obfuscated_key              The obfuscated key to deobfuscate.
	@param[in]      xor_key                     The XOR key used for obfuscation.
	@param[in]      permutation                 The permutation array used for shuffling.
	@param[in]      key_size                    The size of the key.
	@param[out]     key                         The resulting deobfuscated key.


	@retval         EFI_SUCCESS                 The key was successfully deobfuscated.
	@retval         EFI_INVALID_PARAMETER       One or more parameters are invalid.
	@retval         EFI_OUT_OF_RESOURCES        Memory allocation failed.
**/
EFI_STATUS
PreBootConfiguration_UtilsDeobfuscate_DeobfuscateKeyXorPermutation(
	IN              const UINT8                 *obfuscated_key,
	IN              const UINT8                 xor_key,
	IN              const UINT8                 *permutation,
	IN              const UINTN                 key_size,
	OUT             UINT8                       **key
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateKeyXorPermutation) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if input parameters are valid
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateKeyXorPermutation) - Check if input parameters are valid\r\n");

	// Failed
	if (obfuscated_key == NULL || permutation == NULL || key == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateKeyXorPermutation) - Operation failed: Some input parameter is invalid.\r\n");
		return EFI_INVALID_PARAMETER;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Allocate memory for the deobfuscated key
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateKeyXorPermutation) - Allocate memory for the deobfuscated key\r\n");

	// Allocates pool memory.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
	*key = AllocatePool(key_size);

	// Failed
	if (*key == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateKeyXorPermutation) - AllocatePool [key_size] failed.\r\n");
		return EFI_OUT_OF_RESOURCES;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Allocate temporary memory for reversing the permutation
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateKeyXorPermutation) - Allocate temporary memory for reversing the permutation\r\n");

	// Allocates pool memory.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
	UINT8 *temp_key = AllocatePool(key_size);

	// Failed
	if (temp_key == NULL)
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(*key);

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateKeyXorPermutation) - AllocatePool [temp key_size] failed.\r\n");
		return EFI_OUT_OF_RESOURCES;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Reverse the permutation
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateKeyXorPermutation) - Reverse the permutation\r\n");

	// Apply reverse permutation
	for (UINTN i = 0; i < key_size; i++)
	{
		temp_key[permutation[i]] = obfuscated_key[i];
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// XOR deobfuscation
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateKeyXorPermutation) - XOR deobfuscation\r\n");

	// Apply XOR deobfuscation
	for (UINTN i = 0; i < key_size; i++)
	{
		(*key)[i] = temp_key[i] ^ xor_key;
	}

	// Returns pool memory to the system.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
	FreePool(temp_key);


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateKeyXorPermutation) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Reverse the Caesar cipher (character rotation).


	@param[in]      input                       The input string to reverse the Caesar cipher.
	@param[in]      length				        The length of the input string.
	@param[in]      shift				        The value of the Caesar cipher shift.


	@retval         None
**/
VOID
PreBootConfiguration_UtilsDeobfuscate_ReverseCaesarCipher(
	IN              UINT8                       *input,
	IN              UINTN                       length,
	IN              INTN                        shift
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (ReverseCaesarCipher) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Reverse Caesar Cipher
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (ReverseCaesarCipher) - Reverse Caesar Cipher\r\n");

	// Apply reverse
	for (UINTN i = 0; i < length; i++)
	{
		input[i] = (input[i] - shift + 256) % 256;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (ReverseCaesarCipher) - Bye\r\n");
}



/**
	@brief          Deobfuscate an encoded string with XOR and Caesar cipher.


	@param[in]      input_string                The Base64 encoded string to deobfuscate.
	@param[in]      xor_key                     The XOR key used for obfuscation.
	@param[in]      caesar_shift                The Caesar cipher shift value used for obfuscation.
	@param[out]     output_string               A pointer to the buffer that will hold the deobfuscated string.
	@param[out]     output_size                 A pointer to the size of the deobfuscated data.


	@retval         EFI_SUCCESS                 The string was successfully deobfuscated.
	@retval         EFI_OUT_OF_RESOURCES        Memory allocation failed.
**/
EFI_STATUS
PreBootConfiguration_UtilsDeobfuscate_DeobfuscateStringXorCaesar(
	IN              const CHAR8                 *input_string,
	IN              const UINT8                 xor_key,
	IN              const INTN                  caesar_shift,
	OUT             UINT8                       **output_data,
	OUT             UINTN                       *output_size
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	UINT8 *decoded_data = NULL;
	UINTN decoded_size = 0;
	EFI_STATUS Status;


	// ---------------------------------------------------------------------------------------------------------------------
	// Base64 decode the input string
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Base64 decode the input string\r\n");


	// Step 1: Determine the size of the decoded data
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Step 1: Determine the size of the decoded data\r\n");

	// Decode Base64 ASCII encoded data to 8-bit binary representation, based on RFC4648.
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Decode Base64 ASCII encoded data to 8-bit binary representation");

	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L1363
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L633
	Status = Base64Decode(input_string, AsciiStrLen(input_string), NULL, &decoded_size);

	// Failed
	if (Status != RETURN_BUFFER_TOO_SMALL && EFI_ERROR(Status))
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Base64Decode [input_string] failed.\r\n");
		return Status;
	}


	// Step 2: Allocate memory for the decoded data
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Step 2: Allocate memory for the decoded data\r\n");

	// Allocates pool memory.
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Allocates pool memory\r\n");

	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
	decoded_data = AllocatePool(decoded_size);

	// Failed
	if (decoded_data == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - AllocatePool [decoded_size] failed.\r\n");
		return EFI_OUT_OF_RESOURCES;
	}


	// Step 3: Perform the actual Base64 decoding
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Step 3: Perform the actual Base64 decoding\r\n");

	// Decode Base64 ASCII encoded data to 8-bit binary representation, based on RFC4648.
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Decode Base64 ASCII encoded data to 8-bit binary representation\r\n");

	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L1363
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L633
	Status = Base64Decode(input_string, AsciiStrLen(input_string), decoded_data, &decoded_size);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
		FreePool(decoded_data);

		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Base64Decode [input_string decoded_data] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Reverse Caesar Cipher
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Reverse Caesar Cipher\r\n");

	// Apply reverse
	PreBootConfiguration_UtilsDeobfuscate_ReverseCaesarCipher(decoded_data, decoded_size, caesar_shift);


	// ---------------------------------------------------------------------------------------------------------------------
	// XOR deobfuscation
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - XOR deobfuscation\r\n");

	// Apply XOR deobfuscation
	for (UINTN i = 0; i < decoded_size; i++)
	{
		decoded_data[i] ^= xor_key;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Set output parameters
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Set output parameters\r\n");
	*output_data = decoded_data;
	*output_size = decoded_size;


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Deobfuscate.c] (DeobfuscateStringXorCaesar) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
