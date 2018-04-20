#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/nsproxy.h>
#include <linux/spinlock.h>
#include <linux/rwsem.h>
#include <linux/proc_fs.h>
#include <asm/current.h>
#include "mount.h"

/* struct task_struct *current */
	/*struct nsproxy *nsproxy */
		/* struct mnt_namespace *mnt_ns */
			/* struct mount *root */
				/* struct vfsmount */

/* how to handle the list */
/* list count size if too much print error */
/* allocate memory */
/* list - print into buf */

/* name (device fiel ?)		mount points */

static char	*buf = NULL;

DECLARE_RWSEM(namespace_sem);

int		mymounts_open(struct inode *inode, struct file *filp)
{
	ssize_t			retval = 0;
	struct dentry		*dentry;
	struct mnt_namespace	*mnt = NULL;
	struct mount		*tmp;
	size_t			size = 0;

/*	dentry = current->nsproxy->mnt_ns->root->mnt_mountpoint; */
/*	mnt_devname = current->nsproxy->mnt_ns->root->mnt_devname; */

	printk("mymounts_open");
	mnt = current->nsproxy->mnt_ns;
	down_read(&namespace_sem);
	list_for_each_entry(tmp, &mnt->list, mnt_list) {
		printk(" %s\n", tmp->mnt_devname);
	}
	up_read(&namespace_sem);
	return retval;
	printk("dentry name %s\n", mnt->root->mnt_mountpoint->d_iname);
	printk("mnt_devname name %s\n", mnt->root->mnt_devname);


	if (!(buf = kmalloc(strlen(dentry->d_iname) + 2, GFP_KERNEL))) {
	 	retval = -ENOMEM;
		goto err;
	}
	
	strcpy(buf, dentry->d_iname);
	strncat(buf, " ", 1);
/*	strcat(buf, mnt_devname); */

err:
	return retval;
}

/* Assumption : number of mounts points inferior at mem page */

static ssize_t	mymounts_read(struct file *filp, char __user *buffer,
		size_t length, loff_t *offset)
{
	ssize_t		retval = 0;
	size_t		count;
	size_t		size;

	return retval;
	size = strlen(buf);
	count = size - *offset;
	if (length < count)
		count = length;

	printk("count %ld\n", count);

	if ((copy_to_user(buffer, &buf[*offset], count))) {
		retval = -EFAULT;
		goto err;
	}
	else {
		retval = count;
	}
	printk("retval %ld\n", retval);
	
	*offset += count;
err:
	return retval;
}

int		mymounts_flush(struct file *filp, fl_owner_t id)
{
	kfree(buf);

	return 0;
}

static struct file_operations mymounts_ops = {
	.read = mymounts_read,
	.open = mymounts_open,
	.flush = mymounts_flush,
};

static int __init mymounts_init(void) {
	proc_create("mymounts", 0, NULL, &mymounts_ops);
	return (0);
}

static void __exit mymounts_cleanup(void) {
	remove_proc_entry("mymounts", NULL);
}

module_init(mymounts_init);
module_exit(mymounts_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antoine Riard <ariard@student.42.fr>");
MODULE_DESCRIPTION("List Mounts points module");
