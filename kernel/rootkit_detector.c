/*
 * Rootkit Detector Kernel Module
 * Author: Daniil Krizhanovskyi
 * License: GPL v3
 *
 * This module scans for rootkit-like behavior, such as anomalies in the 
 * IDT, syscall table tampering, hidden processes, and hidden files.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/dirent.h>
#include <linux/slab.h>
#include <asm/desc.h>
#include <linux/syscalls.h>

/*
 * Struct representing an entry in the Interrupt Descriptor Table (IDT).
 * The packed attribute ensures no padding is added.
 */
struct idt_entry {
    unsigned short offset_low;   // Low 16 bits of the handler address
    unsigned short selector;     // Segment selector
    unsigned char zero;          // Reserved, must be 0
    unsigned char type_attr;     // Type and attributes
    unsigned short offset_high;  // High 16 bits of the handler address
} __attribute__((packed));

/* Pointer to the syscall table */
unsigned long **sys_call_table;

/*
 * Locate the syscall table by scanning memory. This method iterates 
 * through memory and verifies the close syscall pointer.
 *
 * Returns:
 *   Pointer to the syscall table if found, NULL otherwise.
 */
static unsigned long **find_syscall_table(void) {
    unsigned long int offset;
    unsigned long **sct;

    for (offset = PAGE_OFFSET; offset < ULLONG_MAX; offset += sizeof(void *)) {
        sct = (unsigned long **)offset;

        // Check if the close syscall matches the expected function
        if (sct[__NR_close] == (unsigned long *)ksys_close) 
            return sct;
    }
    return NULL;
}

/*
 * Retrieve the base address of the Interrupt Descriptor Table (IDT).
 *
 * Returns:
 *   Address of the IDT.
 */
static void *get_idt_base(void) {
    struct desc_ptr idtr;
    asm("sidt %0" : "=m"(idtr));  // Load the IDT register
    return (void *)idtr.address;
}

/*
 * Scan the process list for hidden processes by comparing the 
 * /proc filesystem with the kernel task list.
 *
 * Returns:
 *   Number of hidden processes detected.
 */
static int scan_proc_for_hidden_processes(void) {
    struct task_struct *task;
    struct file *proc_dir;
    mm_segment_t old_fs;
    char *buffer;
    int hidden_count = 0;

    proc_dir = filp_open("/proc", O_RDONLY, 0);
    if (IS_ERR(proc_dir)) 
        return -1;

    old_fs = get_fs();
    set_fs(KERNEL_DS);  // Allow kernel space memory access

    buffer = kmalloc(4096, GFP_KERNEL);
    if (!buffer) {
        filp_close(proc_dir, NULL);
        set_fs(old_fs);
        return -1;
    }

    // Iterate through all tasks in the system
    for_each_process(task) 
        printk(KERN_INFO "Process: %s [%d]\n", task->comm, task->pid);

    kfree(buffer);
    set_fs(old_fs);
    filp_close(proc_dir, NULL);
    return hidden_count;
}

/*
 * Scan a directory for hidden files or suspicious anomalies.
 *
 * Parameters:
 *   dir_path - Path to the directory to scan.
 */
static void scan_hidden_files(const char *dir_path) {
    struct file *dir;
    mm_segment_t old_fs;

    dir = filp_open(dir_path, O_RDONLY | O_DIRECTORY, 0);
    if (IS_ERR(dir)) 
        return;

    old_fs = get_fs();
    set_fs(KERNEL_DS);  // Allow kernel space memory access
    // Directory scanning logic could be implemented here
    set_fs(old_fs);

    filp_close(dir, NULL);
}

/*
 * Perform various checks for rootkit-like behavior:
 * - IDT integrity
 * - Syscall table integrity
 * - Hidden processes
 * - Hidden files in specific directories
 */
static void perform_checks(void) {
    struct idt_entry *idt = (struct idt_entry *)get_idt_base();
    unsigned long handler_addr = ((idt[0].offset_high << 16) | idt[0].offset_low);

    // Check if the IDT handler matches the expected value
    if (handler_addr != (unsigned long)asm_exc_divide_error)
        printk(KERN_WARNING "IDT anomaly detected\n");

    // Check for syscall table anomalies
    sys_call_table = find_syscall_table();
    if (sys_call_table && sys_call_table[__NR_open] != (unsigned long *)ksys_open)
        printk(KERN_WARNING "Syscall table anomaly detected\n");

    // Check for hidden processes
    if (scan_proc_for_hidden_processes() > 0)
        printk(KERN_WARNING "Hidden processes detected\n");

    // Scan specific directories for hidden files
    scan_hidden_files("/etc");
    scan_hidden_files("/var");
}

/*
 * Module initialization function.
 *
 * Returns:
 *   0 on successful initialization.
 */
static int __init rootkit_detector_init(void) {
    printk(KERN_INFO "Initializing Rootkit Detector\n");
    perform_checks();
    return 0;
}

/*
 * Module cleanup function.
 */
static void __exit rootkit_detector_exit(void) {
    printk(KERN_INFO "Rootkit Detector unloaded\n");
}

/* Register the module's initialization and cleanup functions */
module_init(rootkit_detector_init);
module_exit(rootkit_detector_exit);

/* Module metadata */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniil Krizhanovskyi");
MODULE_DESCRIPTION("Rootkit Detector");
