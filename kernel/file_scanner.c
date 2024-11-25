/*
 * Hidden File Scanner (Fixed for Modern Kernels)
 * Author: Daniil Krizhanovskyi
 * License: GPL v3
 *
 * This kernel module scans specified directories (e.g., /etc, /var) for files
 * and prints their names to the kernel log. It uses the modern VFS interfaces
 * to traverse directories.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fdtable.h>
#include <linux/dcache.h>
#include <linux/fs_context.h>
#include <linux/types.h>

/*
 * Structure for directory traversal context.
 *
 * The `ctx` field is used for interacting with the VFS iterate_shared
 * function to traverse directory entries.
 */
struct dir_ctx {
    struct dir_context ctx;
};

/*
 * Callback function for handling each directory entry during traversal.
 *
 * Parameters:
 *   ctx      - The directory context.
 *   name     - Name of the directory entry.
 *   namelen  - Length of the name.
 *   offset   - Offset within the directory.
 *   ino      - Inode number of the entry.
 *   d_type   - Type of the directory entry (e.g., file, directory).
 *
 * Returns:
 *   true to continue iteration, false to stop.
 */
static bool dir_actor(struct dir_context *ctx, const char *name, int namelen, loff_t offset, u64 ino, unsigned int d_type) {
    printk(KERN_INFO "Found entry: %.*s\n", namelen, name);
    return true; // Continue iteration
}

/**
 * scan_directory - Scans a directory and prints its entries.
 * @path: The directory path to scan.
 *
 * This function resolves the given directory path, opens the directory,
 * and iterates through its entries using the VFS iterate_shared function.
 */
static void scan_directory(const char *path) {
    struct file *dir_file;
    struct path dir_path;
    struct dir_ctx dir_ctx = {
        .ctx.actor = dir_actor,
        .ctx.pos = 0,
    };

    // Resolve the directory path
    if (kern_path(path, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &dir_path)) {
        printk(KERN_ERR "Failed to resolve directory: %s\n", path);
        return;
    }

    // Open the directory as a file
    dir_file = dentry_open(&dir_path, O_RDONLY, current_cred());
    if (IS_ERR(dir_file)) {
        printk(KERN_ERR "Failed to open directory: %s\n", path);
        path_put(&dir_path);  // Release the path reference
        return;
    }

    // Iterate through the directory entries
    if (dir_file->f_op->iterate_shared(dir_file, &dir_ctx.ctx))
        printk(KERN_ERR "Error iterating directory: %s\n", path);

    fput(dir_file);      // Release the file reference
    path_put(&dir_path); // Release the path reference
}

/**
 * check_hidden_files - Scans common directories for entries.
 *
 * This function scans predefined directories (/etc and /var) for files and
 * prints their names to the kernel log.
 */
static void check_hidden_files(void) {
    scan_directory("/etc");
    scan_directory("/var");
    printk(KERN_INFO "File scan completed\n");
}

/*
 * Module initialization function.
 *
 * This function is called when the module is loaded into the kernel.
 * It logs a message and initiates the file scanning process.
 *
 * Returns:
 *   0 on successful initialization.
 */
static int __init file_scanner_init(void) {
    printk(KERN_INFO "Initializing file scanner\n");
    check_hidden_files();
    return 0;
}

/*
 * Module cleanup function.
 *
 * This function is called when the module is removed from the kernel.
 * It logs a message indicating that the module has been unloaded.
 */
static void __exit file_scanner_exit(void) {
    printk(KERN_INFO "File scanner unloaded\n");
}

/* Register the module's initialization and cleanup functions */
module_init(file_scanner_init);
module_exit(file_scanner_exit);

/* Module metadata */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniil Krizhanovskyi");
MODULE_DESCRIPTION("Hidden File Scanner (Fixed for Modern Kernels)");
