/**
 * File interface hook shared library with multithread support
 * Author: sesiria
 * FileName: hookfile.c
 * Date: 8/18/2021
 * compile:
 * $ gcc -shared -fPIC -o hookfile.so hookfile.c helper.c -lpthread -ldl
 * usage:
 * 1. once command.
 * $ LD_PRELOAD="./hookfile.so" ./main
 * 
 * 2. set environment
 * $ setenv LD_PRELOAD "./hookfile.so"
 * $ ./main
 * $ unsetenv LD_PRELOAD
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dlfcn.h>
#include <unistd.h>
#include <errno.h>
#include "helper.h"


/* function decelaration */
typedef FILE *(*fopen_t)(const char * pathname, const char * mode);
typedef int (*fclose_t)(FILE *stream);
typedef int (*fflush_t)(FILE *stream);
typedef size_t (*fread_t)(void * ptr, size_t size, size_t nmemb,
                    FILE * stream);
typedef size_t (*fwrite_t)(const void * ptr, size_t size, size_t nmemb,
                    FILE * stream);
typedef long (*ftell_t)(FILE *stream);
typedef int (*fseek_t)(FILE *stream, long offset, int whence);
/* end of functiondecelaration */

/* define the function pointer to the original functions */
fopen_t     fopen_f     = NULL;
fclose_t    fclose_f    = NULL;
fflush_t    fflush_f    = NULL;
fread_t     fread_f     = NULL;
fwrite_t    fwrite_f    = NULL;
ftell_t     ftell_f     = NULL;
fseek_t     fseek_f     = NULL;
/* end define the funcion pointer */

/* global function ptr lookup table*/
struct {
    const char * funcName;
    void ** funcPtr;
} funcTable[] = {
    {   "fopen",    (void**)&fopen_f    },
    {   "fclose",   (void**)&fclose_f   },
    {   "fflush",   (void**)&fflush_f   },
    {   "fread",    (void**)&fread_f    },
    {   "fwrite",   (void**)&fwrite_f   },
    {   "ftell",    (void**)&ftell_f    },
    {   "fseek",    (void**)&fseek_f    }
};

static pthread_once_t once = PTHREAD_ONCE_INIT;    /* variable to indicate once initialization. */

/* guarantee that this function must be thread-safety.*/
void hookfunc()
{
    char *error;
    void *ptr;
    int idx;

    for(idx = 0; idx < COUNT_OF(funcTable); idx++) {
        // try to load function address by hook.
        ptr = dlsym(RTLD_NEXT, funcTable[idx].funcName);
        if((error = dlerror()) != NULL) {
            fputs(error, stderr);
            exit(EXIT_FAILURE);
        }

        // load the function address.
        *funcTable[idx].funcPtr = ptr;
    }
}

/* fopen wrapper function */
FILE *fopen(const char * pathname, const char * mode)
{
    int s;
    s = pthread_once(&once, hookfunc);
    if(s != 0) {
        perror("error run pthread_once!\n");
        exit(EXIT_FAILURE);
    }

    pthread_t tid = pthread_self();
    
    //if(isinWatchName(pathname)) 
    {
        fprintf(stderr, "[%lu] before fopen %s\n", tid, pathname);
        FILE * ret = fopen_f(pathname, mode);
        fprintf(stderr, "[%lu] after fopen ret %p \n", tid, ret);
        if(ret != NULL) {
            addWatchStream(ret, pathname);
        }
        return ret;
    }
    return fopen_f(pathname, mode);
}

/* fclose wrapper function */
int fclose(FILE *stream)
{
    int s;
    s = pthread_once(&once, hookfunc);
    if(s != 0) {
        perror("error run pthread_once!\n");
        exit(EXIT_FAILURE);
    }
    pthread_t tid = pthread_self();
    streamNode * p = NULL;
    if((p = getWatchStreamNode(stream)) != NULL) 
    {
        fprintf(stderr, "[%lu] before fclose %p %s\n", tid, stream, p->path);
        int ret = fclose_f(stream);
        fprintf(stderr, "[%lu] after fclose %p %s ret %d \n",tid, stream, p->path, ret);
        if(ret == 0) {
            removeWatchStream(stream);
        }
        return ret;
    }
    return fclose_f(stream);
}

/* fflush wrapper function */
int fflush(FILE *stream)
{
    int s;
    s = pthread_once(&once, hookfunc);
    if(s != 0) {
        perror("error run pthread_once!\n");
        exit(EXIT_FAILURE);
    }
    return fflush_f(stream);
}

/* fread wrapper function */
size_t fread(void * ptr, size_t size, size_t nmemb,
                    FILE * stream)
{
    int s;
    s = pthread_once(&once, hookfunc);
    if(s != 0) {
        perror("error run pthread_once!\n");
        exit(EXIT_FAILURE);
    }
    pthread_t tid = pthread_self();
    streamNode * p = NULL;
    if((p = getWatchStreamNode(stream)) != NULL) 
    {
        fprintf(stderr, "[%lu] before fread %p %s\n",tid, stream, p->path);
        size_t ret = fread_f(ptr, size, nmemb, stream);
        fprintf(stderr, "[%lu] after fread %p %s ret %ld \n",tid, stream, p->path, ret);        
        return ret;
    }
    return fread_f(ptr, size, nmemb, stream);
}

/* fwrite wrapper function */
size_t fwrite(const void * ptr, size_t size, size_t nmemb,
                    FILE * stream)
{
    int s;
    s = pthread_once(&once, hookfunc);
    if(s != 0) {
        perror("error run pthread_once!\n");
        exit(EXIT_FAILURE);
    }

    pthread_t tid = pthread_self();
    streamNode * p = NULL;
    if((p = getWatchStreamNode(stream)) != NULL) 
    {
        fprintf(stderr, "[%lu] before fwrite %p %s\n", tid, stream, p->path);
        size_t ret = fwrite_f(ptr, size, nmemb, stream);
        fprintf(stderr, "[%lu] after fwrite %p %s ret %ld\n", tid, stream, p->path, ret);        
        return ret;
    }
    return fwrite_f(ptr, size, nmemb, stream);
}

/* ftell wrapper function */
long ftell(FILE *stream)
{
    int s;
    s = pthread_once(&once, hookfunc);
    if(s != 0) {
        perror("error run pthread_once!\n");
        exit(EXIT_FAILURE);
    }
    return ftell_f(stream);
}

/* fseek wrapper function */
int fseek(FILE *stream, long offset, int whence)
{
    int s;
    s = pthread_once(&once, hookfunc);
    if(s != 0) {
        perror("error run pthread_once!\n");
        exit(EXIT_FAILURE);
    }
    return fseek_f(stream, offset, whence);
}
