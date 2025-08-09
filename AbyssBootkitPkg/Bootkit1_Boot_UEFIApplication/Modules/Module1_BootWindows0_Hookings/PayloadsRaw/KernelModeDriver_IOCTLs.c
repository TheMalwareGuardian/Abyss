#include <ntddk.h>


#define THREAD_INTERVAL_SEC 30
#define IOCTL_PRINT_HELLO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)


static HANDLE gThread = NULL;
static BOOLEAN gStopThread = FALSE;
static UNICODE_STRING gSymLink;


VOID PrintThread(PVOID StartContext)
{
	UNREFERENCED_PARAMETER(StartContext);

	LARGE_INTEGER interval;
	interval.QuadPart = -(LONGLONG)THREAD_INTERVAL_SEC * 10 * 1000 * 1000;

	while (!gStopThread)
	{
		DbgPrint("KernelModeDriverIOCTLs -> Hello from thread (every 30s)\n");
		KeDelayExecutionThread(KernelMode, FALSE, &interval);
	}

	PsTerminateSystemThread(STATUS_SUCCESS);
}


NTSTATUS DispatchCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}


NTSTATUS DispatchDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);

	if (stack->Parameters.DeviceIoControl.IoControlCode == IOCTL_PRINT_HELLO)
	{
		DbgPrint("KernelModeDriverIOCTLs -> IOCTL_PRINT_HELLO received!\n");
		DbgPrint("KernelModeDriverIOCTLs -> Do you want to develop your own rootkit??? Check out these two repositories:\r\n");
		DbgPrint("KernelModeDriverIOCTLs -> https://github.com/TheMalwareGuardian/Awesome-Bootkits-Rootkits-Development\r\n");
		DbgPrint("KernelModeDriverIOCTLs -> https://github.com/TheMalwareGuardian/Benthic\r\n");
	}

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}


VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	DbgPrint("KernelModeDriverIOCTLs -> DriverUnload\n");

	gStopThread = TRUE;
	if (gThread)
	{
		KeWaitForSingleObject(gThread, Executive, KernelMode, FALSE, NULL);
		ZwClose(gThread);
	}

	IoDeleteSymbolicLink(&gSymLink);
	IoDeleteDevice(DriverObject->DeviceObject);
}


NTSTATUS CustomDriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	DbgPrint("KernelModeDriverIOCTLs -> Driver loaded successfully!\n");

	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName, L"\\Device\\KernelModeDriverIOCTLs");

	RtlInitUnicodeString(&gSymLink, L"\\??\\KernelModeDriverIOCTLs");

	PDEVICE_OBJECT devObj = NULL;
	NTSTATUS st = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, FALSE, &devObj);
	if (!NT_SUCCESS(st))
	{
		return st;
	}

	st = IoCreateSymbolicLink(&gSymLink, &devName);
	if (!NT_SUCCESS(st))
	{
		IoDeleteDevice(devObj);
		return st;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;
	DriverObject->DriverUnload = DriverUnload;

	return PsCreateSystemThread(&gThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, PrintThread, NULL);
}