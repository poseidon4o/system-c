#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int ownpid = getpid();
    const char * orig_str = "working string lqlqlq";
    char * buff = malloc(256);
    int c;

    for(c = 0; c < strlen(orig_str); ++c) {
        buff[c] = orig_str[c];
    }
    buff[c] = 0;
    
    while(1) {
        usleep(1000 * 1000);
        printf("%d %s\n", ownpid, buff);
    }

    return 0;

}
