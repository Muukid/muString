/*
mus.h - Muukid
Public domain simple single-file C library for easy string allocation and manipulation.
No warranty implied; use at your own risk.

Licensed under MIT License or public domain, whichever you prefer.
More explicit license information at the end of file.
*/

#ifndef MUS_H
#define MUS_H

#define MUS_VERSION_MAJOR 1
#define MUS_VERSION_MINOR 0
#define MUS_VERSION_PATCH 0

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef MUSDEF
    #ifdef MUS_STATIC
        #define MUSDEF static
    #else
        #define MUSDEF extern
    #endif
#endif

#ifndef size_m
    #ifndef _SIZE_T_
        #include <stddef.h>
    #endif
    #define size_m size_t
#endif

typedef enum { MUS_FALSE, MUS_TRUE } MUS_BOOL;
typedef struct {
    char* s;
    size_m size;
    size_m len;
} mustring;

MUSDEF size_m mus_strlen(char* s);

MUSDEF size_m mus_string_strlen(mustring s);

MUSDEF mustring mus_string_create(char* s);

MUSDEF void mus_string_destroy(mustring str);

MUSDEF mustring mus_string_size_check(mustring str, size_m size);

MUSDEF MUS_BOOL mus_here(char* str, char* check, size_m i);

MUSDEF MUS_BOOL mus_has(char* str, char* find, size_m beg, size_m end);

MUSDEF mustring mus_string_delete(mustring str, size_m beg, size_m end);

MUSDEF mustring mus_string_insert(mustring str, char* insert, size_m i);

MUSDEF mustring mus_string_replace(mustring str, char* find, char* replace, size_m beg, size_m end);

#ifdef __cplusplus
	}
#endif

#endif /* MUS_H */



#ifdef MUS_IMPLEMENTATION

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef _STDLIB_H
    #include <stdlib.h>
#endif
#define mus_malloc malloc
#define mus_free free
#define mus_realloc realloc

#ifndef _STRING_H
    #include <string.h>
#endif

size_m mus_strlen(char* s) {
    return strlen(s);
}
size_m mus_string_strlen(mustring s) {
    return s.len;
}

mustring mus_string_create(char* s) {
    mustring str;
    str.len = mus_strlen(s);
    str.size = (sizeof(char) * (str.len)) * 2;
    str.s = mus_malloc(str.size);
    for (size_m i = 0; i < str.len; i++) {
        str.s[i] = s[i];
    }
    str.s[str.len] = '\0';
    return str;
}

void mus_string_destroy(mustring str) {
    mus_free(str.s);
}

mustring mus_string_size_check(mustring str, size_m size) {
    MUS_BOOL resized = MUS_FALSE;
    while (str.size < size) {
        str.size *= 2;
        resized = MUS_TRUE;
    }
    if (resized == MUS_TRUE) {
        str.s = mus_realloc(str.s, str.size);
    }
    return str;
}

MUS_BOOL mus_here(char* str, char* check, size_m i) {
    if (str[i] != check[0]) return MUS_FALSE;
    size_m check_len = mus_strlen(check);
    size_m str_len = mus_strlen(str);
    for (size_m j = 0; j < check_len; j++) {
        if (i+j >= str_len || str[i+j] != check[j]) {
            return MUS_FALSE;
        }
    }
    return MUS_TRUE;
}

MUS_BOOL mus_has(char* str, char* find, size_m beg, size_m end) {
    size_m find_len = mus_strlen(find);
    for (size_m i = beg; i + find_len < end; i++) {
        /*if (mus_here(str, find, i) == MUS_TRUE) {
            return MUS_TRUE;
        }*/
        for (size_m j = 0; j < find_len; j++) {
            if (str[i+j] != find[j]) continue;
        }
        return MUS_TRUE;
    }
    return MUS_FALSE;
}

mustring mus_string_delete(mustring str, size_m beg, size_m end) {
    for (size_m i = end; i < mus_string_strlen(str) + 1; i++) {
        str.s[i-(end-beg)] = str.s[i];
    }
    str.len -= (end - beg);
    return str;
}

mustring mus_string_insert(mustring str, char* insert, size_m i) {
    size_m insert_len = mus_strlen(insert);
    str = mus_string_size_check(str, sizeof(char) * (mus_string_strlen(str) + insert_len + 1));
    for (size_m j = mus_string_strlen(str); i < j+1; j--) {
        str.s[j+insert_len] = str.s[j];
    }
    for (size_m j = 0; j < insert_len; j++) {
        str.s[i+j] = insert[j];
    }
    str.len += insert_len;
    return str;
}

mustring mus_string_replace(mustring str, char* find, char* replace, size_m beg, size_m end) {
    size_m find_len = mus_strlen(find);
    size_m replace_len = mus_strlen(replace);
    size_m len_dif = find_len - replace_len;
    if (replace_len > find_len) len_dif = replace_len - find_len;
    for (size_m i = beg; i < end + 1 && i < mus_string_strlen(str); i++) {
        if (mus_here(str.s, find, i) == MUS_TRUE) {
            str = mus_string_delete(str, i, i + find_len);
            if (replace_len > 0) {
                str = mus_string_insert(str, replace, i);
            }
            end -= len_dif;
        }
    }
    str.len = mus_strlen(str.s);
    return str;
}

#ifdef __cplusplus
	}
#endif

#endif /* MUS_IMPLEMENTATION */

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2023 Samuel Carmichael
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
