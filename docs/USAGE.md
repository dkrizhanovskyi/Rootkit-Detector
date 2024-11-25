# **Usage Guide for Rootkit-Detector**

This document explains how to use the `Rootkit-Detector` project, including commands for kernel module operations and user-space tools.

---

## **Overview**

The `Rootkit-Detector` project consists of two main components:

1. **Kernel Modules**:
   - Perform low-level rootkit detection directly in the kernel.
   - Modules log results to the kernel ring buffer (`dmesg`).

2. **User-Space Tools**:
   - Simplify interaction with the kernel modules.
   - Aggregate and display results from kernel logs.

---

## **Kernel Modules**

### **Loading Kernel Modules**
To load a kernel module:
```bash
sudo insmod kernel/<module_name>.ko
```

For example:
```bash
sudo insmod kernel/process_scanner.ko
```

### **Unloading Kernel Modules**
To unload a kernel module:
```bash
sudo rmmod <module_name>
```

For example:
```bash
sudo rmmod process_scanner
```

### **Checking Module Logs**
After loading a module, check the kernel logs for its output:
```bash
dmesg | grep "Detected"
```

For example, after loading `process_scanner.ko`:
```bash
dmesg | grep "Detected process"
```

### **Kernel Module Functions**
Here’s a summary of available modules and their purposes:

| **Module**          | **Functionality**                                                                 |
|----------------------|-----------------------------------------------------------------------------------|
| `process_scanner`    | Scans for hidden processes.                                                      |
| `module_checker`     | Verifies the integrity of loaded kernel modules.                                 |
| `idt_ssdt_checker`   | Checks the Interrupt Descriptor Table (IDT) and System Service Descriptor Table (SSDT). |
| `file_scanner`       | Scans critical directories for hidden files.                                     |
| `syscall_checker`    | Monitors the syscall table for tampering.                                        |

---

## **User-Space Tools**

The user-space Python scripts interact with the kernel modules and aggregate results for easy access.

### **Unified CLI Tool**
The `detector.py` script acts as a unified interface for all checks.

#### **Commands**
```bash
python3 user/detector.py scan-processes
```

| **Command**              | **Description**                                          |
|--------------------------|----------------------------------------------------------|
| `scan-processes`         | Scans for hidden processes.                              |
| `check-modules`          | Checks the integrity of loaded kernel modules.           |
| `check-idt-ssdt`         | Verifies IDT and SSDT integrity.                         |
| `scan-files`             | Scans critical directories (e.g., `/etc`, `/var`) for hidden files. |

#### **Example**
```bash
python3 user/detector.py check-modules
```
Output:
```
Checking loaded kernel modules...
[INFO] No hidden modules detected.
```

---

### **Module-Specific Scripts**
Each module has a dedicated script for targeted checks. These scripts serve as an alternative to the unified CLI tool.

#### **Example: `process_scanner.py`**
```bash
python3 user/process_scanner.py
```
Output:
```
Scanning for processes...
Detected process: bash [1234]
Detected process: sshd [5678]
```

---

## **Advanced Usage**

### **Scanning Specific Directories**
Modify the directory list in `file_scanner.c` to add custom directories:
```c
scan_directory("/custom/directory");
```
Rebuild the kernel module and reload it:
```bash
cd kernel
make
sudo insmod file_scanner.ko
```

### **Real-Time Monitoring**
For real-time monitoring of logs:
```bash
dmesg --follow
```

---

## **Examples**

1. **Checking for Hidden Processes**:
   ```bash
   sudo insmod kernel/process_scanner.ko
   dmesg | grep "Detected process"
   sudo rmmod process_scanner
   ```

2. **Scanning for Hidden Files**:
   ```bash
   sudo insmod kernel/file_scanner.ko
   dmesg | grep "Found entry"
   sudo rmmod file_scanner
   ```

3. **Using Unified CLI**:
   ```bash
   python3 user/detector.py scan-files
   ```

---

## **Tips**

- **Automate Checks**:
  Create a script to load all modules, perform checks, and unload them:
  ```bash
  #!/bin/bash
  echo "Loading modules..."
  sudo insmod kernel/process_scanner.ko
  sudo insmod kernel/module_checker.ko

  echo "Checking results..."
  dmesg | grep "Detected"

  echo "Unloading modules..."
  sudo rmmod process_scanner
  sudo rmmod module_checker
  ```

- **Monitor Logs Continuously**:
  Use `dmesg --follow` to monitor logs in real-time while modules are loaded.

---

## **Troubleshooting**

1. **No Output in Logs**:
   - Ensure the module is loaded:
     ```bash
     lsmod | grep <module_name>
     ```

   - Check kernel logs for errors:
     ```bash
     dmesg | tail
     ```

2. **Module Load Failures**:
   - Verify kernel headers are installed and match the running kernel:
     ```bash
     uname -r
     sudo dnf install kernel-headers-$(uname -r)
     ```

3. **Permission Denied**:
   - Ensure you run the script or command with `sudo`:
     ```bash
     sudo python3 user/detector.py scan-processes
     ```

---

## **Support**

If you encounter issues, refer to the [INSTALL.md](./INSTALL.md) and [ARCHITECTURE.md](./ARCHITECTURE.md) documents. For unresolved issues, open an issue on the repository.
