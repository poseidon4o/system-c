#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <string.h>

#define perr(val,buff)\
    if (!val) {\
        printf("PCAP ERROR: %s", buff);\
        exit(EXIT_FAILURE);\
    }

#define isalnum(x) ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || (x >= '0' && x <= '9'))

void packet_handler(u_char * args, const struct pcap_pkthdr * header, const u_char * packet) {
    int c;
    if (!header->len) {
        return;
    }
    for(c = 0; c < header->len; ++c) {
        printf("%c", packet[c]);
    }
    puts("");
}

int main(int argc, char * argv[]) {
    char errbuf[PCAP_ERRBUF_SIZE], filter_buff[128];
    pcap_t * handle;
    bpf_u_int32 mask, ip;
    struct bpf_program filter;
    
    if (argc != 3) {
        puts("%s <device> <port>");
        exit(EXIT_FAILURE);
    }
   
    if (pcap_lookupnet(argv[1], &ip, &mask, errbuf) == -1) {
        perr(0, errbuf);
    }

    handle = pcap_open_live(argv[1], BUFSIZ, 1, 1000, errbuf);
    perr(handle, errbuf);
 
    snprintf(filter_buff, 128, "port %s", argv[2]); 
    if (pcap_compile(handle, &filter, filter_buff,  0, ip) == -1) {
        perr(0, pcap_geterr(handle));
    }
    
    if (pcap_setfilter(handle, &filter) == -1) {
        perr(0, pcap_geterr(handle));
    }

    if (pcap_loop(handle, -1, packet_handler, NULL) == -1) {
        perr(0, pcap_geterr(handle));
    } 

    puts("wooot");
    return 0;
}
