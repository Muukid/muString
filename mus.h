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
#define MUS_VERSION_MINOR 1
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
    #include <stddef.h>
    #define size_m size_t
#endif

#ifndef wchar_m
    #include <wchar.h>
    #define wchar_m wchar_t
#endif

typedef enum { MUS_FALSE, MUS_TRUE } MUS_BOOL;
typedef enum { MUS_STRING_TYPE_CHAR, MUS_STRING_TYPE_WCHAR } MUS_STRING_TYPE;
typedef struct {
    char* s;
    wchar_m* ws;
    MUS_STRING_TYPE type;
    size_m size;
    size_m len;
} mustring;

MUSDEF size_m mus_wchar_m_string_to_char_size(wchar_m* src);
MUSDEF void mus_wchar_m_string_to_char_string(char* dest, wchar_m* src, size_m dest_size);
MUSDEF size_m mus_char_string_to_wchar_m_size(char* src);
MUSDEF void mus_char_string_to_wchar_m_string(wchar_m* dest, char* src, size_m dest_size);

MUSDEF size_m mus_string_strlen(mustring s);

MUSDEF mustring mus_string_create(char* s);
MUSDEF mustring mus_wide_string_create(wchar_m* ws);
MUSDEF mustring mus_string_destroy(mustring str);

MUSDEF mustring mus_string_size_check(mustring str, size_m size);

MUSDEF MUS_BOOL mus_here(char* str, char* check, size_m i);
MUSDEF MUS_BOOL mus_w_here(wchar_m* str, wchar_m* check, size_m i);
MUSDEF MUS_BOOL mus_has(char* str, char* find, size_m beg, size_m end);
MUSDEF MUS_BOOL mus_w_has(wchar_m* str, wchar_m* find, size_m beg, size_m end);

MUSDEF mustring mus_string_delete(mustring str, size_m beg, size_m end);

MUSDEF mustring mus_string_insert(mustring str, char* insert, size_m i);
MUSDEF mustring mus_string_w_insert(mustring str, wchar_m* insert, size_m i);

MUSDEF mustring mus_string_replace(mustring str, char* find, char* replace, size_m beg, size_m end);
MUSDEF mustring mus_string_w_replace(mustring str, wchar_m* find, wchar_m* replace, size_m beg, size_m end);

#ifdef __cplusplus
	}
#endif

#endif /* MUS_H */



#ifdef MUS_IMPLEMENTATION

