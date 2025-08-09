#include <ntddk.h>
#include <ntstrsafe.h>


#define DEVICE_NAME L"\\Device\\KernelModeDriverMapper"
#define SYMLINK_NAME L"\\??\\KernelModeDriverMapper"
#define THREAD_INTERVAL_SEC 30


#define IOCTL_PRINT_HELLO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_LOAD_DRIVER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_DATA)


static HANDLE gThread = NULL;
static BOOLEAN gStopThread = FALSE;
static UNICODE_STRING gSymLink;


VOID PrintThread(PVOID StartContext)
{
	UNREFERENCED_PARAMETER(StartContext);

	LARGE_INTEGER iv;
	iv.QuadPart = -(LONGLONG)THREAD_INTERVAL_SEC * 10 * 1000 * 1000;

	while (!gStopThread)
	{
		DbgPrint("KernelModeDriverMapper -> Hello from thread (every 30s)\n");
		KeDelayExecutionThread(KernelMode, FALSE, &iv);
	}
	PsTerminateSystemThread(STATUS_SUCCESS);
}


NTSTATUS CreateServiceKeyAndLoad(PCWSTR DosPath)
{
	WCHAR ntPathBuf[512];
	RtlStringCchPrintfW(ntPathBuf, RTL_NUMBER_OF(ntPathBuf), (DosPath[0] == L'\\') ? L"%ws" : L"\\??\\%ws", DosPath);

	PCWSTR fileName = wcsrchr(ntPathBuf, L'\\');
	fileName = fileName ? fileName + 1 : ntPathBuf;

	WCHAR svcName[260];
	wcsncpy_s(svcName, RTL_NUMBER_OF(svcName), fileName, _TRUNCATE);

	WCHAR *dot = wcsrchr(svcName, L'.');
	if (!dot || _wcsicmp(dot, L".sys") != 0)
	{
		return STATUS_INVALID_PARAMETER;
	}
	*dot = L'\0';

	WCHAR regBuf[512];
	RtlStringCchPrintfW(regBuf, RTL_NUMBER_OF(regBuf), L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\%ws", svcName);

	UNICODE_STRING regUS;
	OBJECT_ATTRIBUTES oa;
	RtlInitUnicodeString(&regUS, regBuf);
	InitializeObjectAttributes(&oa, &regUS, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	HANDLE hKey;
	NTSTATUS st = ZwCreateKey(&hKey, KEY_WRITE, &oa, 0, NULL, REG_OPTION_NON_VOLATILE, NULL);
	if (!NT_SUCCESS(st))
	{
		return st;
	}

	ULONG typeVal = 1, startVal = 3;
	UNICODE_STRING name;

	RtlInitUnicodeString(&name, L"Type");
	ZwSetValueKey(hKey, &name, 0, REG_DWORD, &typeVal, sizeof(typeVal));

	RtlInitUnicodeString(&name, L"Start");
	ZwSetValueKey(hKey, &name, 0, REG_DWORD, &startVal, sizeof(startVal));

	RtlInitUnicodeString(&name, L"ImagePath");
	ZwSetValueKey(hKey, &name, 0, REG_EXPAND_SZ, ntPathBuf, (ULONG)((wcslen(ntPathBuf) + 1) * sizeof(WCHAR)));

	ZwClose(hKey);

	st = ZwLoadDriver(&regUS);
	DbgPrint("KernelModeDriverMapper -> ZwLoadDriver(%ws) status=0x%08X\n", svcName, st);
	return st;
}


NTSTATUS DispatchCreateClose(PDEVICE_OBJECT DevObj, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DevObj);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}


NTSTATUS DispatchDeviceControl(PDEVICE_OBJECT DevObj, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DevObj);
	PIO_STACK_LOCATION sp = IoGetCurrentIrpStackLocation(Irp);
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

	if (sp->Parameters.DeviceIoControl.IoControlCode == IOCTL_PRINT_HELLO)
	{
		DbgPrint("KernelModeDriverMapper -> IOCTL_PRINT_HELLO received!\n");
		DbgPrint("KernelModeDriverMapper -> Do you want to develop your own rootkit??? Check out:\n");
		DbgPrint("KernelModeDriverMapper -> https://github.com/TheMalwareGuardian/Awesome-Bootkits-Rootkits-Development\n");
		DbgPrint("KernelModeDriverMapper -> https://github.com/TheMalwareGuardian/Benthic\n");
		status = STATUS_SUCCESS;
	}
	else if (sp->Parameters.DeviceIoControl.IoControlCode == IOCTL_LOAD_DRIVER)
	{
		if (Irp->AssociatedIrp.SystemBuffer &&
			sp->Parameters.DeviceIoControl.InputBufferLength > sizeof(WCHAR))
		{
			PWCHAR path = (PWCHAR)Irp->AssociatedIrp.SystemBuffer;
			path[(sp->Parameters.DeviceIoControl.InputBufferLength / sizeof(WCHAR)) - 1] = L'\0';

			status = CreateServiceKeyAndLoad(path);
			*(NTSTATUS*)Irp->AssociatedIrp.SystemBuffer = status;
			Irp->IoStatus.Information = sizeof(NTSTATUS);
		}
		else
		{
			status = STATUS_INVALID_PARAMETER;
		}
	}

	Irp->IoStatus.Status = status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}


VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	gStopThread = TRUE;
	if (gThread)
	{
		KeWaitForSingleObject(gThread, Executive, KernelMode, FALSE, NULL);
		ZwClose(gThread);
	}
	IoDeleteSymbolicLink(&gSymLink);
	IoDeleteDevice(DriverObject->DeviceObject);
	DbgPrint("KernelModeDriverMapper -> Driver unloaded\n");
}


NTSTATUS CustomDriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName, DEVICE_NAME);
	RtlInitUnicodeString(&gSymLink, SYMLINK_NAME);

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

	st = PsCreateSystemThread(&gThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, PrintThread, NULL);

	if (NT_SUCCESS(st))
	{
		DbgPrint("KernelModeDriverMapper -> Driver loaded successfully\n");
	}

	return st;
}