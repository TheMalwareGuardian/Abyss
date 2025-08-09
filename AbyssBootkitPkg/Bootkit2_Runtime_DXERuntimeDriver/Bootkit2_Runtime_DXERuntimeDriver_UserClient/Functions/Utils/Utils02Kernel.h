// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



// Resolves the kernel-mode address of an export (e.g. DbgPrint, by locating ntoskrnl.exe, loading it in user mode, getting the RVA via GetProcAddress, and adding it to the real kernel base)
FARPROC UtilsKernel_GetKernelExport(const char* name);



// Dump PE section table in a human-readable format
void UtilsKernel_PrintSectionInfo(PIMAGE_SECTION_HEADER sec, int count);



// Apply relocations manually using the relocation table, adapting image base
void UtilsKernel_ApplyRelocations(BYTE* image, ULONGLONG newBase, ULONGLONG originalBase);



// Patch the /GS security cookie in a driver image so that the runtime value matches the one baked into the binary
BOOL UtilsKernel_FixSecurityCookie(BYTE* image, ULONGLONG kernelBase, ULONGLONG originalImageBase);



// Walk every imported module & function, patch each IAT entry with the real kernel-space address obtained via UtilsKernel_GetKernelExport()
BOOL UtilsKernel_ResolveImports(BYTE* image);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
