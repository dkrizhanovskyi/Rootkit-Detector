# **Rootkit-Detector**

A kernel-based security tool to detect hidden processes, kernel modules, and anomalies in the system's IDT/SSDT. The project includes kernel modules and user-space tools to facilitate rootkit detection and analysis.

---

## **Features**

- **Kernel Module Checks**:
  - Detect hidden kernel modules.
  - Monitor syscall table for tampering.
  - Validate IDT and SSDT integrity.

- **File System Scanning**:
  - Identify hidden files in critical directories.

- **Process Scanning**:
  - Detect hidden or malicious processes bypassing the `/proc` filesystem.

- **Modular Design**:
  - Kernel modules (`.ko` files) for various detection mechanisms.
  - User-space Python scripts for seamless interaction and monitoring.

---

## **Installation**

### Prerequisites
1. A Linux system with kernel headers installed.
   - **Fedora**:  
     ```bash
     sudo dnf install kernel-devel kernel-headers
     ```
2. GCC and make tools:
   - **Fedora**:  
     ```bash
     sudo dnf install gcc make
     ```
3. Python 3 and pip:
   - **Fedora**:  
     ```bash
     sudo dnf install python3 python3-pip
     ```

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo/Rootkit-Detector.git
   cd Rootkit-Detector
   ```

2. Build the kernel modules:
   ```bash
   cd kernel
   make
   ```

3. Install the user-space dependencies (if any):
   ```bash
   cd ../user
   pip3 install -r requirements.txt
   ```

4. Load the kernel modules (requires `sudo`):
   ```bash
   sudo insmod kernel/module_checker.ko
   sudo insmod kernel/process_scanner.ko
   # Add other modules as needed
   ```

---

## **Usage**

### Kernel Modules
1. Load the desired kernel module:
   ```bash
   sudo insmod kernel/module_checker.ko
   ```

2. Check the kernel logs for output:
   ```bash
   dmesg | grep "Detected"
   ```

3. Unload the kernel module after use:
   ```bash
   sudo rmmod module_checker
   ```

### User-Space Tools
Run the CLI tool for various checks:
```bash
python3 user/detector.py scan-processes    # Scan for hidden processes
python3 user/detector.py check-modules    # Check loaded kernel modules
python3 user/detector.py scan-files       # Scan for hidden files
python3 user/detector.py check-idt-ssdt   # Validate IDT and SSDT integrity
```

---

## **Architecture**

The project is divided into two main components:

1. **Kernel Modules**:
   - Responsible for performing low-level checks in kernel space.
   - Modules include:
     - `syscall_checker`: Monitors syscalls for tampering.
     - `process_scanner`: Scans for hidden processes.
     - `module_checker`: Detects anomalies in kernel modules.
     - `idt_ssdt_checker`: Checks IDT and SSDT for anomalies.
     - `file_scanner`: Scans directories for hidden files.

2. **User-Space Tools**:
   - Python scripts to load/unload kernel modules and interpret their output.
   - Interact with kernel logs (`dmesg`) to extract relevant results.

---

## **Contributing**

We welcome contributions to improve the project! Here’s how you can help:
1. Fork the repository.
2. Create a feature branch:
   ```bash
   git checkout -b feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -m "Description of your feature"
   ```
4. Push to your fork:
   ```bash
   git push origin feature-name
   ```
5. Open a pull request on the main repository.

---

## **License**

This project is licensed under the **GPL v3**. See the [LICENSE](../LICENSE) file for details.

---

## **Acknowledgements**

Special thanks to:
- Linux Kernel Documentation.
- The open-source community for contributing to kernel module development.

