#pragma once
// #pragma once in a file ensures that the file's contents are only included or processed by the compiler once, even if it's included or imported multiple times in different parts of the code. This directive is often used as an include guard to prevent multiple inclusions of the same file, reducing compilation time and avoiding potential issues related to redefinition of symbols.

typedef struct _UNICODE_STRING {
	UINT16 Length;
	UINT16 MaximumLength;
	CHAR16* Buffer;
} UNICODE_STRING;

typedef UNICODE_STRING *PUNICODE_STRING;
typedef CONST UNICODE_STRING *PCUNICODE_STRING;

typedef struct _KLDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	VOID* ExceptionTable;
	UINT32 ExceptionTableSize;
	VOID* GpValue;
	struct _NON_PAGED_DEBUG_INFO* NonPagedDebugInfo;
	VOID* DllBase;
	VOID* EntryPoint;
	UINT32 SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	UINT32 Flags;
	UINT16 LoadCount;
	union {
		struct {
			UINT16 SignatureLevel : 4;
			UINT16 SignatureType : 3;
			UINT16 Frozen : 2;
			UINT16 HotPatch : 1;
			UINT16 Unused : 6;
		} s;
		UINT16 EntireField;
	} u1;
	VOID* SectionPointer;
	UINT32 CheckSum;
	UINT32 CoverageSectionSize;
	VOID* CoverageSection;
	VOID* LoadedImports;
	union {
		VOID* Spare;
		struct _KLDR_DATA_TABLE_ENTRY* NtDataTableEntry;
	} u2;
	UINT32 SizeOfImageNotRounded;
	UINT32 TimeDateStamp;
} KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;

typedef struct _BLDR_DATA_TABLE_ENTRY {
	KLDR_DATA_TABLE_ENTRY KldrEntry;
	UNICODE_STRING CertificatePublisher;
	UNICODE_STRING CertificateIssuer;
	VOID* ImageHash;
	VOID* CertificateThumbprint;
	UINT32 ImageHashAlgorithm;
	UINT32 ThumbprintHashAlgorithm;
	UINT32 ImageHashLength;
	UINT32 CertificateThumbprintLength;
	UINT32 LoadInformation;
	UINT32 Flags;
} BLDR_DATA_TABLE_ENTRY, *PBLDR_DATA_TABLE_ENTRY;

typedef struct _BL_RETURN_ARGUMENTS {
	UINT32 Version;
	UINT32 Status;
	UINT32 Flags;
	UINT64 DataSize;
	UINT64 DataPage;
} BL_RETURN_ARGUMENTS, *PBL_RETURN_ARGUMENTS;

typedef struct _BL_BCD_OPTION {
	UINT32 Type;
	UINT32 DataOffset;
	UINT32 DataSize;
	UINT32 ListOffset;
	UINT32 NextEntryOffset;
	UINT32 Empty;
} BL_BCD_OPTION, *PBL_BCD_OPTION;

typedef struct _BL_APPLICATION_ENTRY {
	CHAR8 Signature[8];
	UINT32 Flags;
	EFI_GUID Guid;
	UINT32 Unknown[4];
	BL_BCD_OPTION BcdData;
} BL_APPLICATION_ENTRY, *PBL_APPLICATION_ENTRY;

typedef enum _CONFIGURATION_CLASS {
	SystemClass,
	ProcessorClass,
	CacheClass,
	AdapterClass,
	ControllerClass,
	PeripheralClass,
	MemoryClass,
	MaximumClass
} CONFIGURATION_CLASS, *PCONFIGURATION_CLASS;

