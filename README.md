# Project RVBBIT

### Linux Kernel Rootkit Research PoC

RVBBIT is an educational Linux kernel security project built to explore how rootkits manipulate the difference between actual kernel state and what userspace tools are able to observe.

The project implements several established rootkit techniques in a controlled proof-of-concept environment: process and module hiding, syscall interception, filesystem and network concealment, DKOM, persistence experiments, and interference with eBPF-based observability.

The techniques themselves are not presented as new or undetectable.

The purpose of RVBBIT is to understand how they work, what assumptions they rely on, and — increasingly — what they leave behind.

> **Research only:** RVBBIT is intended for isolated laboratory environments, cybersecurity education, and defensive research. The public version is intentionally neutered and does not contain functional mining, propagation, or privilege-escalation capabilities.

---

## Why I built RVBBIT

Reading about kernel rootkits and actually trying to implement their behavior are two very different exercises.

Tools such as `ps`, `ls`, `lsmod`, and interfaces under `/proc` are easy to treat as sources of truth. In reality, they are views of system state produced through particular kernel structures and interfaces.

Change the source of that information, and the view can change while the underlying object continues to exist.

That was the idea that originally interested me.

RVBBIT started as an experiment around a simple question:

> **What happens when the actual state of a Linux system and the state reported to userspace no longer agree?**

That question eventually led to a second project, **RVBBIT Arsenal**, where the same problem is approached from both offensive and defensive perspectives.

---

## Research areas

RVBBIT brings several Linux kernel security concepts into a single experimental project.

| Area                      | Research focus                                                               |
| ------------------------- | ---------------------------------------------------------------------------- |
| **DKOM**                  | Manipulation of kernel objects and their relationships                       |
| **Module hiding**         | Removing a loaded module from normal enumeration paths                       |
| **Process hiding**        | Exploring the difference between process existence and process visibility    |
| **Syscall interception**  | Studying how syscall redirection can alter information returned to userspace |
| **Filesystem visibility** | Filtering directory entries before they reach normal userspace tools         |
| **Network visibility**    | Manipulating information exposed through `/proc/net/tcp`                     |
| **eBPF interaction**      | Experiments around interference with BPF-based observability                 |
| **Persistence**           | Studying persistence behavior and the artifacts it creates                   |

These mechanisms are implemented for research purposes.

RVBBIT should not be interpreted as a claim of reliable stealth against current endpoint security products or modern hardened Linux environments.

---

## Implemented experiments

| Experiment                | Description                                                            | Public repository       |
| ------------------------- | ---------------------------------------------------------------------- | ----------------------- |
| **Module hiding**         | Removes the module from normal module enumeration                      | Enabled                 |
| **Process hiding**        | Demonstrates hiding a simulated process from common enumeration paths  | Enabled                 |
| **File hiding**           | Filters files matching a generated prefix from directory listings      | Enabled                 |
| **TCP connection hiding** | Demonstrates filtering selected TCP information from `/proc/net/tcp`   | Enabled                 |
| **eBPF interference**     | Demonstrates interference with selected BPF program-loading operations | Enabled                 |
| **Persistence**           | Demonstrates persistence through system configuration mechanisms       | Enabled                 |
| **Cryptocurrency mining** | Functional mining payload                                              | **Removed / simulated** |
| **Network propagation**   | Functional self-propagation mechanism                                  | **Removed**             |
| **Privilege escalation**  | Functional privilege-escalation exploit                                | **Not included**        |

---

## A note on stealth

The word *stealth* can easily create the wrong impression.

In RVBBIT it means hiding information from particular observation or enumeration paths. It does **not** mean that an object becomes universally invisible.

A process hidden from one interface may remain observable through another source.

A module removed from normal enumeration can still leave evidence in memory.

A TCP connection filtered from `/proc/net/tcp` still exists elsewhere in the networking stack.

This became one of the most useful lessons from the project:

> **Hiding something from one observer is not the same as removing every observable trace of it.**

That distinction eventually became the bridge between RVBBIT's offensive experiments and the defensive work that followed.

---

## Safety by design

The public repository is intentionally restricted.

It contains **no functional cryptocurrency miner, no operational network-propagation mechanism, and no privilege-escalation exploit**.

Earlier ideas involving potentially harmful behavior were either removed or represented through non-operational simulations.

This was a deliberate decision.

A functional botnet component is not necessary to study process visibility.

Compromising another machine is not necessary to study persistence.

And an educational kernel project does not need to become deployable malware in order to demonstrate how kernel-level concealment works.

The interesting part of RVBBIT is the mechanism and the resulting visibility gap.

---

## From RVBBIT to RVBBIT Arsenal

The original project was mostly built around an offensive question:

> **How can kernel-level manipulation change what userspace sees?**

While working on those experiments, another question became more interesting:

> **If I know how something is being hidden, what evidence can still expose it?**

That question became the basis for the next stage of the project:

### [RVBBIT Arsenal](https://github.com/buter-chkalova/rvbbit-arsenal)

RVBBIT Arsenal expands the original experiment into an offensive-and-defensive Linux kernel security research environment.

The direction is:

```text
Offensive technique
        |
        v
Kernel state modification
        |
        v
Visibility gap
        |
        v
Observable artifact
        |
        v
Telemetry
        |
        v
Detection
```

The long-term idea is to connect meaningful offensive experiments with corresponding defensive investigation:

**Technique → Artifact → Telemetry → Detection → Limitation**

Rather than simply asking how something can be hidden, the project increasingly asks what remains observable after the hiding mechanism has been applied.

---

## Independent research coverage and external visibility

One of the unexpected outcomes of publishing RVBBIT was seeing other cybersecurity researchers examine the project independently.

