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

	fd = open("/sys/kernel/debug/fortytwo/foo", O_RDWR);
	memset(buf, 'A', SIZE);
	if (fd > 0)
	{
		ret = write(fd, buf, SIZE);
		printf("1st write ret %d\n", ret);
		close(fd);
	}
	else
		dprintf(2, "No such file foo\n");
	
	return (0);
}