typedef enum _CONFIGURATION_TYPE {
	ArcSystem,
	CentralProcessor,
	FloatingPointProcessor,
	PrimaryIcache,
	PrimaryDcache,
	SecondaryIcache,
	SecondaryDcache,
	SecondaryCache,
	EisaAdapter,
	TcAdapter,
	ScsiAdapter,
	DtiAdapter,
	MultiFunctionAdapter,
	DiskController,
	TapeController,
	CdromController,
	WormController,
	SerialController,
	NetworkController,
	DisplayController,
	ParallelController,
	PointerController,
	KeyboardController,
	AudioController,
	OtherController,
	DiskPeripheral,
	FloppyDiskPeripheral,
	TapePeripheral,
	ModemPeripheral,
	MonitorPeripheral,
	PrinterPeripheral,
	PointerPeripheral,
	KeyboardPeripheral,
	TerminalPeripheral,
	OtherPeripheral,
	LinePeripheral,
	NetworkPeripheral,
	SystemMemory,
	DockingInformation,
	RealModeIrqRoutingTable,
	RealModePCIEnumeration,
	MaximumType
} CONFIGURATION_TYPE, *PCONFIGURATION_TYPE;

typedef struct _DEVICE_FLAGS {
	UINT32 Failed : 1;
	UINT32 ReadOnly : 1;
	UINT32 Removable : 1;
	UINT32 ConsoleIn : 1;
	UINT32 ConsoleOut : 1;
	UINT32 Input : 1;
	UINT32 Output : 1;
} DEVICE_FLAGS, *PDEVICE_FLAGS;

typedef struct _CONFIGURATION_COMPONENT {
	CONFIGURATION_CLASS Class;
	CONFIGURATION_TYPE Type;
	DEVICE_FLAGS Flags;
	UINT16 Version;
	UINT16 Revision;
	UINT32 Key;
	union {
		UINT32 AffinityMask;
		struct {
			UINT16 Group;
			UINT16 GroupIndex;
		} s;
	} u;
	UINT32 ConfigurationDataLength;
	UINT32 IdentifierLength;
	CHAR8* Identifier;
} CONFIGURATION_COMPONENT, *PCONFIGURATION_COMPONENT;

typedef struct _CONFIGURATION_COMPONENT_DATA {
	struct _CONFIGURATION_COMPONENT_DATA *Parent;
	struct _CONFIGURATION_COMPONENT_DATA *Child;
	struct _CONFIGURATION_COMPONENT_DATA *Sibling;
	CONFIGURATION_COMPONENT ComponentEntry;
	VOID* ConfigurationData;
} CONFIGURATION_COMPONENT_DATA, *PCONFIGURATION_COMPONENT_DATA;

typedef struct _NLS_DATA_BLOCK {
	VOID* AnsiCodePageData;
	VOID* OemCodePageData;
	VOID* UnicodeCaseTableData;
} NLS_DATA_BLOCK, *PNLS_DATA_BLOCK;

typedef struct _ARC_DISK_INFORMATION {
	LIST_ENTRY DiskSignatures;
} ARC_DISK_INFORMATION, *PARC_DISK_INFORMATION;

typedef struct _PROFILE_PARAMETER_BLOCK {
	UINT16 Status;
	UINT16 Reserved;
	UINT16 DockingState;
	UINT16 Capabilities;
	UINT32 DockID;
	UINT32 SerialNumber;
} PROFILE_PARAMETER_BLOCK;

typedef struct _LOADER_PERFORMANCE_DATA {
	UINT64 StartTime;
	UINT64 EndTime;
	UINT64 PreloadEndTime;
	UINT64 TcbLoaderStartTime;
	UINT64 LoadHypervisorTime;
	UINT64 LaunchHypervisorTime;
	UINT64 LoadVsmTime;
	UINT64 LaunchVsmTime;
	UINT64 ExecuteTransitionStartTime;
	UINT64 ExecuteTransitionEndTime;
	UINT64 LoadDriversTime;
	UINT64 CleanupVsmTime;
} LOADER_PERFORMANCE_DATA, *PLOADER_PERFORMANCE_DATA;

