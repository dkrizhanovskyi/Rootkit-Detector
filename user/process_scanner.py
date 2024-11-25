import subprocess

def scan_processes():
    """
    Scans kernel logs for entries related to process detection.

    This function retrieves the output of `dmesg` and filters it for entries
    containing "Detected process," typically logged by a kernel module.
    """
    print("Scanning for processes...")

    try:
        # Run the dmesg command and capture the output
        result = subprocess.run(["dmesg"], capture_output=True, text=True, check=True)
        logs = result.stdout

        # Filter logs for lines mentioning "Detected process"
        process_logs = [line for line in logs.splitlines() if "Detected process" in line]

        if process_logs:
            print("Detected Processes:")
            for process in process_logs:
                print(f"- {process}")
        else:
            print("No processes detected by the kernel logs.")
    
    except subprocess.CalledProcessError as e:
        print(f"[ERROR] Failed to retrieve kernel logs: {e}")
    except Exception as e:
        print(f"[ERROR] An unexpected error occurred: {e}")

if __name__ == "__main__":
    scan_processes()
