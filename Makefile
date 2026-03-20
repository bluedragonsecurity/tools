obj-m += rop.o
ccflags-y := -fno-stack-protector -D_FORTIFY_SOURCE=0 -g -Og
all:
	make   -b -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
clean:
	make   -b  -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
