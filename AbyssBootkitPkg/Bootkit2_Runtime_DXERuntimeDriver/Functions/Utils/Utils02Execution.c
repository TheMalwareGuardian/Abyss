// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Execution
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiRuntimeServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Runtime Services Table. This library does not contain any functions or macros. It simply exports the global variable gRT that is a pointer to the EFI Runtime Services Table as defined in the UEFI Specification. The global variable gRT must be preinitialized to NULL. The library constructor must set gRT to point at the EFI Runtime Services Table so it is available at the module's entry point. Since there is overhead in initializing this global variable, only those modules that actually require access to the EFI Runtime Services Table should use this library. Only available to DXE and UEFI module types.
#include <Library/UefiRuntimeServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseLib.h
// Provides string functions, linked list functions, math functions, synchronization functions, file path functions, and CPU architecture-specific functions.
#include <Library/BaseLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
// Provides copy memory, fill memory, zero memory, and GUID functions. The Base Memory Library provides optimized implementations for common memory-based operations. These functions should be used in place of coding your own loops to do equivalent common functions. This allows optimized library implementations to help increase performance.
#include <Library/BaseMemoryLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Globals/Globals01Operations.h>
#include <Structures/Structures01Execution.h>
#include <Functions/Utils/Utils02Execution.h>
#include <Functions/Hooks/Hooks01RuntimeServices.h>



// START -> RUNTIME DATA BUFFER, KERNEL PAYLOAD REGION ----------------------------------------------------------------------------------------
// START -> RUNTIME DATA BUFFER, KERNEL PAYLOAD REGION ----------------------------------------------------------------------------------------



#pragma section(".text")
__declspec(allocate(".text")) char Global_UtilsExecution_DriverBuffer[MACRO_UTILSEXECUTION_DRIVER_SIZE];



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Executes a command received in a STRUCTURES_EXECUTION_MEMORYCOMMAND structure.
	@details        This function acts as a command dispatcher. It performs memory operations based on the field of the input STRUCTURES_EXECUTION_MEMORYCOMMAND.


	@param[in]      cmd                                          Pointer to a STRUCTURES_EXECUTION_MEMORYCOMMAND structure that describes the operation.


	@retval         EFI_SUCCESS                                  The operation was successfully handled.
	@retval         EFI_ACCESS_DENIED                            The command is invalid or not authorized.
	@retval         EFI_UNSUPPORTED                              The operation code is not recognized.