#ifdef __cplusplus
	extern "C" {
#endif

#if !defined(mus_malloc) || !defined(mus_free) || !defined(mus_realloc) || !defined(mus_mbstowcs) || !defined(mus_wcstombs)
    #include <stdlib.h>
    #ifndef mus_malloc
        #define mus_malloc malloc
    #endif
    #ifndef mus_free
        #define mus_free free
    #endif
    #ifndef mus_realloc
        #define mus_realloc realloc
    #endif
    #ifndef mus_mbstowcs
        #define mus_mbstowcs mbstowcs
    #endif
    #ifndef mus_wcstombs
        #define mus_wcstombs wcstombs
    #endif
#endif

#if !defined(mus_strlen) || !defined(mus_wstrlen)
    #ifndef mus_strlen
        #include <string.h>
        #define mus_strlen strlen
    #endif
    #ifndef mus_wstrlen
        #include <wchar.h>
        #define mus_wstrlen wcslen
    #endif
#endif

MUSDEF size_m mus_wchar_m_string_to_char_size(wchar_m* src) {
    return mus_wcstombs(0, src, 0);
}
MUSDEF void mus_wchar_m_string_to_char_string(char* dest, wchar_m* src, size_m dest_size) {
    mus_wcstombs(dest, src, dest_size);
}

MUSDEF size_m mus_char_string_to_wchar_m_size(char* src) {
    return mus_mbstowcs(0, src, 0);
}
MUSDEF void mus_char_string_to_wchar_m_string(wchar_m* dest, char* src, size_m dest_size) {
    mus_mbstowcs(dest, src, dest_size);
}

MUSDEF size_m mus_string_strlen(mustring s) {
    return s.len;
}

MUSDEF mustring mus_string_create(char* s) {
    mustring str;
    str.len = mus_strlen(s);
    str.size = (sizeof(char) * (str.len)) * 2;
    str.s = mus_malloc(str.size);
    str.ws = 0;
    str.type = MUS_STRING_TYPE_CHAR;
    for (size_m i = 0; i < str.len; i++) {
        str.s[i] = s[i];
    }
    str.s[str.len] = '\0';
    return str;
}
MUSDEF mustring mus_wide_string_create(wchar_m* ws) {
    mustring str;
    str.len = mus_wstrlen(ws);
    str.size = (sizeof(wchar_m) * (str.len)) * 2;
    str.ws = mus_malloc(str.size);
    str.s = 0;
    str.type = MUS_STRING_TYPE_WCHAR;
    for (size_m i = 0; i < str.len; i++) {
        str.ws[i] = ws[i];
    }
    str.ws[str.len] = '\0';
    return str;
}

MUSDEF mustring mus_string_destroy(mustring str) {
    if (str.s) mus_free(str.s);
    str.s = 0;
    if (str.ws) mus_free(str.ws);
    str.ws = 0;
    return str;
}

MUSDEF mustring mus_string_size_check(mustring str, size_m size) {
    MUS_BOOL resized = MUS_FALSE;
    while (str.size < size) {
        str.size *= 2;
        resized = MUS_TRUE;
    }
    if (resized == MUS_TRUE) {
        if (str.type == MUS_STRING_TYPE_CHAR) {
            str.s = mus_realloc(str.s, str.size);
        } else {
            str.ws = mus_realloc(str.ws, str.size);
        }
    }
    return str;
}

MUSDEF MUS_BOOL mus_here(char* str, char* check, size_m i) {
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
MUSDEF MUS_BOOL mus_w_here(wchar_m* str, wchar_m* check, size_m i) {
    if (str[i] != check[0]) return MUS_FALSE;
    size_m check_len = mus_wstrlen(check);
    size_m str_len = mus_wstrlen(str);
    for (size_m j = 0; j < check_len; j++) {
        if (i+j >= str_len || str[i+j] != check[j]) {
            return MUS_FALSE;
        }
    }
    return MUS_TRUE;
}

MUSDEF MUS_BOOL mus_has(char* str, char* find, size_m beg, size_m end) {
    size_m find_len = mus_strlen(find);
    for (size_m i = beg; i + find_len < end; i++) {
        MUS_BOOL found = MUS_TRUE;
        for (size_m j = 0; j < find_len; j++) {
            if (str[i+j] != find[j]) found = MUS_FALSE;
        }
        if (found == MUS_TRUE) return MUS_TRUE;
    }
    return MUS_FALSE;
}
MUSDEF MUS_BOOL mus_w_has(wchar_m* str, wchar_m* find, size_m beg, size_m end) {
    size_m find_len = mus_wstrlen(find);
    for (size_m i = beg; i + find_len < end; i++) {
        MUS_BOOL found = MUS_TRUE;
        for (size_m j = 0; j < find_len; j++) {
            if (str[i+j] != find[j]) found = MUS_FALSE;
        }
        if (found == MUS_TRUE) return MUS_TRUE;
    }
    return MUS_FALSE;
}

MUSDEF mustring mus_string_delete(mustring str, size_m beg, size_m end) {
    for (size_m i = end; i < mus_string_strlen(str) + 1; i++) {
        if (str.type == MUS_STRING_TYPE_CHAR) {
            str.s[i-(end-beg)] = str.s[i];
        } else {
            str.ws[i-(end-beg)] = str.ws[i];
        }
    }
    str.len -= (end - beg);
    return str;
}

MUSDEF mustring mus_string_insert(mustring str, char* insert, size_m i) {
    if (str.type == MUS_STRING_TYPE_WCHAR) return str;
    size_m insert_len = mus_strlen(insert);
    str = mus_string_size_check(str, sizeof(char) * (mus_string_strlen(str) + insert_len + 1));
    for (size_m j = mus_strlen(str.s); i < j+1; j--) {
        if (str.type == MUS_STRING_TYPE_CHAR) {
            str.s[j+insert_len] = str.s[j];
        } else {
            str.ws[j+insert_len] = str.ws[j];
        }
    }
    for (size_m j = 0; j < insert_len; j++) {
        if (str.type == MUS_STRING_TYPE_CHAR) {
            str.s[i+j] = insert[j];
        } else {
            str.ws[i+j] = insert[j];
        }
    }
    str.len += insert_len;
    return str;
}
MUSDEF mustring mus_string_w_insert(mustring str, wchar_m* insert, size_m i) {
    if (str.type == MUS_STRING_TYPE_CHAR) return str;
    size_m insert_len = mus_wstrlen(insert);
    str = mus_string_size_check(str, sizeof(wchar_m) * (mus_string_strlen(str) + insert_len + 1));
    for (size_m j = mus_string_strlen(str); i < j+1; j--) {
        if (str.type == MUS_STRING_TYPE_CHAR) {
            str.s[j+insert_len] = str.s[j];
        } else {
            str.ws[j+insert_len] = str.ws[j];
        }
    }
    for (size_m j = 0; j < insert_len; j++) {
        if (str.type == MUS_STRING_TYPE_CHAR) {
            str.s[i+j] = insert[j];
        } else {
            str.ws[i+j] = insert[j];
        }
    }
    str.len += insert_len;
    return str;
}

MUSDEF mustring mus_string_replace(mustring str, char* find, char* replace, size_m beg, size_m end) {
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
MUSDEF mustring mus_string_w_replace(mustring str, wchar_m* find, wchar_m* replace, size_m beg, size_m end) {
    size_m find_len = mus_wstrlen(find);
    size_m replace_len = mus_wstrlen(replace);
    size_m len_dif = find_len - replace_len;
    if (replace_len > find_len) len_dif = replace_len - find_len;
    for (size_m i = beg; i < end + 1 && i < mus_string_strlen(str); i++) {
        if (mus_w_here(str.ws, find, i) == MUS_TRUE) {
            str = mus_string_delete(str, i, i + find_len);
            if (replace_len > 0) {
                str = mus_string_w_insert(str, replace, i);
            }
            end -= len_dif;
        }
    }
    str.len = mus_wstrlen(str.ws);
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
