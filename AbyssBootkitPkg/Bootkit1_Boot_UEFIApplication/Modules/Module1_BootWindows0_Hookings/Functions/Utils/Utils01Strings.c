// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Strings
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



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
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if either of the input strings is NULL or if Length is 0

	if (FirstString == NULL || SecondString == NULL || Length == 0)
	{
		return 0;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Convert first characters of both strings to uppercase

	// Convert a Unicode character to upper case only if it maps to a valid small-case ASCII character.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L307
	CHAR16 UpperFirstChar = CharToUpper(*FirstString);
	CHAR16 UpperSecondChar = CharToUpper(*SecondString);


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate through the strings

	while ((*FirstString != L'\0') && (*SecondString != L'\0') && (UpperFirstChar == UpperSecondChar) && (Length > 1))
	{
		FirstString++;
		SecondString++;

		// Convert a Unicode character to upper case only if it maps to a valid small-case ASCII character.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L307
		UpperFirstChar = CharToUpper(*FirstString);
		UpperSecondChar = CharToUpper(*SecondString);

		Length--;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return UpperFirstChar - UpperSecondChar;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
