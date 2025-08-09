# ***🛠️ Quick and Dirty Kernel Driver PoCs***

> **Heads-up:**  
> This folder contains a collection of ultra-minimal kernel-mode driver PoCs I use for quick and isolated experiments. I usually avoid leaving code uncommented, because it drives me crazy, but in this case it's intentional: these snippets are just meant to be dropped into a test VM to validate a specific idea in seconds.
>
> If you're starting out in malicious kernel driver (Rootkit) development, I recommend checking out these more complete resources instead:
>
> - https://github.com/TheMalwareGuardian/Awesome-Bootkits-Rootkits-Development  
> - https://github.com/TheMalwareGuardian/Benthic


| File/Pattern                         | Purpose |
|--------------------------------------|---------|
| KernelModeDriver_Hello.*             | Simplest "Hello, kernel!" driver. |
| KernelModeDriver_HelloThreading.*    | Same as above, but spawns a periodic worker thread. |
| KernelModeDriver_IOCTLs.*            | Adds a basic IOCTL handler so user-mode can ping the driver. |
| KernelModeDriver_IOCTLsApp.*         | Tiny user-mode app that sends the IOCTL above. |
| KernelModeDriver_Mapper.*            | PoC that loads another .sys from kernel space via ZwLoadDriver. |
| KernelModeDriver_MapperApp.*         | User-mode helper to trigger the mapper driver. |

Every driver comes in three flavours:
1. .c – raw source  
2. .sys – pre-built x64 binary (Release, unsigned)  
3. .sys.c – the binary converted to a C array (exported with HxD -> Export -> C)
