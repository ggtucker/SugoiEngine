#pragma once
#include <stdint.h>
// Like Team 1
typedef char        int8;
typedef int16_t     int16;
typedef int32_t     int32;
typedef int64_t     int64;
typedef uint8_t     uint8;
typedef uint16_t    uint16;
typedef uint32_t    uint32;
typedef uint64_t    uint64;
typedef float       real32;
typedef double	    real64;
// This int will always be the right size to hold a pointer, 32bit or 64 bit.
typedef intptr_t    intptr;

typedef void*       pvoid;

typedef const char* pcstr;
typedef char*       pstr;

#define NULL 0

