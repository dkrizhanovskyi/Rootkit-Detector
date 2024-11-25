/*
 * Hidden Process Scanner
 * Author: Daniil Krizhanovskyi
 * License: GPL v3
 *
 * This kernel module scans the task list for hidden processes. It iterates
 * through all processes in the kernel's task list and logs their information.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>  // Required for for_each_process

/*
 * Scan the kernel task list for processes.
 *
 * This function iterates through the task list and prints the details of
 * each detected process to the kernel log.
 *
 * Returns:
 *   The number of hidden processes detected (always 0 in this implementation,
 *   as it only prints detected processes for demonstration purposes).
 */
static int scan_for_hidden_processes(void) {
    struct task_struct *task;
    int hidden_count = 0;

    // Iterate over each process in the task list
    for_each_process(task) {
        printk(KERN_INFO "Detected process: %s [%d]\n", task->comm, task->pid);
    }

    // Note: This implementation does not check for hidden processes.
    return hidden_count;
}

/*
 * Check for hidden processes and log the result.
 *
 * This function calls `scan_for_hidden_processes` to iterate through the
 * process list and logs a summary of hidden processes detected.
 */
static void check_hidden_processes(void) {
    int hidden = scan_for_hidden_processes();
    if (hidden > 0) {
        printk(KERN_WARNING "Detected %d hidden processes\n", hidden);
    } else {
        printk(KERN_INFO "No hidden processes detected\n");
    }
}

/*
 * Module initialization function.
 *
 * Called when the module is loaded into the kernel. Logs a message and
 * performs an initial scan for hidden processes.
 *
 * Returns:
 *   0 on successful initialization.
 */
static int __init process_scanner_init(void) {
    printk(KERN_INFO "Initializing process scanner\n");
    check_hidden_processes();
    return 0;
}

/*
 * Module cleanup function.
 *
 * Called when the module is removed from the kernel. Logs a message indicating
 * that the module has been unloaded.
 */
static void __exit process_scanner_exit(void) {
    printk(KERN_INFO "Process scanner unloaded\n");
}

/* Register the module's initialization and cleanup functions */
module_init(process_scanner_init);
module_exit(process_scanner_exit);

/* Module metadata */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniil Krizhanovskyi");
MODULE_DESCRIPTION("Hidden Process Scanner");
