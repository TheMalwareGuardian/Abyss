// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Hooks
**/



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils09Hooks.h>



// --------------------------------------------------------------------------------------------------------------------------------------------
// ORIGINAL FUNCTIONS
// --------------------------------------------------------------------------------------------------------------------------------------------



// Boot Services -> LoadImage
EFI_IMAGE_LOAD Global_BootWindowsHookings_UtilsHooks_BSLoadImageOriginal;



// \EFI\Microsoft\Boot\bootmgfw.efi | bootmgfw.efi -> ImgArchStartBootApplication
VOID* Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationOriginalPointer = NULL;                                                                       // Pointer
UINT8 Global_BootWindowsHookings_UtilsHooks_BootmgfwEfiImgArchStartBootApplicationBackupBytes[sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate)] = { 0 };      // Bytes



// C:\Windows\System32\winload.efi | winload.efi -> OslFwpKernelSetupPhase1
VOID* Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1OriginalPointer = NULL;                                                                            // Pointer
UINT8 Global_BootWindowsHookings_UtilsHooks_WinloadEfiOslFwpKernelSetupPhase1BackupBytes[sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate)] = { 0 };           // Bytes



// C:\Windows\System32\winload.efi | winload.efi -> BlImgAllocateImageBuffer
VOID* Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferOriginalPointer = NULL;                                                                           // Pointer
UINT8 Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferBackupBytes[sizeof(Global_BootWindowsHookings_UtilsHooks_FauxCallHookTemplate)] = { 0 };          // Bytes
VOID* Global_BootWindowsHookings_UtilsHooks_WinloadEfiBlImgAllocateImageBufferHookAddress = NULL;                                                                               // Address



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
