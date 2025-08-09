// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Functions Patch/Hook Uefi
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Hooks a service table pointer, replacing its original function with a new one.
	@details        This function modifies an entry in the EFI service table. It replaces the existing function pointed to by ServiceTableFunction with a new function provided by NewFunction. Additionally, it returns the address of the original function to allow for later restoration if needed.


	@param[in]      ServiceTableHeader          A pointer to the EFI service table header. This header is updated as part of the hooking process.
	@param[in]      ServiceTableFunction        A double reference to the EFI service table function. This argument is expected to point to the function to be replaced.
	@param[in]      NewFunction                 A pointer to the new function that will replace the existing function in the EFI service table.


	@retval         VOID*                       Returns a pointer to the original function that was in the service table before the modification.
**/
VOID*
BootWindowsHookings_FunctionsUefi_HookServiceTablePointer(
	IN              EFI_TABLE_HEADER            *ServiceTableHeader,
	IN              VOID                        **ServiceTableFunction,
	IN              VOID                        *NewFunction
);



/**
	@brief          Hooks and processes the Boot Service's LoadImage function.
	@details        This function serves as a custom hook for UEFI's LoadImage function. It performs additional processing before calling the original LoadImage function.


	@param[in]      BootPolicy                  Indicates the policy for loading the image. If TRUE, the image is loaded as a boot option; otherwise, it is loaded without boot option.
	@param[in]      ParentImageHandle           The handle of the image that is loading this image.
	@param[in]      DevicePath                  The pointer to the device path of the image.
	@param[in]      SourceBuffer                If not NULL, a pointer to the memory location containing a copy of the image to be loaded.
	@param[in]      SourceSize                  The size in bytes of SourceBuffer.
	@param[out]     ImageHandle                 The pointer where the handle of the loaded image will be returned.


	@retval         EFI_SUCCESS                 The image is successfully loaded.
	@retval         other                       An error occurred.
**/
EFI_STATUS
BootWindowsHookings_FunctionsUefi_HookgBSLoadImage(
	IN              BOOLEAN                     BootPolicy,
	IN              EFI_HANDLE                  ParentImageHandle,
	IN              EFI_DEVICE_PATH_PROTOCOL    *DevicePath,
	IN              VOID                        *SourceBuffer OPTIONAL,
	IN              UINTN                       SourceSize,
	OUT             EFI_HANDLE                  *ImageHandle
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
