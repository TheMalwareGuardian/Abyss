// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Memory
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



#include <Functions/Utils/Utils01Memory.h>
#include <Functions/Utils/Utils00Registers.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Copy memory from source to destination.
	@details        This function copies data from the source memory location to the destination memory location, with the specified length.


	@param[out]     Destination             Pointer to the destination memory location.
	@param[in]      Source                  Pointer to the source memory location.
	@param[in]      Length                  Number of bytes to copy.


	@retval         VOID*                   Returns a pointer to the destination memory location after copying.
**/
VOID*
UtilsMemory_CopyMemory(
	OUT             VOID                    *Destination,
	IN              CONST VOID              *Source,
	IN              UINTN                   Length
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	// Print(L"[INFO]  AbyssBootkit2 -> [UtilsMemory.c] (CopyMemory) - Hello\r\n");
	

	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	BOOLEAN WpEnabled;


	// ---------------------------------------------------------------------------------------------------------------------
	// Disable write protection
	UtilsRegisters_DisableWriteProtection(&WpEnabled);


	// ---------------------------------------------------------------------------------------------------------------------
	// Copy the content of one buffer to another
	
	// Copies a source buffer to a destination buffer, and returns the destination buffer.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-copymem
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseMemoryLib/CopyMemWrapper.c#L41
	VOID* Result = CopyMem(Destination, Source, Length);


	// ---------------------------------------------------------------------------------------------------------------------
	// Enable write protection
	UtilsRegisters_EnableWriteProtection(WpEnabled);


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	// Print(L"[INFO]  AbyssBootkit2 -> [UtilsMemory.c] (CopyMemory) - Bye\r\n");
	

	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return Result;
}



/**
	@brief          Check if the given memory address is canonical.
	@details        This function checks whether the provided memory address is canonical or not. A canonical address is a memory address that complies with the requirements of the x86-64 architecture, considering the current paging mode and certain architectural features.


	@param[in]      Address                 Memory address to be checked.


	@retval         TRUE                    The address is canonical.
	@retval         FALSE                   The address is not canonical.
**/
BOOLEAN
UtilsMemory_IsAddressCanonical(
	IN              UINTN                   Address
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	// Print(L"[INFO]  AbyssBootkit2 -> [UtilsMemory.c] (IsAddressCanonical) - Hello\r\n");
	

	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	BOOLEAN isCanonical, isUpperAddressIgnoreEnableEnabled, isTrue;


	// ---------------------------------------------------------------------------------------------------------------------
	// Linear address bits based on paging mode

	// https://en.wikipedia.org/wiki/X86-64
	CONST UINTN LinearAddressBits = UtilsRegisters_IsFiveLevelPagingEnabled() ? 57 : 48;


	// ---------------------------------------------------------------------------------------------------------------------
	// Calculate masked address
	UINTN MaskedAddress = (UINTN)(((INTN)Address >> LinearAddressBits) + 1);


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if canonical
	isCanonical = (MaskedAddress <= 1);


	// ---------------------------------------------------------------------------------------------------------------------
	// Check EFER UAIE (Upper Address Ignore Enable)

	// Returns a 64-bit Machine Specific Register(MSR). Reads and returns the 64-bit MSR specified by Index. No parameter checking is performed on Index, and some Index values may cause CPU exceptions. The caller must either guarantee that Index is valid, or the caller must set up exception handlers to catch the exceptions. This function is only available on IA-32 and x64.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadMsr64.c
	isUpperAddressIgnoreEnableEnabled = ((AsmReadMsr64(MSR_EFER) & EFER_UAIE) != 0);


	// ---------------------------------------------------------------------------------------------------------------------
	// Is?
	isTrue = isCanonical || isUpperAddressIgnoreEnableEnabled;


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	// Print(L"[INFO]  AbyssBootkit2 -> [UtilsMemory.c] (IsAddressCanonical) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return isTrue;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
