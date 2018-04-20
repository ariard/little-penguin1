#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <strings.h>

#define SIZE	8192

int	main(void)
{
	int	fd;
	int	ret;
	char	buf[SIZE];

	bzero(buf, SIZE);
	fd = open("/sys/kernel/debug/fortytwo/foo", O_RDONLY);
	if (fd > 0)
	{
		printf("\n___ FOO ___\n");
		memset(buf, 0, SIZE);
		ret = read(fd, buf, SIZE);
		printf("1st read [%s] ret %d\n", buf, ret);
		close(fd);
	}
	else
		dprintf(2, "No such file foo\n");
	return (0);
}
