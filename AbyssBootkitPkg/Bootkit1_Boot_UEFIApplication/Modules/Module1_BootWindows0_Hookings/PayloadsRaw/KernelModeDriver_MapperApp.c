#include <windows.h>
#include <stdio.h>


#define IOCTL_PRINT_HELLO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_LOAD_DRIVER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_DATA)


#define NT_SUCCESS(Status) (((LONG)(Status)) >= 0)


int wmain(int argc, wchar_t* argv[])
{
	if (!(argc == 2 && _wcsicmp(argv[1], L"hello") == 0) && !(argc == 3 && _wcsicmp(argv[1], L"load") == 0))
	{
		wprintf(L"Usage:\n");
		wprintf(L"  %s hello\n", argv[0]);
		wprintf(L"  %s load <PathToDriver.sys>\n", argv[0]);
		return 1;
	}

	HANDLE h = CreateFileW(L"\\\\.\\KernelModeDriverMapper", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (h == INVALID_HANDLE_VALUE)
	{
		wprintf(L"[-] Failed to open device: %lu\n", GetLastError());
		return 1;
	}

	DWORD bytesReturned = 0;
	BOOL ok = FALSE;
	DWORD drvStatus = 0;

	if (_wcsicmp(argv[1], L"hello") == 0)
	{
		ok = DeviceIoControl(h, IOCTL_PRINT_HELLO, NULL, 0, NULL, 0, &bytesReturned, NULL);
	}
	else if (_wcsicmp(argv[1], L"load") == 0)
	{
		WCHAR fullPath[MAX_PATH];
		if (!GetFullPathNameW(argv[2], MAX_PATH, fullPath, NULL))
		{
			wprintf(L"[-] Failed to resolve full path for '%s' (err=%lu)\n", argv[2], GetLastError());
			CloseHandle(h);
			return 1;
		}

		ok = DeviceIoControl(h, IOCTL_LOAD_DRIVER, fullPath, (DWORD)((wcslen(fullPath) + 1) * sizeof(WCHAR)), &drvStatus, sizeof(drvStatus), &bytesReturned, NULL);
	}

	if (!ok)
	{
		wprintf(L"[-] DeviceIoControl failed (err=%lu)\n", GetLastError());
	}
	else
	{
		wprintf(L"[+] IOCTL sent successfully\n");
		if (_wcsicmp(argv[1], L"load") == 0)
		{
			wprintf(L"[+] ZwLoadDriver returned: 0x%08X (%s)\n", drvStatus, NT_SUCCESS(drvStatus) ? L"SUCCESS" : L"FAIL");
		}
	}

	CloseHandle(h);
	return 0;
}