typedef enum _BOOT_ENTROPY_SOURCE_ID {
	BootEntropySourceNone = 0,
	BootEntropySourceSeedfile = 1,
	BootEntropySourceExternal = 2,
	BootEntropySourceTpm = 3,
	BootEntropySourceRdrand = 4,
	BootEntropySourceTime = 5,
	BootEntropySourceAcpiOem0 = 6,
	BootEntropySourceUefi = 7,
	BootEntropySourceCng = 8,
	BootEntropySourceTcbTpm = 9,
	BootEntropySourceTcbRdrand = 10,
	BootMaxEntropySources = 10,
} BOOT_ENTROPY_SOURCE_ID;

typedef enum _BOOT_ENTROPY_SOURCE_RESULT_CODE {
	BootEntropySourceStructureUninitialized = 0,
	BootEntropySourceDisabledByPolicy = 1,
	BootEntropySourceNotPresent = 2,
	BootEntropySourceError = 3,
	BootEntropySourceSuccess = 4,
} BOOT_ENTROPY_SOURCE_RESULT_CODE, *PBOOT_ENTROPY_SOURCE_RESULT_CODE;

typedef INT32 NTSTATUS;

#define BOOT_ENTROPY_SOURCE_DATA_SIZE	(64)

typedef struct _BOOT_ENTROPY_SOURCE_LDR_RESULT {
	BOOT_ENTROPY_SOURCE_ID SourceId;
	UINT64 Policy;
	BOOT_ENTROPY_SOURCE_RESULT_CODE ResultCode;
	NTSTATUS ResultStatus;
	UINT64 Time;
	UINT32 EntropyLength;
	UINT8 EntropyData[BOOT_ENTROPY_SOURCE_DATA_SIZE];
} BOOT_ENTROPY_SOURCE_LDR_RESULT, *PBOOT_ENTROPY_SOURCE_LDR_RESULT;

#define BootMaxEntropySources			(10)

#define BOOT_SEED_BYTES_FOR_CNG			(48)

#define BOOT_RNG_BYTES_FOR_NTOSKRNL		(1024)

typedef struct _BOOT_ENTROPY_LDR_RESULT {
	UINT32 maxEntropySources;
	BOOT_ENTROPY_SOURCE_LDR_RESULT EntropySourceResult[BootMaxEntropySources];
	UINT8 SeedBytesForCng[BOOT_SEED_BYTES_FOR_CNG];
	UINT8 RngBytesForNtoskrnl[BOOT_RNG_BYTES_FOR_NTOSKRNL];
	UINT8 KdEntropy[32];
} BOOT_ENTROPY_LDR_RESULT, *PBOOT_ENTROPY_LDR_RESULT;

typedef struct _LOADER_PARAMETER_HYPERVISOR_EXTENSION {
	UINT32 InitialHypervisorCrashdumpAreaPageCount;
	UINT32 HypervisorCrashdumpAreaPageCount;
	UINT64 InitialHypervisorCrashdumpAreaSpa;
	UINT64 HypervisorCrashdumpAreaSpa;
	UINT64 HypervisorLaunchStatus;
	UINT64 HypervisorLaunchStatusArg1;
	UINT64 HypervisorLaunchStatusArg2;
	UINT64 HypervisorLaunchStatusArg3;
	UINT64 HypervisorLaunchStatusArg4;
} LOADER_PARAMETER_HYPERVISOR_EXTENSION, *PLOADER_PARAMETER_HYPERVISOR_EXTENSION;

typedef struct _LOADER_BUGCHECK_PARAMETERS {
	UINT32 BugcheckCode;
	UINTN BugcheckParameter1;
	UINTN BugcheckParameter2;
	UINTN BugcheckParameter3;
	UINTN BugcheckParameter4;
} LOADER_BUGCHECK_PARAMETERS, *PLOADER_BUGCHECK_PARAMETERS;

typedef struct _OFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V2 {
	UINT32 Version;
	UINT32 AbnormalResetOccurred;
	UINT32 OfflineMemoryDumpCapable;
	PHYSICAL_ADDRESS ResetDataAddress;
	UINT32 ResetDataSize;
} OFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V2, *POFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V2;

typedef struct _OFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V1 {
	UINT32 Version;
	UINT32 AbnormalResetOccurred;
	UINT32 OfflineMemoryDumpCapable;
} OFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V1, *POFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V1;

