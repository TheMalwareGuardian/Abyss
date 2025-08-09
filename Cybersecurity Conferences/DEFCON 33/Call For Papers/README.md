# Talk


---
---
---


## 1. Submit anonymously? 

No, including my name is fine

---


## 2. Are you submitting for the DEF CON Policy Track? Is this a Policy themed talk?

No, I'm only interested in the main talk tracks. 

---


## 3. Primary Speaker Name

Alejandro Vazquez Vazquez

---


## 4. Primary Speaker Pseudonym or Handle

TheMalwareGuardian

---


## 5. Name Publishing

Both Name and Handle

---


## 6. Primary Speaker's preferred pronouns:

He/Him

---


## 7. Primary Speaker Affiliation

Independent Researcher

---


## 8. Primary Speaker Email Address:

@protonmail.com

---


## 9. Backup Email or Contact method

@gmail.com

---


## 10. Primary Speaker Phone Number:

+34 600000000

---


## 11. Primary Speaker's Social Media / Personal Site Links

https://www.linkedin.com/in/vazquez-vazquez-alejandro/

---


## 12. Primary Speaker Bio

Alejandro Vázquez Vázquez is a security researcher and Red Team Operator with deep expertise in Windows Internals, malware development, and advanced threat emulation. He is one of the few professionals who has publicly presented live bootkit and rootkit development, including real-world demos and open-source examples such as Abyss and Benthic.

He has been behind some of the most hands-on offensive projects out there: crafting custom malware for Red Team ops, deploying stealthy UEFI implants for long-term persistence, developing real OT honeypots to lure attackers targeting critical infrastructure, building AI-powered frameworks that automate and scale pentest workflows, and designing platforms to hunt and profile ransomware groups.

By day, he conducts offensive security operations while also serving as an instructor in several master's degrees, teaching malware analysis, exploit development, bootkits, and rootkits to the next generation of cybersecurity professionals. By night, he writes implants that play nice with modern security mechanisms. From pre-boot to the kernel, if it runs low enough, he wants to control it. And if it's undocumented, even better.

He doesn't just give talks. He builds the tools, shares the code, and gives you the full presentation, so you can run it yourself and teach others.

---


## 13. Has the speaker spoken at a previous DEF CON?

No

---


## 14. ADA / Accessibility Accommodations

No

---


## 15. Speaker 2 Information

- Name: Maria San Jose
- Handle: drkrysSrng
- Name publishing preference: Both Name and Handle
- Pronouns: She/Her
- Title and company: Cyber Threat Analyst at Guardia Civil
- Email address: @gmail.com
- Phone number: +34 600000000
- Social media: https://www.linkedin.com/in/mariasanjose/, https://x.com/merytkdsj
- Previous DEFCON presentations: No
- ADA accommodations: No

---


## 16. Speaker 2 Bio

Maria is a cybersecurity specialist working for the Guardia Civil, Spain's national military police force. She has served in some of the most specialized cyber units within the organization, including the Cyberterrorism Group and, currently, the Cybercrime Department of the Central Operative Unit (UCO), where she focuses on cybercrime investigations and threat intelligence.

Before joining the Guardia Civil, Maria built a strong foundation as a software engineer, contributing to flight simulation systems for major air navigation entities such as ENAIRE (AENA) and ROMATSA (Romania).

Outside her official duties, she is passionate about malware analysis and reverse engineering, dedicating personal time to studying advanced threats and attack techniques. Her combined experience in software development and threat investigation gives her a unique, well-rounded perspective on both offensive and defensive security.

---


## 23. Basecamp Permission

Yes

---


## 24. Visa Requirement

Yes

---


## 25. Visa Information

- Speaker 1
    - Name as is on passport: NAME
    - Passport number: WWWDDDDDD
    - Country of issue: SPAIN
    - Date of issue: DD-MM-YYYY
    - Date of expiration: DD-MM-YYYY
    - Country of origin: SPAIN

- Speaker 2
    - Name as is on passport: NAME
    - Passport number: WWWDDDDDD
    - Country of issue: SPAIN
    - Date of issue: DD-MM-YYYY
    - Date of expiration: DD-MM-YYYY
    - Country of origin: SPAIN

---


## 26. Presentation Title

Infecting the Boot to Own the Kernel: Bootkits and Rootkits Development

