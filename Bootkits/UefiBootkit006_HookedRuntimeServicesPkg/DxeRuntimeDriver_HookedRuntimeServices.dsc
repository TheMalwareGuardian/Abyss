# .DSC File
# https://github.com/tianocore/tianocore.github.io/wiki/Build-Description-Files#the-dsc-file

# This file describes how to build a package; a package being a set of components to be provided together. Note the Build will need at least one .DSC file to be successful.


[Defines]
	PLATFORM_NAME               = UefiBootkit006_HookedRuntimeServicesPkg
	PLATFORM_GUID               = 00000000-0000-0000-0000-000000000000                                          # See http://www.guidgen.com/
	PLATFORM_VERSION            = 1.00
	DSC_SPECIFICATION           = 1.26
	OUTPUT_DIRECTORY            = Build/UefiBootkit006_HookedRuntimeServicesPkg
	SUPPORTED_ARCHITECTURES     = X64
	BUILD_TARGETS               = DEBUG|RELEASE
	SKUID_IDENTIFIER            = DEFAULT


# List the various libraries the components of this package may use. This tells the build system where the library to link with is located. The modules .INF file indicates the LibNameToReference in its [LibraryClasses] section and the build system looks to this section for how to find it. The build system does not use your [Packages] section of the .INF to find the library to link with; it uses the [Packages] section to find the location of the header files for a library in a packages .DEC file.
[LibraryClasses]
	# Entry point
	UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
		# Module entry point library for UEFI drivers, DXE Drivers, DXE Runtime Drivers, and DXE SMM Drivers.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiDriverEntryPoint.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
	# Tables
	UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
		# Provides a service to retrieve a pointer to the EFI Boot Services Table. Only available to DXE and UEFI module types.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
	UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
		# Provides a service to retrieve a pointer to the EFI Runtime Services Table.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiRuntimeServicesTableLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
	# Base
	UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
		# Provides library functions for common UEFI operations. Only available to DXE and UEFI module types.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/UefiLib/UefiLib.inf
	PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
		# Provides services to print a formatted string to a buffer. All combinations of Unicode and ASCII strings are supported.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BasePrintLib/BasePrintLib.inf
	DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
		# Provides services to print debug and assert messages to a debug output device.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DebugLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
	BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
		# Provides string functions, linked list functions, math functions, synchronization functions, file path functions, and CPU architecture-specific functions.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/BaseLib.inf
	# Memory
	BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
		# Provides copy memory, fill memory, zero memory, and GUID functions.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
	MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
		# Provides services to allocate and free memory buffers of various memory types and alignments.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/MemoryAllocationLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
	# Files
	DevicePathLib|MdePkg/Library/UefiDevicePathLibDevicePathProtocol/UefiDevicePathLibDevicePathProtocol.inf
		# Provides library functions to construct and parse UEFI Device Paths.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DevicePathLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/UefiDevicePathLibDevicePathProtocol/UefiDevicePathLibDevicePathProtocol.inf
	# Stack Cookies
	StackCheckLib|MdePkg/Library/StackCheckLib/StackCheckLib.inf
		# This library provides stack cookie checking functions for symbols inserted by the compiler. This header is not intended to be used directly by modules, but rather defines the expected interfaces to each supported compiler, so that if the compiler interface is updated it is easier to track.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/StackCheckLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/StackCheckLib/StackCheckLib.inf
	StackCheckFailureHookLib|MdePkg/Library/StackCheckFailureHookLibNull/StackCheckFailureHookLibNull.inf
		# Library provides a hook called when a stack cookie check fails.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/StackCheckFailureHookLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/StackCheckFailureHookLibNull/StackCheckFailureHookLibNull.inf
	# Other
	PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
		# Provides library services to get and set Platform Configuration Database entries.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PcdLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/DxePcdLib/DxePcdLib.inf
	RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
		# Public include file for the Port IO/MMIO/MSR RegisterFilterLib.
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/RegisterFilterLib.h
		# https://github.com/tianocore/edk2/blob/master/MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf


# List the various components or modules to build for this package specifically built as a result of when the package .DSC file is built; not when the package is referenced by the [Packages] section of an .INF file. You can also specify architecture specific sections by appending a period and architecture to the end of Components (e.g.[Components.IA32]). This section can also be used for building a library referenced in the [Packages] section of an .INF file.    This is used when you want to build a separate library and link to it in a traditional way or for debugging the library to ensure it builds properly. There must be at least one .inf file listed in the components section for the build to be successful. Note: that the relative path is from the edk2 base directory and not the package directory (also referred to as the Work Space Directory)
[Components]
	UefiBootkit006_HookedRuntimeServicesPkg/DxeRuntimeDriver_HookedRuntimeServices.inf                          # DXE Runtime Driver
