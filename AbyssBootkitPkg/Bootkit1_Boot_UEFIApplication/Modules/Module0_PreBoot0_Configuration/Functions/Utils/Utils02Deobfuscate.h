// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Deobfuscate
**/



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
);



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
