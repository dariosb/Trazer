/*
 * printrgb.c
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void
rgb_printf(int color, char* fmt, ...)
{
    va_list args;

    va_start(args,fmt);

    vfprintf(stderr, fmt, args);

    va_end(args);
}
