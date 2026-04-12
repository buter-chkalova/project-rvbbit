# Project RVBBIT: Linux Kernel Rootkit Proof-of-Concept

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Platform: Linux](https://img.shields.io/badge/Platform-Linux%20x86__64-blue)
![Status: Educational](https://img.shields.io/badge/Status-Educational%20PoC-red)

## 📖 Overview

**Project RVBBIT** is an **educational proof-of-concept** demonstrating advanced stealth techniques used by Linux kernel rootkits. It is intended **exclusively for cybersecurity research, defensive training, and understanding how modern rootkits operate**.

This project illustrates:
- Direct Kernel Object Manipulation (DKOM) to hide processes, files, and network connections.
- System call hooking (`sys_kill`, `sys_getdents64`, `sys_openat`) by directly modifying the syscall table (bypassing write protection via `write_cr0`).
- Bypassing eBPF‑based detection tools.
- Masking as a legitimate ACPI kernel driver.
- Persistence mechanisms via `systemd` and `modules-load.d`.

> ⚠️ **WARNING**: This software is intentionally **NEUTERED**. It contains **NO** cryptocurrency mining payload, **NO** network propagation (worm), and **NO** privilege escalation exploits. All potentially harmful capabilities have been replaced with harmless simulations. The code remains functional for demonstrating stealth, but it is **incapable of causing real damage**.

---

## 🚨 Legal & Ethical Disclaimer

This project is provided **"AS IS"** for **educational and defensive purposes only**.  
The author **does not endorse** malicious use.  
You are solely responsible for complying with all applicable laws and regulations in your jurisdiction.  
**Do not deploy this on any system you do not own or have explicit written permission to test.**

By using this software, you agree that the author cannot be held liable for any damages or legal issues arising from its use.

---

## ✨ Features (Demonstration Only)

| Feature | Description | Status in this Repo |
| :--- | :--- | :---: |
| **Module Hiding** | Removes itself from `/proc/modules` and `lsmod` via DKOM. | ✅ Enabled |
| **Process Hiding** | Hides a simulated "miner" process from `ps` and `/proc`. | ✅ Enabled |
| **File Hiding** | Hides files with a specific prefix from directory listings. | ✅ Enabled |
| **TCP Port Hiding** | Hides connections to port `3333` from `/proc/net/tcp`. | ✅ Enabled |
| **eBPF Bypass** | Blocks loading of unsigned eBPF programs (anti‑detection). | ✅ Enabled |
| **Persistence** | Installs a systemd service and a modules‑load.d entry. | ✅ Enabled |
| **Real Cryptocurrency Mining** | Monero (XMR) mining payload. | ❌ REMOVED (simulated) |
| **Network Propagation** | SSH brute‑force and self‑spreading worm. | ❌ REMOVED |

---

## 🛠️ Build Instructions

### Prerequisites
- Linux kernel headers (e.g., `linux-headers-$(uname -r)`)
- `build-essential`, `make`

### Compilation
```bash
git clone https://github.com/buter-chkalova/project-rvbbit.git
cd project-rvbbit
make