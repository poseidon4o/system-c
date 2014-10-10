// cp
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define BUFF_SIZE 1024

int main(int argc, char * argv[]) {
    FILE * source, * destination;
    int read_size;

    char buff[BUFF_SIZE];
    
    if ( argc != 3 ) {
        exit(EXIT_FAILURE);
    }
    
    source = fopen(argv[1], "r");
    if (!source) {
        exit(EXIT_FAILURE);
    }
    
    destination = fopen(argv[2], "w+");
    if (!destination) {
        fclose(source);
        exit(EXIT_FAILURE);
    }

    do {
        read_size = fread(buff, BUFF_SIZE - 1, 1, source);
        if (read_size && fwrite(buff, read_size, 1, destination) != read_size) {
            fclose(source);
            fclose(destination);
            exit(EXIT_FAILURE);
        }
    } while (read_size);
    
    fclose(source);
    fclose(destination);

    exit(EXIT_SUCCESS);
}

