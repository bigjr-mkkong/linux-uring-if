CROSS_COMPILE=riscv64-linux-gnu-
ARCH=riscv

KERN_SRC := ../../

obj-m += pi_mod.o
pi_mod-objs += pi_main.o pi_ops.o

all:
	$(MAKE) -C $(KERN_SRC) M=$(shell pwd) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KERN_SRC) M=$(shell pwd) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
