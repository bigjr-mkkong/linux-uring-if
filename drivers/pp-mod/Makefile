obj-m			+= pp_main.o

KDIR = $(realpath ../../)

all:
	make -C $(KDIR) \
			ARCH=riscv \
			CROSS_COMPILE=riscv64-linux-gnu- \
			M=$(PWD) \
			modules

clean:
	make -C ../../ M=$(PWD) clean
