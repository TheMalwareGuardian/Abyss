// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	UefiApplication_BootmgfwLauncher.c

	A minimal UEFI application that locates and launches the default Windows Boot Manager (bootmgfw.efi).

	This example serves as the foundation for understanding UEFI applications, bootkits, and low-level development within the UEFI pre-boot environment.


	@note

	Instructions to Compile and Run this UEFI Application (in a Virtual Machine using VMware Workstation):

	1. Open the file 'Conf/target.txt' located in the root of your EDK2 directory. Modify the following line to point to this DSC file:
		ACTIVE_PLATFORM = UefiBootkit003_BootmgfwLauncherPkg/UefiApplication_BootmgfwLauncher.dsc

	2. Open a command prompt and set up the EDK2 environment:
		> edksetup.bat Rebuild

	3. Build the application:
		> build

	4. Once built successfully, locate the generated .EFI binary at:
		C:\edk2\Build\UefiBootkit003_BootmgfwLauncherPkg\DEBUG_VS2019\X64\UefiBootkit003_BootmgfwLauncher.efi

	5. To test the application:

		a. If you're already inside a Windows virtual machine (VM):

			- Mount the EFI System Partition (ESP):
				> mountvol U: /S

			- Copy the generated .efi binary to the EFI\Boot\Bootkits directory:
				> mkdir U:\EFI\Boot\Bootkits
				> copy C:\edk2\Build\UefiBootkit003_BootmgfwLauncherPkg\DEBUG_VS2019\X64\UefiBootkit003_BootmgfwLauncher.efi U:\EFI\Boot\Bootkits\UefiBootkit003_BootmgfwLauncher.efi

			- Dismount the ESP:
				> mountvol U: /D

			- Shut down the VM

			- Power on the VM directly to firmware:
				Right-click the VM -> Power -> Power On to Firmware

			- From the firmware menu, select 'EFI Internal Shell, then press 'Enter'.

			- Run the application in the shell:
				Shell> map -r
				Shell> fs0:
				Shell> dir
				Shell> cd EFI\Boot\Bootkits
				Shell> dir
				Shell> UefiBootkit003_BootmgfwLauncher.efi

	b. If testing from outside the VM:

		- Create a shared folder between the host and guest, and place the .efi file inside it.

		- Power on the VM directly to firmware:
				Right-click the VM -> Power -> Power On to Firmware

			- From the firmware menu, select 'EFI Internal Shell, then press 'Enter'.

			- Run the application in the shell:
				Shell> map -r
				Shell> fs0:
				Shell> dir
				Shell> cd EFI\Boot\Bootkits
				Shell> cp fsnt0:\UefiBootkit003_BootmgfwLauncher.efi .
				Shell> dir
				Shell> UefiBootkit003_BootmgfwLauncher.efi
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



// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------
// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h
// UEFI 2.0 Loaded image protocol definition. Every EFI driver and application is passed an image handle when it is loaded. This image handle will contain a Loaded Image Protocol.
#include <Protocol/LoadedImage.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Global Path to bootmgfw.efi
STATIC CONST CHAR16 *Global_BootManagerPath = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          The entry point for the UEFI Application.
	@details        This function is invoked by the UEFI firmware when the application is loaded. It prints a simple message to the screen and returns successfully.


	@param[in]      ImageHandle         The firmware allocated handle for the EFI image.
	@param[in]      SystemTable         A pointer to the EFI System Table.


	@retval         EFI_SUCCESS         The entry point is executed successfully.
	@retval         other               Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
UefiMain (
	IN              EFI_HANDLE          ImageHandle,
	IN              EFI_SYSTEM_TABLE    *SystemTable
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	Print(L"[INFO]  Hello World from UefiBootkit003_BootmgfwLauncher (UEFI Application)\r\n");
	Print(L"[INFO]  Attempting to launch Windows Boot Manager\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
	EFI_FILE_HANDLE Root;
	EFI_FILE_HANDLE File;
	EFI_DEVICE_PATH_PROTOCOL *FilePathDevicePath;
	EFI_HANDLE FileImageHandle;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the LoadedImage protocol for the current image
	Print(L"[INFO]  Get the LoadedImage protocol for the current image\r\n");

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h#L79
	Status = gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);

	// Failed
	if (EFI_ERROR(Status))
	{
		Print(L"[ERROR]	gBS->HandleProtocol [gEfiLoadedImageProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the Simple File System Protocol of the device we're on
	Print(L"[INFO]  Get the Simple File System Protocol of the device we're on\r\n");

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/SimpleFileSystem.h#L551
	Status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&SimpleFileSystem);

	// Failed
	if (EFI_ERROR(Status))
	{
		Print(L"[ERROR]	gBS->HandleProtocol [gEfiSimpleFileSystemProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return EFI_ABORTED;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Open the volume to get the root directory of the ESP
	Print(L"[INFO]  Open the volume to get the root directory of the ESP\r\n");

	// Opens the root directory on a volume.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html?highlight=openvolume#efi-simple-file-system-protocol-openvolume
	Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &Root);

	// Failed
	if (EFI_ERROR(Status))
	{
		Print(L"[ERROR] SimpleFileSystem->OpenVolume failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Attempt to open the file in read mode
	Print(L"[INFO]  Attempt to open the file in read mode\r\n");

	// Opens a new file relative to the source file’s location.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-open
	Status = Root->Open(Root, &File, (CHAR16 *)Global_BootManagerPath, EFI_FILE_MODE_READ, 0);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		Root->Close(Root);

		Print(L"[ERROR] Root->Open failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Convert file path to device path
	Print(L"[INFO]  Convert file path to device path\r\n");

	// Allocates a device path for a file and appends it to an existing device path.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/UefiDevicePathLibDevicePathProtocol/UefiDevicePathLib.c#L688
	FilePathDevicePath = FileDevicePath(LoadedImage->DeviceHandle, (CHAR16 *)Global_BootManagerPath);

	// Failed
	if (FilePathDevicePath == NULL)
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		File->Close(File);
		Root->Close(Root);

		Print(L"[ERROR] FileDevicePath failed.\r\n");
		return EFI_OUT_OF_RESOURCES;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Load the image
	Print(L"[INFO]  Load the image\r\n");

	// Loads an EFI image into memory.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-loadimage
	Status = gBS->LoadImage(FALSE, gImageHandle, FilePathDevicePath, NULL, 0, &FileImageHandle);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		File->Close(File);
		Root->Close(Root);

		Print(L"[ERROR] gBS->LoadImage failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Start the image
	Print(L"[INFO]  Start the image\r\n");

	// Transfers control to a loaded image’s entry point.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-startimage
	Status = gBS->StartImage(FileImageHandle, NULL, NULL);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		File->Close(File);
		Root->Close(Root);

		Print(L"[ERROR] gBS->StartImage failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	Print(L"[INFO]  Bye\r\n");

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