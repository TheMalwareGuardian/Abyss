#pragma once
// #pragma once in a file ensures that the file's contents are only included or processed by the compiler once, even if it's included or imported multiple times in different parts of the code. This directive is often used as an include guard to prevent multiple inclusions of the same file, reducing compilation time and avoiding potential issues related to redefinition of symbols.



// START -> STANDARDS -------------------------------------------------------------------------------------------------------------------------
// START -> STANDARDS -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/IndustryStandard/PeImage.h
// EFI image format for PE32, PE32+ and TE. Please note some data structures are different for PE32 and PE32+. EFI_IMAGE_NT_HEADERS32 is for PE32 and EFI_IMAGE_NT_HEADERS64 is for PE32+.
#include <IndustryStandard/PeImage.h>



// ----------------------------------------------------------------------------------------------------------------------------------------------------
// TYPEDEFS
// ----------------------------------------------------------------------------------------------------------------------------------------------------


// EFI image format for PE32, PE32+ and TE
typedef EFI_IMAGE_DOS_HEADER *PEFI_IMAGE_DOS_HEADER;
typedef EFI_IMAGE_NT_HEADERS64 *PEFI_IMAGE_NT_HEADERS64;
typedef EFI_IMAGE_NT_HEADERS32 *PEFI_IMAGE_NT_HEADERS32;
typedef EFI_IMAGE_NT_HEADERS64 EFI_IMAGE_NT_HEADERS, *PEFI_IMAGE_NT_HEADERS;
typedef EFI_IMAGE_SECTION_HEADER *PEFI_IMAGE_SECTION_HEADER;
typedef EFI_IMAGE_DATA_DIRECTORY *PEFI_IMAGE_DATA_DIRECTORY;
typedef EFI_IMAGE_EXPORT_DIRECTORY *PEFI_IMAGE_EXPORT_DIRECTORY;
typedef EFI_IMAGE_BASE_RELOCATION *PEFI_IMAGE_BASE_RELOCATION;
// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/IndustryStandard/PeImage.h



// ----------------------------------------------------------------------------------------------------------------------------------------------------
// DEFINES
// ----------------------------------------------------------------------------------------------------------------------------------------------------


#define LOWORD(l)						((UINT16)(((UINTN)(l)) & 0xffff))
#define HIWORD(l)						((UINT16)((((UINTN)(l)) >> 16) & 0xffff))
#define MAKELANGID(Primary, Sub)		((((UINT16)(Sub)) << 10) | (UINT16)(Primary))
#define LANG_NEUTRAL					0x00
#define SUBLANG_NEUTRAL					0x00
#define RT_VERSION						16
#define VS_VERSION_INFO					1
#define RUNTIME_FUNCTION_INDIRECT		0x1 // https://doxygen.reactos.org/d5/df7/ndk_2rtltypes_8h_source.html
#define FIELD_OFFSET(Type, Field)		((INT32)(INTN)&(((Type *)0)->Field)) // https://doxygen.reactos.org/d7/d69/typedefs_8h.html#a770868c8a4f09c4abb048d60d9a36144
#define LDR_IS_DATAFILE(x)				(((UINTN)(x)) & (UINTN)1)
#define LDR_DATAFILE_TO_VIEW(x)			((VOID*)(((UINTN)(x)) & ~(UINTN)1))
#define HEADER_FIELD(NtHeaders, Field) (IMAGE64(NtHeaders) ? ((PEFI_IMAGE_NT_HEADERS64)(NtHeaders))->OptionalHeader.Field : ((PEFI_IMAGE_NT_HEADERS32)(NtHeaders))->OptionalHeader.Field)
#define IMAGE_FIRST_SECTION(NtHeaders) ((PEFI_IMAGE_SECTION_HEADER)	((UINTN)(NtHeaders) + FIELD_OFFSET(EFI_IMAGE_NT_HEADERS, OptionalHeader) + ((NtHeaders))->FileHeader.SizeOfOptionalHeader)) // https://doxygen.reactos.org/d5/d44/ntimage_8h.html#a7b448457aaf9c406815b331ef4b51067
#define IMAGE64(NtHeaders) ((NtHeaders)->OptionalHeader.Magic == EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC)
#define IMAGE32(NtHeaders) ((NtHeaders)->OptionalHeader.Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC)
#define IMAGE_ORDINAL_FLAG64			(0x8000000000000000)
#define IMAGE_ORDINAL_FLAG32			(0x80000000)



// ----------------------------------------------------------------------------------------------------------------------------------------------------
// STRUCTS
// ----------------------------------------------------------------------------------------------------------------------------------------------------


/**
	VS_FIXEDFILEINFO

	Contains version information for a file. This information is language and code page independent.
**/

typedef struct _VS_FIXEDFILEINFO
{
	UINT32 dwSignature;												// Contains the value 0xFEEF04BD
	UINT32 dwStrucVersion;											// The binary version number of this structure.
	UINT32 dwFileVersionMS;											// The most significant 32 bits of the file's binary version number.
	UINT32 dwFileVersionLS;											// The least significant 32 bits of the file's binary version number.
	UINT32 dwProductVersionMS;										// The most significant 32 bits of the binary version number of the product with which this file was distributed.
	UINT32 dwProductVersionLS;										// The least significant 32 bits of the binary version number of the product with which this file was distributed.
	UINT32 dwFileFlagsMask;											// Contains a bitmask that specifies the valid bits in dwFileFlags.
	UINT32 dwFileFlags;												// Contains a bitmask that specifies the Boolean attributes of the file.
	UINT32 dwFileOS;												// The operating system for which this file was designed.
	UINT32 dwFileType;												// The general type of file. This member can be one of the following values.
	UINT32 dwFileSubtype;											// The function of the file.
	UINT32 dwFileDateMS;											// The most significant 32 bits of the file's 64-bit binary creation date and time stamp.
	UINT32 dwFileDateLS;											// The least significant 32 bits of the file's 64-bit binary creation date and time stamp.
} VS_FIXEDFILEINFO; // https://learn.microsoft.com/en-us/windows/win32/api/VerRsrc/ns-verrsrc-vs_fixedfileinfo


