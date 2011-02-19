#ifndef _eyes_defines_hxx
#define _eyes_defines_hxx

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

typedef signed int sint32;
typedef signed short sing16;
typedef signed char sint8;

#define nil 0
#define abstract = 0

struct animation
{
    int     length;
    char ** frames;
};

#endif //eyes_defines_hxx
