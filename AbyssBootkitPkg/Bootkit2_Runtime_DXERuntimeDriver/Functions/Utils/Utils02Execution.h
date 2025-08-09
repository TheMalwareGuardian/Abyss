// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Execution
**/



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Globals/Globals00Protocol.h>
#include <Functions/Utils/Utils01Memory.h>
#include <Structures/Structures02Driver.h>
#include <Structures/Structures01Execution.h>



// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------
// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------



#define MACRO_UTILSEXECUTION_DRIVER_SIZE     26214400
#define MACRO_UTILSEXECUTION_TAG             "THEMALWAREGUARDIAN"
#define MACRO_UTILSEXECUTION_TAG_LEN         18



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



extern char Global_UtilsExecution_DriverBuffer[MACRO_UTILSEXECUTION_DRIVER_SIZE];



// START -> TYPEDEFS --------------------------------------------------------------------------------------------------------------------------
// START -> TYPEDEFS --------------------------------------------------------------------------------------------------------------------------



typedef VOID (*Typedef_UtilsExecution_StandardFuncFast)(VOID);
typedef int (*Typedef_UtilsExecution_DriverEntry)(STRUCTURES_DRIVER_FAKEDRIVEROBJECT*, STRUCTURES_DRIVER_FAKEUNICODESTRING*);



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
