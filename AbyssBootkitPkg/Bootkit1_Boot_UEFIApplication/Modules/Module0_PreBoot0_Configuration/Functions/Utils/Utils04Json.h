// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Json
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Parse JSON content to extract specific fields.


	@param[in]      JsonString          The JSON string to parse.
	@param[in]      Key                 The key to look for in the JSON string.
	@param[out]     Value               A pointer to the extracted value.
	@param[out]     ValueCount          A pointer to the number of values extracted


	@retval         EFI_SUCCESS         The key was found and the value was extracted.
**/
EFI_STATUS
PreBootConfiguration_UtilsJson_ParseJson(
	IN              CHAR8               *JsonString,
	IN              CONST CHAR8         *Key,
	OUT             CHAR8               ***Value,
	OUT             UINTN               *ValueCount
);



/**
	@brief          Normalizes a UEFI file path by removing duplicate backslashes.
	@details        This function takes a UEFI-style file path and removes any consecutive backslashes (e.g., turns "\\EFI\\\\Boot\\ntfs.efi" into "\\EFI\\Boot\\ntfs.efi"). This is useful when dealing with file paths that are built dynamically or parsed from external sources such as configuration files, where formatting errors may occur.


	@param[in,out]  Path                A pointer to a null-terminated CHAR16 string containing the UEFI path. On output, the path will be normalized in-place with redundant backslashes removed.


	@retval         None                This function does not return a value. The result is written back into the input buffer.
**/
VOID
PreBootConfiguration_UtilsJson_NormalizeUefiPath(
	IN OUT          CHAR16              *Path
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
