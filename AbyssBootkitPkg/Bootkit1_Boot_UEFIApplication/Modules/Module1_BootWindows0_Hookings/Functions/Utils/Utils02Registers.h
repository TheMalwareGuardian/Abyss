// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Registers
**/



// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------
// START -> MACROS ----------------------------------------------------------------------------------------------------------------------------



// https://en.wikipedia.org/wiki/Control_register
// https://www.amd.com/content/dam/amd/en/documents/processor-tech-docs/programmer-references/24593.pdf
#define CR0_WP      ((UINTN)0x00010000) // CR0 WP       (Write protect)                         When set, the CPU can't write to read-only pages when privilege level is 0.
#define CR0_PG      ((UINTN)0x80000000) // CR0 PG       (Paging)                                If 1, enable paging and use the § CR3 register, else disable paging.
#define CR4_CET     ((UINTN)0x00800000) // CR4 CET      (Control-flow Enforcement Technology)   If set, enables control-flow enforcement technology.
#define CR4_LA57    ((UINTN)0x00001000) // CR4 LA57     (57-Bit Linear Addresses)               If set, enables 5-Level Paging.
#define MSR_EFER    ((UINTN)0xC0000080) // MSR EFER     (Extended Feature Enable Register)      Contains control bits that enable additional processor features not controlled by the legacy control registers.
#define EFER_LMA    ((UINTN)0x00000400) // EFER LMA     (Long Mode Active)                      When LMA=1, the processor is running either in compatibility mode or 64-bit mode, but when LMA=0, the processor is running in legacy mode.
#define EFER_UAIE   ((UINTN)0x00100000) // EFER UAIE    (Upper Address Ignore Enable)           Provides a way for software to use bits 63:57 of an address as an arbitrary software-assigned and software-interpreted tag. When this feature is enabled, these address bits are excluded from the canonicality check that is done when the address is used for certain memory references.



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Disable WP if supported and retrieve their current state.
	@details        This function disables WP (Write Protection) if it is supported by the system's hardware. It also retrieves the current state of this feature and stores it in the output parameter.


	@param[out]     WpEnabled                   A pointer to a boolean variable to store the current write protection state. True if write protection is currently enabled, False otherwise.


	@retval         None
**/
VOID
BootWindowsHookings_UtilsRegisters_DisableWriteProtection(
	OUT             BOOLEAN                     *WpEnabled
);



/**
	@brief          Enable WP if supported.
	@details        This function enables WP (Write Protection) if it is supported by the system's hardware. It takes one boolean parameter to control the enabling/disabling of this feature.


	@param[in]      WpEnabled                   A boolean indicating whether write protection should be enabled.


	@retval         None
**/
VOID
EFIAPI
BootWindowsHookings_UtilsRegisters_EnableWriteProtection(
	IN              BOOLEAN                     WpEnabled
);



/**
	@brief          Check if the system is configured for five-level paging.
	@details        This function checks if the x86-64 system is configured for five-level paging, which is a specific paging mode used in modern x86-64 systems. It determines this by examining certain control registers and processor features.


	@retval         TRUE                        Five-level paging is enabled.
	@retval         FALSE                       Five-level paging is not enabled.
**/
BOOLEAN
BootWindowsHookings_UtilsRegisters_IsFiveLevelPagingEnabled();



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
