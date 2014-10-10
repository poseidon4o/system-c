// file write
#include <stdio.h>
#include <string.h>


int main(int argc, char * argv[]) {
    char buff[1024];
    FILE * file;

    if (argc != 2) {
        return -1;
    }
    
    file = fopen(argv[1], "w+");
    if( !file ) {
        return -1;
    }

    if( 1 != scanf("%1023s", buff) ) {
        fclose(file);
        return -1;
    }

    if ( 1 != fwrite(buff, strlen(buff), 1, file) ) {
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}