---


## 27. Duration  

45 Minutes

---


## 28. Demonstration

The demonstration will guide attendees through the full infection chain of a real UEFI Bootkit with a custom Kernel-mode Rootkit, showing each step in detail from firmware manipulation to full kernel control. It starts with the installation of custom platform keys into the firmware, enabling self-signed components to run without violating boot policies. Then, the Bootkit is configured using a dedicated tool that defines its behavior through a modular architecture.

Once configured, attendees will see how the first stage of the Bootkit consists of a custom UEFI application that is executed during boot. This application immediately establishes internet connectivity from the firmware environment and downloads remote payloads before the operating system is loaded and before any defensive mechanisms are active. It then loads a DXE Runtime Driver into memory, which remains active after boot and hooks UEFI runtime services that are still accessible from the operating system, enabling stealth and post-boot interaction.

The demo then focuses on the hooking phase, where the Bootkit modifies selected functions in three critical Windows components: Windows Boot Manager, OS Loader, and the NT Kernel. Attendees will see which functions are hooked, how they are located and patched in memory, and how this process disables Driver Signature Enforcement and redirects execution to allow rootkit injection.

Finally, the demonstration concludes with the deployment of the Kernel-mode Rootkit, executed according to the configuration defined at the beginning. During this stage, attendees will observe how it establishes communication with a command and control server, while also dropping a lightweight user-mode application that enables direct interaction with the rootkit. Through this interface, the audience will see the rootkit in action as it hides files, processes, and network connections, intercepts or blocks traffic, and downloads additional payloads from remote sources. All operations are carried out entirely from kernel space, without relying on any user-mode components for execution.

---


## 29. Live Demos

I understand I will need to create a demo video for my presentation. 

---


## 30. Audience Participation

No

---


## 31. New Tool

Yes

---


## 32. Tool License

GNU General Public License v3.0

---


## 33. Tool Details

Full tool suite

---


## 34. Is the tool currently public? 

No

---


## 35. What language is the tool written in?

C

---


## 36.Tool Description

This toolset is a modular and fully customizable UEFI Bootkit and Kernel-mode Rootkit framework designed for offensive security research and red team operations. It includes several components: a configurable Bootkit (UEFI Application / DXE Runtime Driver) and a Kernel-mode Rootkit (Kernel-mode Driver / User-mode Application), all working together to provide end-to-end persistence and stealth capabilities.

The Bootkit operates based on a configuration generated by an external tool, also provided, which defines how the implant behaves. During execution, the Bootkit performs actions such as establishing internet connectivity before the OS loads, downloading and encrypting files, disabling Driver Signature Enforcement (DSE), and hooking critical boot components to control the system's execution flow. It also loads a DXE Runtime Driver that persists after the OS has booted, hooking UEFI runtime services to maintain stealth and enable post-boot interaction.

The Kernel-mode Rootkit is deployed according to the Bootkit's configuration and provides advanced functionality, including file, process, and connection hiding, traffic blocking, and Command and Control communication entirely from kernel space. A lightweight user-mode application is also included to interact with the rootkit, allowing operators to send targeted commands and trigger specific actions during post-exploitation.

---


## 37. Presentation is about Tool

No, the tool is just one component of the talk

---


## 38. Are you releasing a new exploit/vulnerability?

No

---


## 39. Prior Submission

Yes

---


## 40. Prior Submission Details

Submitted to Black Hat USA 2025, August 2-7, 2025, Mandalay Bay, Las Vegas.

---


## 41. Are you submitting this or any other topic to Black Hat USA?

Yes

---


## 42. Black Hat USA Submissions

Yes, a related version of this talk was submitted to Black Hat USA 2025. That version focuses on presenting the full toolset and walking through the complete attack chain, showing all features in action to help demystify Bootkit and Rootkit development.

The DEF CON version takes a deeper dive into the internals. It focuses on how each component is built, how they interact with each other, and offers a more reverse engineering-oriented perspective for better understanding of low-level behavior.

---


## 43. Dependency upon Black Hat Acceptance

No, I can present at DEF CON 33 regardless

---


## 44. Other Submissions

No

---


## 45. Presentation Abstract

