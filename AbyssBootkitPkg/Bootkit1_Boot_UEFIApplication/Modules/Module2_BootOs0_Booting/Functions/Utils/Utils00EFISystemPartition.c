// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils EFI System Partition
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiApplicationEntryPoint.h
// Module entry point library for UEFI Applications.
#include <Library/UefiApplicationEntryPoint.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Boot Services Table. Only available to DXE and UEFI module types.
#include <Library/UefiBootServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DevicePathLib.h
// Provides library functions to construct and parse UEFI Device Paths. This library provides defines, macros, and functions to help create and parse EFI_DEVICE_PATH_PROTOCOL structures.
#include <Library/DevicePathLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/FileHandleLib.h
// Provides interface to EFI_FILE_HANDLE functionality.
#include <Library/FileHandleLib.h>



// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------
// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h
// UEFI 2.0 Loaded image protocol definition. Every EFI driver and application is passed an image handle when it is loaded. This image handle will contain a Loaded Image Protocol.
#include <Protocol/LoadedImage.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/SimpleFileSystem.h
// SimpleFileSystem protocol as defined in the UEFI 2.0 specification. The SimpleFileSystem protocol is the programmatic access to the FAT (12,16,32) file system specified in UEFI 2.0. It can also be used to abstract a file system other than FAT. UEFI 2.0 can boot from any valid EFI image contained in a SimpleFileSystem.
#include <Protocol/SimpleFileSystem.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Locates a file within the current EFI System Partition.
	@details        This function searches for a specified file (denoted by ImagePath) in the EFI System Partition where the UEFI application is currently running. If found, the function provides a device path to the located file.


	@param[in]      ImagePath                   The path of the file to locate, relative to the root of the EFI System Partition.
	@param[out]     DevicePath                  On return, points to the device path of the located file.


	@retval         EFI_SUCCESS                 The function successfully located the file and returned its device path.
	@retval         EFI_DEVICE_ERROR            A device related issue occurred.
	@retval         other                       An error occurred while trying to locate the file.
**/
EFI_STATUS
BootOsBooting_UtilsEFISystemPartition_LocateFileInEFIPartition(
	IN              CHAR16*                     ImagePath,
	OUT             EFI_DEVICE_PATH**           DevicePath
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
	EFI_FILE_HANDLE Root;
	EFI_FILE_HANDLE File;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the LoadedImage protocol for the current image

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h#L79
	Status = gBS->HandleProtocol(gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);

	// Failed
	if (EFI_ERROR(Status))
	{
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the Simple File System Protocol of the device we're on

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/SimpleFileSystem.h#L551
	Status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&SimpleFileSystem);

	// Failed
	if (EFI_ERROR(Status))
	{
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Open the volume to get the root directory of the ESP

	// Opens the root directory on a volume.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html?highlight=openvolume#efi-simple-file-system-protocol-openvolume
	Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &Root);

	// Failed
	if (EFI_ERROR(Status))
	{
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Open file

	// Opens a new file relative to the source file’s location.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-open
	Status = Root->Open(Root, &File, ImagePath, EFI_FILE_MODE_READ, 0);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		Root->Close(Root);

		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Convert file path to device path
	
	// Allocates a device path for a file and appends it to an existing device path.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/UefiDevicePathLibDevicePathProtocol/UefiDevicePathLib.c#L688
	*DevicePath = FileDevicePath(LoadedImage->DeviceHandle, ImagePath);

	// Failed
	if (*DevicePath == NULL)
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		File->Close(File);
		Root->Close(Root);

		return EFI_OUT_OF_RESOURCES;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------------------------------------
	// Bye


	// Closes a specified file handle.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
	File->Close(File);
	Root->Close(Root);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