**/
EFI_STATUS
UtilsExecution_RunCommand(
	IN              STRUCTURES_EXECUTION_MEMORYCOMMAND*          cmd
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Validate input command: must not be NULL and must contain the expected magic number
	if (cmd == NULL || cmd->magic != MACRO_GLOBALSOPERATIONS_COMMAND_MAGIC)
	{
		return EFI_ACCESS_DENIED;
	}


	// -----------------------------------------------------------------------------------------------------------------
	// Mark the command as processed by the DXE Runtime Driver
	cmd->data[0] = 0x7;


	// ---------------------------------------------------------------------------------------------------------------------
	// Command dispatcher based on operation ID
	switch (cmd->operation)
	{

		// -----------------------------------------------------------------------------------------------------------------
		// Driver Presence Proof
		case GLOBALS_OPERATIONS_COMMANDID_OP_DRIVER_PRESENCE: {
			// Copies the message "DXE Runtime Driver is alive" string to the address provided in cmd->data[1]
			CHAR8* msg = "DXE Runtime Driver is alive";
			if (cmd->data[1])
			{
				UtilsMemory_CopyMemory((VOID*)(UINTN)cmd->data[1], msg, AsciiStrLen(msg)+1);
			}
			return EFI_SUCCESS;
		}

		// -----------------------------------------------------------------------------------------------------------------
		// Return Buffer Boundaries
		case GLOBALS_OPERATIONS_COMMANDID_OP_GET_BUFFER_BOUNDS: {
			// Returns the start and end addresses of the internal driver buffer
			*(UINTN*)cmd->data[1] = (UINTN)&Global_UtilsExecution_DriverBuffer;
			*(UINTN*)cmd->data[2] = (UINTN)&Global_UtilsExecution_DriverBuffer + MACRO_UTILSEXECUTION_DRIVER_SIZE;
			return EFI_SUCCESS;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Read
		case GLOBALS_OPERATIONS_COMMANDID_OP_READ_MEMORY: {
			// Extract source (arbitrary kernel address) and destination (user buffer) addresses
			UINTN src = (UINTN)cmd->data[1];
			UINTN dst = (UINTN)cmd->data[2];
			UINTN len = cmd->size;

			// Validate basic arguments
			if (src && dst && len > 0)
			{
				// Check if all involved addresses are canonical (valid in x64 address space)
				if (UtilsMemory_IsAddressCanonical(src) && UtilsMemory_IsAddressCanonical(src + len - 1) && UtilsMemory_IsAddressCanonical(dst) && UtilsMemory_IsAddressCanonical(dst + len - 1))
				{
					// Perform the copy
					UtilsMemory_CopyMemory((VOID*)dst, (VOID*)src, len);
					return EFI_SUCCESS;
				}
			}

			// Deny access if parameters are invalid or address checks fail
			return EFI_ACCESS_DENIED;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Write Tag at Buffer Start
		case GLOBALS_OPERATIONS_COMMANDID_OP_WRITE_TAG: {
			// Copy the predefined tag to offset 0 of the internal driver buffer
			UtilsMemory_CopyMemory((VOID*)&Global_UtilsExecution_DriverBuffer[0], MACRO_UTILSEXECUTION_TAG, MACRO_UTILSEXECUTION_TAG_LEN);
			return EFI_SUCCESS;
		}

		// -----------------------------------------------------------------------------
		// Write to internal driver buffer (bounds-checked)
		case GLOBALS_OPERATIONS_COMMANDID_OP_WRITE_IN_BUFFER: {
			// Extract destination (must be within internal buffer) and source (user buffer) addresses
			UINTN dst = (UINTN)cmd->data[1];
			UINTN src = (UINTN)cmd->data[2];
			UINTN len = cmd->size;

			// Check if destination is within the driver buffer
			if (dst >= (UINTN)&Global_UtilsExecution_DriverBuffer && dst + len <= (UINTN)&Global_UtilsExecution_DriverBuffer + MACRO_UTILSEXECUTION_DRIVER_SIZE)
			{
				// Perform the copy
				UtilsMemory_CopyMemory((VOID*)dst, (VOID*)src, len);
				return EFI_SUCCESS;
			}

			// Deny access if outside bounds
			return EFI_ACCESS_DENIED;
		}

		// -----------------------------------------------------------------------------
		// Write to arbitrary memory location (unrestricted)
		case GLOBALS_OPERATIONS_COMMANDID_OP_WRITE_RAW: {
			// Extract destination (arbitrary kernel address) and source (user buffer) addresses
			UINTN dst = (UINTN)cmd->data[1];
			UINTN src = (UINTN)cmd->data[2];
			UINTN len = cmd->size;

			// Blindly copy from src to dst, no bounds checking
			if (dst && src && len > 0)
			{
				UtilsMemory_CopyMemory((VOID*)dst, (VOID*)src, len);
				return EFI_SUCCESS;
			}

			return EFI_ACCESS_DENIED;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Call
		case GLOBALS_OPERATIONS_COMMANDID_OP_CALL_FAST: {
			// Calls a function pointer (fastcall) with no arguments
			Typedef_UtilsExecution_StandardFuncFast func = (Typedef_UtilsExecution_StandardFuncFast)(UINTN)cmd->data[1];
			func();
			return EFI_SUCCESS;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// DriverEntry
		case GLOBALS_OPERATIONS_COMMANDID_OP_DRIVER_ENTRY: {
			// Simulates calling a driver's DriverEntry with a fake DriverObject and optional registry path
			Typedef_UtilsExecution_DriverEntry entry = (Typedef_UtilsExecution_DriverEntry)(UINTN)cmd->data[1];
			STRUCTURES_DRIVER_FAKEDRIVEROBJECT fakeObj;
			ZeroMem(&fakeObj, sizeof(fakeObj));
			STRUCTURES_DRIVER_FAKEUNICODESTRING* reg = (STRUCTURES_DRIVER_FAKEUNICODESTRING*)(UINTN)cmd->data[2];
			int status = entry(&fakeObj, reg);
			cmd->data[3] = status;
			return EFI_SUCCESS;
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Operation not recognized
	return EFI_UNSUPPORTED;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
