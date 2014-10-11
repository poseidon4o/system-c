#include <pcre.h>
#include <stdio.h>
#include <string.h>

#include "matcher.h"

matcher_cb callback = NULL;
pcre * regex = NULL;
pcre_extra * regex_opti = NULL;

#define MBUF 65536
char buff[MBUF];
int buff_offset = 0;
int last_match = 0;


int matcher_init(matcher_cb cb) {    
    const char * err;
    int err_offset;

    callback = cb;
    regex = pcre_compile("\\b(\\w+)=\\\"?(\\w+)(?:\\b|\\\")",
        PCRE_MULTILINE | PCRE_EXTENDED | PCRE_CASELESS,
        &err, &err_offset, NULL);
    
    if (regex == NULL) {
        return -1;
    }
    
    regex_opti = pcre_study(regex, 0, &err);
    if (err != NULL) {
        pcre_free(regex);
        regex = NULL;
        return -1;
    }

    memset(buff, 0, MBUF);

    return 0;
}

int matcher_append(char * str) {
    int str_sz = strlen(str), exec_res;
    int groups[60], max_group = 60;
    
    if (str_sz > MBUF) {
        return -1;
    }
  
    if (str_sz + buff_offset >= MBUF) {
        strcpy(buff, str);
        buff_offset = 0;
    } else {
        strcpy(buff + buff_offset, str);
    }
    //printf("matcher: appending [%s]\n\n\n", str);

    exec_res = pcre_exec(regex, regex_opti, buff, buff_offset + str_sz, last_match, 0, groups, max_group);
    if (exec_res == PCRE_ERROR_NOMATCH) {
        puts(buff);
    }
    /*switch(exec_res) {
        case PCRE_ERROR_NOMATCH      : printf("String did not match the pattern\n");        break;
        case PCRE_ERROR_NULL         : printf("Something was null\n");                      break;
        case PCRE_ERROR_BADOPTION    : printf("A bad option was passed\n");                 break;
        case PCRE_ERROR_BADMAGIC     : printf("Magic number bad (compiled re corrupt?)\n"); break;
        case PCRE_ERROR_UNKNOWN_NODE : printf("Something kooky in the compiled re\n");      break;
        case PCRE_ERROR_NOMEMORY     : printf("Ran out of memory\n");                       break;
        default                      : printf("Unknown error\n");                           break;
    }*/
    if (exec_res >= 0) {
        int c;
        const char * match;
        exec_res = exec_res ? exec_res : max_group / 3;
        for (c = 0; c < exec_res; c += 2) {
            //if (c == 0 || c % 3 == 0) continue;
            pcre_get_substring(buff/* + offset ?*/, groups, exec_res, c, &match);
            //printf("Match(%2d/%2d): (%2d,%2d): '%s'\n", c, exec_res-1, groups[c*2], groups[c*2+1], match);
            callback(match);
            pcre_free_substring(match);
            last_match = c * 2 + 1;
        }
    } else {
        last_match = buff_offset;
    }

    //puts(buff);
    //printf("matcher: buff [%s]\n\n", buff);
    buff_offset = (buff_offset + str_sz) - last_match;
    memmove(buff, buff + last_match, buff_offset);
    buff[buff_offset] = '\0';
    
    return 0;
}

void matcher_uninit() {
    if (regex) {
        pcre_free(regex);
    }

    if (regex_opti) {
        pcre_free(regex_opti);
    }
}



