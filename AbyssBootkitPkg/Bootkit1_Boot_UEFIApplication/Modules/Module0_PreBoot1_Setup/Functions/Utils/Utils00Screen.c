// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Screen
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Boot Services Table. Only available to DXE and UEFI module types.
#include <Library/UefiBootServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Functions/Utils/Utils00Debug.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief      Set the console output to the highest resolution.
	@details    This function iterates through all available text modes supported by the console output device, and sets the console to the mode with the highest resolution (i.e., the mode with the most rows and columns). It is useful for maximizing the amount of information that can be displayed on the console.


	@retval     None
**/
VOID
UtilsScreen_SetConsoleOutputToHighestResolution()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Screen.c] (SetConsoleOutputToHighestResolution) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	UINTN MaxMode = gST->ConOut->Mode->MaxMode;
	UINTN MaxRows = 0;
	UINTN MaxColumns = 0;
	UINTN BestMode = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Iterate through all available text modes
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Screen.c] (SetConsoleOutputToHighestResolution) - Iterate through all available text modes\r\n");

	// Iterate
	for (UINTN Mode = 0; Mode < MaxMode; Mode++)
	{
		UINTN Columns, Rows;

		// Query the console for the number of rows and columns in the current mode
		// https://uefi.org/specs/UEFI/2.11/12_Protocols_Console_Support.html#efi-simple-text-output-protocol-querymode
		EFI_STATUS Status = gST->ConOut->QueryMode(gST->ConOut, Mode, &Columns, &Rows);
		if (!EFI_ERROR(Status))
		{

			// If the current mode has a greater area (rows * columns) than the previous, update the best mode
			if ((Rows * Columns) > (MaxRows * MaxColumns))
			{
				MaxRows = Rows;
				MaxColumns = Columns;
				BestMode = Mode;
			}
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Set the console to the best mode found
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Screen.c] (SetConsoleOutputToHighestResolution) - Set the console to the best mode found\r\n");

	// https://uefi.org/specs/UEFI/2.11/12_Protocols_Console_Support.html#efi-simple-text-output-protocol-setmode
	gST->ConOut->SetMode(gST->ConOut, BestMode);


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	PreBootConfiguration_UtilsDebug_DebuggingAndOrPrintingAndOrLogging(MACRO_PREBOOTCONFIGURATION_UTILSDEBUG_LEVELINFO, L"AbyssBootkit1 -> PreBootSetup [Utils/Screen.c] (SetConsoleOutputToHighestResolution) - Bye\r\n");
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