typedef OFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V2 OFFLINE_CRASHDUMP_CONFIGURATION_TABLE;
typedef POFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V2 POFFLINE_CRASHDUMP_CONFIGURATION_TABLE;

typedef union _LARGE_INTEGER {
	struct {
		UINT32 LowPart;
		INT32 HighPart;
	} s;
	struct {
		UINT32 LowPart;
		INT32 HighPart;
	} u;
	INT64 QuadPart;
} LARGE_INTEGER;

#define ANYSIZE_ARRAY				1

typedef struct _LOADER_PARAMETER_CI_EXTENSION {
	UINT32 CodeIntegrityOptions;
	struct {
		UINT32 UpgradeInProgress : 1;
		UINT32 IsWinPE : 1;
		UINT32 CustomKernelSignersAllowed : 1;
		UINT32 StateSeparationEnabled : 1;
		UINT32 Reserved : 28;
	} s;
	LARGE_INTEGER WhqlEnforcementDate;
	UINT32 RevocationListOffset;
	UINT32 RevocationListSize;
	UINT32 CodeIntegrityPolicyOffset;
	UINT32 CodeIntegrityPolicySize;
	UINT32 CodeIntegrityPolicyHashOffset;
	UINT32 CodeIntegrityPolicyHashSize;
	UINT32 CodeIntegrityPolicyOriginalHashOffset;
	UINT32 CodeIntegrityPolicyOriginalHashSize;
	INT32 WeakCryptoPolicyLoadStatus;
	UINT32 WeakCryptoPolicyOffset;
	UINT32 WeakCryptoPolicySize;
	UINT32 SecureBootPolicyOffset;
	UINT32 SecureBootPolicySize;
	UINT32 Reserved2;
	UINT8 SerializedData[ANYSIZE_ARRAY];
} LOADER_PARAMETER_CI_EXTENSION, *PLOADER_PARAMETER_CI_EXTENSION;

typedef struct _LOADER_HIVE_RECOVERY_INFO {
	struct {
		UINT32 Recovered : 1;
		UINT32 LegacyRecovery : 1;
		UINT32 SoftRebootConflict : 1;
		UINT32 MostRecentLog : 3;
		UINT32 LoadedFromSnapshot : 1;
		UINT32 Spare		: ((sizeof(UINT32) * 8) - 7);
	} s;
	UINT32 LogNextSequence;
	UINT32 LogMinimumSequence;
	UINT32 LogCurrentOffset;
} LOADER_HIVE_RECOVERY_INFO, *PLOADER_HIVE_RECOVERY_INFO;

typedef struct _LEAP_SECOND_DATA {
	BOOLEAN Enabled;
	UINT32 Count;
	LARGE_INTEGER Data[1];
} LEAP_SECOND_DATA, *PLEAP_SECOND_DATA;

typedef struct _LOADER_RESET_REASON {
	UINT8 Supplied;
	union {
		struct {
			UINT64 Pch : 1;
			UINT64 EmbeddedController : 1;
			UINT64 Reserved : 6;
		} Component;
		UINT64 AsULONG64;
		UINT8 AsBytes[8];
	} Basic;
	UINT32 AdditionalInfo[8];
} LOADER_RESET_REASON, *PLOADER_RESET_REASON;

typedef struct _VSM_PERFORMANCE_DATA {
	UINT64 LaunchVsmMark[8];
} VSM_PERFORMANCE_DATA, *PVSM_PERFORMANCE_DATA;

typedef struct _NUMA_MEMORY_RANGE {
	UINT32 ProximityId;
	UINT64 BasePage;
	UINT64 EndPage;
} NUMA_MEMORY_RANGE, *PNUMA_MEMORY_RANGE;

