# **Rootkit-Detector Architecture**

This document explains the overall architecture of the `Rootkit-Detector` project, including the interaction between its kernel-space and user-space components.

---

## **Overview**

The `Rootkit-Detector` project is designed to detect rootkit-like behavior in a Linux system. It comprises two main components:

1. **Kernel Modules**:
   - Perform low-level system checks within the kernel.
   - Detect anomalies such as hidden processes, tampered system call tables, and hidden kernel modules.
   - Provide logs via the kernel ring buffer (`dmesg`).

2. **User-Space Tools**:
   - Simplify interaction with kernel modules.
   - Aggregate results from kernel logs for user-friendly output.

---

## **Component Breakdown**

### 1. **Kernel Modules**
The kernel modules are implemented in C and perform targeted security checks. Each module focuses on a specific aspect of rootkit detection:

#### **a. `syscall_checker.c`**
- **Purpose**:
  - Monitors the syscall table (`sys_call_table`) for unauthorized modifications.
- **Key Functions**:
  - Locates the syscall table in memory.
  - Compares expected addresses (e.g., `ksys_open`) with actual syscall handlers.

#### **b. `process_scanner.c`**
- **Purpose**:
  - Scans the task list to detect hidden processes.
- **Key Functions**:
  - Iterates through the kernel's `task_struct` list.
  - Logs all detected processes and flags inconsistencies.

#### **c. `module_checker.c`**
- **Purpose**:
  - Verifies the integrity of loaded kernel modules.
- **Key Functions**:
  - Reads `/proc/modules` to list all loaded modules.
  - Detects anomalies or modules marked as hidden.

#### **d. `idt_ssdt_checker.c`**
- **Purpose**:
  - Checks the integrity of the Interrupt Descriptor Table (IDT) and System Service Descriptor Table (SSDT).
- **Key Functions**:
  - Extracts and analyzes IDT and SSDT entries.
  - Detects tampering or unexpected handlers.

#### **e. `file_scanner.c`**
- **Purpose**:
  - Scans critical directories for hidden or suspicious files.
- **Key Functions**:
  - Iterates through directories using the VFS layer.
  - Logs entries found in directories such as `/etc` and `/var`.

---

### 2. **User-Space Tools**

The user-space tools are implemented in Python for easy interaction with the kernel modules and for processing their output. These tools simplify the process of loading/unloading modules and interpreting results.

#### **a. `detector.py`**
- **Purpose**:
  - A unified CLI tool for managing and interacting with the rootkit detector.
- **Key Features**:
  - Load/unload kernel modules.
  - Trigger specific checks (e.g., process scan, file scan).
  - Aggregate results from kernel logs (`dmesg`).

#### **b. Module-Specific Scripts**
- Each Python script corresponds to a kernel module and provides an interface to trigger its functionality and retrieve results:
  - `process_scanner.py`
  - `module_checker.py`
  - `idt_ssdt_checker.py`
  - `file_scanner.py`

---

## **Data Flow**

### **1. User Input**
- Users interact with the system via the user-space CLI tool (`detector.py`) or individual scripts.
- Example:
  ```bash
  python3 detector.py scan-processes
  ```

### **2. Kernel Module Execution**
- The CLI tool loads the relevant kernel module (e.g., `process_scanner.ko`).
- The kernel module performs its checks (e.g., scanning the task list).
- Results are logged to the kernel ring buffer (`dmesg`).

### **3. Output Retrieval**
- The CLI tool retrieves results from `dmesg`.
- Example Output:
  ```
  Detected process: bash [1234]
  Detected process: sshd [5678]
  ```

---

## **Key Interactions**

1. **Kernel Modules**:
   - Kernel modules interact directly with low-level system structures (e.g., `task_struct`, `sys_call_table`, IDT).
   - Results are logged via `printk`.

2. **User-Space Tools**:
   - User-space tools interact with kernel modules through:
     - Module loading/unloading commands (`insmod`, `rmmod`).
     - System interfaces like `/proc`, `/sys`, and `dmesg`.

3. **Logs**:
   - Kernel modules use `printk` to log results and anomalies.
   - These logs are retrieved via `dmesg`.

---

## **Diagrams**

### **High-Level Architecture**
```
+--------------------+         +--------------------+
| User-Space Tools   |         | Kernel Modules     |
|--------------------|         |--------------------|
| - detector.py      |<------->| - process_scanner  |
| - file_scanner.py  |<------->| - file_scanner     |
| - module_checker.py|<------->| - module_checker   |
+--------------------+         +--------------------+
           ^
           |
           v
      +---------+
      | dmesg   |
      +---------+
```

### **Kernel Module Workflow**
```
1. Load Module (insmod)
   |
   v
2. Perform Checks (e.g., scan processes)
   |
   v
3. Log Results via printk
   |
   v
4. Retrieve Results (dmesg)
```

---

## **Future Enhancements**

1. **Enhanced Reporting**:
   - Save detection results to a log file in user-space.
   - Add summary reports for better readability.

2. **Modular CLI**:
   - Combine all Python scripts into a single CLI with subcommands.

3. **Real-Time Detection**:
   - Introduce hooks to detect anomalies in real-time (e.g., syscall tampering).

4. **Cross-Platform Support**:
   - Add compatibility for multiple Linux distributions.