Bootkits and Rootkits represent some of the most complex and stealthy forms of malware, capable of achieving full system control before and after the OS is loaded. While often discussed in theory, their actual construction, interaction, and execution flow remain mostly hidden from public view. This talk sheds light on how these implants are built and how their components interact across boot stages and kernel space.

We'll explore the internals of a fully functional UEFI Bootkit and Kernel-mode Rootkit, examining their modular design, runtime interactions, and the mechanisms used to hook critical parts of the Windows boot chain. Attendees will see how these implants operate across pre-boot and post-boot phases, including early internet connectivity from firmware, dynamic payload delivery, runtime service hooking, deep kernel control, and advanced capabilities like hiding files, processes, and network activity, blocking traffic, capturing keystrokes, and maintaining command and control directly from kernel space.

Everything shown on stage will be yours to explore: a complete Bootkit and Rootkit framework, fully customizable and ready to simulate real threats, test defenses, or build something even stealthier.

---


## 46. Publishable References

UEFI Specification, Version 2.11. Unified Extensible Firmware Interface Forum.  
https://uefi.org/specs/UEFI/2.11/

Alex Matrosov, Eugene Rodionov, Sergey Bratus – Rootkits and Bootkits: Reversing Modern Malware and Next Generation Threats.

Pavel Yosifovich - Windows Kernel Programming, 2nd Edition.

Pavel Yosifovich, Andrea Allievi, Alex Ionescu, Mark E. Russinovich, David A. Solomon – Windows Internals, Part 1 & 2, 7th Edition.

Martin Smolár and Anton Cherepanov (ESET Research team) – UEFI threats moving to the ESP: Introducing ESPecter bootkit  
https://www.welivesecurity.com/2021/10/05/uefi-threats-moving-esp-introducing-especter-bootkit/

Martin Smolár (ESET Research team) – BlackLotus UEFI bootkit: Myth confirmed  
https://www.welivesecurity.com/2023/03/01/blacklotus-uefi-bootkit-myth-confirmed/

Lior Rochberger and Dan Yashnik (Palo Alto Networks Unit 42) – Diving Into Glupteba's UEFI Bootkit  
https://unit42.paloaltonetworks.com/glupteba-malware-uefi-bootkit/

Takahiro Haruyama, Fabio Pagani, Yegor Vasilenko, Anton Ivanov, and Sam Thomas (Binarly Research team) – UEFI Bootkit Hunting: In-Depth Search for Unique Code Behavior  
https://www.binarly.io/blog/uefi-bootkit-hunting-in-depth-search-for-unique-code-behavior

Alejandro Vazquez Vazquez – Awesome Bootkits & Rootkits Development (curated learning repository)  
https://github.com/TheMalwareGuardian/Awesome-Bootkits-Rootkits-Development

---


## 47. Confidential References

There are no confidential references. All sources used in the talk, including books, technical articles, and public bootkit/rootkit samples, are either listed in the public references or included in the shared GitHub repository.

---


## 48. Detailed Outline: 

1. Introduction
    1. Who we are: Alejandro (security researcher and malware developer for Red Team operations) and Maria (cyber threat analyst at Guardia Civil, military police cybercrime unit).
    2. Why this talk matters: Bootkits and Rootkits represent the peak of stealth and persistence, but their real internals remain rarely taught or shared.
    3. Our goal: Show how these implants are architected, how they interact across the boot process and kernel, and how to analyze or simulate them from both attacker and reverse engineering perspectives.

2. Overview of the Infection Chain
    1. High-level flow from firmware to kernel and post-boot control.
    2. Three core components:
        1. Configurator tool (pre-deployment, defines Bootkit behavior)
        2. UEFI Bootkit (UEFI application + DXE Runtime Driver)
        3. Kernel-mode Rootkit (driver + optional userland interface)

3. Boot Process Fundamentals
    1. Step-by-step walkthrough of the Windows boot process (UEFI -> Windows Boot Manager -> OS Loader -> Kernel)
    2. When and where malware can intervene in execution.
    3. Using firmware tools and debugging techniques to reverse boot components.

4. Bootkit Internals
    1. Structure and modular design of the Bootkit: highly customizable logic and payloads.
    2. Pre-boot actions:
        1. Network connectivity using UEFI protocols (HTTP, DNS)
        2. Downloading config and payloads before OS-level protections are active
    3. Runtime extension:
        1. Loading a DXE Runtime Driver that persists into Windows
        2. Hooking UEFI Runtime Services accessible from OS context (e.g., GetVariable)
    4. Hooking techniques:
        1. In-memory patching of LoadImage, ExitBootServices, and key OS loader functions (ImgArchStartBootApplication, BlImgAllocateImageBuffer, OslFwpKernelSetupPhase1)
        2. Redirection logic and execution flow tampering
    5. Configurable behaviors:
        1. Optional ransomware mode, payload chaining, stealth injection points

5. Rootkit Deployment & Capabilities
    1. Triggered from Bootkit based on prior configuration
    2. Delivered as a kernel-mode driver with advanced stealth capabilities
    3. Drops a user-mode controller for direct operator interaction
    4. Features demonstrated:
        1. File, process, and connection hiding
        2. Keystroke logging
        3. Traffic blocking
        4. Covert in-kernel C2 with no userland beaconing
    5. Techniques:
        1. DKOM, Minifilter, WFP, WSK, runtime service abuse

6. Live Demo: Infection Chain in Action
    1. From platform key installation to full system compromise
    2. Pre-OS network communications, payload fetching, runtime driver loading
    3. Boot component hooking with visual indicators showing where execution flow is intercepted and redirected during the transition from firmware to kernel.
    4. Rootkit execution: stealth features, runtime hooks, kernel-resident C2
    5. Demonstration of bidirectional communication through userland tool
    6. Emphasis on transparency: each step fully documented and reproducible with the provided toolset

7. What Attendees Receive
    1. Full toolset with source code:
        1. UEFI Bootkit (App + DXE driver), kernel Rootkit, configurator
    2. Development resources:
        1. Automated setup scripts to create a full development environment
        2. Tools and guides to analyze UEFI firmware on your own system
        3. Kernel debugging utilities and documentation for Windows
    3. Educational materials:
        1. A curated repository of over 400 links to PoCs, articles, videos and tools designed for research

8. Final Takeaways & Q&A
    1. Summary of key concepts and practical knowledge shared
    2. How this talk helps demystify low-level implants through engineering, reversing, and hands-on experimentation
    3. Time reserved for Q&A and discussion of custom extensions or use cases

---


## 49. Why DEF CON?

This talk belongs at DEF CON because it's not just about malware - it's about showing how a real Bootkit, like the ones used by advanced adversaries, is built step by step. We don't just explain what these implants do - we walk through their construction, how they interact with firmware and the OS, and how they can be reversed, and understood in depth. Every stage of the infection chain is shown in action, from firmware manipulation to full kernel control, with tools and source code shared openly.

This isn't a talk for the corporate crowd. It's for hackers who want to reverse firmware, tamper with bootloaders, inject at runtime, and explore undocumented system internals. It's content made for DEF CON - raw, real, and built to be explored.

---


## 50. Speaker Perspectives

Alejandro: As a Red Team malware developer, I spend my time building implants that bypass modern protections and help teams understand how real threats behave. I've spent years digging into Windows Internals, boot mechanisms, and kernel-mode persistence. This talk is a way to share what I've learned and release something real - something people can learn from, and use to build their own research.

Maria: I come from the law enforcement side, investigating advanced threats and analyzing malware targeting critical infrastructure. With a strong background in software engineering and national-level cybercrime investigations, I bring a forensic and defensive view to how these implants are understood, detected, and analyzed in the wild. Together, we present the full picture.

---


## 51. What else about you or this talk do you want us to know?

We believe in open knowledge. That's why we're releasing the complete toolset shown in the demo: Bootkit, Rootkit, configuration tools, scripts, and a full learning pack with hundreds of curated resources. This isn't a theoretical talk. It's malware engineering and reverse engineering presented openly, technically, and without holding anything back - in the true spirit of DEF CON. Attendees won't just leave with ideas. They'll leave with the actual code.

---


## 52. Specific Availability

No

---


## 53. Projector Feeds

No

---


## 54. Special Equipment

No

---


## 55. As detailed above, I have read and agree to the Grant of Copyright Use & Terms of Speaking Requirements.

Yes

---


## 56. As detailed above, I have read and agree to the Terms of Remuneration

Yes

---


## 57. As detailed above, I have read and agree to the Community & Code of Conduct Commitment.

Yes

---


## 58. Press Support Response

Yes

---
