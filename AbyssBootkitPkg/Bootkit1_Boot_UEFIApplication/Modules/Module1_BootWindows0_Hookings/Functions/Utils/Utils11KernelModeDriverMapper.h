// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Kernel Mode Driver Mapper
**/



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



// Structure that holds the state of the mapped kernel-mode driver. This structure tracks the memory region used to store a manually mapped kernel-mode payload in RWX memory. It is updated after a successful mapping operation and can be queried or reused by other components.
typedef struct {
	// Pointer to the RWX memory region where the payload resides
	VOID *AllocatedBuffer;
	// Result of the last allocation attempt
	EFI_STATUS AllocatedBufferStatus;
} STRUCTURE_BOOTWINDOWSHOOKINGS_STRUCTURESKERNELMODEDRIVERMAPPER_MAPPED_KERNEL_MODE_DRIVER_STATE;



// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------
// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------



// Size (in bytes) of a typical x86_64 absolute jump stub (e.g., mov rax, addr; jmp rax)
#define MACRO_BOOTWINDOWSHOOKINGS_UTILSMAPPER_JMP_SIZE (14)
// Total size to overwrite in the first exported function of the mapped image. Includes the jump stub (14 bytes) + optional space for metadata, padding or inline routines.
#define MACRO_BOOTWINDOWSHOOKINGS_UTILSMAPPER_MAPPER_DATA_SIZE (MACRO_BOOTWINDOWSHOOKINGS_UTILSMAPPER_JMP_SIZE + 7)



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Global variable that tracks the current RWX memory buffer and status of the mapped kernel-mode driver payload
extern STRUCTURE_BOOTWINDOWSHOOKINGS_STRUCTURESKERNELMODEDRIVERMAPPER_MAPPED_KERNEL_MODE_DRIVER_STATE Global_BootWindowsHookings_UtilsKernelModeDriverMapper_MappedKernelModeDriverState;



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Get a loaded module entry from the boot loader's LoadOrderList based on its name.
	@details        Iterates through the bootloader's LoadOrderList to find a module whose BaseDllName matches the specified name (case-insensitive). This is useful for locating known drivers that can be hijacked during the early boot stage.


	@param[in]      LoadOrderListHead       Pointer to the head of the LoadOrderList.
	@param[in]      ModuleName              Case-insensitive name of the module to locate (e.g., L"acpiex.sys").


	@retval         !NULL                   Pointer to the driver's KLDR_DATA_TABLE_ENTRY.
	@retval         NULL                    Module not found in the load list.
**/
PKLDR_DATA_TABLE_ENTRY
BootWindowsHookings_UtilsMapper_FindLoadedBootDriverByName(
	IN              LIST_ENTRY*             LoadOrderListHead,
	IN              CHAR16*                 ModuleName
);



/**
	@brief          Redirects a boot-loaded driver's entry point to a memory-mapped payload.
	@details        This function hijacks the execution flow of a legitimate driver by overwriting its entry point with a JMP rel32 instruction pointing to the mapper payload. The payload must already be mapped in RWX memory.


	@param[in]      Ntoskrnl                Pointer to the kernel's KLDR_DATA_TABLE_ENTRY (used for resolving imports).
	@param[in]      TargetDriver            Pointer to the target driver's KLDR_DATA_TABLE_ENTRY to patch.


	@retval         EFI_SUCCESS             Driver entry point successfully patched.
	@retval         EFI_NOT_READY           The mapper buffer was not allocated or initialized.
	@retval         Other EFI_ERROR         Error occurred while mapping the payload.
**/
EFI_STATUS
BootWindowsHookings_UtilsMapper_PatchDriverEntryWithMapperPayload(
	IN              KLDR_DATA_TABLE_ENTRY   *Ntoskrnl,
	IN              KLDR_DATA_TABLE_ENTRY   *TargetModule
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
