#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <string.h>


#define EVENT_SIZE (sizeof(struct inotify_event) + FILENAME_MAX + 1)

void poll_for_file(int fd);

int main (int argc, char * argv[]) {
    int fd, wd;
    int ok = EXIT_FAILURE;

    printf("watching for %s\n", argv[1]);
  
    fd = inotify_init();
    if (fd < 0) {
        puts(strerror(errno));
        ok = EXIT_FAILURE;
        goto end;
    }

    wd = inotify_add_watch(fd, argv[1], IN_CREATE | IN_DELETE);
    if (wd < 0) {
        puts(strerror(errno));
        ok = EXIT_FAILURE;
        goto close_fd;
    }

    poll_for_file(fd);

    inotify_rm_watch(fd, wd);
close_fd:
    close(fd);
end:
    exit(ok);
}

void poll_for_file(int fd) {
    char buff[EVENT_SIZE];

    for(;;) {
        int event_size, read_sz = read(fd, buff, EVENT_SIZE);
        if (read_sz > 0) {
            struct inotify_event * event = (struct inotify_event*)&buff;
            
            if ( !(event->mask & IN_ISDIR) ) {
                if (event->mask & IN_CREATE) {
                    printf("File created: [%s]\n", event->len ? event->name : "");
                } else if (event->mask & IN_DELETE) {
                    printf("File deleted: [%s]\n", event->len ? event->name : "");
                }
            }
            event_size = sizeof(struct inotify_event) + event->len;

            // hacky way to repair half read events
            if (EVENT_SIZE - event_size > 0) {
                memmove(buff, buff + event_size, EVENT_SIZE - event_size);
            }
        }
    }



}
