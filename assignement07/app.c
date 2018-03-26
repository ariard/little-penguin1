#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <strings.h>

int	main(void)
{
	char	buf[10];
	char	jbuf[64];
	int	fd;
	int	ret;

	bzero(buf, 10);	
	fd = open("/sys/kernel/debug/fortytwo/id", O_RDWR);
	if (fd > 0)
	{
		printf("\n___ ID ___\n");
		ret = read(fd, buf, 10);
		printf("1st read [%s] ret : %d\n", buf, ret);
		ret = read(fd, buf, 10);
		printf("2st read [%s] ret : %d\n", buf, ret);
		ret = write(fd, "ariard", 6);
		printf("1st write (ariard, 6) ret : %d\n", ret);
		ret = write(fd, "abcd", 4);
		printf("2st write (abcd, 4) ret : %d\n", ret);
		ret = write(fd, "abcdefgh", 20);
		printf("3st write (abcdefgh, 20) ret : %d\n", ret);
		close(fd);
	}
	else
		dprintf(2, "No such file id\n");

	bzero(jbuf, 64);
	fd = open("/sys/kernel/debug/fortytwo/jiffies", O_RDONLY);
	if (fd > 0) 
	{
		printf("\n___ JIFFIES ___\n");
		ret = read(fd, jbuf, 64);
		printf("read [%s] ret : %d\n", jbuf, ret);
	}
	else
		dprintf(2, "No such file jiffies\n");

	return (0);
}
