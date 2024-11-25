# **Installation Guide for Rootkit-Detector**

This document explains how to install and set up the `Rootkit-Detector` project on a Linux system.

---

## **Prerequisites**

Ensure the following are installed on your system:

1. **Linux Kernel Development Tools**:
   - Kernel headers and development tools must be installed for building kernel modules.
   - For **Fedora**:
     ```bash
     sudo dnf install kernel-devel kernel-headers
     ```

2. **Build Tools**:
   - GCC, Make, and related tools are required for compiling kernel modules.
   - For **Fedora**:
     ```bash
     sudo dnf install gcc make
     ```

3. **Python 3**:
   - Required for running the user-space CLI tools.
   - Ensure `pip` is installed for managing Python dependencies:
     ```bash
     sudo dnf install python3 python3-pip
     ```

4. **Superuser Privileges**:
   - You need `sudo` access to load/unload kernel modules.

---

## **Installation Steps**

### 1. Clone the Repository
Clone the `Rootkit-Detector` repository from GitHub:
```bash
git clone https://github.com/your-repo/Rootkit-Detector.git
cd Rootkit-Detector
```

### 2. Build Kernel Modules
Navigate to the `kernel` directory and compile the kernel modules:
```bash
cd kernel
make
```

This will generate `.ko` files for each kernel module (e.g., `process_scanner.ko`, `module_checker.ko`).

### 3. Install User-Space Tools
Navigate to the `user` directory and install any Python dependencies:
```bash
cd ../user
pip3 install -r requirements.txt
```

If `requirements.txt` is empty, skip this step.

---

## **Usage**

### Loading Kernel Modules
To load a kernel module, use the `insmod` command:
```bash
sudo insmod kernel/process_scanner.ko
```

### Verifying Module Logs
Check the kernel logs for output:
```bash
dmesg | grep "Detected process"
```

### Unloading Kernel Modules
Unload a kernel module using the `rmmod` command:
```bash
sudo rmmod process_scanner
```

### Using User-Space CLI
Run the Python-based CLI tool to trigger and aggregate checks:
```bash
python3 user/detector.py scan-processes
python3 user/detector.py check-modules
python3 user/detector.py check-idt-ssdt
python3 user/detector.py scan-files
```

---

## **Troubleshooting**

1. **Kernel Header Issues**:
   - If you encounter errors while building kernel modules, ensure that your kernel headers are installed and match your running kernel:
     ```bash
     uname -r  # Check kernel version
     sudo dnf install kernel-headers-$(uname -r) kernel-devel-$(uname -r)
     ```

2. **Permission Errors**:
   - Running `dmesg` or loading/unloading kernel modules requires `sudo` privileges. Use `sudo` to avoid permission-related issues.

3. **Python Dependency Errors**:
   - If a required Python package is missing, install it using `pip`:
     ```bash
     pip3 install <package_name>
     ```

4. **Module Load Failures**:
   - Check kernel logs for errors:
     ```bash
     dmesg | tail
     ```

---

## **Uninstallation**

To remove the project:

1. Unload all kernel modules:
   ```bash
   sudo rmmod process_scanner
   sudo rmmod module_checker
   # Repeat for all loaded modules
   ```

2. Clean up compiled files:
   ```bash
   cd kernel
   make clean
   ```

3. Delete the project directory:
   ```bash
   cd ..
   rm -rf Rootkit-Detector
   ```

---

## **Support**

For further assistance, refer to the [USAGE.md](./USAGE.md) document or open an issue in the repository.