### Hackplayers — Spain

In April 2026, Spanish cybersecurity publication **Hackplayers** published a technical analysis:

**[RVBBIT: anatomía de un rootkit LKM moderno basado en stealth, DKOM y anti-eBPF](https://www.hackplayers.com/2026/04/rvbbit-anatomia-de-un-rootkit-lkm.html)**

The article examined several parts of RVBBIT, including its overall architecture, syscall hooking, DKOM, module and process hiding, filesystem and network visibility, persistence, and interaction with eBPF-based observability.

### CSDN — China

RVBBIT was later independently analyzed by a Chinese author on **CSDN**:

**[内核里的“幽灵”：一套Linux Rootkit隐身术完全拆解](https://blog.csdn.net/chen1415886044/article/details/161462914)**

The article provides a separate technical walkthrough of the project and discusses its kernel-level concealment mechanisms, persistence behavior, observability implications, and possible defensive considerations.

Seeing the project independently analyzed by Hackplayers and CSDN outside its original language and community was one of the reasons I decided to continue developing RVBBIT as a broader security research project rather than simply adding more rootkit functionality.

### LinkedIn — Community sharing

Project RVBBIT was also independently shared on LinkedIn by Yavor Tsakov through a direct link to the repository:

[Project RVBBIT — LinkedIn post by Yavor Tsakov](https://www.linkedin.com/posts/yavor-tsakov-807b16243_github-buter-chkalovaproject-rvbbit-linux-activity-7449048505547096064-Osin)

This is not a technical analysis of the project, but it is an example of the research being circulated independently outside the project's own publication channels.

### RVBBIT Arsenal — Security indexing

The broader RVBBIT research also began appearing in specialized cybersecurity indexing services through RVBBIT Arsenal.

[Vulners — RVBBIT Arsenal](https://vulners.com/kitploit/KITPLOIT%3ATOOLS-GITHUB-BUTER-CHKALOVA-RVBBIT-ARSENAL)

[Sploitus — RVBBIT Arsenal](https://sploitus.com/exploit?id=KITPLOIT%3ATOOLS-GITHUB-BUTER-CHKALOVA-RVBBIT-ARSENAL)

The entries are related to a KitPloit-originated record and should be understood as external security indexing rather than independent technical reviews.

Some third-party indexes may preserve older descriptions or snapshots of RVBBIT Arsenal. Those descriptions may refer to functionality that is no longer present in the current public repository. The current repository and its documentation should be treated as the authoritative description of the project's present capabilities.

---

## Building

### Requirements

Use a disposable Linux virtual machine or another isolated research environment.

Typical build requirements include:

```bash
sudo apt install build-essential linux-headers-$(uname -r)
```

Clone the repository:

```bash
git clone https://github.com/buter-chkalova/project-rvbbit.git
cd project-rvbbit
```

Compile:

```bash
make
```

> Kernel internals change between versions. RVBBIT is research code and should not be expected to compile or behave identically across arbitrary Linux distributions and kernel releases.

---

## Limitations

RVBBIT is a personal learning and security research project.

It does not claim to:

* introduce previously unknown Linux rootkit techniques;
* provide reliable stealth against modern EDR products;
* bypass modern hypervisor-based integrity mechanisms;
* work unchanged across arbitrary Linux kernel versions;
* represent production-quality malware;
* provide comprehensive coverage of modern Linux rootkit development.

Several mechanisms intentionally reproduce established techniques because understanding their assumptions and weaknesses is part of the research.

A technique being detectable is not considered a failure of the project.

In many cases, understanding **why** it is detectable is the more interesting result.

---

## Research direction

RVBBIT began with concealment.

The next stage is increasingly about visibility.

Areas I am interested in exploring further include:

* cross-view detection;
* kernel integrity validation;
* observable artifacts of kernel manipulation;
* eBPF-based telemetry;
* detection engineering;
* differences between kernel and userspace views of system state;
* reproducible attack-versus-detection experiments.

The objective is not to build the largest collection of rootkit techniques.

It is to better understand the relationship between **kernel manipulation, visibility, and detection**.

---

## Related project

### RVBBIT Arsenal

**Offensive & Defensive Linux Kernel Security Research**

[github.com/buter-chkalova/rvbbit-arsenal](https://github.com/buter-chkalova/rvbbit-arsenal)

RVBBIT Arsenal continues the original research by placing offensive experiments and defensive investigation in the same environment.

The project explores a simple relationship:

```text
ATTACK
  |
  v
What changed?
  |
  v
What disappeared?
  |
  v
What remains observable?
  |
  v
DEFENSE
```

---

## Responsible use

RVBBIT is provided for cybersecurity education, controlled experimentation, and defensive security research.

Only use the project:

* on systems you own;
* in isolated laboratory environments; or
* where you have explicit authorization to perform security testing.

Do not deploy or test the project against third-party systems without permission.

---

## Feedback

Technical criticism is welcome.

I am particularly interested in feedback related to:

* Linux kernel internals;
* incorrect assumptions in the implementation;
* kernel-version compatibility;
* detection opportunities;
* cross-view analysis;
* eBPF observability;
* defensive telemetry.

Finding where an assumption breaks is often more useful than making a proof of concept appear more capable than it actually is.

---

## Author

**buter-chkalova**

Security research focused on Linux kernel internals, offensive security, rootkit behavior, and defensive detection.

### Projects

* **Project RVBBIT** — Linux Kernel Rootkit Research PoC
* **[RVBBIT Arsenal](https://github.com/buter-chkalova/rvbbit-arsenal)** — Offensive & Defensive Linux Kernel Security Research

---

## License

This project is released under the **MIT License**.

See [LICENSE](LICENSE) for details.
