/*
 * Kernel Module Checker
 * Author: Daniil Krizhanovskyi
 * License: GPL v3
 *
 * This kernel module scans the /proc/modules file for potentially hidden
 * kernel modules. It identifies any module with a specific name that is 
 * suspected to be hidden.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/fs.h>

/*
 * Detect hidden kernel modules by scanning the /proc/modules file.
 *
 * This function reads the /proc/modules file into a buffer and searches for
 * a specific module name ("hidden_module_name") to identify potentially
 * hidden kernel modules.
 *
 * Returns:
 *   The number of hidden modules detected.
 *   -1 if an error occurs during file access or memory allocation.
 */
static int detect_hidden_modules(void) {
    struct file *proc_modules;
    char *buffer;
    size_t buffer_size = 4096;  // Size of the buffer for reading file data
    int hidden_count = 0;
    ssize_t bytes_read;
    loff_t pos = 0;

    // Open the /proc/modules file
    proc_modules = filp_open("/proc/modules", O_RDONLY, 0);
    if (IS_ERR(proc_modules)) {
        printk(KERN_ERR "Failed to open /proc/modules\n");
        return -1;
    }

    // Allocate memory for the buffer
    buffer = kmalloc(buffer_size, GFP_KERNEL);
    if (!buffer) {
        printk(KERN_ERR "Failed to allocate memory for buffer\n");
        filp_close(proc_modules, NULL);
        return -1;
    }

    // Read data from /proc/modules into the buffer
    bytes_read = kernel_read(proc_modules, buffer, buffer_size - 1, &pos);  // Leave space for null-termination
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the buffer for string operations

        // Check if the buffer contains the name of the hidden module
        if (strnstr(buffer, "hidden_module_name", bytes_read)) {
            hidden_count++;
            printk(KERN_WARNING "Hidden module detected: hidden_module_name\n");
        }
    } else {
        printk(KERN_ERR "Failed to read /proc/modules\n");
    }

    // Free allocated memory and close the file
    kfree(buffer);
    filp_close(proc_modules, NULL);

    return hidden_count;
}

/*
 * Check the system for hidden kernel modules and log the results.
 *
 * This function calls `detect_hidden_modules` and logs the number of
 * hidden modules detected.
 */
static void check_modules(void) {
    int hidden = detect_hidden_modules();
    if (hidden > 0)
        printk(KERN_WARNING "Detected %d hidden modules\n", hidden);
    else
        printk(KERN_INFO "No hidden modules detected\n");
}

/*
 * Module initialization function.
 *
 * Called when the module is loaded into the kernel. Logs a message and
 * performs a module check to detect hidden kernel modules.
 *
 * Returns:
 *   0 on successful initialization.
 */
static int __init module_checker_init(void) {
    printk(KERN_INFO "Initializing module checker\n");
    check_modules();
    return 0;
}

/*
 * Module cleanup function.
 *
 * Called when the module is removed from the kernel. Logs a message indicating
 * the module has been unloaded.
 */
static void __exit module_checker_exit(void) {
    printk(KERN_INFO "Module checker unloaded\n");
}

/* Register the module's initialization and cleanup functions */
module_init(module_checker_init);
module_exit(module_checker_exit);

/* Module metadata */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniil Krizhanovskyi");
MODULE_DESCRIPTION("Kernel Module Checker");
