import subprocess

def check_modules():
    """
    Checks and lists currently loaded kernel modules by reading /proc/modules.

    This function retrieves the contents of /proc/modules and displays them
    in a user-friendly format.
    """
    print("Checking loaded kernel modules...")
    
    try:
        # Read the /proc/modules file
        result = subprocess.run(["cat", "/proc/modules"], capture_output=True, text=True, check=True)
        modules = result.stdout.strip()

        if modules:
            print("Loaded Kernel Modules:")
            print(modules)
        else:
            print("[INFO] No kernel modules are currently loaded (unlikely scenario).")
    
    except subprocess.CalledProcessError as e:
        print(f"[ERROR] Failed to read /proc/modules: {e}")
    except Exception as e:
        print(f"[ERROR] An unexpected error occurred: {e}")

if __name__ == "__main__":
    check_modules()