typedef struct _LOADER_FEATURE_CONFIGURATION_DIAGNOSTIC_INFORMATION {
	UINT8 OriginalBootStatus;
	UINT8 NewBootStatus;
	UINT8 ConfigurationLoaded;
	UINT8 Spare;
	union {
		union {
			UINT32 AllFlags;
			UINT32 LkgSupported : 1;
			UINT32 FinalBootBeforeRecovery : 1;
			UINT32 ConfigurationComparisonAttempted : 1;
			UINT32 CurrentConfigurationLoadAttempted : 1;
			UINT32 LkgConfigurationLoadAttempted : 1;
			UINT32 UsageSubscriptionLoadAttempted : 1;
			UINT32 Spare : 26;
		} u;
	} Flags;
	INT32 ConfigurationComparisonStatus;
	INT32 CurrentConfigurationLoadStatus;
	INT32 LkgConfigurationLoadStatus;
	INT32 UsageSubscriptionLoadStatus;
} LOADER_FEATURE_CONFIGURATION_DIAGNOSTIC_INFORMATION, *PLOADER_FEATURE_CONFIGURATION_DIAGNOSTIC_INFORMATION;

typedef struct _LOADER_FEATURE_CONFIGURATION_INFORMATION {
	VOID* FeatureConfigurationBuffer;
	UINTN FeatureConfigurationBufferSize;
	VOID* UsageSubscriptionBuffer;
	UINTN UsageSubscriptionBufferSize;
	VOID* DelayedUsageReportBuffer;
	UINTN DelayedUsageReportBufferSize;
	LOADER_FEATURE_CONFIGURATION_DIAGNOSTIC_INFORMATION DiagnosticInformation;
} LOADER_FEATURE_CONFIGURATION_INFORMATION, *PLOADER_FEATURE_CONFIGURATION_INFORMATION;

typedef struct _ETW_BOOT_CONFIG
{
	UINT32 MaxLoggers;
	LIST_ENTRY BootLoggersList;
} ETW_BOOT_CONFIG, *PETW_BOOT_CONFIG;

typedef struct _BOOT_FIRMWARE_RAMDISK_INFO {
	UINT32 Version;
	UINT32 BlockSize;
	UINT64 BaseAddress;
	UINT64 Size;
} BOOT_FIRMWARE_RAMDISK_INFO, *PBOOT_FIRMWARE_RAMDISK_INFO;

typedef struct _INSTALLED_MEMORY_RANGE {
	UINT64 BasePage;
	UINT64 PageCount;
} INSTALLED_MEMORY_RANGE, *PINSTALLED_MEMORY_RANGE;

