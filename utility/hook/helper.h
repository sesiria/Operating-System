#ifndef HELPER_H_
#define HELPER_H_

#define COUNT_OF(x) (sizeof(x)/sizeof(x[0]))

typedef struct _streamNode {
    FILE * stream;
    char * path;
    struct _streamNode * next;
} streamNode;


int isinWatchName(const char * name);
int isinWatchStream(FILE * stream);
streamNode * getWatchStreamNode(FILE * stream);
void addWatchStream(FILE * stream, const char * pathname);
void removeWatchStream(FILE * stream);

#endif