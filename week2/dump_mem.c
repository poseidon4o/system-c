#include <sys/ptrace.h>
#include <stdio.h>

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

int main (int argc, char * argv[]) {
    pid = atoi(argv[1]);
    int mem_fd, map_fd;
    
    unsigned long ptr;
    int mem_sz;

    char mem_file[1024], map_file[1024];
 
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

void print_mem(int mem_file, unsigned long ptr, int size) {
    lseek(mem_file, ptr, SEEK_SET);
    char * buff = malloc(size);
    char * buff2 = malloc(size);
    int rd = read(mem_file, buff, size);
    int c;
    if (rd != size ) {
        puts("read less");
    } else {
        printf("%d\n", rd);
    }
    for(c = 0; c < rd; ++c) {
        if (isprint(buff[c])) {
            printf("%c", buff[c]);
            buff2[c] = 'z';
        }
    }
    buff2[c] = '\0';
    lseek(mem_file, ptr, SEEK_SET);
    
    ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    waitpid(pid, NULL, 0);

    write(mem_file, buff2, rd);
    
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
}



