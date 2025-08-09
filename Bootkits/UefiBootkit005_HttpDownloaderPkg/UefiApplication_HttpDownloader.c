// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	UefiApplication_HttpDownloader.c

	A minimal UEFI application that downloads a file over HTTP and saves it to the EFI System Partition (ESP).

	This example serves as the foundation for understanding UEFI applications, bootkits, and low-level development within the UEFI pre-boot environment.


	@note

	Instructions to Compile and Run this UEFI Application (in a Virtual Machine using VMware Workstation):

	1. Open the file 'Conf/target.txt' located in the root of your EDK2 directory. Modify the following line to point to this DSC file:
		ACTIVE_PLATFORM = UefiBootkit005_HttpDownloaderPkg/UefiApplication_HttpDownloader.dsc

	2. Open a command prompt and set up the EDK2 environment:
		> edksetup.bat Rebuild

	3. Build the application:
		> build

	4. Once built successfully, locate the generated .EFI binary at:
		C:\edk2\Build\UefiBootkit005_HttpDownloaderPkg\DEBUG_VS2019\X64\UefiBootkit005_HttpDownloader.efi

	5. To test the application:

		a. If you're already inside a Windows virtual machine (VM):

			- Mount the EFI System Partition (ESP):
				> mountvol U: /S

			- Copy the generated .efi binary to the EFI\Boot\Bootkits directory:
				> mkdir U:\EFI\Boot\Bootkits
				> copy C:\edk2\Build\UefiBootkit005_HttpDownloaderPkg\DEBUG_VS2019\X64\UefiBootkit005_HttpDownloader.efi U:\EFI\Boot\Bootkits\UefiBootkit005_HttpDownloader.efi

			- Dismount the ESP:
				> mountvol U: /D

			- Shut down the VM

			- Power on the VM directly to firmware:
				Right-click the VM -> Power -> Power On to Firmware

			- From the firmware menu, select 'EFI Internal Shell, then press 'Enter'.

			- Run the application in the shell:
				Shell> map -r
				Shell> fs0:
				Shell> dir
				Shell> cd EFI\Boot\Bootkits
				Shell> dir
				Shell> ping 8.8.8.8
				Shell> CTRL+C
				Shell> UefiBootkit005_HttpDownloader.efi
				Shell> dir

	b. If testing from outside the VM:

		- Create a shared folder between the host and guest, and place the .efi file inside it.

		- Power on the VM directly to firmware:
				Right-click the VM -> Power -> Power On to Firmware

			- From the firmware menu, select 'EFI Internal Shell, then press 'Enter'.

			- Run the application in the shell:
				Shell> map -r
				Shell> fs0:
				Shell> dir
				Shell> cd EFI\Boot\Bootkits
				Shell> cp fsnt0:\UefiBootkit005_HttpDownloader.efi .
				Shell> dir
				Shell> ping 8.8.8.8
				Shell> CTRL+C
				Shell> UefiBootkit005_HttpDownloader.efi
				Shell> dir
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiApplicationEntryPoint.h
// Module entry point library for UEFI Applications.
#include <Library/UefiApplicationEntryPoint.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Boot Services Table. Only available to DXE and UEFI module types.
#include <Library/UefiBootServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiRuntimeServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Runtime Services Table. It simply exports the global variable gRT that is a pointer to the EFI Runtime Services Table as defined in the UEFI Specification. The global variable gRT must be preinitialized to NULL. The library constructor must set gRT to point at the EFI Runtime Services Table so it is available at the module's entry point. Since there is overhead in initializing this global variable, only those modules that actually require access to the EFI Runtime Services Table should use this library. Only available to DXE and UEFI module types.
#include <Library/UefiRuntimeServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
// Provides copy memory, fill memory, zero memory, and GUID functions. The Base Memory Library provides optimized implementations for common memory-based operations. These functions should be used in place of coding your own loops to do equivalent common functions. This allows optimized library implementations to help increase performance.
#include <Library/BaseMemoryLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/MemoryAllocationLib.h
// Provides services to allocate and free memory buffers of various memory types and alignments. The Memory Allocation Library abstracts various common memory allocation operations. This library allows code to be written in a phase-independent manner because the allocation of memory in PEI, DXE, and SMM (for example) is done via a different mechanism. Using a common library interface makes it much easier to port algorithms from phase to phase.
#include <Library/MemoryAllocationLib.h>



// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------
// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h
// UEFI 2.0 Loaded image protocol definition. Every EFI driver and application is passed an image handle when it is loaded. This image handle will contain a Loaded Image Protocol.
#include <Protocol/LoadedImage.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/ServiceBinding.h
// UEFI Service Binding Protocol is defined in UEFI specification. The file defines the generic Service Binding Protocol functions. It provides services that are required to create and destroy child handles that support a given set of protocols.
#include <Protocol/ServiceBinding.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/Http.h
// This file defines the EFI HTTP Protocol interface. It is split into the following two main sections: HTTP Service Binding Protocol (HTTPSB) HTTP Protocol (HTTP)
#include <Protocol/Http.h>



// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------
// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------



// Define the buffer size for HTTP operations (both request and response bodies)
#define MACRO_HTTPBUFFERSIZE 8192



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Global flag to indicate when the HTTP request operation has completed
STATIC BOOLEAN Global_UtilsHttp_HttpRequestDone = FALSE;

// Global flag to indicate when the HTTP response operation has completed
STATIC BOOLEAN Global_UtilsHttp_HttpResponseDone = FALSE;



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          HTTP request completion callback.
	@details        This callback function is triggered when the HTTP request operation has been completed. It sets a global flag to indicate that the request has finished, allowing the main flow to continue.


	@param[in]      Event               The event that triggered the callback.
	@param[in]      Context             Optional context information provided when the event was created.


	@retval         None
**/
STATIC
VOID
Function_RequestCallback(
	IN              EFI_EVENT           Event,
	IN              VOID                *Context
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Log the event for debugging purposes
	Print(L"[INFO]  Request callback triggered\r\n");

	// ---------------------------------------------------------------------------------------------------------------------
	// Set the request completion flag
	Global_UtilsHttp_HttpRequestDone = TRUE;
}



/**
	@brief          HTTP response completion callback.
	@details        This callback function is triggered when the HTTP response has been fully received. It sets a global flag to indicate that the response has been completed, allowing the main flow to continue.


	@param[in]      Event               The event that triggered the callback.
	@param[in]      Context             Optional context information provided when the event was created.


	@retval         None
**/
STATIC
VOID
Function_ResponseCallback(
	IN              EFI_EVENT           Event,
	IN              VOID                *Context
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Log the event for debugging purposes
	Print(L"[INFO]  Response callback triggered\r\n");

	// ---------------------------------------------------------------------------------------------------------------------
	// Set the response completion flag
	Global_UtilsHttp_HttpResponseDone = TRUE;
}



