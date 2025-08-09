// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Signatures
**/



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------------------------------
// BOOTMGFW.EFI
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	Signature for [bootmgfw]ImgArchStartBootApplication

	ImgArchStartBootApplication between windows versions 2004-1709 is invoked to start winload.efi. We can use ImgArchStartBootApplication to detect the point at which the Windows OS loader (winload.efi) is loaded into memory but has not yet been executed.

	Signature bytes:
		41 B8 09 00 00 D0

	Instructions:
		mov r8d, 0D0000009h

	References:
		https://guidedhacking.com/threads/hyper-v-hacking-framework-works-on-every-version-of-windows-10-2004-1511-amd-intel.16251/

	This byte pattern can appear in multiple places within bootmgfw.efi. However, we always hook only the first match found (the one located at the lowest memory address). This first occurrence reliably corresponds to ImgArchStartBootApplication across supported Windows versions.

	To locate and verify this:

	1. Mount the EFI System Partition (ESP):
		- mountvol.exe U: /s
	
	2. Copy the Windows Boot Manager to your user's Downloads folder:
		- copy U:\EFI\Microsoft\Boot\bootmgfw.efi %USERPROFILE%\Downloads\bootmgfw.efi

	3. Unmount the EFI System Partition (ESP):
		- mountvol.exe U: /d
	
	4. Import bootmgfw.efi into IDA as a PE file (64-bit).

	5. Open the Hex View and search for the byte pattern:
		- Go to Search -> Sequence of bytes... or press Alt + B
		- Enter the string: 41 B8 09 00 00 D0
		- Make sure to check "Find all occurrences" and "Hex"
		- Click OK

	6. Among the results, focus on the first occurrence (the one with the lowest memory address), as it corresponds to ImgArchStartBootApplication and is the one we hook.
**/
STATIC CONST UINT8 Global_BootWindowsHookings_SigImgArchStartBootApplication[] = {
	0x41, 0xB8, 0x09, 0x00, 0x00, 0xD0
	// mov r8d, 0D0000009h
};



// --------------------------------------------------------------------------------------------------------------------------------------------
// WINLOAD.EFI
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	Signature for [winload]OslFwpKernelSetupPhase1

	The Windows OS Loader has two primary responsibilities. First, it loads the operating system into memory, and second, it manages the transition from the Bootloader to the operating system. The second task is handled by a function called OslExecuteTransition, particularly within the sub-function known as OslArchTransferToKernel.

	If the OS loading process is successful, the UEFI Loader gains the ability to invoke the ExitBootService function. This function releases all EFI Drivers categorized as EFI_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER. If the result of this call is EFI_SUCCESS, it means that the UEFI Loader has taken control of all available system memory and is now responsible for the ongoing system execution.

	EFI Drivers classified as EFI_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER remain in memory and can be used effectively with paging and virtual addresses, as long as the service has accurately defined the virtual memory space it intends to utilize by invoking the SetVirtualAddressMap function.

	Winload.efi is as a UEFI Loader, so logically it has to take charge of everything. Within the OslExecuteTransition function, the first called sub-function is OslFwpKernelSetupPhase1, which is responsible for carrying out these tasks and also handles the mapping of physical to virtual addresses by invoking the SetVirtualAddressMap function.

	Signature bytes:
		89 CC 24 01 00 00
		E8 CC CC CC CC
		CC 8B CC

	Instructions:
		89 CC 24 01 00 00
			Likely:
				mov [reg+offset], reg
			Example:
				44 89 AF 24 01 00 00 -> mov [rdi+124h], r13d
			Explanation:
				44: REX prefix.
				89: Opcode for mov r/m32, r32. Moves a 32-bit value from a register into memory or another register.
				AF: ModR/M byte. Varies depending on the source register.
				24 01 00 00: 4-byte displacement. Here, it's 0x124.
		E8 CC CC CC CC
			Likely:
				call <relative offset>
			Example:
				call BlBdStop
			Explanation:
				E8: Opcode for CALL rel32. Calls a function using a 4-byte relative offset.
		CC 8B CC
			Likely:
				mov <reg>, <reg>
			Example:
				45 8B E5 -> mov r12d, r13d
			Explanation:
				45: REX prefix.
				8B: Opcode for mov r32, r/m32. Moves a 32-bit value from memory or a register into a register.

	References:
		https://n4r1b.com/posts/2019/09/transition-from-an-efi-bootloader-to-the-kernel-in-windows-x64/

	This byte pattern can appear in multiple places within winload.efi. However, we always hook only the first match found (the one located at the lowest memory address). This first occurrence reliably corresponds to OslFwpKernelSetupPhase1 across supported Windows versions.

	To locate and verify this:

	1. Copy the Windows OS Loader to your user's Downloads folder:
		- copy C:\Windows\System32\winload.efi %USERPROFILE%\Downloads\winload.efi
	
	2. Import winload.efi into IDA as a PE file (64-bit).

	3. Open the Hex View and search for the byte pattern:
		- Go to Search -> Sequence of bytes... or press Alt + B
		- Enter the string: 24 01 00 00 E8
		- Make sure to check "Find all occurrences" and "Hex"
		- Click OK

	4. Among the results, focus on the first occurrence (the one with the lowest memory address), as it corresponds to OslFwpKernelSetupPhase1 and is the one we hook.
