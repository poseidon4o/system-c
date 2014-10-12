#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <string.h>
#include <ctype.h>

#include "packet_definitions.h"
#include "matcher.h"

#define perr(val,buff)\
    if (!val) {\
        printf("PCAP ERROR: %s", buff);\
        exit(EXIT_FAILURE);\
    }

char packet_buff[4096];

void packet_handler(u_char * args, const struct pcap_pkthdr * header, const u_char * packet) {
    int c, r = 0, offset = 0, data_length;
    struct radiotap_header * rt_header; 
    struct wireless_header * wireless_header;

    if (!header->len) {
        return;
    }
    
    rt_header = (struct radiotap_header *)packet;
    wireless_header = (struct wireless_header*)(packet + rt_header->it_len);
    
    if (
        !wireless_header->fc.protected_frame && 
        wireless_header->fc.sub_type == 0 &&
        wireless_header->fc.type == DATA
    ) {
        offset =  rt_header->it_len - sizeof(wireless_header);
        data_length = header->len - offset;
        if (data_length > 4096) {
            return;
        }
        
        for(c = 0; c < data_length; ++c) {
            if(isprint(packet[offset + c])) {
                packet_buff[r++] = packet[offset + c];
            } else {
                packet_buff[r++] = ' ';
            }
        }
        packet_buff[r] = '\0';
    }

    matcher_append(packet_buff);
}

void match_cb(const char * str) {
    printf("match: %s\n", str);
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
    
    if(matcher_init(match_cb) == -1) {
        puts("matcher failed to init");
        exit(EXIT_FAILURE);
    }

    if (pcap_loop(handle, -1, packet_handler, NULL) == -1) {
        matcher_uninit();
        perr(0, pcap_geterr(handle));
    } 

    matcher_uninit();
    return 0;
}
