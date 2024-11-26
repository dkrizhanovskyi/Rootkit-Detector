# **Contributing to Rootkit-Detector**

Thank you for considering contributing to the `Rootkit-Detector` project! We welcome contributions of all kinds, including bug fixes, feature implementations, documentation improvements, and more.

---

## **How Can I Contribute?**

### 1. **Report Issues**
If you encounter any bugs, inconsistencies, or unexpected behavior, let us know by opening an issue:

1. Go to the [Issues](https://github.com/dkrizhanovskyi/Rootkit-Detector/issues) tab.
2. Click **New Issue** and provide the following:
   - A clear and concise title.
   - A detailed description of the issue, including steps to reproduce it.
   - The environment (e.g., Linux distribution, kernel version).
   - Relevant logs, screenshots, or error messages.

### 2. **Suggest Features**
Have an idea to improve the project? Open a feature request:

1. Go to the [Issues](https://github.com/dkrizhanovskyi/Rootkit-Detector/issues) tab.
2. Select **Feature Request** and provide:
   - A detailed explanation of the feature.
   - Why it would be useful.
   - Any relevant examples or use cases.

### 3. **Fix Bugs or Add Features**
Contributions in the form of code are always welcome! Follow these steps:

1. **Fork the Repository**:
   - Click the **Fork** button on the top right of the repository page.
   - Clone your fork locally:
     ```bash
     git clone https://github.com/dkrizhanovskyi/Rootkit-Detector.git
     cd Rootkit-Detector
     ```

2. **Create a Feature Branch**:
   - Use a descriptive branch name:
     ```bash
     git checkout -b feature/my-feature
     ```

3. **Make Changes**:
   - Follow the [Code Style Guidelines](#code-style-guidelines).
   - Include relevant comments in your code.
   - Test your changes thoroughly.

4. **Commit Changes**:
   - Use descriptive commit messages:
     ```bash
     git add .
     git commit -m "Add feature to scan custom directories for hidden files"
     ```

5. **Push Changes**:
   - Push the changes to your forked repository:
     ```bash
     git push origin feature/my-feature
     ```

6. **Open a Pull Request**:
   - Go to the original repository and click **New Pull Request**.
   - Provide a clear title and description of your changes.

---

## **Code Style Guidelines**

1. **Kernel Modules (C)**:
   - Follow the Linux kernel coding style. Use `checkpatch.pl` to verify your changes:
     ```bash
     ./scripts/checkpatch.pl --file kernel/my_module.c
     ```
   - Add a GPL v3 header to all new `.c` files.
   - Ensure proper documentation for functions and structures.

2. **User-Space Scripts (Python)**:
   - Use Python 3. Ensure compatibility by testing with `python3`.
   - Follow the [PEP 8](https://peps.python.org/pep-0008/) coding standard:
     ```bash
     pip install flake8
     flake8 user/
     ```
   - Add docstrings for all functions and classes.

3. **Commit Messages**:
   - Use the following format for commit messages:
     ```
     Short summary of changes (50 characters or less)

     Detailed explanation of what was changed, why, and any relevant context.
     ```

---

## **Testing Your Changes**

1. **Kernel Modules**:
   - Build the modules to ensure they compile without errors:
     ```bash
     cd kernel
     make
     ```
   - Load and test each module:
     ```bash
     sudo insmod <module_name>.ko
     dmesg | tail
     ```

2. **User-Space Tools**:
   - Run the Python scripts and verify expected behavior:
     ```bash
     python3 user/detector.py scan-processes
     ```

3. **End-to-End Testing**:
   - Perform end-to-end testing to ensure that kernel modules and user-space tools interact correctly.

---

## **Pull Request Guidelines**

- Ensure your pull request:
  - Is focused on a single feature or bug fix.
  - Includes detailed explanations of changes.
  - Includes tests or examples where applicable.
- Address any feedback from maintainers promptly.

---

## **Community Guidelines**

- Be respectful in all interactions. See our [Code of Conduct](./CODE_OF_CONDUCT.md).
- Help others by reviewing open pull requests or offering advice in issues.

---

## **Support**

If you have questions about contributing, feel free to reach out by opening an issue or contacting the maintainers.

---

We’re excited to have you contribute to the `Rootkit-Detector` project and look forward to working with you!


