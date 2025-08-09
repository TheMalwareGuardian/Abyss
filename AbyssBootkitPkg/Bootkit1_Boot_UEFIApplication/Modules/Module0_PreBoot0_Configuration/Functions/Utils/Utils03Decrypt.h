// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Decrypt
**/



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
