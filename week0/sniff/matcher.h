#ifndef MATCHER_INCLUDED
#define MATCHER_INCLUDED

typedef void(*matcher_cb)(const char*);

// init the matcher and pass callback to be called on every match
// after the callback has returned the pointer to the match will be invalidated!
int matcher_init(matcher_cb cb);

// append text to the matcher
int matcher_append(char * str);

// uninit the matcher
void matcher_uninit();

#endif
