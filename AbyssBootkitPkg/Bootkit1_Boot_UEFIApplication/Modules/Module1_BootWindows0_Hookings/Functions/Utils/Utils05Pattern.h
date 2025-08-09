// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Pattern
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Searches for a specified pattern in a memory region.
	@details        This function searches for the specified pattern (byte sequence) in the memory region defined. It compares the memory content against the provided pattern, which can contain wildcard bytes to represent don't care values. If the pattern is found within the specified memory region, the pointer is updated with the address of the FIRST OCCURRENCE.


	@param[in]      Pattern                     The pattern to search for.
	@param[in]      Wildcard                    The wildcard value used in the pattern.
	@param[in]      PatternLength               The length of the pattern to search for.
	@param[in]      Base                        The base address of the memory region to search within.
	@param[in]      Size                        The size of the memory region to search within.
	@param[out]     Found                       On return, points to the address of the first occurrence of the pattern if found.


	@retval         EFI_SUCCESS                 The pattern was found, and Found is updated with the address of the first occurrence.
	@retval         EFI_NOT_FOUND               The pattern was not found within the specified memory region.
	@retval         EFI_INVALID_PARAMETER       One or more input parameters are invalid.
**/
EFI_STATUS
BootWindowsHookings_UtilsPattern_FindPattern(
	IN              CONST UINT8*                Pattern,
	IN              UINT8                       Wildcard,
	IN              UINT32                      PatternLength,
	IN              CONST VOID*                 Base,
	IN              UINT32                      Size,
	OUT             VOID                        **Found
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
