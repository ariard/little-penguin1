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
#include <linux/dcache.h>
#include <asm/current.h>
#include "mount.h"

static char	*buf = NULL;

DECLARE_RWSEM(namespace_sem);

int		mymounts_open(struct inode *inode, struct file *filp)
{
	ssize_t			retval = 0;
	struct mnt_namespace	*mnt = NULL;
	struct mount		*tmp;
	struct mount		*father = NULL;
	size_t			size = 0;
	char			raw[256];
	char			b[600];
	char			*path;
	char			fullpath[512];
	int			prev_id;
	int			i;

	if (!(buf = kmalloc(PAGE_SIZE, GFP_KERNEL))) {
	 	retval = -ENOMEM;
		goto err;
	}

	mnt = current->nsproxy->mnt_ns;
	down_read(&namespace_sem);
	list_for_each_entry(tmp, &mnt->list, mnt_list) {
		if (!tmp->mnt_mp)
			continue;
		memset(fullpath, 0, 512);
		memset(raw, 0, 256);
		path = dentry_path_raw(tmp->mnt_mountpoint, raw, 256);
		i = 512 - strlen(path) - 1;
		strcat(&fullpath[i], path);
		father = tmp->mnt_parent;
		while (father && father->mnt_mountpoint && prev_id != father->mnt_id) {
			memset(raw, 0, 256);
			path = dentry_path_raw(father->mnt_mountpoint, raw, 256);
			if ((i = i - strlen(path)) < 0)
				break;
			memcpy(&fullpath[i], path, strlen(path));
			prev_id = father->mnt_id;
			father = father->mnt_parent;
		}
		memset(b, 0, 600);
		while (fullpath[i + 1] && fullpath[i + 1] == '/')
			i++;
		sprintf(b, "%-20s %s\n", tmp->mnt_devname, &fullpath[i]);
		size += strlen(b);
		if (size > PAGE_SIZE)
			break;
		strcat(buf, b);
	}
	up_read(&namespace_sem);

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

	size = strlen(buf);
	count = size - *offset;
	if (length < count)
		count = length;

	if ((copy_to_user(buffer, &buf[*offset], count))) {
		retval = -EFAULT;
		goto err;
	}
	else {
		retval = count;
	}
	
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
