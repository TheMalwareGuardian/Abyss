// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Hooks
**/



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures00ArcSystemFirmware.h>



// --------------------------------------------------------------------------------------------------------------------------------------------
// HOOKS
// --------------------------------------------------------------------------------------------------------------------------------------------


/**
	Hook Template

	A "faux call" is a technique used in low-level programming and reverse engineering to divert or redirect a program's execution to a custom function or code snippet.
	
	The technique involves inserting machine instructions into a program's execution flow to force it to execute specific code at a particular point, rather than continuing with its normal flow. This is achieved by manipulating existing machine instructions or inserting new instructions that perform custom tasks.

	48 B8
	00 00 00 00 00 00 00 00
	50
	C3	

	https://github.com/Mattiwatti/EfiGuard/blob/master/EfiGuardDxe/PatchBootmgr.c
**/
static CONST UINT8 Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate[] = {
	0x48, 0xB8,												// mov rax,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			// <addr>
	0x50,													// push [e|r]ax
	0xC3													// ret
};

// Offset
static CONST UINTN Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplateAddressOffset = 2;



// --------------------------------------------------------------------------------------------------------------------------------------------
// ORIGINAL FUNCTIONS
// --------------------------------------------------------------------------------------------------------------------------------------------



// Boot Services -> LoadImage
extern EFI_IMAGE_LOAD Global_BootWindowsHookings_UtilsHooks_BSLoadImageOriginal;



// \EFI\Microsoft\Boot\bootmgfw.efi | bootmgfw.efi -> ImgArchStartBootApplication
typedef
EFI_STATUS
(EFIAPI*
t_ImgArchStartBootApplication)(
	IN PBL_APPLICATION_ENTRY AppEntry,
	IN VOID* ImageBase,
	IN UINT32 ImageSize,
	IN UINT32 BootOption,
	OUT PBL_RETURN_ARGUMENTS ReturnArguments
);																																												// Function
extern VOID* Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationOriginalPointer;																		// Pointer
extern UINT8 Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationBackupBytes[sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate)];		// Bytes



// C:\Windows\System32\winload.efi | winload.efi -> OslFwpKernelSetupPhase1
typedef
EFI_STATUS
(EFIAPI*
t_OslFwpKernelSetupPhase1)(
	IN PLOADER_PARAMETER_BLOCK LoaderBlock
);																																												// Function
extern VOID* Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1OriginalPointer;																			// Pointer
extern UINT8 Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1BackupBytes[sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate)];			// Bytes



// C:\Windows\System32\winload.efi | winload.efi -> BlImgAllocateImageBuffer
typedef
EFI_STATUS
(EFIAPI*
t_BlImgAllocateImageBuffer)(
	OUT VOID** ImageBuffer,
	IN UINTN ImageSize,
	IN UINT32 MemoryType,
	IN UINT32 Attributes,
	IN VOID* Unused,
	IN UINT32 Flags
);																																												// Function
extern VOID* Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer;																			// Pointer
extern UINT8 Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferBackupBytes[sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate)];			// Bytes
extern VOID* Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferHookAddress;



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