typedef struct _LOADER_PARAMETER_EXTENSION {
	UINT32 Size;
	PROFILE_PARAMETER_BLOCK Profile;
	VOID* EmInfFileImage;
	UINT32 EmInfFileSize;
	VOID* TriageDumpBlock;
	struct _HEADLESS_LOADER_BLOCK *HeadlessLoaderBlock;
	struct _SMBIOS3_TABLE_HEADER *SMBiosEPSHeader;
	VOID* DrvDBImage;
	UINT32 DrvDBSize;
	VOID* DrvDBPatchImage;
	UINT32 DrvDBPatchSize;
	struct _NETWORK_LOADER_BLOCK *NetworkLoaderBlock;
#if defined(_X86_)
	UINT8* HalpIRQLToTPR;
	UINT8* HalpVectorToIRQL;
#endif
	LIST_ENTRY FirmwareDescriptorListHead;
	VOID* AcpiTable;
	UINT32 AcpiTableSize;
	struct {
		UINT32 LastBootSucceeded : 1;
		UINT32 LastBootShutdown : 1;
		UINT32 IoPortAccessSupported : 1;
		UINT32 BootDebuggerActive : 1;
		UINT32 StrongCodeGuarantees : 1;
		UINT32 HardStrongCodeGuarantees : 1;
		UINT32 SidSharingDisabled : 1;
		UINT32 TpmInitialized : 1;
		UINT32 VsmConfigured : 1;
		UINT32 IumEnabled : 1;
		UINT32 IsSmbboot : 1;
		UINT32 BootLogEnabled : 1;
		UINT32 DriverVerifierEnabled : 1;
		UINT32 SuppressMonitorX : 1;
		UINT32 SuppressSmap : 1;
		UINT32 PointerAuthKernelIpEnabled : 1;
		UINT32 SplitLargeNumaNodes : 1;
		UINT32 Unused : 3;
		UINT32 FeatureSimulations : 6;
		UINT32 MicrocodeSelfHosting : 1;
		UINT32 XhciLegacyHandoffSkip : 1;
		UINT32 DisableInsiderOptInHVCI : 1;
		UINT32 MicrocodeMinVerSupported : 1;
		UINT32 GpuIommuEnabled : 1;
	} s;
	LOADER_PERFORMANCE_DATA LoaderPerformanceData;
	LIST_ENTRY BootApplicationPersistentData;
	VOID* WmdTestResult;
	GUID BootIdentifier;
	UINT32 ResumePages;
	VOID* DumpHeader;
	VOID* BgContext;
	VOID* NumaLocalityInfo;
	VOID* NumaGroupAssignment;
	LIST_ENTRY AttachedHives;
	UINT32 MemoryCachingRequirementsCount;
	VOID* MemoryCachingRequirements;
	BOOT_ENTROPY_LDR_RESULT BootEntropyResult;
	UINT64 ProcessorCounterFrequency;
	LOADER_PARAMETER_HYPERVISOR_EXTENSION HypervisorExtension;
	GUID HardwareConfigurationId;
	LIST_ENTRY HalExtensionModuleList;
	LIST_ENTRY PrmUpdateModuleList;
	LIST_ENTRY PrmFirmwareModuleList;
	LARGE_INTEGER SystemTime;
	UINT64 TimeStampAtSystemTimeRead;
	union {
		UINT64 BootFlags;
		struct {
			UINT64 DbgMenuOsSelection : 1;
			UINT64 DbgHiberBoot : 1;
			UINT64 DbgSoftRestart : 1;
			UINT64 DbgMeasuredLaunch : 1;
		} s;
	} u1;
	union {
		UINT64 InternalBootFlags;
		struct {
			UINT64 DbgUtcBootTime : 1;
			UINT64 DbgRtcBootTime : 1;
			UINT64 DbgNoLegacyServices : 1;
		} s;
	} u2;
	VOID* WfsFPData;
	UINT32 WfsFPDataSize;
	LOADER_BUGCHECK_PARAMETERS BugcheckParameters;
	VOID* ApiSetSchema;
	UINT32 ApiSetSchemaSize;
	LIST_ENTRY ApiSetSchemaExtensions;
	UNICODE_STRING AcpiBiosVersion;
	UNICODE_STRING SmbiosVersion;
	UNICODE_STRING EfiVersion;
	struct _DEBUG_DEVICE_DESCRIPTOR *KdDebugDevice;
	OFFLINE_CRASHDUMP_CONFIGURATION_TABLE OfflineCrashdumpConfigurationTable;
	UNICODE_STRING ManufacturingProfile;
	VOID* BbtBuffer;
#if defined(_X86_) || defined (_AMD64_)
	UINT64 XsaveAllowedFeatures;
	UINT32 XsaveFlags;
#endif
	VOID* BootOptions;
	UINT32 IumEnablement;
	UINT32 IumPolicy;
	INT32 IumStatus;
	UINT32 BootId;
	PLOADER_PARAMETER_CI_EXTENSION CodeIntegrityData;
	UINT32 CodeIntegrityDataSize;
	LOADER_HIVE_RECOVERY_INFO SystemHiveRecoveryInfo;
	UINT32 SoftRestartCount;
	INT64 SoftRestartTime;
#if defined(_AMD64_)
	VOID* HypercallCodeVa;
#endif
#if defined(_AMD64_) || defined(_ARM64_)
	VOID* HalVirtualAddress;
	UINT64 HalNumberOfBytes;
#endif
	PLEAP_SECOND_DATA LeapSecondData;
	UINT32 MajorRelease;
	UINT32 Reserved1;
	CHAR8 NtBuildLab[224];
	CHAR8 NtBuildLabEx[224];
	LOADER_RESET_REASON ResetReason;
	UINT32 MaxPciBusNumber;
	UINT32 FeatureSettings;
	UINT32 HotPatchReserveSize;
	UINT32 RetpolineReserveSize;
#if defined(_AMD64_)
	struct
	{
		VOID* CodeBase;
		UINTN CodeSize;
	} MiniExecutive;
#endif
	VSM_PERFORMANCE_DATA VsmPerformanceData;
	PNUMA_MEMORY_RANGE NumaMemoryRanges;
	UINT32 NumaMemoryRangeCount;
	UINT32 IommuFaultPolicy;
	LOADER_FEATURE_CONFIGURATION_INFORMATION FeatureConfigurationInformation;
	ETW_BOOT_CONFIG EtwBootConfig;
	PBOOT_FIRMWARE_RAMDISK_INFO FwRamdiskInfo;
	VOID* IpmiHwContext;
#if defined(_AMD64_)
	UINT64 IdleThreadShadowStack;
	UINT64 TransitionShadowStack;
	UINT64* IstShadowStacksTable;
	UINT64 ReservedForKernelCet[2];
	PMEMORY_MIRRORING_DATA MirroringData;
#elif defined(_ARM64_)
	UINT64 PointerAuthKernelIpKey[2];
#endif
	LARGE_INTEGER Luid;
	struct {
		PINSTALLED_MEMORY_RANGE Ranges;
		UINT32 RangeCount;
	} InstalledMemory;
	LIST_ENTRY HotPatchList;
} LOADER_PARAMETER_EXTENSION, *PLOADER_PARAMETER_EXTENSION;

