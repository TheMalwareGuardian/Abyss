// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



#include <windows.h>
#include <stdio.h>
#include <psapi.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include "Utils03Communication.h"
#include "../../Globals/Globals01Operations.h"
#include "../../Globals/Globals00Communication.h"
#include "../../Structures/Structures00Execution.h"



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



// Send command to DXE Runtime Driver
BOOL UtilsCommunication_SendCommand(STRUCTURES_EXECUTION_MEMORYCOMMAND* cmd, const wchar_t* failMsg)
{
	// -----------------------------------------------------------------------------------------------------------------
	// Send the command to the DXE Runtime Driver via SetFirmwareEnvironmentVariableExW

	// Sets the value of the specified firmware environment variable and the attributes that indicate how this variable is stored and maintained.
	// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setfirmwareenvironmentvariableexw
	BOOL status = SetFirmwareEnvironmentVariableExW(MACRO_GLOBALSOPERATIONS_VARIABLE_NAME, MACRO_GLOBALSCOMMUNICATION_EFI_VARIABLE_GUID, cmd, sizeof(*cmd), MACRO_GLOBALSCOMMUNICATION_EFI_VARIABLE_NON_VOLATILE | MACRO_GLOBALSCOMMUNICATION_EFI_VARIABLE_BOOTSERVICE_ACCESS | MACRO_GLOBALSCOMMUNICATION_EFI_VARIABLE_RUNTIME_ACCESS);


	// -----------------------------------------------------------------------------------------------------------------
	// If the DXE Runtime Driver did not update the status field, assume it is not running
	if (cmd->data[0] != 0x7 && failMsg)
	{
		wprintf(L"[-] %s (err=DXE Runtime Driver unavailable)\n", failMsg);
		return FALSE;
	}


	// -----------------------------------------------------------------------------------------------------------------
	// If the call failed and an error message was provided, print it with the error code
	if (!status && failMsg)
	{
		wprintf(L"[-] %s (err=%lu)\n", failMsg, GetLastError());
	}


	// -----------------------------------------------------------------------------------------------------------------
	// Return
	return status;
}



// Request presence from DXE Runtime Driver
BOOL UtilsCommunication_RequestDriverPresence(char* outMsg, DWORD bufSize)
{
	// -----------------------------------------------------------------------------------------------------------------
	// Build STRUCTURES_EXECUTION_MEMORYCOMMAND for opcode (Driver Presence)

	// Initialise command structure with all fields set to zero
	STRUCTURES_EXECUTION_MEMORYCOMMAND cmdDriverPresence = { 0 };

	// Set magic value to identify the command as valid and trusted
	cmdDriverPresence.magic = MACRO_GLOBALSOPERATIONS_COMMAND_MAGIC;

	// Set operation ID to check if the driver is alive
	cmdDriverPresence.operation = GLOBALS_OPERATIONS_COMMANDID_OP_DRIVER_PRESENCE;

	// Pass pointer to user buffer where the driver will copy its status string
	cmdDriverPresence.data[1] = (ULONGLONG)(ULONG_PTR)outMsg;

	// Tell the driver how many bytes it may write
	cmdDriverPresence.size = bufSize;


	// -----------------------------------------------------------------------------------------------------------------
	// Send command to DXE Runtime Driver
	return UtilsCommunication_SendCommand(&cmdDriverPresence, L"Driver Presence request failed");
}



// Request buffer boundaries from DXE Runtime Driver
BOOL UtilsCommunication_RequestGetBufferBounds(ULONGLONG* bufferStart, ULONGLONG* bufferEnd)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Build STRUCTURES_EXECUTION_MEMORYCOMMAND for opcode (Get Buffer Bounds)

	// Initialize command structure with all fields set to zero
	STRUCTURES_EXECUTION_MEMORYCOMMAND cmdGetBufferBounds = { 0 };

	// Set magic value to identify the command as valid and trusted
	cmdGetBufferBounds.magic = MACRO_GLOBALSOPERATIONS_COMMAND_MAGIC;

	// Set operation ID to request buffer boundaries
	cmdGetBufferBounds.operation = GLOBALS_OPERATIONS_COMMANDID_OP_GET_BUFFER_BOUNDS;

	// Pass pointer to variable where driver will write buffer start address
	cmdGetBufferBounds.data[1] = (ULONGLONG)(ULONG_PTR)bufferStart;

	// Pass pointer to variable where driver will write buffer end address
	cmdGetBufferBounds.data[2] = (ULONGLONG)(ULONG_PTR)bufferEnd;


	// ---------------------------------------------------------------------------------------------------------------------
	// Send command to DXE Runtime Driver
	return UtilsCommunication_SendCommand(&cmdGetBufferBounds, L"Get buffer bounds request failed");
}



