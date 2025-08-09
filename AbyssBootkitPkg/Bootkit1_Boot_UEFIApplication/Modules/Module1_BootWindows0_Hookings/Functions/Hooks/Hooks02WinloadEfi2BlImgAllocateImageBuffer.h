// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks winload.efi!BlImgAllocateImageBuffer
**/




// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------
// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------



// https://www-cnblogs-com.translate.goog/DirWang/p/17294545.html?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_pto=wapp
// https://github.com/ASkyeye/CVE-2022-21894-Payload/blob/master/main.c
// https://github.com/btbd/umap/blob/master/boot/util.h

// BL_MEMORY_TYPE_APPLICATION
#define MACRO_BOOTWINDOWSHOOKINGS_HOOKSWINLOADEFI_BLIMGALLOCATEIMAGEBUFFER_BL_MEMORY_TYPE_APPLICATION (0xE0000012)
// BL_MEMORY_ATTRIBUTE_RWX
#define MACRO_BOOTWINDOWSHOOKINGS_HOOKSWINLOADEFI_BLIMGALLOCATEIMAGEBUFFER_BL_MEMORY_ATTRIBUTE_RWX (0x424000)



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Hooks the BlImgAllocateImageBuffer function.
	@details        This function replaces the original BlImgAllocateImageBuffer function with a custom hook that allows for additional functionality.


	@param[out]     ImageBuffer                 A pointer to a pointer that will receive the address of the allocated image buffer.
	@param[in]      ImageSize                   The size of the image buffer to be allocated, in bytes.
	@param[in]      MemoryType                  The type of memory to allocate for the image buffer (e.g., EfiBootServicesData).
	@param[in]      Attributes                  Attributes to set for the allocated memory buffer (e.g., EFI_MEMORY_RO).
	@param[in]      Unused                      A pointer reserved for future use, currently not used by this function.
	@param[in]      Flags                       Flags that may affect the allocation behavior (e.g., alignment constraints).


	@retval         EFI_SUCCESS                 The image buffer was successfully allocated and initialized.
	@retval         EFI_OUT_OF_RESOURCES        There was not enough memory available to allocate the image buffer.
	@retval         EFI_INVALID_PARAMETER       One or more input parameters are invalid.
	@retval         EFI_ABORTED                 The image buffer allocation process was aborted due to an internal error.
**/
EFI_STATUS
BootWindowsHookings_HooksWinloadEfi_BlImgAllocateImageBuffer(
	OUT             VOID**                      ImageBuffer,
	IN              UINTN                       ImageSize,
	IN              UINT32                      MemoryType,
	IN              UINT32                      Attributes,
	IN              VOID*                       Unused,
	IN              UINT32                      Flags
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