typedef struct _I386_LOADER_BLOCK {
#if defined(_X86_) || defined(_AMD64_)
	VOID* CommonDataArea;
	UINT32 MachineType;
	UINT32 VirtualBias;
#else
	UINT32 PlaceHolder;
#endif
} I386_LOADER_BLOCK, *PI386_LOADER_BLOCK;

typedef struct _ARM_LOADER_BLOCK {
#if defined(_ARM_) || defined(_ARM64_)
	UINTN VirtualBias;
	VOID* KdCpuBuffer;
#else
	UINT32 PlaceHolder;
#endif
} ARM_LOADER_BLOCK, *PARM_LOADER_BLOCK;


typedef struct _VIRTUAL_EFI_RUNTIME_SERVICES {
	UINTN GetTime;
	UINTN SetTime;
	UINTN GetWakeupTime;
	UINTN SetWakeupTime;
	UINTN SetVirtualAddressMap;
	UINTN ConvertPointer;
	UINTN GetVariable;
	UINTN GetNextVariableName;
	UINTN SetVariable;
	UINTN GetNextHighMonotonicCount;
	UINTN ResetSystem;
	UINTN UpdateCapsule;
	UINTN QueryCapsuleCapabilities;
	UINTN QueryVariableInfo;
} VIRTUAL_EFI_RUNTIME_SERVICES, *PVIRTUAL_EFI_RUNTIME_SERVICES;

typedef struct _EFI_FIRMWARE_INFORMATION {
	UINT32 FirmwareVersion;
	PVIRTUAL_EFI_RUNTIME_SERVICES VirtualEfiRuntimeServices;
	NTSTATUS SetVirtualAddressMapStatus;
	UINT32 MissedMappingsCount;
	LIST_ENTRY FirmwareResourceList;
	VOID* EfiMemoryMap;
	UINT32 EfiMemoryMapSize;
	UINT32 EfiMemoryMapDescriptorSize;
} EFI_FIRMWARE_INFORMATION, *PEFI_FIRMWARE_INFORMATION;

typedef struct _PCAT_FIRMWARE_INFORMATION {
	UINT32 PlaceHolder;
} PCAT_FIRMWARE_INFORMATION, *PPCAT_FIRMWARE_INFORMATION;