// Request memory read from DXE Runtime Driver
BOOL UtilsCommunication_RequestReadMemory(ULONGLONG srcAddr, BYTE* userBuf, DWORD size)
{
	// -----------------------------------------------------------------------------------------------------------------
	// Build STRUCTURES_EXECUTION_MEMORYCOMMAND for opcode (Read Memory)

	// Initialise command structure with all fields set to zero
	STRUCTURES_EXECUTION_MEMORYCOMMAND cmdReadMemory = { 0 };

	// Set magic value to identify the command as valid and trusted
	cmdReadMemory.magic = MACRO_GLOBALSOPERATIONS_COMMAND_MAGIC;

	// Set operation ID to request a kernel-to-user copy
	cmdReadMemory.operation = GLOBALS_OPERATIONS_COMMANDID_OP_READ_MEMORY;

	// Provide the kernel virtual address where the read starts
	cmdReadMemory.data[1] = srcAddr;

	// Pass the user-mode destination buffer
	cmdReadMemory.data[2] = (ULONGLONG)(ULONG_PTR)userBuf;

	// Number of bytes to read
	cmdReadMemory.size = (int)size;


	// -----------------------------------------------------------------------------------------------------------------
	// Send command to DXE Runtime Driver
	return UtilsCommunication_SendCommand(&cmdReadMemory, L"Read memory request failed");
}



// Request to write tag into DXE Runtime Driver buffer
BOOL UtilsCommunication_RequestWriteTag()
{
	// -----------------------------------------------------------------------------------------------------------------
	// Build STRUCTURES_EXECUTION_MEMORYCOMMAND for opcode (Write Tag)

	// Initialise command structure with all fields set to zero
	STRUCTURES_EXECUTION_MEMORYCOMMAND cmdWriteTag = { 0 };

	// Set magic value to identify the command as valid and trusted
	cmdWriteTag.magic = MACRO_GLOBALSOPERATIONS_COMMAND_MAGIC;

	// Set operation ID to stamp the tag
	cmdWriteTag.operation = GLOBALS_OPERATIONS_COMMANDID_OP_WRITE_TAG;

	// (No extra data / size needed for this opcode)


	// -----------------------------------------------------------------------------------------------------------------
	// Send command to DXE Runtime Driver
	return UtilsCommunication_SendCommand(&cmdWriteTag, L"Tag write request failed");
}



// Request to write data into DXE Runtime Driver buffer
BOOL UtilsCommunication_RequestWriteInBuffer(ULONGLONG dst, BYTE* src, DWORD size)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Build STRUCTURES_EXECUTION_MEMORYCOMMAND for opcode (Write in Buffer)

	// Initialize command structure with all fields set to zero
	STRUCTURES_EXECUTION_MEMORYCOMMAND cmd = {0};

	// Set magic value to identify the command as valid and trusted
	cmd.magic = MACRO_GLOBALSOPERATIONS_COMMAND_MAGIC;

	// Set operation ID to write data into the buffer
	cmd.operation = GLOBALS_OPERATIONS_COMMANDID_OP_WRITE_IN_BUFFER;

	// Destination address in kernel memory
	cmd.data[1] = dst;

	// Pointer to source buffer
	cmd.data[2] = (ULONGLONG)(ULONG_PTR)src;

	// Total number of bytes to transfer
	cmd.size = size;


	// ---------------------------------------------------------------------------------------------------------------------
	// Send command to DXE Runtime Driver
	return UtilsCommunication_SendCommand(&cmd, L"Write in buffer request failed");
}



// Request execution of shellcode at specified address in DXE Runtime Driver buffer
BOOL UtilsCommunication_RequestCallFast(ULONGLONG entryPoint)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Build STRUCTURES_EXECUTION_MEMORYCOMMAND for opcode (Call Fast)

	// Initialize command structure with all fields set to zero
	STRUCTURES_EXECUTION_MEMORYCOMMAND cmd = { 0 };

	// Set magic value to identify the command as valid and trusted
	cmd.magic = MACRO_GLOBALSOPERATIONS_COMMAND_MAGIC;

	// Set operation ID to execute fastcall at the specified address
	cmd.operation = GLOBALS_OPERATIONS_COMMANDID_OP_CALL_FAST;

	// Address in kernel memory where the shellcode entry point resides
	cmd.data[1] = entryPoint;


	// ---------------------------------------------------------------------------------------------------------------------
	// Send command to DXE Runtime Driver
	return UtilsCommunication_SendCommand(&cmd, L"Fastcall execution request failed");
}



// Request DXE Runtime Driver to invoke DriverEntry at given address
BOOL UtilsCommunication_RequestDriverEntry(ULONGLONG driverEntryVA, ULONGLONG unicodeStructVA, ULONGLONG* outStatus)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Build STRUCTURES_EXECUTION_MEMORYCOMMAND for opcode (Driver Entry)

	// Initialize command structure with all fields set to zero
	STRUCTURES_EXECUTION_MEMORYCOMMAND cmd = {0};

	// Set magic value to identify the command as valid and trusted
	cmd.magic = MACRO_GLOBALSOPERATIONS_COMMAND_MAGIC;

	// Set operation ID to invoke DriverEntry in kernel
	cmd.operation = GLOBALS_OPERATIONS_COMMANDID_OP_DRIVER_ENTRY;

	// DriverEntry address
	cmd.data[1] = driverEntryVA;

	// Pointer to fake UNICODE_STRING
	cmd.data[2] = unicodeStructVA;

	// Reserved for receiving NTSTATUS result
	cmd.data[3] = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Send command to DXE Runtime Driver
	if (!UtilsCommunication_SendCommand(&cmd, L"DriverEntry execution failed"))
	{
		return FALSE;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Return NTSTATUS result to caller
	if (outStatus)
	{
		*outStatus = cmd.data[3];
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return TRUE;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
