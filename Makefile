ifneq ($(KERNELRELEASE),)
obj-m := sda5708.o

else
ARCH=arm
CROSS_COMPILE=arm-buildroot-linux-uclibcgnueabi-
KDIR=~/embedded/raspi/buildroot-2013.05/output/build/linux-e959a8e/
PWD  :=$(shell pwd)

default:
	$(MAKE) CONFIG_DEBUG_SECTION_MISMATCH=y ARCH=arm \
	CROSS_COMPILE=arm-buildroot-linux-uclibcgnueabi- -C $(KDIR) M=$(PWD) modules
endif

clean:
	rm -rf *ko *o .*.cmd .tmp_versions Module.symvers
	rm -rf modules.order *.mod.c
