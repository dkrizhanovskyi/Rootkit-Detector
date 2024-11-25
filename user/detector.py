import os

def load_module(module_name):
    """
    Load a kernel module using insmod.
    
    Args:
        module_name (str): The name of the kernel module (without .ko extension).
    
    Returns:
        None
    """
    print(f"Attempting to load module: {module_name}")
    result = os.system(f"sudo insmod {module_name}.ko")
    if result != 0:
        print(f"[ERROR] Failed to load module: {module_name}")
    else:
        print(f"[INFO] Module {module_name} loaded successfully.")

def unload_module(module_name):
    """
    Unload a kernel module using rmmod.
    
    Args:
        module_name (str): The name of the kernel module.
    
    Returns:
        None
    """
    print(f"Attempting to unload module: {module_name}")
    result = os.system(f"sudo rmmod {module_name}")
    if result != 0:
        print(f"[ERROR] Failed to unload module: {module_name}")
    else:
        print(f"[INFO] Module {module_name} unloaded successfully.")

def main():
    """
    Main function to manage the loading and unloading of kernel modules.
    
    Modules are loaded sequentially, and the user is prompted to proceed 
    with unloading them.
    """
    modules = ["syscall_checker", "process_scanner", "idt_ssdt_checker", "file_scanner", "module_checker"]

    print("[INFO] Loading modules...")
    for module in modules:
        load_module(module)

    print("[INFO] Modules loaded. Check 'dmesg' for details about their initialization.")
    input("Press Enter to unload modules...")

    print("[INFO] Unloading modules...")
    for module in modules:
        unload_module(module)

    print("[INFO] Modules unloaded. System cleanup complete.")

if __name__ == "__main__":
    main()
