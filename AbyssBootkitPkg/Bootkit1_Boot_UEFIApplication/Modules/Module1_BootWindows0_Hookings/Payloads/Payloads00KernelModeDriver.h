// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



#pragma once



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



/**
	How to generate this binary array from a .SYS file
	--------------------------------------------------

	1. Create a new "Kernel Mode Driver, Empty (KMDF)" or "Empty WDM Driver" project in Visual Studio 2022.

	2. Go to:
		- Right-click Project -> Properties -> Linker -> Advanced -> Entry Point -> Replace default "DriverEntry" with "CustomDriverEntry"

	3. Minimal driver code (no WDF):

'''
#include <ntddk.h>

NTSTATUS
CustomDriverEntry(
	_In_        PDRIVER_OBJECT          pDriverObject,
	_In_        PUNICODE_STRING         pRegistryPath
)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(pRegistryPath);

	DbgPrint("Hello");
	DbgPrint("Do you want to develop your own rootkit??? Check out these two repositories:\r\n");
	DbgPrint("https://github.com/TheMalwareGuardian/Awesome-Bootkits-Rootkits-Development\r\n");
	DbgPrint("https://github.com/TheMalwareGuardian/Benthic\r\n");

	return STATUS_SUCCESS;
}
'''

	4. Configuration:
		- Configuration: Release x64

	5. Build the project and locate the compiled '.sys' in 'YourProject\x64\Release\YourDriver.sys'

	6. Convert the SYS file to a C array using HxD:
		- Download HxD: https://mh-nexus.de/en/hxd/
		- File -> Open -> Select '.sys'
		- File -> Export -> C -> Save as '.sys.c'

	7. Copy the contents of the exported array and paste it here:
		- The driver will now be embedded in this binary buffer.
**/


// Declares a raw kernel-mode driver binary payload, to be defined in a corresponding .c file. This payload is used during boot to map and execute a custom driver directly from memory.
extern const UINT8 Global_BootWindowsHookings_PayloadsKernelModeDriver_KernelModeDriver[];



// Size of the PE image based on headers (used to allocate memory for mapping the kernel-mode driver payload)
#define MACRO_BOOTWINDOWSHOOKINGS_PAYLOADSKERNELMODEDRIVER_KERNELMODEDRIVERHELLOTHREADING_IMAGE_SIZE (((EFI_IMAGE_NT_HEADERS64 *)(Global_BootWindowsHookings_PayloadsKernelModeDriver_KernelModeDriver + ((EFI_IMAGE_DOS_HEADER *)Global_BootWindowsHookings_PayloadsKernelModeDriver_KernelModeDriver)->e_lfanew))->OptionalHeader.SizeOfImage)



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
