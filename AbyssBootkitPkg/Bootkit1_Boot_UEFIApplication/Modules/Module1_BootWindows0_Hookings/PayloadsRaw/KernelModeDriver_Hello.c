#include <ntddk.h>


NTSTATUS
CustomDriverEntry(
	_In_        PDRIVER_OBJECT          pDriverObject,
	_In_        PUNICODE_STRING         pRegistryPath
)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(pRegistryPath);

	DbgPrint("KernelModeDriver_Hello -> Hello\r\n");
	DbgPrint("KernelModeDriver_Hello -> Do you want to develop your own rootkit??? Check out these two repositories:\r\n");
	DbgPrint("KernelModeDriver_Hello -> https://github.com/TheMalwareGuardian/Awesome-Bootkits-Rootkits-Development\r\n");
	DbgPrint("KernelModeDriver_Hello -> https://github.com/TheMalwareGuardian/Benthic\r\n");

	return STATUS_SUCCESS;
}