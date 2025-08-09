#include <Windows.h>
#include <stdio.h>


#define IOCTL_PRINT_HELLO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)


int main()
{
	HANDLE hDevice = CreateFileW(L"\\\\.\\KernelModeDriverIOCTLs", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("[!] Failed to open driver: 0x%lx\n", GetLastError());
		return 1;
	}

	DWORD bytesReturned;
	BOOL ok = DeviceIoControl(hDevice, IOCTL_PRINT_HELLO, NULL, 0, NULL, 0, &bytesReturned, NULL);
	if (!ok)
	{
		printf("[!] IOCTL failed: 0x%lx\n", GetLastError());
		CloseHandle(hDevice);
		return 1;
	}

	printf("[+] IOCTL_PRINT_HELLO sent successfully.\n");
	CloseHandle(hDevice);
	return 0;
}