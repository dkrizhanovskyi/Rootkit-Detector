import subprocess

def check_idt_ssdt():
    """
    Checks kernel logs for entries related to IDT and SSDT integrity.

    This function uses `dmesg` to search for log entries mentioning IDT and SSDT,
    typically indicating anomalies or integrity checks logged by kernel modules.
    """
    print("Checking IDT and SSDT integrity...")

    try:
        # Run the dmesg command and capture the output
        result = subprocess.run(["dmesg"], capture_output=True, text=True, check=True)
        logs = result.stdout

        # Filter for lines containing both "IDT" and "SSDT"
        relevant_logs = [line for line in logs.splitlines() if "IDT" in line and "SSDT" in line]

        if relevant_logs:
            print("Potential IDT or SSDT anomalies detected:")
            for log_entry in relevant_logs:
                print(f"- {log_entry}")
        else:
            print("No IDT or SSDT anomalies found in kernel logs.")

    except subprocess.CalledProcessError as e:
        print(f"[ERROR] Failed to retrieve kernel logs: {e}")
    except Exception as e:
        print(f"[ERROR] An unexpected error occurred: {e}")

if __name__ == "__main__":
    check_idt_ssdt()
