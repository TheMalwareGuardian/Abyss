// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Registers
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
// Provides copy memory, fill memory, zero memory, and GUID functions. The Base Memory Library provides optimized implementations for common memory-based operations. These functions should be used in place of coding your own loops to do equivalent common functions. This allows optimized library implementations to help increase performance.
#include <Library/BaseMemoryLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Functions/Utils/Utils00Registers.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Disable WP if supported and retrieve their current state.
	@details        This function disables WP (Write Protection) if it is supported by the system's hardware. It also retrieves the current state of this feature and stores it in the output parameter.


	@param[out]     WpEnabled               A pointer to a boolean variable to store the current write protection state. True if write protection is currently enabled, False otherwise.


	@retval         None
**/
VOID
UtilsRegisters_DisableWriteProtection(
	OUT             BOOLEAN                 *WpEnabled
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	// Print(L"[INFO]  AbyssBootkit2 -> [UtilsRegisters.c] (DisableWriteProtection) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Read CR0

	// Reads the current value of the Control Register 0 (CR0). Reads and returns the current value of CR0. This function is only available on IA-32 and x64. This returns a 32-bit value on IA-32 and a 64-bit value on x64.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadCr0.c
	CONST UINTN Cr0 = AsmReadCr0();


	// ---------------------------------------------------------------------------------------------------------------------
	// Store whether WP was originally enabled
	*WpEnabled = (Cr0 & CR0_WP) != 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// If WP is enabled, clear the CR0_WP bit to disable write protection
	if (*WpEnabled)
	{
		// Disable Write Protection

		// Writes a value to Control Register 0 (CR0). Writes and returns a new value to CR0. This function is only available on IA-32 and x64. This writes a 32-bit value on IA-32 and a 64-bit value on x64.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/WriteCr0.c
		AsmWriteCr0(Cr0 & ~CR0_WP);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	// Print(L"[INFO]  AbyssBootkit2 -> [UtilsRegisters.c] (DisableWriteProtection) - Bye\r\n");
}



/**
	@brief          Enable WP if supported.
	@details        This function enables WP (Write Protection) if it is supported by the system's hardware. It takes one boolean parameter to control the enabling/disabling of this feature.


	@param[in]      WpEnabled               A boolean indicating whether write protection should be enabled.


	@retval         None
**/
VOID
UtilsRegisters_EnableWriteProtection(
	IN              BOOLEAN                 WpEnabled
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	// Print(L"[INFO]  AbyssBootkit2 -> [UtilsRegisters.c] (EnableWriteProtection) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Restore WP bit only if it was originally enabled
	if (WpEnabled)
	{
		// Writes a value to Control Register 0 (CR0). Writes and returns a new value to CR0. This function is only available on IA-32 and x64. This writes a 32-bit value on IA-32 and a 64-bit value on x64.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/WriteCr0.c
		// Reads the current value of the Control Register 0 (CR0). Reads and returns the current value of CR0. This function is only available on IA-32 and x64. This returns a 32-bit value on IA-32 and a 64-bit value on x64.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadCr0.c
		AsmWriteCr0(AsmReadCr0() | CR0_WP);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	// Print(L"[INFO]  AbyssBootkit2 -> [UtilsRegisters.c] (EnableWriteProtection) - Bye\r\n");
}



/**
	@brief          Check if the system is configured for five-level paging.
	@details        This function checks if the x86-64 system is configured for five-level paging, which is a specific paging mode used in modern x86-64 systems. It determines this by examining certain control registers and processor features.


	@retval         TRUE                    Five-level paging is enabled.
	@retval         FALSE                   Five-level paging is not enabled.
**/
BOOLEAN
UtilsRegisters_IsFiveLevelPagingEnabled()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	// Print(L"[INFO]  AbyssBootkit2 -> [UtilsRegisters.c] (IsFiveLevelPagingEnabled) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	BOOLEAN isPagingEnabled, isLongModeActiveEnabled, is57BitLinearAddressessEnabled, isTrue;


	// ---------------------------------------------------------------------------------------------------------------------
	// Check CR0 PG (Paging)

	// Reads the current value of the Control Register 0 (CR0). Reads and returns the current value of CR0. This function is only available on IA-32 and x64. This returns a 32-bit value on IA-32 and a 64-bit value on x64.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadCr0.c
	isPagingEnabled = (AsmReadCr0() & CR0_PG) != 0; 


	// ---------------------------------------------------------------------------------------------------------------------
	// Check EFER LMA (Long Mode Active)

	// Returns a 64-bit Machine Specific Register(MSR). Reads and returns the 64-bit MSR specified by Index. No parameter checking is performed on Index, and some Index values may cause CPU exceptions. The caller must either guarantee that Index is valid, or the caller must set up exception handlers to catch the exceptions. This function is only available on IA-32 and x64.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadMsr64.c
	isLongModeActiveEnabled = (AsmReadMsr64(MSR_EFER) & EFER_LMA) != 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Check CR4 LA57 (57-Bit Linear Addresses)

	// Reads the current value of the Control Register 4 (CR4). Reads and returns the current value of CR4. This function is only available on IA-32 and x64. This returns a 32-bit value on IA-32 and a 64-bit value on x64.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadCr4.c
	is57BitLinearAddressessEnabled = (AsmReadCr4() & CR4_LA57) != 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Is?
	isTrue = isPagingEnabled && isLongModeActiveEnabled && is57BitLinearAddressessEnabled;


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	// Print(L"[INFO]  AbyssBootkit2 -> [UtilsRegisters.c] (IsFiveLevelPagingEnabled) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return isTrue;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
