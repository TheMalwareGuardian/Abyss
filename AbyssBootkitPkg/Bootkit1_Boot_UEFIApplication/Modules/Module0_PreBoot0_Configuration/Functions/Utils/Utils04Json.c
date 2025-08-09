// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Json
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
// Provides copy memory, fill memory, zero memory, and GUID functions. The Base Memory Library provides optimized implementations for common memory-based operations. These functions should be used in place of coding your own loops to do equivalent common functions. This allows optimized library implementations to help increase performance.
#include <Library/BaseMemoryLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/MemoryAllocationLib.h
// Provides services to allocate and free memory buffers of various memory types and alignments. The Memory Allocation Library abstracts various common memory allocation operations. This library allows code to be written in a phase-independent manner because the allocation of memory in PEI, DXE, and SMM (for example) is done via a different mechanism. Using a common library interface makes it much easier to port algorithms from phase to phase.
#include <Library/MemoryAllocationLib.h>



// START -> INCLUDES --------------------------------------------------------------------------------------------------------------------------
// START -> INCLUDES --------------------------------------------------------------------------------------------------------------------------



// https://github.com/akheron/jansson/blob/master/src/jansson.h
// Jansson is a C library for encoding, decoding and manipulating JSON data. Its main features and design principles are:
// - Simple and intuitive API and data model
// - Comprehensive documentation
// - No dependencies on other libraries
// - Full Unicode support (UTF-8)
// - Extensive test suite
// #include <jansson.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>



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
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	CHAR8 *KeyLocation;
	CHAR8 *StartValue;
	CHAR8 *EndValue;
	UINTN KeyLength;
	UINTN ValueLength;
	UINTN Count = 0;
	CHAR8 **TempList;
	CHAR8 *Token;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the length of the key
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Get the length of the key\r\n");

	// Returns the length of a Null-terminated ASCII string.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L633
	KeyLength = AsciiStrLen(Key);


	// ---------------------------------------------------------------------------------------------------------------------
	// Find the key in the JSON string
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Find the key in the JSON string\r\n");

	// Returns the first occurrence of a Null-terminated ASCII sub-string in a Null-terminated ASCII string.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L923
	KeyLocation = AsciiStrStr(JsonString, Key);

	// Failed
	if (KeyLocation == NULL)
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - AsciiStrStr failed.\r\n");
		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Find the colon after the key
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Find the key in the JSON string\r\n");

	StartValue = KeyLocation + KeyLength;
	while (*StartValue != '\0' && *StartValue != ':')
	{
		StartValue++;
	}
	if (*StartValue == '\0')
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Operation failed: Colon not found.\r\n");
		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Skip colon and any whitespace
	StartValue++;
	while (*StartValue != '\0' && (*StartValue == ' ' || *StartValue == '\t' || *StartValue == '\n'))
	{
		StartValue++;
	}
	if (*StartValue == '\0')
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Operation failed: Value not found.\r\n");
		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the value is a string or a list
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Check if the value is a string or a list\r\n");

	if (*StartValue == '\"')
	{
		// -----------------------------------------------------------------------------------------------------------------
		// It's a string
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - It's a string\r\n");


		// -----------------------------------------------------------------------------------------------------------------
		// Skip the opening quote
		StartValue++;

		EndValue = StartValue;
		while (*EndValue != '\0' && *EndValue != '\"')
		{
			EndValue++;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Closing quote not found
		if (*EndValue == '\0')
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Operation failed: Closing quote not found.\r\n");
			return EFI_NOT_FOUND;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Calculate the length of the value
		ValueLength = EndValue - StartValue;


		// -----------------------------------------------------------------------------------------------------------------
		// Allocate memory for the value pointer

		// Allocates pool memory.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
		*Value = AllocatePool(sizeof(CHAR8*));

		// Failed
		if (*Value == NULL)
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - AllocatePool [CHAR8] failed.\r\n");
			return EFI_OUT_OF_RESOURCES;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Allocate memory for the value

		// Allocates pool memory.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
		(*Value)[0] = AllocatePool(ValueLength + 1);

		// Failed
		if ((*Value)[0] == NULL)
		{
			// Returns pool memory to the system.
			// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
			FreePool(*Value);

			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - AllocatePool [ValueLength] failed.\r\n");
			return EFI_OUT_OF_RESOURCES;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Copy the value

		// The CopyMem() function copies the contents of one buffer to another buffer.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-copymem
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseMemoryLib/CopyMemWrapper.c#L41
		CopyMem((*Value)[0], StartValue, ValueLength);


		// -----------------------------------------------------------------------------------------------------------------
		// Null-terminate the value
		(*Value)[0][ValueLength] = '\0';


		// -----------------------------------------------------------------------------------------------------------------
		// Set the value count to 1
		*ValueCount = 1;
	}
	else if (*StartValue == '[')
	{
		// -----------------------------------------------------------------------------------------------------------------
		// It's a list
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - It's a list\r\n");


		// -----------------------------------------------------------------------------------------------------------------
		// Skip the opening bracket
		StartValue++;
		EndValue = StartValue;

		while (*EndValue != '\0' && *EndValue != ']')
		{
			EndValue++;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Closing bracket not found
		if (*EndValue == '\0')
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Operation failed: Closing bracket not found.\r\n");
			return EFI_NOT_FOUND;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Allocate initial memory for the list

		// Allocates pool memory.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
		TempList = AllocatePool(10 * sizeof(CHAR8*));

		// Failed
		if (TempList == NULL)
		{
			PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - AllocatePool [CHAR8] failed.\r\n");
			return EFI_OUT_OF_RESOURCES;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Tokenize the list items

		// Returns the first occurrence of a Null-terminated ASCII sub-string in a Null-terminated ASCII string.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L923
		Token = AsciiStrStr(StartValue, "\"");
		while (Token != NULL && Token < EndValue)
		{
			// -------------------------------------------------------------------------------------------------------------
			// Returns the first occurrence of a Null-terminated ASCII sub-string in a Null-terminated ASCII string.
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c
			CHAR8 *NextToken = AsciiStrStr(Token + 1, "\"");
			if (NextToken == NULL || NextToken >= EndValue)
			{
				// No more tokens or beyond the list end
				break;
			}


			// -------------------------------------------------------------------------------------------------------------
			// Calculate the length of the token
			ValueLength = NextToken - Token - 1;


			// -------------------------------------------------------------------------------------------------------------
			// Allocate memory for the token

			// Allocates pool memory.
			// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
			TempList[Count] = AllocatePool(ValueLength + 1);

			// Failed
			if (TempList[Count] == NULL)
			{
				for (UINTN i = 0; i < Count; i++)
				{
					// Returns pool memory to the system.
					// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
					// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
					FreePool(TempList[i]);
				}

				// Returns pool memory to the system.
				// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-freepool
				// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L841
				FreePool(TempList);

				PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - AllocatePool [ValueLength] failed.\r\n");
				return EFI_OUT_OF_RESOURCES;
			}


			// -------------------------------------------------------------------------------------------------------------
			// Copy the token

			// The CopyMem() function copies the contents of one buffer to another buffer.
			// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-copymem
			CopyMem(TempList[Count], Token + 1, ValueLength);


			// -------------------------------------------------------------------------------------------------------------
			// Null-terminate the token
			TempList[Count][ValueLength] = '\0';


			// -------------------------------------------------------------------------------------------------------------
			// Increment the count
			Count++;


			// -------------------------------------------------------------------------------------------------------------
			// Find the next token

			// Returns the first occurrence of a Null-terminated ASCII sub-string in a Null-terminated ASCII string.
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c
			Token = AsciiStrStr(NextToken + 1, "\"");
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Set the value pointer to the temporary list
		*Value = TempList;


		// -----------------------------------------------------------------------------------------------------------------
		// Set the value count to the number of tokens
		*ValueCount = Count;
	}
	else
	{
		PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Operation failed: Invalid JSON format.\r\n");
		return EFI_UNSUPPORTED;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Normalizes a UEFI file path by removing duplicate backslashes.
	@details        This function takes a UEFI-style file path and removes any consecutive backslashes (e.g., turns "\\EFI\\\\Boot\\ntfs.efi" into "\\EFI\\Boot\\ntfs.efi"). This is useful when dealing with file paths that are built dynamically or parsed from external sources such as configuration files, where formatting errors may occur.


	@param[in,out]  Path                A pointer to a null-terminated CHAR16 string containing the UEFI path. On output, the path will be normalized in-place with redundant backslashes removed.


	@retval         None                This function does not return a value. The result is written back into the input buffer.
**/
VOID
PreBootConfiguration_UtilsJson_NormalizeUefiPath(
	IN OUT          CHAR16              *Path
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootConfiguration [Utils/Json.c] (ParseJson) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	UINTN writeIndex = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate through each character of the input path, copying characters while skipping redundant backslashes
	for (UINTN readIndex = 0; Path[readIndex] != L'\0'; ++readIndex)
	{
		// Copy current character from read position to write position
		Path[writeIndex] = Path[readIndex];

		// Skip advancing the write index if the next character is another backslash (i.e., '\\')
		// This prevents writing consecutive backslashes into the output
		if (!(Path[readIndex] == L'\\' && Path[readIndex + 1] == L'\\'))
		{
			writeIndex++;
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Null-terminate the resulting normalized string
	Path[writeIndex] = L'\0';
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
