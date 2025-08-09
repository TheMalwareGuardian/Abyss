// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Http
**/



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Download a file over HTTP and saves it to the specified path.
	@details        This function establishes an HTTP connection, sends a GET request to the provided URL,  waits for the server's response, downloads the data in chunks if necessary, and saves the downloaded content into the destination file. It uses UEFI protocols for HTTP communication and file system access.


	@param[in]      Url                 The URL from which the file should be downloaded.
	@param[in]      DestinationPath     The full path where the downloaded file will be saved.


	@retval         EFI_SUCCESS         The file was successfully downloaded and saved.
	@retval         EFI_TIMEOUT         The request or response operation timed out.
	@retval         EFI_ABORTED         Content-Length header was missing or invalid.
	@retval         Others              Errors returned by UEFI services (e.g., failed to allocate memory, configure HTTP, etc.).
**/
EFI_STATUS
PreBootSetup_UtilsHttp_DownloadFile(
	IN              CHAR16              *Url,
	IN              CHAR16              *DestinationPath
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
