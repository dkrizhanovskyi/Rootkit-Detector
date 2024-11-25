# **Security Policy**

## **Supported Versions**

The following table lists the versions of `Rootkit-Detector` that are actively supported with security updates:

| Version       | Supported          |
|---------------|--------------------|
| Latest (main) | :white_check_mark: |
| Previous      | :white_check_mark: |
| Older         | :x:                |

If you are using an unsupported version, we recommend upgrading to the latest version.

---

## **Reporting a Vulnerability**

We take security vulnerabilities seriously. If you discover a security issue in `Rootkit-Detector`, please report it privately to the maintainers. Do **not** disclose it publicly until we have addressed the issue.

### How to Report
1. **Contact Email**: [INSERT EMAIL HERE]
2. **Subject**: "Security Issue: [Brief Description]"
3. **Details to Include**:
   - Description of the vulnerability.
   - Steps to reproduce the issue.
   - Any relevant logs or screenshots.
   - Affected version or commit hash.

### What to Expect
- **Acknowledgment**: We will respond to your report within 3 business days.
- **Resolution**: We aim to resolve critical security issues within 14 days, depending on the severity and complexity.

---

## **Best Practices for Secure Usage**

To ensure the secure use of `Rootkit-Detector`, follow these guidelines:

1. **Run in a Trusted Environment**:
   - Ensure the system is properly secured before deploying `Rootkit-Detector`.
   - Use strong access controls for privileged commands.

2. **Regular Updates**:
   - Always use the latest version of `Rootkit-Detector` to benefit from security patches.

3. **Kernel Module Security**:
   - Verify that all kernel modules (`.ko` files) are compiled from trusted source code.
   - Avoid loading third-party or unsigned kernel modules.

4. **Secure Communication**:
   - If using remote logging or interactions, ensure communication is encrypted (e.g., SSH, TLS).

5. **Monitor Logs**:
   - Regularly monitor system logs (`dmesg`) for unusual activity flagged by the tool.

---

## **Acknowledgments**

We appreciate the efforts of the community to help us improve the security of `Rootkit-Detector`. Special thanks to contributors who responsibly disclose vulnerabilities.

---

## **Resources**

- [GNU General Public License (GPL) v3](https://www.gnu.org/licenses/gpl-3.0.en.html)
- [Linux Kernel Security Documentation](https://www.kernel.org/doc/html/latest/security/index.html)
