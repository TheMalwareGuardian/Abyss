// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



#include <windows.h>
#include <stdio.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include "Utils01Privileges.h"



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



// Check if the current process is running with administrative rights
BOOL UtilsPrivilege_IsRunningAsAdmin(void)
{
	// -----------------------------------------------------------------------------------------------------------------
	// Variables
	HANDLE hToken = NULL;
	TOKEN_ELEVATION elevation = {0};
	DWORD dwSize = 0;
	BOOL isElevated = FALSE;


	// ---------------------------------------------------------------------------------------------------------------------
	// Open the current process token with TOKEN_QUERY rights

	// The OpenProcessToken function opens the access token associated with a process.
	// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocesstoken
	// Retrieves a pseudo handle for the current process.
	// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getcurrentprocess
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}


	// -----------------------------------------------------------------------------------------------------------------
	// Query token elevation status

	// The GetTokenInformation function retrieves a specified type of information about an access token. The calling process must have appropriate access rights to obtain the information. To determine if a user is a member of a specific group, use the CheckTokenMembership function. To determine group membership for app container tokens, use the CheckTokenMembershipEx function.
	// https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-gettokeninformation
	if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
	{
		isElevated = elevation.TokenIsElevated;
	}
	else
	{
		isElevated = FALSE;
	}


	// -----------------------------------------------------------------------------------------------------------------
	// Clean up resources
	if (hToken)
	{
		// Closes an open object handle.
		// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		CloseHandle(hToken);
	}


	// -----------------------------------------------------------------------------------------------------------------
	// Return
	return isElevated;
}



// Enable SeSystemEnvironmentPrivilege, required to call SetFirmwareEnvironmentVariableExW
BOOL UtilsPrivilege_EnableSeSystemEnvironmentPrivilege(LPCWSTR privilegeName)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	HANDLE token;
	TOKEN_PRIVILEGES tp;
	LUID luid;


	// ---------------------------------------------------------------------------------------------------------------------
	// Open the access token of the current process to modify its privileges

	// The OpenProcessToken function opens the access token associated with a process.
	// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocesstoken
	// Retrieves a pseudo handle for the current process.
	// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getcurrentprocess
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
	{
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Lookup the locally unique identifier (LUID) for the requested privilege (e.g., SeSystemEnvironmentPrivilege)

	// The LookupPrivilegeValue function retrieves the locally unique identifier (LUID) used on a specified system to locally represent the specified privilege name.
	// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lookupprivilegevaluew
	if (!LookupPrivilegeValueW(NULL, privilegeName, &luid))
	{
		// Closes an open object handle.
		// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		CloseHandle(token);
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Setup the TOKEN_PRIVILEGES structure to enable the privilege

	// Only one privilege to enable
	tp.PrivilegeCount = 1;
	// The LUID retrieved above
	tp.Privileges[0].Luid = luid;
	// Set it as enabled
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;


	// ---------------------------------------------------------------------------------------------------------------------
	// Apply the privilege change to the current process token

	// The AdjustTokenPrivileges function enables or disables privileges in the specified access token. Enabling or disabling privileges in an access token requires TOKEN_ADJUST_PRIVILEGES access.
	// https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-adjusttokenprivileges
	AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), NULL, NULL);


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the result of the privilege adjustment

	// Retrieves the calling thread's last-error code value. The last-error code is maintained on a per-thread basis. Multiple threads do not overwrite each other's last-error code.
	// https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
	DWORD err = GetLastError();


	// ---------------------------------------------------------------------------------------------------------------------
	// Close handle

	// Closes an open object handle.
	// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
	CloseHandle(token);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return TRUE only if the adjustment succeeded
	return (err == ERROR_SUCCESS);
}



// Ensures the process is running with administrative rights and enables SeSystemEnvironmentPrivilege, which is required for calling SetFirmwareEnvironmentVariableExW successfully.
BOOL UtilsPrivilege_EnsureSeSystemEnvironmentPrivilege(void)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Must run elevated
	if (!UtilsPrivilege_IsRunningAsAdmin())
	{
		wprintf(L"[!] This tool must be run as Administrator.\n");
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Enable SeSystemEnvironmentPrivilege
	if (!UtilsPrivilege_EnableSeSystemEnvironmentPrivilege(SE_SYSTEM_ENVIRONMENT_NAME))
	{
		wprintf(L"[-] Failed to enable SeSystemEnvironmentPrivilege.\n");
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Ready to call firmware variables
	wprintf(L"[+] SeSystemEnvironmentPrivilege enabled.\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return TRUE;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
