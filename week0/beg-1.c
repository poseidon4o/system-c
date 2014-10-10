// cat
#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 1024

int main(int argc, char * argv[]) {
    FILE * input;
    char buff[BUFF_SIZE];
    
    if ( 2 != argc ) {
        exit(EXIT_FAILURE);
    }
    
    input = fopen(argv[1], "r");
    if( !input ) {
        exit(EXIT_FAILURE);
    }
    
    do {
        int read = fread(buff, 1, BUFF_SIZE-1, input);
        buff[read] = 0;
        fputs(buff, stdout);
    } while(!feof(input) && !ferror(input));

    fclose(input);
    exit(EXIT_SUCCESS);
}
