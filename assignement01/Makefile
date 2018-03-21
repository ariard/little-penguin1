NAME    	= main

ifneq ($(KERNELRELEASE),)
	obj-m		+= $(NAME).o
else

	KERNELDIR	?= /lib/modules/$(shell uname -r)/build
	PWD		= $(shell pwd)

default :
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

endif

clean:
	@rm -f Module.symvers $(NAME).ko $(NAME).mod.c $(NAME).mod.o $(NAME).o modules.order
	@rm -f built-in.o
