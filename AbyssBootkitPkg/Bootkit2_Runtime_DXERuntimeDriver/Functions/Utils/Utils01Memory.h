// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Memory
**/



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
);



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
