// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include "../../Structures/Structures00Execution.h"



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



// Send command to DXE Runtime Driver
BOOL UtilsCommunication_SendCommand(STRUCTURES_EXECUTION_MEMORYCOMMAND* cmd, const wchar_t* failMsg);



// Request presence from DXE Runtime Driver
BOOL UtilsCommunication_RequestDriverPresence(char* outMsg, DWORD bufSize);



// Request buffer boundaries from DXE Runtime Driver
BOOL UtilsCommunication_RequestGetBufferBounds(ULONGLONG* buferStart, ULONGLONG* bufferEnd);



// Request memory read from DXE Runtime Driver
BOOL UtilsCommunication_RequestReadMemory(ULONGLONG srcAddr, BYTE* userBuf, DWORD size);



// Request to write tag into DXE Runtime Driver buffer
BOOL UtilsCommunication_RequestWriteTag();



// Request to write data into DXE Runtime Driver buffer
BOOL UtilsCommunication_RequestWriteInBuffer(ULONGLONG dst, BYTE* src, DWORD size);



// Request execution of shellcode at specified address in DXE Runtime Driver buffer
BOOL UtilsCommunication_RequestCallFast(ULONGLONG entryPoint);



// Request DXE Runtime Driver to invoke DriverEntry at given address
BOOL UtilsCommunication_RequestDriverEntry(ULONGLONG driverEntryVA, ULONGLONG unicodeStructVA, ULONGLONG* outStatus);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
