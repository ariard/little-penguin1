#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antoine Riard <ariard@student.42.fr>");
MODULE_DESCRIPTION("Debugfs test module");

static struct dentry *debugfs_dir = NULL;

static ssize_t id_read(struct file *filp, char __user *buffer,
		size_t length, loff_t *offset)
{
	ssize_t		retval = 0;
	ssize_t		r;

	if ((r = copy_to_user(buffer, "ariard", 6)))
		retval = -EFAULT;
	else
		retval = r;

	return retval;
}

static ssize_t	id_write(struct file *filp, const char __user *buffer,
		size_t length, loff_t *offset)
{
	ssize_t		retval = 0;
	char 		wr_buf[6];

	memset(wr_buf, 0, 6);
	if (copy_from_user(wr_buf, buffer, 6)) {
		retval = -EFAULT;
		goto out;
	}

	if (!(memcmp(wr_buf, "ariard", 6)))
		retval = 6;
	else
		retval = -EINVAL;

out:
	return retval;
}

static struct file_operations id_fops = {
	.read = id_read,
	.write = id_write,
};

static int __init debugfs_init(void) 
{
	int	retval = 0;

	if ((debugfs_dir = debugfs_create_dir("fortytwo", NULL)) == -ENODEV) {
		printk(KERN_ERR"CONFIG_DEBUG_FS need to be set to use debug driver\n");
		retval = -ENODEV;
		goto out;
	}

	if (!debugfs_dir) {
		printk(KERN_ERR"debug driver : something went wrong\n");
		retval = -EEXIST;
		goto out;
	}


	if (!debugfs_create_file("id", 0666, debugfs_dir, NULL, &id_fops))
		printk((KERN_ERR"debug driver : id file debug creation failed\n"));

	printk(KERN_INFO "Loading debug driver\n");

out:
	return retval;
}

static void __exit debugfs_cleanup(void) 
{
	debugfs_remove_recursive(debugfs_dir);
	printk(KERN_INFO "Unloading debug driver...\n");
}

module_init(debugfs_init);
module_exit(debugfs_cleanup);
