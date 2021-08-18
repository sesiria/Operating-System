#include "vgastr.h"

#define VGA_BUFFER_ADDR  0xb8000

void _strwrite(char * string)
{
    char* p_strdst = (char*)(VGA_BUFFER_ADDR);
    while(*string)
    {
        *p_strdst = *string++;
        p_strdst += 2;
    }
    return;
}

void printf(char* fmt, ...)
{
    _strwrite(fmt);
    return;
}