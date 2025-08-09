// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Strings
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Performs a case-insensitive comparison of two ASCII strings with a specified length.
	@details        This function compares two ASCII strings, FirstString and SecondString, up to the specified Length characters (https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strnicmp-wcsnicmp-mbsnicmp-strnicmp-l-wcsnicmp-l-mbsnicmp-l). The comparison is case-insensitive. If FirstString is equal to SecondString, it returns 0. If FirstString is less than SecondString, it returns a negative value. If FirstString is greater than SecondString, it returns a positive value.


	@param[in]      FirstString         A pointer to the first ASCII string.
	@param[in]      SecondString        A pointer to the second ASCII string.
	@param[in]      Length              The maximum number of characters to compare.


	@retval         0                   The two strings are equal up to Length characters.
	@retval         Negative            FirstString is less than SecondString.
	@retval         Positive            FirstString is greater than SecondString.
**/
INTN
BootWindowsHookings_UtilsStrings_StrniCmp(
	IN              CHAR16              *FirstString,
	IN              CHAR16              *SecondString,
	IN              UINTN               Length
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
