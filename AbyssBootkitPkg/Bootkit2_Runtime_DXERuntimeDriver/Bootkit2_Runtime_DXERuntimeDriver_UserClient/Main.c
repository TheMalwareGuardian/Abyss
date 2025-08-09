// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



#include <windows.h>
#include <stdio.h>
#include <string.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include "Functions/Utils/Utils01Privileges.h"
#include "Functions/Utils/Utils00Help.h"
#include "Functions/Functions00Status.h"
#include "Functions/Functions01Read.h"
#include "Functions/Functions02Write.h"
#include "Functions/Functions03Call.h"
#include "Functions/Functions04Driver.h"



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



// Main
int wmain(int argc, wchar_t* argv[])
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Try to enable SeSystemEnvironmentPrivilege
	if (!UtilsPrivilege_EnsureSeSystemEnvironmentPrivilege())
	{
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Banner
	UtilsHelp_ShowBanner();


	// ---------------------------------------------------------------------------------------------------------------------
	// No arguments
	if (argc < 2)
	{
		UtilsHelp_ShowUsage();
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Help
	if (wcscmp(argv[1], L"--help") == 0 || wcscmp(argv[1], L"help") == 0)
	{
		UtilsHelp_ShowUsage();
		return 0;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Presence Check
	else if (wcscmp(argv[1], L"presence") == 0)
	{
		wprintf(L"[*] Command Selected : presence - Checking DXE Runtime Driver heartbeat...\n");
		return FunctionsStatus_CommandPresence();
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Buffer Info
	else if (wcscmp(argv[1], L"buffer-info") == 0)
	{
		wprintf(L"[*] Command selected : buffer-info - Querying DXE Runtime Driver buffer boundaries...\n");
		return FunctionsStatus_CommandBufferInfo();
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Tag
	else if (wcscmp(argv[1], L"tag") == 0)
	{
		wprintf(L"[*] Command selected : tag - Writing identification tag at buffer start...\n");
		return FunctionsWrite_CommandWriteTag();
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Read Kernel Address
	if (wcscmp(argv[1], L"read") == 0)
	{
		if (argc != 4)
		{
			wprintf(L"[!] Missing arguments. Use: %s help\n", UtilsHelp_GetExeName());
			return 1;
		}
		ULONGLONG addr = _wcstoui64(argv[2], NULL, 0);
		DWORD size = _wtoi(argv[3]);
		if (addr == 0 || size == 0)
		{
			wprintf(L"[!] Invalid arguments.\n");
			return 1;
		}
		wprintf(L"[*] Command selected : read - Dumping 0x%X bytes from 0x%llX...\n", size, addr);
		return FunctionsRead_CommandReadMemory(addr, size);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// DbgPrint message
	else if (wcscmp(argv[1], L"print") == 0)
	{
		wprintf(L"[*] Command selected : print - Executing kernel-mode DbgPrint shellcode...\n");
		return FunctionsCall_CommandDbgPrint();
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Load driver
	else if (wcscmp(argv[1], L"load") == 0)
	{
		if (argc != 3)
		{
			wprintf(L"[!] Missing arguments. Use: %s help\n", UtilsHelp_GetExeName());
			return 1;
		}
		wprintf(L"[*] Command selected : load - Loading driver '%s' into kernel memory...\n", argv[2]);
		return FunctionsDriver_CommandLoadDriver(argv[2]);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Unknown command
	else
	{
		wprintf(L"[!] Unknown command: %s\n", argv[1]);
		UtilsHelp_ShowUsage();
		return 1;
	}
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