/**
	VS_VERSIONINFO

	Represents the organization of data in a file-version resource. It is the root structure that contains all other file-version information structures.
	Raw version info data as it appears in a PE file resource directory. This struct is not in any SDK headers, not because it is super secret, but because MS is ashamed of it.
**/

typedef struct _VS_VERSIONINFO
{
	UINT16 wLength;													// The length, in bytes, of the VS_VERSIONINFO structure.
	UINT16 wValueLength;											// The length, in bytes, of the Value member.
	UINT16 wType;													// The type of data in the version resource.
	CHAR16 Name[sizeof(L"VS_VERSION_INFO") / sizeof(CHAR16)];		// The Unicode string L"VS_VERSION_INFO". Size includes null terminator.
	VS_FIXEDFILEINFO ValueFixedFileInfo;							// The Unicode string L"VS_VERSION_INFO".
	// Omitted: padding fields that do not contribute to TotalSize
} VS_VERSIONINFO, *PVS_VERSIONINFO; // https://learn.microsoft.com/en-us/windows/win32/menurc/vs-versioninfo


/**
	IMAGE_RUNTIME_FUNCTION_ENTRY

	Represents an entry in the function table on 64-bit Windows.
**/
typedef struct _IMAGE_RUNTIME_FUNCTION_ENTRY
{
	UINT32 BeginAddress;											// The address of the start of the function.
	UINT32 EndAddress;												// The address of the end of the function.
	union
	{
		UINT32 UnwindInfoAddress;									// The address of the unwind information for the function.
		UINT32 UnwindData;
	} u;
} IMAGE_RUNTIME_FUNCTION_ENTRY, *PIMAGE_RUNTIME_FUNCTION_ENTRY; // https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-runtime_function


/**
	IMAGE_IMPORT_BY_NAME

	The structure is just a WORD, followed by a string naming the imported API. The WORD value is a "hint" to the loader as to what the ordinal of the imported API might be. When the loader brings in the executable, it overwrites each IAT entry with the actual address of the imported function.
**/

typedef struct _IMAGE_IMPORT_BY_NAME
{
	UINT16 Hint;
	CHAR8 Name[1];
} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME; // https://learn.microsoft.com/en-us/archive/msdn-magazine/2002/march/inside-windows-an-in-depth-look-into-the-win32-portable-executable-file-format-part-2


/**
	IMAGE_THUNK_DATA64
**/

typedef struct _IMAGE_THUNK_DATA64
{
	union
	{
		UINT64 ForwarderString;										// UINT8* 
		UINT64 Function;											// UINT32*
		UINT64 Ordinal;
		UINT64 AddressOfData;										// PIMAGE_IMPORT_BY_NAME
	} u1;
} IMAGE_THUNK_DATA64, *PIMAGE_THUNK_DATA64; // https://doxygen.reactos.org/d0/df0/struct__IMAGE__THUNK__DATA64.html


/**
	IMAGE_THUNK_DATA32
**/

typedef struct _IMAGE_THUNK_DATA32
{
	union
	{
		UINT32 ForwarderString;										// UINT8*  
		UINT32 Function;											// UINT32*
		UINT32 Ordinal;
		UINT32 AddressOfData;										// PIMAGE_IMPORT_BY_NAME
	} u1;
} IMAGE_THUNK_DATA32, *PIMAGE_THUNK_DATA32; // https://doxygen.reactos.org/d2/da5/struct__IMAGE__THUNK__DATA32.html


/**
	IMAGE_IMPORT_DESCRIPTOR

	Anchor of the imports data. For every DLL the executable is loading functions from, there will be an IMAGE_IMPORT_DESCRIPTOR within the Image Directory Table. The IMAGE_IMPORT_DESCRIPTOR will contain the name of the DLL, and two fields holding RVAs of the ILT (Import Lookup Table) and the IAT (Import Address Table). The ILT will contain references for all the functions that are being imported from the DLL. The IAT will be identical to the ILT until the executable is loaded in memory, then the loader will fill the IAT with the actual addresses of the imported functions.
**/

typedef struct _IMAGE_IMPORT_DESCRIPTOR
{
	union
	{
		UINT32 Characteristics;										// 0 for terminating null import descriptor.
		UINT32 OriginalFirstThunk;									// RVA to original unbound IAT (PIMAGE_THUNK_DATA).
	} u;
	UINT32 TimeDateStamp;											// 0 if not bound, -1 if bound, and real date\time stamp in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND) O.W. date/time stamp of DLL bound to (Old BIND).
	UINT32 ForwarderChain;											// -1 if no forwarders.
	UINT32 Name;													// An RVA of an ASCII string that contains the name of the imported DLL.
	UINT32 FirstThunk;												// RVA to IAT (if bound this IAT has actual addresses).
} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR; // https://learn.microsoft.com/en-us/archive/msdn-magazine/2002/march/inside-windows-an-in-depth-look-into-the-win32-portable-executable-file-format-part-2, https://0xrick.github.io/win-internals/pe6/
