#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antoine Riard <ariard@student.42.fr>");
MODULE_DESCRIPTION("usb keyboard module");

static int __init keyboard_init(void) 
{
	printk(KERN_INFO "usb keyboard module : init \n");
	return 0;
}

static void __exit keyboard_cleanup(void) 
{
	printk(KERN_INFO "usb keyboard module : cleanup \n");
}

module_init(keyboard_init);
module_exit(keyboard_cleanup);