**/
STATIC CONST UINT8 Global_BootWindowsHookings_SigOslFwpKernelSetupPhase1[] = {
	0x89, 0xCC, 0x24, 0x01, 0x00, 0x00,
	0xE8, 0xCC, 0xCC, 0xCC, 0xCC,
	0xCC, 0x8B, 0xCC
	// mov [reg+offset], reg
	// call <relative offset>
	// mov <reg>, <reg>
};



/**
	Signature for [winload]BlImgAllocateImageBuffer

	Signature bytes:
		48 89 5C 24 CC 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC CC 4C 8B 39
		48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 50

	References:
		https://www.virusbulletin.com/uploads/pdf/conference_slides/2011/Rodionov-Matrosov-VB2011.pdf
		https://www.welivesecurity.com/2023/03/01/blacklotus-uefi-bootkit-myth-confirmed
		https://tulach.cc/from-firmware-to-vbs-enclave-bootkitting-hyper-v

	This byte pattern can appear in multiple places within winload.efi. However, we always hook only the first match found (the one located at the lowest memory address). This first occurrence reliably corresponds to BlImgAllocateImageBuffer across supported Windows versions.

	To locate and verify this:

	1. Copy the Windows OS Loader to your user's Downloads folder:
		- copy C:\Windows\System32\winload.efi %USERPROFILE%\Downloads\winload.efi
	
	2. Import winload.efi into IDA as a PE file (64-bit).

	3. Open the Hex View and search for the byte pattern:
		- Go to Search -> Sequence of bytes... or press Alt + B
		- Enter the string: 48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 50
		- Make sure to check "Find all occurrences" and "Hex"
		- Click OK

	4. Among the results, focus on the first occurrence (the one with the lowest memory address), as it corresponds to BlImgAllocateImageBuffer and is the one we hook.
**/
STATIC CONST UINT8 Global_BootWindowsHookings_SigBlImgAllocateImageBuffer[] = {
	0x48, 0x89, 0x5C, 0x24, 0x18,
	0x55,
	0x56,
	0x57,
	0x41, 0x54,
	0x41, 0x55,
	0x41, 0x56,
	0x41, 0x57,
	0x48, 0x8B, 0xEC,
	0x48, 0x83, 0xEC, 0x50
};



