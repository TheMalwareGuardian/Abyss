// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



// Name:                            ReadUefiVariables
// IDE:                             Open Visual Studio
// Template:                        Create a new project -> Search for templates (Alt + S) -> Empty Project -> Next
// Project:                         Project Name: ReadUefiVariables -> Solution Name: Utilities -> Create
// Source File:                     Source Files -> Add -> New Item... -> Main.c
// Source Code:                     Open Main.c and copy the corresponding source code
// Build Project:                   Set Configuration to Release, x64 -> Build -> Build Solution
// Locate Executable:               C:\Users\%USERNAME%\source\repos\Utilities\x64\Release\ReadUefiVariables.exe
// Test Execution:                  CMD (as Administrator) -> ReadUefiVariables.exe <VariableName> <GUID>
// Example:                         ReadUefiVariables.exe UefiBootkit004_RuntimeVariable {00000000-0000-0000-0000-000000000000}



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://en.wikibooks.org/wiki/Windows_Programming/windows.h
// The primary C header file for accessing the Win32 API is the <windows.h> header file. To make a Win32 executable, the first step is to include this header file in your source code. The windows.h header file should be included before any other library include, even the C standard library files such as stdio.h or stdlib.h. This is because the windows.h file includes macros and other components that may modify, extend, or replace things in these libraries. This is especially true when dealing with UNICODE, because windows.h will cause all the string functions to use UNICODE instead. Also, because many of the standard C library functions are already included in the Windows kernel, many of these functions will be available to the programmer without needing to load the standard libraries. For example, the function sprintf is included in windows.h automatically. 
#include <windows.h>

// https://en.wikibooks.org/wiki/C_Programming/stdio.h
// The C programming language provides many standard library functions for file input and output. These functions make up the bulk of the C standard library header <stdio.h>. The I/O functionality of C is fairly low-level by modern standards; C abstracts all file operations into operations on streams of bytes, which may be "input streams" or "output streams". Unlike some earlier programming languages, C has no direct support for random-access data files; to read from a record in the middle of a file, the programmer must create a stream, seek to the middle of the file, and then read bytes in sequence from the stream. 
#include <stdio.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief		Enables the SeSystemEnvironmentPrivilege for the current process. This is required to call GetFirmwareEnvironmentVariableA on Windows.


	@return		TRUE if privilege was successfully enabled, FALSE otherwise.
