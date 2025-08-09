// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	UefiApplication_HelloWorld.c

	A minimal UEFI application that prints "Hello World" to the UEFI console.

	This example serves as the foundation for understanding UEFI applications, bootkits, and low-level development within the UEFI pre-boot environment.


	@note

	Instructions to Compile and Run this UEFI Application (in a Virtual Machine using VMware Workstation):

	1. Open the file 'Conf/target.txt' located in the root of your EDK2 directory. Modify the following line to point to this DSC file:
		ACTIVE_PLATFORM = UefiBootkit001_HelloWorldPkg/UefiApplication_HelloWorld.dsc

	2. Open a command prompt and set up the EDK2 environment:
		> edksetup.bat Rebuild

	3. Build the application:
		> build

	4. Once built successfully, locate the generated .EFI binary at:
		C:\edk2\Build\UefiBootkit001_HelloWorldPkg\DEBUG_VS2019\X64\UefiBootkit001_HelloWorld.efi

	5. To test the application:

		a. If you're already inside a Windows virtual machine (VM):

			- Mount the EFI System Partition (ESP):
				> mountvol U: /S

			- Copy the generated .efi binary to the EFI\Boot\Bootkits directory:
				> mkdir U:\EFI\Boot\Bootkits
				> copy C:\edk2\Build\UefiBootkit001_HelloWorldPkg\DEBUG_VS2019\X64\UefiBootkit001_HelloWorld.efi U:\EFI\Boot\Bootkits\UefiBootkit001_HelloWorld.efi

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
				Shell> UefiBootkit001_HelloWorld.efi

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
				Shell> cp fsnt0:\UefiBootkit001_HelloWorld.efi .
				Shell> dir
				Shell> UefiBootkit001_HelloWorld.efi
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiApplicationEntryPoint.h
// Module entry point library for UEFI Applications.
#include <Library/UefiApplicationEntryPoint.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



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
	Print(L"Hello World from UefiBootkit001_HelloWorld (UEFI Application)\r\n");
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------