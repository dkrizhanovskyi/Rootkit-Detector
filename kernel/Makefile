# Define modules to compile
obj-m += syscall_checker.o
obj-m += process_scanner.o
obj-m += idt_ssdt_checker.o
obj-m += file_scanner.o
obj-m += module_checker.o

# Kernel build directory
KDIR := /lib/modules/$(shell uname -r)/build

# Build all modules
all:
	make -C $(KDIR) M=$(PWD) modules

# Clean build artifacts
clean:
	make -C $(KDIR) M=$(PWD) clean
