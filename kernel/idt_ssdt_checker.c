/*
 * Syscall Monitoring Using Kprobes
 * Author: Daniil Krizhanovskyi
 * License: GPL v3
 *
 * This kernel module demonstrates how to use kprobes to monitor system calls.
 * It hooks the `sys_open` and `sys_close` system calls to log their usage.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>

/* Kprobes for sys_open and sys_close */
static struct kprobe kp_open;
static struct kprobe kp_close;

/*
 * Pre-handler for sys_open.
 *
 * Logs the filename pointer passed to the sys_open system call.
 *
 * Parameters:
 *   p    - Pointer to the kprobe structure.
 *   regs - CPU registers at the time of the syscall.
 *
 * Returns:
 *   0 (always) to continue execution.
 */
static int pre_handler_open(struct kprobe *p, struct pt_regs *regs) {
    printk(KERN_INFO "sys_open called with filename pointer: %p\n", (void *)regs->di);
    return 0;
}

/*
 * Pre-handler for sys_close.
 *
 * Logs the file descriptor passed to the sys_close system call.
 *
 * Parameters:
 *   p    - Pointer to the kprobe structure.
 *   regs - CPU registers at the time of the syscall.
 *
 * Returns:
 *   0 (always) to continue execution.
 */
static int pre_handler_close(struct kprobe *p, struct pt_regs *regs) {
    printk(KERN_INFO "sys_close called with fd: %lu\n", regs->di);
    return 0;
}

/*
 * Module initialization function.
 *
 * Registers kprobes for `sys_open` and `sys_close` system calls. If any
 * registration fails, it cleans up previously registered probes.
 *
 * Returns:
 *   0 on successful initialization, or a negative error code on failure.
 */
static int __init kprobe_init(void) {
    int ret;

    // Hook sys_open
    kp_open.symbol_name = "sys_open";
    kp_open.pre_handler = pre_handler_open;
    ret = register_kprobe(&kp_open);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register kprobe for sys_open: %d\n", ret);
        return ret;
    }

    // Hook sys_close
    kp_close.symbol_name = "sys_close";
    kp_close.pre_handler = pre_handler_close;
    ret = register_kprobe(&kp_close);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register kprobe for sys_close: %d\n", ret);
        unregister_kprobe(&kp_open);
        return ret;
    }

    printk(KERN_INFO "Kprobes for sys_open and sys_close registered\n");
    return 0;
}

/*
 * Module cleanup function.
 *
 * Unregisters the kprobes for `sys_open` and `sys_close`.
 */
static void __exit kprobe_exit(void) {
    unregister_kprobe(&kp_open);
    unregister_kprobe(&kp_close);
    printk(KERN_INFO "Kprobes unregistered\n");
}

/* Register the module's initialization and cleanup functions */
module_init(kprobe_init);
module_exit(kprobe_exit);

/* Module metadata */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniil Krizhanovskyi");
MODULE_DESCRIPTION("Syscall Monitoring Using Kprobes");