**/
BOOL EnableFirmwareEnvironmentPrivilege()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables

	// Handle to the current process access token
	HANDLE hToken;
	// Structure used to specify the privilege to adjust
	TOKEN_PRIVILEGES tkp;


	// ---------------------------------------------------------------------------------------------------------------------
	// Open the access token associated with the current process. Request permission to adjust privileges (TOKEN_ADJUST_PRIVILEGES) and to query the token (TOKEN_QUERY).

	// The OpenProcessToken function opens the access token associated with a process.
	// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocesstoken

	// Retrieves a pseudo handle for the current process.
	// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getcurrentprocess

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{

		// Retrieves the calling thread's last-error code value. The last-error code is maintained on a per-thread basis. Multiple threads do not overwrite each other's last-error code.
		// https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
		printf("[ERROR] OpenProcessToken failed (Error Code: %lu).\n", GetLastError());

		// Return
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Look up the LUID (Locally Unique Identifier) for the SeSystemEnvironmentPrivilege. This privilege allows a process to modify firmware environment variables.

	// The LookupPrivilegeValue function retrieves the locally unique identifier (LUID) used on a specified system to locally represent the specified privilege name.
	// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lookupprivilegevaluea
	if (!LookupPrivilegeValue(NULL, SE_SYSTEM_ENVIRONMENT_NAME, &tkp.Privileges[0].Luid))
	{

		// Retrieves the calling thread's last-error code value. The last-error code is maintained on a per-thread basis. Multiple threads do not overwrite each other's last-error code.
		// https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
		printf("[ERROR] LookupPrivilegeValue failed (Error Code: %lu).\n", GetLastError());

		// Closes an open object handle.
		// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		CloseHandle(hToken);

		// Return
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------

	// Set the privilege count to 1 (we are only enabling one privilege).
	tkp.PrivilegeCount = 1;

	// Enable the SeSystemEnvironmentPrivilege.
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;


	// ---------------------------------------------------------------------------------------------------------------------
	// Apply the privilege change to the current token. If successful, the current process can now access firmware environment variables.

	// The AdjustTokenPrivileges function enables or disables privileges in the specified access token. Enabling or disabling privileges in an access token requires TOKEN_ADJUST_PRIVILEGES access.
	// https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-adjusttokenprivileges
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{

		// Retrieves the calling thread's last-error code value. The last-error code is maintained on a per-thread basis. Multiple threads do not overwrite each other's last-error code.
		// https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
		printf("[ERROR] AdjustTokenPrivileges failed (Error Code: %lu).\n", GetLastError());

		// Closes an open object handle.
		// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		CloseHandle(hToken);

		// Return
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the privilege adjustment succeeded. AdjustTokenPrivileges returns TRUE even if it fails partially, so we use GetLastError.
	if (GetLastError() != ERROR_SUCCESS)
	{

		// Retrieves the calling thread's last-error code value. The last-error code is maintained on a per-thread basis. Multiple threads do not overwrite each other's last-error code.
		// https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
		printf("[ERROR] AdjustTokenPrivileges reported an error (Error Code: %lu).\n", GetLastError());

		// Closes an open object handle.
		// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
		CloseHandle(hToken);

		// Return
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Close the token handle.

	// Closes an open object handle.
	// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
	CloseHandle(hToken);


	// ---------------------------------------------------------------------------------------------------------------------
	// Privilege successfully enabled.
	return TRUE;
}



/**
	@brief		Reads a UEFI variable from NVRAM and prints its value.

	@details	This program takes two arguments from the command line:
	@details		- Variable name (string)
	@details		- Vendor GUID (in registry format, e.g. {00000000-0000-0000-0000-000000000000})

	@details	It uses the Windows API GetFirmwareEnvironmentVariableA to access the firmware variable.
**/
int main(int argc, char* argv[])
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Verify that the user has provided exactly two arguments. If not, print usage instructions and exit.
	if (argc != 3)
	{
		printf("Usage: %s <VariableName> <VendorGUID>\n", argv[0]);
		printf("Example: %s UefiBootkit004_RuntimeVariable {00000000-0000-0000-0000-000000000000}\n", argv[0]);
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Attempt to enable the SeSystemEnvironmentPrivilege. Without this, access to UEFI variables will be denied.
	if (!EnableFirmwareEnvironmentPrivilege())
	{
		printf("[ERROR] Could not enable SeSystemEnvironmentPrivilege. Exiting...\n");
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables

	// Extract the variable name and GUID from the arguments.
	LPCSTR varName = argv[1];
	LPCSTR guidStr = argv[2];

	// Define a buffer to hold the variable contents (up to 1024 bytes).
	CHAR buffer[1024] = { 0 };


	// ---------------------------------------------------------------------------------------------------------------------
	// Try to retrieve the firmware environment variable. If successful, the contents will be copied into 'buffer'.

	// Retrieves the value of the specified firmware environment variable.
	// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getfirmwareenvironmentvariablea
	DWORD result = GetFirmwareEnvironmentVariableA(varName, guidStr, buffer, sizeof(buffer));

	// Failed
	if (result == 0)
	{
		printf("[ERROR] GetFirmwareEnvironmentVariableA failed (Error Code: %lu).\n", GetLastError());
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Print the contents of the variable to stdout.
	printf("[INFO]  UEFI variable name:  %s\n", varName);
	printf("[INFO]  UEFI variable value: %s\n", buffer);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return 0;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------