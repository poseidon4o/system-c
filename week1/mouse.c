#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/input.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <linux/kd.h>

#define MOUSEFILE "/dev/input/event17"

int main ()
{
    int fd;
    struct input_event ie;

    if ((fd = open (MOUSEFILE, O_RDONLY)) == -1) {
        perror ("opening device");
        exit (EXIT_FAILURE);
    }
    
    while (1) {
        int res = read (fd, &ie, sizeof(struct input_event));

        if (!res)
            continue;
    }
    return 0;
}
