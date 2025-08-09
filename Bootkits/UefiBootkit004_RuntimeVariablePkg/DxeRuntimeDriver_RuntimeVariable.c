// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	DxeRuntimeDriver_RuntimeVariable.c

	A minimal DXE Runtime Driver that writes a UEFI variable visible to the OS.

	This example serves as a foundation for understanding DXE runtime drivers, their persistent execution after ExitBootServices, and how they can be leveraged for developing bootkits or enabling low-level runtime interaction with UEFI services.


	@note

	Instructions to Compile and Run this DXE Runtime Driver (in a Virtual Machine using VMware Workstation):

	1. Open the file 'Conf/target.txt' located in the root of your EDK2 directory. Modify the following line to point to this DSC file:
		ACTIVE_PLATFORM = UefiBootkit004_RuntimeVariablePkg/DxeRuntimeDriver_RuntimeVariable.dsc

	2. Open a command prompt and set up the EDK2 environment:
		> edksetup.bat Rebuild

	3. Build the driver:
		> build

	4. Once built successfully, locate the generated .EFI binary at:
		C:\edk2\Build\UefiBootkit004_RuntimeVariablePkg\DEBUG_VS2019\X64\UefiBootkit004_RuntimeVariable.efi

	5. To test the driver:

		a. If you're already inside a Windows virtual machine (VM):

			- Mount the EFI System Partition (ESP):
				> mountvol U: /S

			- Copy the generated .efi binary to the EFI\Boot\Bootkits directory:
				> mkdir U:\EFI\Boot\Bootkits
				> copy C:\edk2\Build\UefiBootkit004_RuntimeVariablePkg\DEBUG_VS2019\X64\UefiBootkit004_RuntimeVariable.efi U:\EFI\Boot\Bootkits\UefiBootkit004_RuntimeVariable.efi

			- Dismount the ESP:
				> mountvol U: /D

			- Shut down the VM

			- Power on the VM directly to firmware:
				Right-click the VM -> Power -> Power On to Firmware

			- From the firmware menu, select 'EFI Internal Shell', then press 'Enter'.

			- Run the driver in the shell:
				Shell> map -r
				Shell> fs0:
				Shell> dir
				Shell> cd EFI\Boot\Bootkits
				Shell> dir
				Shell> load UefiBootkit004_RuntimeVariable.efi

			- Then, launch Windows:
				Option 1 – Launch your chainloader:
					Shell> UefiBootkit003_BootmgfwLauncher.efi

				Option 2 – Exit the shell and manually select 'Windows Boot Manager' from the firmware boot menu.

			- Once Windows boots, read the runtime variable using the provided utility:
				> ReadUefiVariables.exe UefiBootkit004_RuntimeVariable {00000000-0000-0000-0000-000000000000}

		b. If testing from outside the VM:

			- Create a shared folder between the host and guest, and place the .efi file inside it.

			- Power on the VM directly to firmware:
				Right-click the VM -> Power -> Power On to Firmware

			- From the firmware menu, select 'EFI Internal Shell', then press 'Enter'.

			- Run the driver in the shell:
				Shell> map -r
				Shell> fs0:
				Shell> dir
				Shell> cd EFI\Boot\Bootkits
				Shell> cp fsnt0:\UefiBootkit004_RuntimeVariable.efi .
				Shell> dir
				Shell> load UefiBootkit004_RuntimeVariable.efi

			- Then, launch Windows:
				Option 1 – Launch your chainloader:
					Shell> UefiBootkit003_BootmgfwLauncher.efi

				Option 2 – Exit the shell and manually select 'Windows Boot Manager' from the firmware boot menu.

			- Once Windows boots, read the runtime variable using the provided utility from the repository:
				> ReadUefiVariables.exe UefiBootkit004_RuntimeVariable {00000000-0000-0000-0000-000000000000}
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiDriverEntryPoint.h
// Module entry point library for UEFI drivers, DXE Drivers, DXE Runtime Drivers, and DXE SMM Drivers.
#include <Library/UefiDriverEntryPoint.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiRuntimeServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Runtime Services Table. This library does not contain any functions or macros. It simply exports the global variable gRT that is a pointer to the EFI Runtime Services Table as defined in the UEFI Specification. The global variable gRT must be preinitialized to NULL. The library constructor must set gRT to point at the EFI Runtime Services Table so it is available at the module's entry point. Since there is overhead in initializing this global variable, only those modules that actually require access to the EFI Runtime Services Table should use this library. Only available to DXE and UEFI module types.
#include <Library/UefiRuntimeServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Name of the UEFI variable
STATIC CONST CHAR16 *Global_VariableName = L"UefiBootkit004_RuntimeVariable";

// Unique vendor GUID for our variable (https://www.guidgen.com/)
STATIC EFI_GUID Global_VariableGuid = { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

// Value to write
STATIC CONST CHAR8 *Global_VariableValue = "UefiBootkit004_RuntimeVariable is Active";



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          The entry point for the DXE Runtime Driver.
	@details        This function is invoked by the UEFI firmware when the driver is loaded. It prints a message to the screen to indicate its execution and remains resident in memory even after the ExitBootServices call has been made. This persistence allows the driver to continue operating at runtime, after the operating system has taken control.


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
	Print(L"[INFO]  Hello World from UefiBootkit004_RuntimeVariable (DXE Runtime Driver)\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;


	// ---------------------------------------------------------------------------------------------------------------------
	// Set variable
	Print(L"[INFO]  Set variable\r\n");

	// Sets the value of a variable. This service can be used to create a new variable, modify the value of an existing variable, or to delete an existing variable.
	// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#setvariable
	Status = gRT->SetVariable((CHAR16 *)Global_VariableName, &Global_VariableGuid, EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS, AsciiStrLen(Global_VariableValue) + 1, (VOID *)Global_VariableValue);

	// Failed
	if (EFI_ERROR(Status))
	{
		Print(L"[ERROR] gRT->SetVariable failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	Print(L"[INFO]  Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------