/**
	@brief          Writes binary data to a file in the EFI system partition (ESP).
	@details        This function creates or overwrites a file at the specified FilePath and writes the provided binary data to it.


	@param[in]      FilePath            The full path to the file where the binary data will be written.
	@param[in]      Buffer              A pointer to the binary data to be written to the file.
	@param[in]      BufferSize          The size of the binary data to be written, in bytes.


	@retval         EFI_SUCCESS         The data was successfully written to the file.
	@retval         EFI_ABORTED         An error occurred while writing to the file.
	@retval         Others              An error returned by underlying UEFI services such as HandleProtocol or OpenVolume.
**/
EFI_STATUS
Function_SaveToFile(
	IN              CHAR16              *FilePath,
	IN              UINT8               *Buffer,
	IN              UINTN               BufferSize
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	Print(L"[INFO]  Function_SaveToFile - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
	EFI_FILE_PROTOCOL *Root;
	EFI_FILE_PROTOCOL *File;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the LoadedImage protocol for the current image
	Print(L"[INFO]  Function_SaveToFile - Get the LoadedImage protocol for the current image\r\n");

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h#L79
	Status = gBS->HandleProtocol(gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);

	// Failed
	if (EFI_ERROR(Status))
	{
		Print(L"[ERROR] Function_SaveToFile - gBS->HandleProtocol [gEfiLoadedImageProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get the Simple File System Protocol of the device we're on
	Print(L"[INFO]  Function_SaveToFile - Get the Simple File System Protocol of the device we're on\r\n");

	// Queries a handle to determine if it supports a specified protocol.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/SimpleFileSystem.h#L551
	Status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&SimpleFileSystem);

	// Failed
	if (EFI_ERROR(Status))
	{
		Print(L"[ERROR] Function_SaveToFile - gBS->HandleProtocol [gEfiSimpleFileSystemProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Open the volume to get the root directory of the ESP
	Print(L"[INFO]  Function_SaveToFile - Open the volume to get the root directory of the ESP\r\n");

	// Opens the root directory on a volume.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html?highlight=openvolume#efi-simple-file-system-protocol-openvolume
	Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &Root);

	// Failed
	if (EFI_ERROR(Status))
	{
		Print(L"[ERROR] Function_SaveToFile - SimpleFileSystem->OpenVolume failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Create the file (if it already exists, it will be overwritten)
	Print(L"[INFO]  Function_SaveToFile - Create the file (if it already exists, it will be overwritten)\r\n");

	// Opens a new file relative to the source file’s location.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-open
	Status = Root->Open(Root, &File, FilePath, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		Root->Close(Root);

		Print(L"[ERROR] Function_SaveToFile - Root->Open failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Write the data
	Print(L"[INFO]  Function_SaveToFile - Write the data\r\n");

	// Writes data to a file.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-write
	Status = File->Write(File, &BufferSize, Buffer);

	// Failed
	if (EFI_ERROR(Status))
	{
		// Closes a specified file handle.
		// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
		File->Close(File);
		Root->Close(Root);

		Print(L"[ERROR] Function_SaveToFile - File->Write failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	Print(L"[INFO]  Function_SaveToFile - Bye\r\n");


	// Closes a specified file handle.
	// https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-close
	File->Close(File);
	Root->Close(Root);


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return Status;
}



/**
	@brief          Downloads a file over HTTP and saves it to the specified path.
	@details        This function establishes an HTTP connection, sends a GET request to the provided URL, waits for the server's response, downloads the data in chunks if necessary, and saves the downloaded content into the destination file. It uses UEFI protocols for HTTP communication and file system access.


	@param[in]      Url                 The URL from which the file should be downloaded.
	@param[in]      DestinationPath     The full path where the downloaded file will be saved.


	@retval         EFI_SUCCESS         The file was successfully downloaded and saved.
	@retval         EFI_TIMEOUT         The request or response operation timed out.
	@retval         EFI_ABORTED         Content-Length header was missing or invalid.
	@retval         Others              Errors returned by UEFI services (e.g., failed to allocate memory, configure HTTP, etc.).
**/
EFI_STATUS
Function_DownloadFile(
	IN              CHAR16              *Url,
	IN              CHAR16              *DestinationPath
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	Print(L"[INFO]  Function_DownloadFile - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_SERVICE_BINDING_PROTOCOL *ServiceBinding = NULL;
	EFI_HTTP_PROTOCOL *Http = NULL;
	EFI_HANDLE HttpHandle = NULL;
	EFI_HTTP_CONFIG_DATA Config;
	EFI_HTTPv4_ACCESS_POINT AccessPoint;
	EFI_HTTP_REQUEST_DATA RequestData;
	EFI_HTTP_HEADER RequestHeader;
	EFI_HTTP_MESSAGE RequestMessage;
	EFI_HTTP_TOKEN RequestToken;
	EFI_HTTP_RESPONSE_DATA ResponseData;
	EFI_HTTP_MESSAGE ResponseMessage;
	EFI_HTTP_TOKEN ResponseToken;
	UINT8 *BufferChunk = NULL;
	UINT8 *BufferDownload = NULL;
	UINTN ContentLength = 0;
	UINTN Downloaded = 0;
	EFI_TIME Start, Now;
	UINTN Timer;


	// ---------------------------------------------------------------------------------------------------------------------
	// Download file
	Print(L"[INFO]  Function_DownloadFile - Download file %s to path %s\r\n", Url, DestinationPath);


	do {
		// -----------------------------------------------------------------------------------------------------------------
		// Locate the HTTP Service Binding Protocol
		Print(L"[INFO]  Function_DownloadFile - Locate the HTTP Service Binding Protocol\r\n");

		// Returns the first protocol instance that matches the given protocol.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-locateprotocol
		Status = gBS->LocateProtocol(&gEfiHttpServiceBindingProtocolGuid, NULL, (VOID**)&ServiceBinding);

		// Failed
		if (EFI_ERROR(Status))
		{
			Print(L"[ERROR] Function_DownloadFile - gBS->LocateProtocol [gEfiHttpServiceBindingProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
			break;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Create a child HTTP handle to initiate a connection
		Print(L"[INFO]  Function_DownloadFile - Create a child HTTP handle to initiate a connection\r\n");

		// Creates a child handle and installs a protocol.
		// https://uefi.org/specs/UEFI/2.11/11_Protocols_UEFI_Driver_Model.html#efi-service-binding-protocol-createchild
		Status = ServiceBinding->CreateChild(ServiceBinding, &HttpHandle);

		// Failed
		if (EFI_ERROR(Status))
		{
			Print(L"[ERROR] Function_DownloadFile - ServiceBinding->CreateChild failed: %r (%llx).\r\n", Status, Status);
			break;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Open the HTTP Protocol on the created child handle
		Print(L"[INFO]  Function_DownloadFile - Open the HTTP Protocol on the created child handle\r\n");

		// Queries a handle to determine if it supports a specified protocol.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-handleprotocol
		Status = gBS->HandleProtocol(HttpHandle, &gEfiHttpProtocolGuid, (VOID**)&Http);

		// Failed
		if (EFI_ERROR(Status))
		{
			Print(L"[ERROR] Function_DownloadFile - gBS->HandleProtocol [gEfiHttpProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
			break;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Zero out the AccessPoint structure to initialize default values
		ZeroMem(&AccessPoint, sizeof AccessPoint);

		// -----------------------------------------------------------------------------------------------------------------
		// Use the default network address configured by the system
		AccessPoint.UseDefaultAddress = TRUE;


		// -----------------------------------------------------------------------------------------------------------------
		// Configure HTTP settings
		Config.HttpVersion = HttpVersion11;
		Config.TimeOutMillisec = 0;
		Config.LocalAddressIsIPv6 = FALSE;
		Config.AccessPoint.IPv4Node = &AccessPoint;


		// -----------------------------------------------------------------------------------------------------------------
		// Configure the HTTP connection using the specified settings
		Print(L"[INFO]  Function_DownloadFile - Configure the HTTP connection using the specified settings\r\n");

		Status = Http->Configure(Http, &Config);

		// Failed
		if (EFI_ERROR(Status))
		{
			Print(L"[ERROR] Function_DownloadFile - Http->Configure failed: %r (%llx).\r\n", Status, Status);
			break;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Prepare the HTTP GET request
		RequestData.Method = HttpMethodGet;
		RequestData.Url = Url;

		// Prepare the mandatory Host header
		RequestHeader.FieldName = "Host";
		RequestHeader.FieldValue = "localhost";

		// Prepare the full HTTP message for the request
		RequestMessage.Data.Request = &RequestData;
		RequestMessage.HeaderCount = 1;
		RequestMessage.Headers = &RequestHeader;
		RequestMessage.BodyLength = 0;
		RequestMessage.Body = NULL;


		// -----------------------------------------------------------------------------------------------------------------
		// Create an event that will signal when the request completes
		Print(L"[INFO]  Function_DownloadFile - Create an event that will signal when the request completes\r\n");

		// Creates an event.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-createevent
		Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, Function_RequestCallback, NULL, &RequestToken.Event);

		// Failed
		if (EFI_ERROR(Status))
		{
			Print(L"[ERROR] Function_DownloadFile - gBS->CreateEvent [Function_RequestCallback] failed: %r (%llx).\r\n", Status, Status);
			break;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Initialize the request token
		RequestToken.Status = EFI_SUCCESS;
		RequestToken.Message = &RequestMessage;
		Global_UtilsHttp_HttpRequestDone = FALSE;


		// -----------------------------------------------------------------------------------------------------------------
		// Send HTTP request
		Print(L"[INFO]  Function_DownloadFile - Send HTTP request\r\n");

		Status = Http->Request(Http, &RequestToken);

		// Failed
		if (EFI_ERROR(Status))
		{
			Print(L"[ERROR] Function_DownloadFile - Http->Request failed: %r (%llx).\r\n", Status, Status);
			break;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Wait for the request to complete with a timeout mechanism
		Print(L"[INFO]  Function_DownloadFile - Wait for the request to complete with a timeout mechanism\r\n");

		// Returns the current time and date information, and the time-keeping capabilities of the hardware platform.
		// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#gettime
		gRT->GetTime(&Start, NULL);
		Timer = 0;
		while (!Global_UtilsHttp_HttpRequestDone && Timer < 10)
		{
			// Polls for incoming data packets and processes outgoing data packets.
			// https://uefi.org/specs/UEFI/2.11/29_Network_Protocols_ARP_and_DHCP.html#efi-http-protocol-poll
			Http->Poll(Http);

			// Returns the current time and date information, and the time-keeping capabilities of the hardware platform.
			// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#gettime
			gRT->GetTime(&Now, NULL);
			if (Now.Second != Start.Second)
			{
				Start = Now;
				Timer++;
			}
		}
		// Timeout
		if (!Global_UtilsHttp_HttpRequestDone)
		{
			Status = EFI_TIMEOUT;
			Print(L"[ERROR] Function_DownloadFile - Timeout [Global_UtilsHttp_HttpRequestDone]: %r (%llx).\r\n", Status, Status);
			break;
		}
		// Closes an event.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-closeevent
		gBS->CloseEvent (RequestToken.Event);
		RequestToken.Event = NULL;


		// -----------------------------------------------------------------------------------------------------------------
		// Allocate a buffer for receiving the HTTP body
		Print(L"[INFO]  Function_DownloadFile - Allocate a buffer for receiving the HTTP body\r\n");

		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
		Status = gBS->AllocatePool(EfiBootServicesData, MACRO_HTTPBUFFERSIZE, (VOID**)&BufferChunk);

		// Failed
		if (EFI_ERROR(Status))
		{
			Print(L"[ERROR] Function_DownloadFile - gBS->AllocatePool [BufferChunk] failed: %r (%llx).\r\n", Status, Status);
			break;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Create an event to signal when the response is received
		Print(L"[INFO]  Function_DownloadFile - Create an event to signal when the response is received\r\n");

		// Creates an event.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-createevent
		Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, Function_ResponseCallback, NULL, &ResponseToken.Event);

		// Failed
		if (EFI_ERROR(Status))
		{
			Print(L"[ERROR] Function_DownloadFile - gBS->CreateEvent [Function_ResponseCallback] failed: %r (%llx).\r\n", Status, Status);
			break;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Prepare the response structures
		ResponseData.StatusCode = HTTP_STATUS_UNSUPPORTED_STATUS;
		ResponseMessage.Data.Response = &ResponseData;
		ResponseMessage.HeaderCount = 0;
		ResponseMessage.Headers = NULL;
		ResponseMessage.Body = BufferChunk;
		ResponseMessage.BodyLength = MACRO_HTTPBUFFERSIZE;
		ResponseToken.Status = EFI_SUCCESS;
		ResponseToken.Message = &ResponseMessage;
		Global_UtilsHttp_HttpResponseDone = FALSE;


		// -----------------------------------------------------------------------------------------------------------------
		// Start receiving HTTP response
		Print(L"[INFO]  Function_DownloadFile - Start receiving HTTP response\r\n");

		// The Response() function queues an HTTP response to this HTTP instance, similar to Receive() function in the EFI TCP driver. When the HTTP response is received successfully, or if there is an error, Status in token will be updated and Event will be signaled.
		// https://uefi.org/specs/UEFI/2.11/29_Network_Protocols_ARP_and_DHCP.html#efi-http-protocol-response
		Status = Http->Response(Http, &ResponseToken);

		// Failed
		if (EFI_ERROR(Status))
		{
			Print(L"[ERROR] Function_DownloadFile - Http->Response failed: %r (%llx).\r\n", Status, Status);
			break;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Wait for the response to complete with a timeout
		Print(L"[INFO]  Function_DownloadFile - Wait for the response to complete with a timeout\r\n");

		// Returns the current time and date information, and the time-keeping capabilities of the hardware platform.
		// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#gettime
		gRT->GetTime(&Start, NULL);
		Timer=0;
		while (!Global_UtilsHttp_HttpResponseDone && Timer < 10)
		{
			// Polls for incoming data packets and processes outgoing data packets.
			// https://uefi.org/specs/UEFI/2.11/29_Network_Protocols_ARP_and_DHCP.html#efi-http-protocol-poll
			Http->Poll(Http);
			// Returns the current time and date information, and the time-keeping capabilities of the hardware platform.
			// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#gettime
			gRT->GetTime(&Now, NULL);
			if (Now.Second != Start.Second)
			{
				Start = Now;
				Timer++;
			}
		}
		// Timeout
		if (!Global_UtilsHttp_HttpResponseDone)
		{
			Status = EFI_TIMEOUT;
			Print(L"[ERROR] Function_DownloadFile - Timeout [Global_UtilsHttp_HttpResponseDone]: %r (%llx).\r\n", Status, Status);
			break;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Parse the HTTP headers to find Content-Length
		for (UINTN i = 0; i < ResponseMessage.HeaderCount; ++i)
		{
			// Performs a case insensitive comparison of two Null-terminated ASCII strings, and returns the difference between the first mismatched ASCII characters.
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L806
			if (!AsciiStriCmp(ResponseMessage.Headers[i].FieldName, "Content-Length"))
			{
				// Convert a Null-terminated ASCII decimal string to a value of type UINTN. This function returns a value of type UINTN by interpreting the contents of the ASCII string String as a decimal number. The format of the input ASCII string String is: [spaces] [decimal digits].
				// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/String.c#L1006
				ContentLength = AsciiStrDecimalToUintn(ResponseMessage.Headers[i].FieldValue);

				// Content-Length
				Print(L"[INFO]  Function_DownloadFile - Content-Length: %d\r\n", ContentLength);
				break;
			}
		}
		// If Content-Length is missing or zero, abort
		if (ContentLength == 0)
		{
			Status = EFI_ABORTED;
			Print(L"[ERROR] Function_DownloadFile - Content-Length header not found or zero\r\n");
			break;
		}

		// -----------------------------------------------------------------------------------------------------------------
		// Allocate a buffer to store the entire downloaded file
		Print(L"[INFO]  Function_DownloadFile - Allocate a buffer to store the entire downloaded file\r\n");

		// Allocates pool memory.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
		Status = gBS->AllocatePool(EfiBootServicesData, ContentLength, (VOID**)&BufferDownload);

		// Failed
		if (EFI_ERROR(Status))
		{
			Print(L"[ERROR] Function_DownloadFile - gBS->AllocatePool 1 [BufferDownload] failed: %r (%llx).\r\n", Status, Status);
			break;
		}


		// -----------------------------------------------------------------------------------------------------------------
		// Initialize the download buffer and copy initial data
		ZeroMem(BufferDownload, ContentLength);
		UINTN ToCopy = MIN(ResponseMessage.BodyLength, ContentLength);
		CopyMem(BufferDownload, BufferChunk, ToCopy);
		Downloaded = ToCopy;


		// -----------------------------------------------------------------------------------------------------------------
		// Current HTTP chunk length
		Print(L"[INFO]  Function_DownloadFile - Current HTTP chunk length %u\r\n",(UINT32)ResponseMessage.BodyLength);


		// -----------------------------------------------------------------------------------------------------------------
		// Clean up for next chunk

		// Close the event associated with the previous HTTP response

		// Closes an event.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-closeevent
		gBS->CloseEvent(ResponseToken.Event);
		ResponseToken.Event = NULL;

		// Free memory used for the previous buffer chunk

		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-freepool
		FreePool(BufferChunk);
		BufferChunk = NULL;
		


		// -----------------------------------------------------------------------------------------------------------------
		// Loop over remaining chunks
		Print(L"[INFO]  Function_DownloadFile - Loop over remaining chunks\r\n");

		while (Downloaded < ContentLength)
		{

			// -------------------------------------------------------------------------------------------------------------
			// Allocate memory for the next HTTP chunk

			// Allocates pool memory.
			// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-allocatepool
			// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/PeiMemoryAllocationLib/MemoryAllocationLib.c#L400
			Status = gBS->AllocatePool(EfiBootServicesData, ContentLength, (VOID**)&BufferChunk);

			// Failed
			if (EFI_ERROR(Status))
			{
				Print(L"[ERROR] Function_DownloadFile - gBS->AllocatePool [While-BufferChunk] failed: %r (%llx).\r\n", Status, Status);
				break;
			}


			// -------------------------------------------------------------------------------------------------------------
			// Create event to signal when the chunk is received

			// Creates an event.
			// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-createevent
			Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, Function_ResponseCallback, NULL, &ResponseToken.Event);

			// Failed
			if (EFI_ERROR(Status))
			{
				Print(L"[ERROR] Function_DownloadFile - gBS->CreateEvent [While-Function_ResponseCallback] failed: %r (%llx).\r\n", Status, Status);
				break;
			}


			// -------------------------------------------------------------------------------------------------------------
			// Set up HTTP response message fields for the new chunk
			ResponseMessage.Data.Response = NULL;
			ResponseMessage.HeaderCount = 0;
			ResponseMessage.Headers = NULL;
			ResponseMessage.Body = BufferChunk;
			ResponseMessage.BodyLength = MACRO_HTTPBUFFERSIZE;
			ResponseToken.Status = EFI_SUCCESS;
			Global_UtilsHttp_HttpResponseDone = FALSE;


			// -------------------------------------------------------------------------------------------------------------
			// Send HTTP response request and receive next chunk

			// The Response() function queues an HTTP response to this HTTP instance, similar to Receive() function in the EFI TCP driver. When the HTTP response is received successfully, or if there is an error, Status in token will be updated and Event will be signaled.
			// https://uefi.org/specs/UEFI/2.11/29_Network_Protocols_ARP_and_DHCP.html#efi-http-protocol-response
			Status = Http->Response(Http, &ResponseToken);

			// Failed
			if (EFI_ERROR(Status) && Status != EFI_CONNECTION_FIN)
			{
				Print(L"[ERROR] Function_DownloadFile - Http->Response [While-ResponseToken] failed: %r (%llx).\r\n", Status, Status);
				break;
			}


			// -------------------------------------------------------------------------------------------------------------
			// Wait for the request to complete with a timeout mechanism

			// Returns the current time and date information, and the time-keeping capabilities of the hardware platform.
			// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#gettime
			gRT->GetTime(&Start, NULL);
			Timer=0;
			while (!Global_UtilsHttp_HttpResponseDone && Timer < 10)
			{
				// Polls for incoming data packets and processes outgoing data packets.
				// https://uefi.org/specs/UEFI/2.11/29_Network_Protocols_ARP_and_DHCP.html#efi-http-protocol-poll
				Http->Poll(Http);

				// Returns the current time and date information, and the time-keeping capabilities of the hardware platform.
				// https://uefi.org/specs/UEFI/2.11/08_Services_Runtime_Services.html#gettime
				gRT->GetTime(&Now, NULL);
				if (Now.Second != Start.Second)
				{
					Start = Now;
					Timer++;
				}
			}
			// Timeout
			if (!Global_UtilsHttp_HttpResponseDone || ResponseMessage.BodyLength == 0)
			{
				Status = EFI_TIMEOUT;
				Print(L"[ERROR] Function_DownloadFile - Timeout [While-Global_UtilsHttp_HttpResponseDone]: %r (%llx).\r\n", Status, Status);
				break;
			}


			// -------------------------------------------------------------------------------------------------------------
			// HTTP chunk length
			Print(L"[INFO]  Function_DownloadFile - [While] HTTP chunk length %u\r\n",(UINT32)ResponseMessage.BodyLength);


			// -------------------------------------------------------------------------------------------------------------
			// Set ToCopy to the number of bytes received in this chunk
			ToCopy = ResponseMessage.BodyLength;


			// -------------------------------------------------------------------------------------------------------------
			// If the received chunk would exceed the total expected Content-Length, adjust it to avoid overflow
			if (Downloaded + ToCopy > ContentLength)
			{
				ToCopy = ContentLength - Downloaded;
			}


			// -------------------------------------------------------------------------------------------------------------
			// Copy the received chunk into the correct offset inside the full download buffer (this accumulates the complete file chunk by chunk)
			CopyMem(BufferDownload + Downloaded, BufferChunk, ToCopy);


			// -------------------------------------------------------------------------------------------------------------
			// Update the total number of bytes received so far
			Downloaded += ToCopy;


			// -------------------------------------------------------------------------------------------------------------
			// Log how many total bytes have been downloaded so far, for debugging/tracking purposes
			Print(L"[INFO]  Function_DownloadFile - Bytes received %d / %d bytes\r\n", Downloaded, ContentLength);


			// -------------------------------------------------------------------------------------------------------------
			// Clean up the event object associated with the current response chunk

			// Releasing the event avoids leaks and ensures a clean state before the next chunk

			// Closes an event.
			// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-closeevent
			gBS->CloseEvent(ResponseToken.Event);
			ResponseToken.Event = NULL;


			// -------------------------------------------------------------------------------------------------------------
			// Free the temporary buffer used to hold the current chunk

			// Returns pool memory to the system.
			// https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-freepool
			FreePool(BufferChunk);
			BufferChunk = NULL;

		}

		// Failed
		if (EFI_ERROR(Status))
		{
			Print(L"[ERROR] Function_DownloadFile - while (Downloaded < ContentLength) failed: %r (%llx).\r\n", Status, Status);
			break;
		}

		// -----------------------------------------------------------------------------------------------------------------
		// Save the downloaded content to the specified file
		Status = Function_SaveToFile(DestinationPath, BufferDownload, Downloaded);

	} while(FALSE);

	// ---------------------------------------------------------------------------------------------------------------------
	// Clean
	Print(L"[INFO]  Function_DownloadFile - Clean\r\n");


	// If a chunk buffer was still allocated, free it (this is the buffer used for receiving HTTP body chunks)
	if(BufferChunk)
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-freepool
		FreePool(BufferChunk);
		BufferChunk = NULL;
	}
	// If the response event is still open, close it
	if (ResponseToken.Event)
	{
		// Closes an event.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-closeevent
		gBS->CloseEvent(ResponseToken.Event);
		ResponseToken.Event = NULL;
	}	
	// If the request event is still open, close it
	if (RequestToken.Event)
	{
		// Closes an event.
		// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-closeevent
		gBS->CloseEvent(RequestToken.Event);
		RequestToken.Event = NULL;
	}
	// If the full download buffer was allocated, free it (this buffer contains the full file downloaded via HTTP)
	if(BufferDownload)
	{
		// Returns pool memory to the system.
		// https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-freepool
		FreePool(BufferDownload);
		BufferDownload = NULL;
	}
	// Destroy HTTP handle
	if(HttpHandle && ServiceBinding)
	{
		// Destroys a child handle with a protocol installed on it.
		// https://uefi.org/specs/UEFI/2.11/11_Protocols_UEFI_Driver_Model.html#efi-service-binding-protocol-destroychild
		ServiceBinding->DestroyChild(ServiceBinding, HttpHandle);
		HttpHandle = NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	Print(L"[INFO]  Function_DownloadFile - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return Status;
}



/**
	@brief          The entry point for the UEFI Application.
	@details        This function is invoked by the UEFI firmware when the application is loaded. It prints a simple message to the screen and returns successfully.


	@param[in]      ImageHandle         The firmware allocated handle for the EFI image.
	@param[in]      SystemTable         A pointer to the EFI System Table.


	@retval         EFI_SUCCESS         The entry point is executed successfully.
	@retval         other               Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
UefiMain (
	IN              EFI_HANDLE          ImageHandle,
	IN              EFI_SYSTEM_TABLE    *SystemTable
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	Print(L"[INFO]  Hello World from UefiBootkit005_HTTP Downloader (UEFI Application)\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	CHAR16 *DestinationPath = L"\\EFI\\Boot\\payload.bin";
	CHAR16 *Url = L"http://192.168.1.100/payload.bin";


	// ---------------------------------------------------------------------------------------------------------------------
	// Start Download
	Print(L"[INFO]  Target URL: %s\r\n", Url);
	Print(L"[INFO]  Saving to: %s\r\n", DestinationPath);

	Status = Function_DownloadFile(Url, DestinationPath);

	if (EFI_ERROR(Status))
	{
		Print(L"[ERROR] Download failed: %r\r\n", Status);
	} else
	{
		Print(L"[INFO]  Download completed and file saved successfully.\r\n");
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return Status;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------