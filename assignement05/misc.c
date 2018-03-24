#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antoine Riard <ariard@student.42.fr>");
MODULE_DESCRIPTION("misc char device driver module");

static ssize_t misc_read(struct file *filp, char __user *buffer,
			size_t length, loff_t *offset)
{
	;
}

static ssize_t misc_write(struct file *filp, const char __user *buffer,
			size_t length, loff_t *offset)
{
	;
}

static struct file_operations misc_fops = {
	.read = misc_read,
	.write = misc_write,
};

static struct miscdevice misc_driver = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &misc_fops,
};

static int __init misc_init(void) 
{
	int	result;

	result = misc_register(&misc_driver);
	if (result)
		printk(KERN_INFO "misc module : misc_register failed with %d\n", result);
	else
		printk(KERN_INFO "misc module : misc_register successful\n");

	return result;
}

static void __exit misc_cleanup(void) 
{
	misc_deregister(&misc_driver);
	printk(KERN_INFO "misc module : misc_deregister\n");
}

module_init(misc_init);
module_exit(misc_cleanup);
