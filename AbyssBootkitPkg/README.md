# ***🧭 Abyss Windows UEFI Bootkit***

<p align="center">
	<img src="../Images/Logos/Abyss_Bootkit.png">
</p>

The Abyss Windows UEFI Bootkit is a sophisticated implant designed to compromise Windows systems at the earliest stages of execution by targeting the UEFI boot process. Its goal is to achieve persistent kernel-level access while remaining stealthy and undetectable by standard OS-level defenses.

This bootkit operates by tampering with three critical stages of the Windows boot sequence:
- Windows Boot Manager (bootmgfw.efi)
- Windows OS Loader (winload.efi)
- Windows Kernel (ntoskrnl.exe)

Its primary purpose is to disable protections and load a fully featured kernel-mode rootkit with capabilities such as:
- File, process, and network connection hiding
- Kernel memory manipulation
- Keystroke logging
- Persistent command and control

These modifications to the kernel are performed stealthily and enable long-term persistence without triggering standard detection mechanisms.

The framework draws inspiration from advanced threats seen in the wild, including ESPecter and BlackLotus, while introducing a modular and extensible design aimed at offensive research and red team simulation.


---
---
---


## ***📑 Table of Contents***

<ul>
<li><a href="#components">Components</a></li>
<li><a href="#compilation">Compilation</a></li>
<li><a href="#installation">Installation</a></li>
<li><a href="#disclaimer">Disclaimer</a></li>
</ul>


---
---
---


<div id='components'/>

## ***🧩 Components***

This package includes the core elements that make up the Abyss Bootkit:

- **Tool_BootkitInstallation**: A set of scripts to automate the deployment of the bootkit on a compromised physical machine. These scripts simplify copying the bootkit binaries and configuration to the target's EFI System Partition (ESP) and preparing it for execution.
- **Tool_BootkitConfiguration**: A Python program used to define and encrypt the bootkit's configuration. It allows the operator to specify parameters such as payload paths, runtime options, and activation stages. The output is an encrypted blob that is parsed by the UEFI application during boot execution.
- **Bootkit1_Boot_UEFIApplication**: The main UEFI Bootkit. It executes at boot time, hooks into the Windows boot chain, patches bootmgfw.efi, winload.efi, and ntoskrnl.exe, and disables security features.
- **Bootkit2_Runtime_DXERuntimeDriver**: (Optional) A DXE runtime driver that installs itself as a persistent backdoor in firmware. It remains active after ExitBootServices to read/write memory or launch payloads.


---
---
---


<div id='compilation'/>

## ***🛠️ Compilation***

The AbyssBootkitPkg can be compiled using the EDK2 toolchain. It follows the same procedure as other UEFI Applications and DXE Drivers included in this repository. For reference, check the "Bootkits/" folder, which contains working examples and build structure.

If you're unsure how to set up the environment, refer to the "Configuring the Environment" sections or use the automation scripts available in the "Development Environment/" folder.

The compilation steps are:

- Initialize the required submodules by running 'git submodule update --init --recursive'
- Copy the AbyssBootkitPkg/ folder to the root of your EDK2 workspace (next to MdePkg, ShellPkg, etc.).
- Edit the Conf/target.txt file and set the target package as AbyssBootkitPkg/AbyssBootkitPkg.dsc
- Initialize the EDK2 environment using edksetup.bat or edksetup.sh
- Build the package

The resulting .efi binaries will be located in the Build/AbyssBootkitPkg/[BUILDTYPE]_[TOOLCHAIN]/X64/ directory. You can deploy these manually to the ESP or use them as part of your own bootkit implementation.


---
---
---


<div id='installation'/>

## ***📦 Installation***

Installation assumes you're familiar with bootkit deployment techniques.

In a controlled test environment, you can manually copy the compiled .efi files to the ESP (EFI System Partition), or chainload them via GRUB or a custom UEFI boot entry.

The deployment flow mirrors the structure used in the proof-of-concept bootkits included in this repository. For guidance, refer to the "Bootkits/" folder, where similar examples demonstrate manual installation.


---
---
---


<div id='disclaimer'/>

## ***⚠️ Disclaimer***

This tool is intended strictly for research and educational purposes.

Deployment assumes one of the following:
- You are working inside a virtual machine with Secure Boot disabled, allowing unsigned EFI binaries to execute freely for testing and development purposes.
- You have physical access to the target machine, such as in an evil maid scenario or a controlled lab setting.
- You are using advanced evasion techniques, like PKfail/LogoFAIL or compromised Platform Keys, to bypass Secure Boot validation in a security research context.

Misuse of this software outside a controlled lab environment may violate laws. You are fully responsible for how you apply this knowledge.

If you decide to use Abyss as a foundation for building your own bootkit, I take no responsibility for its use. This framework was developed to help security teams simulate real-world APT low-level threats, and to provide the knowledge needed to understand and defend against them.
