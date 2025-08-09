// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



#include <windows.h>
#include <stdio.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include "Utils00Help.h"



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



// Prints the tool banner
void UtilsHelp_ShowBanner()
{
	// -----------------------------------------------------------------------------------------------------------------
	// User-Client logo
	// https://patorjk.com/software/taag
	wprintf(L"#-----------------------------------------------------------------------------------------\n");
	wprintf(
		L"\n"
		L"     ____ _  __ ______   ____              __  _                   ____       _                    __  __                     _________            __ \n"
		L"    / __ \\ |/ // ____/  / __ \\__  ______  / /_(_)___ ___  ___     / __ \\_____(_)   _____  _____   / / / /_______  _____      / ____/ (_)__  ____  / /_\n"
		L"   / / / /   // __/    / /_/ / / / / __ \\/ __/ / __ `__ \\/ _ \\   / / / / ___/ / | / / _ \\/ ___/  / / / / ___/ _ \\/ ___/_____/ /   / / / _ \\/ __ \\/ __/\n"
		L"  / /_/ /   |/ /___   / _, _/ /_/ / / / / /_/ / / / / / /  __/  / /_/ / /  / /| |/ /  __/ /     / /_/ (__  )  __/ /  /_____/ /___/ / /  __/ / / / /_  \n"
		L" /_____/_/|_/_____/  /_/ |_|\\__,_/_/ /_/\\__/_/_/ /_/ /_/\\___/  /_____/_/  /_/ |___/\\___/_/      \\____/____/\\___/_/         \\____/_/_/\\___/_/ /_/\\__/  \n"
		L"\n"
		L"                                                                                                         Bootkit2_Runtime_DXERuntimeDriver_UserClient\n"
		L"\n"
		L"NOTE: This client is designed to easily load a rootkit (malicious kernel driver) by communicating directly with the DXE Runtime Driver, capable of reading, writing, and executing code in memory. Update this base and be creative: patch, hook, and modify anything in kernel space.\n\n"
	);
	wprintf(L"#-----------------------------------------------------------------------------------------\n");
}



// Returns the executable's file name only (path stripped)
wchar_t* UtilsHelp_GetExeName(void)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	static wchar_t exeName[MAX_PATH] = L"<tool>";
	wchar_t fullPath[MAX_PATH];


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the full path of the running executable

	// Retrieves the fully qualified path for the file that contains the specified module. The module must have been loaded by the current process. To locate the file for a module that was loaded by another process, use the GetModuleFileNameEx function.
	// https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew
	DWORD len = GetModuleFileNameW(NULL, fullPath, MAX_PATH);


	// ---------------------------------------------------------------------------------------------------------------------
	// Ensure the call succeeded and did not overflow the buffer
	if (len && len < MAX_PATH)
	{
		// -----------------------------------------------------------------------------------------------------------------
		// Search for the last backslash to isolate the file name

		// Scans a string for the last occurrence of a character.
		// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strrchr-wcsrchr-mbsrchr-mbsrchr-l?view=msvc-170
		const wchar_t* base = wcsrchr(fullPath, L'\\');
		if (base)
		{
			// skip the back-slash
			++base;
		}
		else
		{
			// no slash means the whole string is the name
			base = fullPath;
		}

		// -----------------------------------------------------------------------------------------------------------------
		// Copy the base name into the static buffer, safely

		// Copies characters of one string to another. These versions of strncpy, _strncpy_l, wcsncpy, _wcsncpy_l, _mbsncpy, _mbsncpy_l have security enhancements, as described in Security features in the CRT.
		// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strncpy-s-strncpy-s-l-wcsncpy-s-wcsncpy-s-l-mbsncpy-s-mbsncpy-s-l?view=msvc-170
		wcsncpy_s(exeName, MAX_PATH, base, _TRUNCATE);
	}


	// -----------------------------------------------------------------------------------------------------------------
	// Return pointer to the static name buffer
	return exeName;
}



// Prints a dynamic usage block that adapts to the exe name
void UtilsHelp_ShowUsage()
{
	// -----------------------------------------------------------------------------------------------------------------
	// Get the actual name of the current executable to display in usage lines
	const wchar_t* exe = UtilsHelp_GetExeName();


	// -----------------------------------------------------------------------------------------------------------------
	// Print usage syntax and available commands, inserting the current exe name
	wprintf(L"#-----------------------------------------------------------------------------------------\n");
	wprintf(L"Usage:\n");
	wprintf(L"  %s help                      Show this help message\n", exe);
	wprintf(L"  %s presence                  Check if DXE Runtime Driver is alive\n", exe);
	wprintf(L"  %s buffer-info               Get buffer start/end addresses\n", exe);
	wprintf(L"  %s tag                       Write predefined tag at start of buffer\n", exe);
	wprintf(L"  %s read <addr> <bytes>       Read x bytes from kernel address\n", exe);
	wprintf(L"  %s print                     Execute kernel-mode DbgPrint via shellcode\n", exe);
	wprintf(L"  %s load <driver.sys>         Load and run DriverEntry from a .sys file\n", exe);
	wprintf(L"#-----------------------------------------------------------------------------------------\n");
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
