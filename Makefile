obj-y += fan_driver.ko

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules