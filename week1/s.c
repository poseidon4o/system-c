#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/kd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int freq[] = { /* C   D    E    F    G    A    B    C */
                    523, 587, 659, 698, 784, 880, 988, 1046 };
    int i;
    int dev = open("/dev/console", O_RDWR, 0);
 
    printf("%d\n", dev);

    for (i=0; i<8; i++)
    {
            ioctl(dev, KIOCSOUND, 1193180/freq[i]);
            usleep(1000 * 100);
    }
    ioctl(dev, KIOCSOUND, 0); /*Stop silly sound*/
    return 0;
}
