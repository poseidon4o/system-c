#include <stdio.h>

int main(int argc, char * argv[]) {
    if (argc == 2) {
        puts(argv[1]);
        return 0;
    } else {
        return -1;
    }
}