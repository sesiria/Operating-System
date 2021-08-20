/**
 * 
 * 
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#define TRUE    1
#define FALSE   0

#define MAX_PATH_LEN 512

// Add monitor file path below
static const char * watchNames[] = {"./pfc_run.sh"};

static streamNode streamList = {NULL, NULL};
streamNode * head = &streamList;

/* helper functions */
int isinWatchName(const char * name)
{
    int i;
    int size = COUNT_OF(watchNames);
    for(i = 0; i < size; i++) {
        if (strcmp(name, watchNames[i]) == 0)
            return TRUE;
    }
    return FALSE;
}

int isinWatchStream(FILE * stream)
{
    streamNode *p = head->next;
    while(p) {
        if(p->stream == stream)
            return TRUE;
        p = p->next;
    }
    return FALSE;
}

streamNode * getWatchStreamNode(FILE * stream)
{
    streamNode *p = head->next;
    while(p) {
        if(p->stream == stream)
            return p;
        p = p->next;
    }
    return NULL;    
}

void addWatchStream(FILE * stream, const char * pathname)
{
    streamNode *p = malloc(sizeof(streamNode));
    if(p == NULL) {
        perror("addWatchStream - malloc error!\n");
        exit(EXIT_FAILURE);
    }

    char * path = malloc(strlen(pathname) + 1);
    if(path == NULL) {
        perror("addWatchStream - malloc error!\n");
        exit(EXIT_FAILURE);
    }
    strcpy(path, pathname);
    p->stream = stream;
    p->path = path;
    p->next = head->next;
    head->next = p;
}

void removeWatchStream(FILE * stream)
{
    streamNode *p = head->next;
    streamNode *prev = head;
    while(p) {
        if(p->stream == stream) {
            prev->next = p->next;
            free(p->path);
            free(p);
            return;
        }
        prev = p;
        p = p->next;
    }
}
