// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Structures Driver
**/



#ifndef STRUCTURES02DRIVER_H
#define STRUCTURES02DRIVER_H



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



// The UNICODE_STRING structure is used to define Unicode strings.
// https://learn.microsoft.com/en-us/windows/win32/api/ntdef/ns-ntdef-_unicode_string
typedef struct {
	UINT16 Length;
	UINT16 MaximumLength;
	CHAR8* Buffer;
} STRUCTURES_DRIVER_FAKEUNICODESTRING;



// Each driver object represents the image of a loaded kernel-mode driver. A pointer to the driver object is an input parameter to a driver's DriverEntry, AddDevice, and optional Reinitialize routines and to its Unload routine, if any. A driver object is partially opaque. Driver writers must know about certain members of a driver object to initialize a driver and to unload it if the driver is unloadable.
// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/ns-wdm-_driver_object
typedef struct {
	UINT16 Type;
	UINT16 Size;
	VOID*  DeviceObject;
	VOID*  Flags;
	VOID*  DriverStart;
	VOID*  DriverSize;
	VOID*  DriverSection;
	VOID*  DriverExtension;
	VOID*  DriverName;
	VOID*  HardwareDatabase;
	VOID*  FastIoDispatch;
	VOID*  DriverInit;
	VOID*  DriverStartIo;
	VOID*  DriverUnload;
	VOID*  MajorFunction[28];
} STRUCTURES_DRIVER_FAKEDRIVEROBJECT;



#endif // STRUCTURES02DRIVER_H



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
