NAME    	= debugfs

ifneq ($(KERNELRELEASE),)
	obj-m		+= $(NAME).o
else

	KERNELDIR	?= /lib/modules/$(shell uname -r)/build
	PWD		= $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

endif

clean:
	@rm -f Module.symvers $(NAME).ko $(NAME).mod.c $(NAME).mod.o $(NAME).o modules.order
	@rm -f built-in.o

re: clean default

test:
	@gcc -o app_id app_id.c
	@ ./app_id /sys/kernel/debug/fortytwo/id

test-clean:
	@rm -f app_id
