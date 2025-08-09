# DEF CON Hackers' Almanack Accepted Speakers Form

Congratulations on being accepted as a speaker at DEF CON 33! Please use this form to submit information on your talk(s), if you would like them to be included in the DEF CON 33 Hackers' Almanack.

The DEF CON Hackers' Almanack is a first-of-its-kind technology policy report, created by DEF CON and the University of Chicago - Harris Cyber Policy Initiative. Each year, editors comb through hundreds of talks and findings to present the most interesting and impactful DEF CON findings, with an emphasis on policy implications.

We invite you to submit any talks that you would like to see included in this year Almanack.

For your reference, last year's report can be accessed at the link below. https://media.defcon.org/DEF%20CON%2032/DEF%20CON%2032%20Hackers'%20Almanack.pdf

---

1.Do you want your talk to be included in the DEF CON 33 Hackers' Almanack report?

Yes

---

2.What is the title of your talk?

Infecting the Boot to Own the Kernel: Bootkits and Rootkits Development

---

3.Please give a brief, one-paragraph description of this talk, including the "bottom line" findings and any outstanding technical discoveries.

This is the first talk that fully demystifies the creation of a modern bootkit and rootkit from scratch. Instead of showing a black-box implant, we walk through every stage of development, from firmware code execution and boot-time networking, to runtime service hooking, multi-path in-memory rootkit injection, and stealthy in-kernel C2. The talk explains how and why each technique works, showing how attackers can persist across reboots and operate fully within the kernel without userland presence. To bridge theory and practice, we release the complete malware framework alongside hundreds of curated learning resources, giving red teams, researchers, and educators everything they need to replicate, analyze, or extend this work. This isn't a postmortem or a proof-of-concept, it's the blueprint you were never supposed to have.

---

4.Why should this talk be included in the DEF CON 33 Hackers' Almanack? Why do you think this matters from a public policy perspective?

For years, firmware-level threats have existed in a realm of secrecy, referenced in threat intelligence reports, feared in policy circles, but rarely understood in practice. This talk breaks that barrier. It unveils a fully working, reproducible bootkit framework that operates at the UEFI level, communicates directly with the Windows kernel, and persists entirely outside of userland, without relying on any commercial tools or classified techniques.

What makes this talk unique is not just the technical sophistication, it's the fact that these capabilities are now accessible. They're no longer reserved for state-sponsored actors or nation-level budgets. Red teamers, academic researchers, and even independent security professionals can now study, replicate, and iterate on real-world bootkits from scratch. That shift changes the landscape entirely.

From a public policy perspective, this raises urgent questions: Are firmware security baselines sufficient? Are incident responders equipped to detect and counter implants that never touch disk or user space? Should device security standards include hardware-layer integrity checks as mandatory?

By lowering the barrier to entry and showing how firmware implants can be built, deployed, and stealthily maintained, this talk forces a rethink of what "advanced threat" truly means in 2025. It calls for a new class of defensive readiness, one that includes the boot process, firmware interfaces, and runtime UEFI behavior as first-class security domains.

---

5.How many new vulnerabilities are disclosed (and patched, if applicable)?

No new CVEs are disclosed, but the talk introduces original offensive techniques that push the boundaries of firmware-based persistence and stealth. This includes establishing outbound connections from the pre-boot environment to download files or interact with a command-and-control server, a concept rarely discussed in public research, especially at the firmware level. It also showcases seamless communication with a C2 from within the Windows kernel, enabling full remote control without ever touching userland. These capabilities illustrate how modern bootkits can maintain control over a system across reboots, evade detection entirely, and operate from layers of the platform that most security models ignore.
