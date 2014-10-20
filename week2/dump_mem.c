#include <sys/ptrace.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <ctype.h>
#include <fcntl.h>
#include <string.h>

void get_mem_len(int map_file, unsigned long * ptr, int * size);
void print_mem(int mem_file, unsigned long ptr, int size);

int pid;
char * search, * replace;


int main (int argc, char * argv[]) {

    int mem_fd, map_fd;
    
    unsigned long ptr;
    int mem_sz;

    char mem_file[1024], map_file[1024];

    if (argc == 2) {
        pid = atoi(argv[1]);
    } else if (argc == 4) {
        search = argv[2];
        replace = argv[3];
        pid = atoi(argv[1]);
    }

    sprintf(map_file, "/proc/%d/maps", pid);
    sprintf(mem_file, "/proc/%d/mem", pid);

    map_fd = open(map_file, O_RDONLY);
    mem_fd = open(mem_file, O_RDWR);
    
    if( map_fd < 1 || mem_fd < 1) {
        puts("failed to open some file");
        return 0;
    }

    get_mem_len(map_fd, &ptr, &mem_sz);
    print_mem(mem_fd, ptr, mem_sz);

    return 0;
}

int read_line(int file, char * buff, int sz) {
    int c = 0;
    char it;
    while(read(file, &it, 1) == 1) {
        if (it == '\n') {
            buff[c++] = 0;
            return c;
        } else {
            buff[c++] = it;
            if (c == sz - 1) {
                buff[c++] = 0;
                return c;
            }
        }
    }
    return 0;
}

void get_mem_len(int map_file, unsigned long * ptr, int * size) {
    char buff[1024];
    
    while(read_line(map_file, buff,  1024)) {
       if (NULL != strstr(buff, "[heap]")) {
            int from, to;
            sscanf(buff, "%x-%x", &from, &to);
            *ptr = from;
            *size = (to - from);          
            return;
        }
    }
}

char * _memchr(char * source, char search, int max_len) {
    char * start = source;
    printf("_memchr %c in %d bytes\n", search, max_len);
    while (source < start + max_len) {
        if( *source == search ) {
            return source;
        }
        if ((source - start) % 1000 == 0) {
            printf("%d bytes remaining\n", max_len - (source - start));
        }
        ++source;
    }
    return NULL;
}

void print_mem(int mem_file, unsigned long ptr, int size) {
    lseek(mem_file, ptr, SEEK_SET);
    char * buff = (char*)malloc(size);
    int rd;
    ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    waitpid(pid, NULL, 0);
    
    rd = read(mem_file, buff, size);
    if (rd != size ) {
        puts("read less");
    } else {
        printf("%d\n", rd);
    }
 
    if (search && replace) {
        char * where = buff;
        int search_size = strlen(search);
        puts("searching...");
        while( (where = memchr(where, search[0], rd - (where - buff))) != NULL ) {
            if (!memcmp(where, search, search_size)) {
                int c = 0;
                printf("HIT! [");
                while(isprint(where[c])) printf("%c", where[c++]);
                puts("]");

                memcpy(where, replace, search_size);
            }
            where += search_size;
        }
    }
    lseek(mem_file, ptr, SEEK_SET);
    
    write(mem_file, buff, rd);
    
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    free(buff);
}



