/*
 * Syscall Table Checker
 * Author: Daniil Krizhanovskyi
 * License: GPL v3
 *
 * This module demonstrates reading from a file in the /proc filesystem
 * to check syscall operations and verify the kernel's integrity.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

/*
 * Function to check syscall table operations by performing a test
 * read from /proc/modules.
 *
 * It allocates a buffer, reads the contents of /proc/modules,
 * and logs the output to the kernel log. This function helps validate
 * that the syscall table is functioning correctly.
 */
static void check_syscall_table(void) {
    struct file *file;
    char *buffer;
    loff_t pos = 0;  // Offset for file reading
    ssize_t bytes_read;
    const size_t buffer_size = 256;

    // Open the /proc/modules file in read-only mode
    file = filp_open("/proc/modules", O_RDONLY, 0);
    if (IS_ERR(file)) {
        printk(KERN_ERR "Failed to open /proc/modules\n");
        return;
    }

    // Allocate a buffer to store the file content
    buffer = kmalloc(buffer_size, GFP_KERNEL);
    if (!buffer) {
        printk(KERN_ERR "Failed to allocate memory for buffer\n");
        filp_close(file, NULL);
        return;
    }

    // Read data from the file into the buffer
    bytes_read = kernel_read(file, buffer, buffer_size - 1, &pos); // Leave space for null terminator
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the buffer for safety
        printk(KERN_INFO "Read from /proc/modules: %s\n", buffer);
    } else {
        printk(KERN_ERR "Failed to read from /proc/modules\n");
    }

    // Free the allocated memory and close the file
    kfree(buffer);
    filp_close(file, NULL);
}

/*
 * Module initialization function.
 *
 * This function is called when the module is loaded into the kernel.
 * It logs a message and calls the syscall table check function.
 *
 * Returns:
 *   0 on successful initialization.
 */
static int __init syscall_checker_init(void) {
    printk(KERN_INFO "Initializing syscall checker\n");
    check_syscall_table();
    return 0;
}

/*
 * Module cleanup function.
 *
 * This function is called when the module is unloaded from the kernel.
 * It logs a message indicating the module has been unloaded.
 */
static void __exit syscall_checker_exit(void) {
    printk(KERN_INFO "Syscall checker unloaded\n");
}

/* Register the module's initialization and cleanup functions */
module_init(syscall_checker_init);
module_exit(syscall_checker_exit);

/* Module metadata */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniil Krizhanovskyi");
MODULE_DESCRIPTION("Syscall Table Checker");