typedef struct _FIRMWARE_INFORMATION_LOADER_BLOCK {
	struct {
		UINT32 FirmwareTypeEfi: 1;
		UINT32 EfiRuntimeUseIum: 1;
		UINT32 EfiRuntimePageProtectionSupported: 1;
#if defined (_ARM64_)
		UINT32 FirmwareStartedInEL2: 1;
		UINT32 Reserved: 28;
#else
		UINT32 Reserved: 29;
#endif
	} s;
	union {
		EFI_FIRMWARE_INFORMATION EfiInformation;
		PCAT_FIRMWARE_INFORMATION PcatInformation;
	} u;
} FIRMWARE_INFORMATION_LOADER_BLOCK, *PFIRMWARE_INFORMATION_LOADER_BLOCK;

typedef struct _RTL_BALANCED_NODE {
	union {
		struct _RTL_BALANCED_NODE *Children[2];
		struct {
			struct _RTL_BALANCED_NODE *Left;
			struct _RTL_BALANCED_NODE *Right;
		} s;
	} u1;
	union {
		UINT8 Red : 1;
		UINT8 Balance : 2;
		UINTN ParentValue;
	} u2;
} RTL_BALANCED_NODE, *PRTL_BALANCED_NODE;

typedef struct _RTL_RB_TREE {
	PRTL_BALANCED_NODE Root;
	union {
		UINT8 Encoded : 1;
		PRTL_BALANCED_NODE Min;
	} u;
} RTL_RB_TREE, *PRTL_RB_TREE;

/**
	The LOADER_PARAMETER_BLOCK (formally _LOADER_PARAMETER_BLOCK) is the structure through which the kernel and HAL learn the initialisation data that was gathered by the loader. Historically, and for most practical purposes even in recent Windows versions, there is only ever the one instance of this structure. It is prepared by the loader as its means of handing over to the kernel. When the loader calls the kernel's initialisation routine, the address of the loader block is the one argument. The kernel saves the address for a while in the exported variable KeLoaderBlock. At the end of the kernel's initialisation, the structure gets freed and the variable gets cleared. During the system's initialisation, however, knowledge of this structure can be very helpful when debugging. 
**/
typedef struct _LOADER_PARAMETER_BLOCK {
	UINT32 OsMajorVersion;
	UINT32 OsMinorVersion;
	UINT32 Size;
	UINT32 OsLoaderSecurityVersion;
	LIST_ENTRY LoadOrderListHead;
	LIST_ENTRY MemoryDescriptorListHead;
	LIST_ENTRY BootDriverListHead;
	LIST_ENTRY EarlyLaunchListHead;
	LIST_ENTRY CoreDriverListHead;
	LIST_ENTRY CoreExtensionsDriverListHead;
	LIST_ENTRY TpmCoreDriverListHead;
	UINTN KernelStack;
	UINTN Prcb;
	UINTN Process;
	UINTN Thread;
	UINT32 KernelStackSize;
	UINT32 RegistryLength;
	VOID* RegistryBase;
	PCONFIGURATION_COMPONENT_DATA ConfigurationRoot;
	CHAR8* ArcBootDeviceName;
	CHAR8* ArcHalDeviceName;
	CHAR8* NtBootPathName;
	CHAR8* NtHalPathName;
	CHAR8* LoadOptions;
	PNLS_DATA_BLOCK NlsData;
	PARC_DISK_INFORMATION ArcDiskInformation;
	PLOADER_PARAMETER_EXTENSION Extension;
	union {
		I386_LOADER_BLOCK I386;
		ARM_LOADER_BLOCK Arm;
	} u;
	FIRMWARE_INFORMATION_LOADER_BLOCK FirmwareInformation;
	CHAR8* OsBootstatPathName;
	CHAR8* ArcOSDataDeviceName;
	CHAR8* ArcWindowsSysPartName;
	RTL_RB_TREE MemoryDescriptorTree;
} LOADER_PARAMETER_BLOCK, *PLOADER_PARAMETER_BLOCK;