// --------------------------------------------------------------------------------------------------------------------------------------------
// NTOSKRNL.EXE
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	Signature for [nt]SeCodeIntegrityQueryInformation

	SeCodeIntegrityQueryInformation function, called through NtQuerySystemInformation -> SystemCodeIntegrityInformation (https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntquerysysteminformation) is a crucial part of the Code Integrity feature in the Windows operating system.
	
	Code Integrity enhances system security by verifying the integrity of drivers and system files whenever they are loaded into memory. On x64-based versions of the operating system, kernel-mode drivers must be digitally signed.

	Signature bytes:
		48 83 EC CC
		48 83 3D CC CC CC CC 00
		4D 8B C8
		4C 8B D1
		74 CC

	Instructions:
		48 83 EC CC
			Likely:
				sub rsp, <imm8>
			Example:
				48 83 EC 38 -> sub rsp, 38h
			Explanation:
				48: REX prefix.
				83: Opcode for immediate arithmetic on r/m64
				EC: ModR/M byte for sub r/m64, imm8 where the destination is rsp
				38: Subtracts 0x38 from the stack pointer.
		48 83 3D CC CC CC CC 00
			Likely:
				cmp qword ptr [rip+offset], 0
			Example:
				48 83 3D 12 34 56 00 00 -> cmp qword ptr [rip+00563412h], 0
			Explanation:
				48: REX prefix.
				83: Opcode for immediate arithmetic on r/m64
				3D: ModR/M byte for RIP-relative addressing.
				CC CC CC CC: 32-bit offset relative to RIP.
				00: Immediate value for comparison.
		4D 8B C8
			Example:
				mov r9, r8
			Explanation:
				4D: REX prefix.
				8B: Opcode for mov r64, r/m64.
				C8: ModR/M byte.
		4C 8B D1
			Example:
				mov r10, rcx
			Explanation:
				4C: REX prefix.
				8B: Opcode for mov r64, r/m64.
				D1: ModR/M byte.
		74 CC
			Likely:
				je <offset>
			Example:
				74 15 -> je short +0x15
			Explanation:
				74: Opcode for JE (Jump if Equal)
				CC: 8-bit signed offset relative to the next instruction.

	References:
		https://j00ru.vexillium.org/2010/06/insight-into-the-driver-signature-enforcement/

	This byte pattern can appear in multiple places within ntoskrnl.exe. However, we always hook only the first match found (the one located at the lowest memory address). This first occurrence reliably corresponds to SeCodeIntegrityQueryInformation across supported Windows versions.

	To locate and verify this:

	1. Copy the Windows NT Kernel to your user's Downloads folder:
		- copy C:\Windows\System32\ntoskrnl.exe %USERPROFILE%\Downloads\ntoskrnl.exe
	
	2. Import ntoskrnl.exe into IDA as a PE file (64-bit).

	3. Open the Hex View and search for the byte pattern:
		- Go to Search -> Sequence of bytes... or press Alt + B
		- Enter the string: 00 4D 8B C8 4C 8B D1 74
		- Make sure to check "Find all occurrences" and "Hex"
		- Click OK

	4. Among the results, focus on the first occurrence (the one with the lowest memory address), as it corresponds to SeCodeIntegrityQueryInformation and is the one we hook.
**/
STATIC CONST UINT8 Global_BootWindowsHookings_SigSeCodeIntegrityQueryInformation[] = {
	0x48, 0x83, 0xEC, 0xCC,
	0x48, 0x83, 0x3D, 0xCC, 0xCC, 0xCC, 0xCC, 0x00,
	0x4D, 0x8B, 0xC8,
	0x4C, 0x8B, 0xD1,
	0x74, 0xCC
	// sub rsp, <imm8>
	// cmp qword ptr [rip+offset], 0
	// mov r9, r8
	// mov r10, rcx
	// je <offset>
};



/**
	Patch SeCodeIntegrityQueryInformation to report Disable Signature Enforcement (DSE) is disabled.

	41 C7 00 08 00 00 00
	33 C0
	C7 41 04 01 00 00 00
	C3

	https://github.com/Mattiwatti/EfiGuard/blob/master/EfiGuardDxe/PatchNtoskrnl.c
**/
STATIC CONST UINT8 Global_BootWindowsHookings_SeCodeIntegrityQueryInformationPatch[] = {
	0x41, 0xC7, 0x00, 0x08, 0x00, 0x00, 0x00,
	0x33, 0xC0,
	0xC7, 0x41, 0x04, 0x01, 0x00, 0x00, 0x00,
	0xC3
	// mov dword ptr [r8], 8
	// xor eax, eax
	// mov dword ptr [rcx+4], 1
	// ret
};



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
