#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/namespace.h>
#include <asm/current.h>

/* Assumption : number of mounts points inferior at mem page */

int		mymounts_read(char *page, char **start, off_t offset, int count,
		int *eof, void *data)
{
	ssize_t		retval = 0;
	ssize_t		r;
	char		*buf;

	/* struct task_struct *current */
		/*struct nsproxy *nsproxy */
			/* struct mnt_namespace *mnt_ns */
				/* struct vfsmount */
	
	/* how to handle the list */
	/* list count size if too much print error */
	/* allocate memory */
	/* list - print into buf */
	
	/* name (device fiel ?)		mount points */

	current;	
	if ((r = copy_to_user(page, buf, count))) {
			retval = -EFAULT;
			goto err;
	}
	else {
			retval = r;
	}
	
err:
	*eof = 1;
	return retval;
}

static int __init mymounts_init(void) {
	create_proc_read_entry("mymounts", 0, NULL, mymounts_read, NULL);
	return (0);
}

static void __exit hello_cleanup(void) {
	remove_proc_entry("mymounts", NULL);
}

module_init(hello_init);
module_exit(hello_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antoine Riard <ariard@student.42.fr>");
MODULE_DESCRIPTION("List Mounts points module");
