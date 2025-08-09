#include <ntddk.h>


HANDLE gThread;
BOOLEAN gStop;


VOID ThreadProc(PVOID StartContext)
{
	UNREFERENCED_PARAMETER(StartContext);
	LARGE_INTEGER iv;
	iv.QuadPart = -30 * 1000 * 1000 * 10;
	while (!gStop)
	{
		DbgPrint("KernelModeDriver_HelloThreading -> Hello from thread (every 30s)\r\n");
		DbgPrint("KernelModeDriver_HelloThreading -> Do you want to develop your own rootkit??? Check out these two repositories:\r\n");
		DbgPrint("KernelModeDriver_HelloThreading -> https://github.com/TheMalwareGuardian/Awesome-Bootkits-Rootkits-Development\r\n");
		DbgPrint("KernelModeDriver_HelloThreading -> https://github.com/TheMalwareGuardian/Benthic\r\n");
		KeDelayExecutionThread(KernelMode, FALSE, &iv);
	}
	PsTerminateSystemThread(STATUS_SUCCESS);
}


VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	gStop = TRUE;
	if (gThread)
	{
		KeWaitForSingleObject(gThread, Executive, KernelMode, FALSE, NULL);
		ZwClose(gThread);
	}
}


NTSTATUS CustomDriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = DriverUnload;
	PsCreateSystemThread(&gThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc, NULL);
	return STATUS_SUCCESS;
}