#include <stdio.h>


int main () {
    char buff[1024];
    if( 1 == scanf("%1024s", buff)) {
        puts(buff);
        return -1;
    }
    return -1;
}
