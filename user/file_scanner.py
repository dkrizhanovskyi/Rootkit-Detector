import os
import subprocess

def scan_files():
    """
    Scans kernel logs for entries related to found files or hidden files.

    This function runs `dmesg` and filters the output for entries containing
    "Found file" or any relevant logs related to file scanning.
    """
    print("Scanning for hidden files...")
    
    try:
        # Execute the dmesg command and capture its output
        result = subprocess.run(["dmesg"], capture_output=True, text=True, check=True)
        logs = result.stdout
        
        # Filter for lines mentioning "Found file"
        hidden_files = [line for line in logs.splitlines() if "Found entry" in line]

        if hidden_files:
            print("Hidden files or scanned entries detected:")
            for file_entry in hidden_files:
                print(f"- {file_entry}")
        else:
            print("No hidden files or suspicious entries found.")
    
    except subprocess.CalledProcessError as e:
        print(f"[ERROR] Failed to retrieve kernel logs: {e}")
    except Exception as e:
        print(f"[ERROR] An unexpected error occurred: {e}")

if __name__ == "__main__":
    scan_files()
