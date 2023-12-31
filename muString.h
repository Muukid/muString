/*
muString.h - Muukid
Public domain simple single-file C library for easy string allocation and manipulation.
No warranty implied; use at your own risk.

Licensed under MIT License or public domain, whichever you prefer.
More explicit license information at the end of file.
*/

#ifndef MUS_H
#define MUS_H

#define MUS_VERSION_MAJOR 3
#define MUS_VERSION_MINOR 1
#define MUS_VERSION_PATCH 0

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef MUDEF
    #ifdef MU_STATIC
        #define MUDEF static
    #else
        #define MUDEF extern
    #endif
#endif

#if !defined(int64_m)

    #include <stdint.h>

    #ifndef int64_m
        #define int64_m int64_t
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

#ifndef MU_NULL
    #define MU_NULL 0
#endif

#ifndef MU_BOOL
    #define MU_BOOL
    typedef enum { MU_FALSE, MU_TRUE } muBool;
#endif

typedef enum { MU_STRING_TYPE_CHAR, MU_STRING_TYPE_WCHAR } muStringType;
typedef struct {
    char* s;
    wchar_m* ws;
    muStringType type;
    size_m size;
    size_m len;
} muString;

/* muString */

MUDEF size_m mu_string_strlen(muString s);

MUDEF muString mu_string_create (char* s);
MUDEF muString mu_wstring_create(wchar_m* ws);

MUDEF muString mu_string_destroy(muString str);

MUDEF muString mu_string_size_check(muString str, size_m size);

MUDEF muString mu_string_delete(muString str, size_m beg, size_m end);

MUDEF muString mu_string_insert (muString str, char* insert,    size_m i);
MUDEF muString mu_wstring_insert(muString str, wchar_m* insert, size_m i);

MUDEF muString mu_string_insert_integer (muString str, int64_m n, size_m i);
MUDEF muString mu_wstring_insert_integer(muString str, int64_m n, size_m i);

MUDEF muString mu_string_insert_float (muString str, double n, size_m max_decimals, size_m i);
MUDEF muString mu_wstring_insert_float(muString str, double n, size_m max_decimals, size_m i);

MUDEF muString mu_string_replace (muString str, char* find,    char* replace,    size_m beg, size_m end);
MUDEF muString mu_wstring_replace(muString str, wchar_m* find, wchar_m* replace, size_m beg, size_m end);

MUDEF muString mu_string_to_lowercase(muString str, size_m beg, size_m end);
MUDEF muString mu_string_to_uppercase(muString str, size_m beg, size_m end);

/* string */

MUDEF muBool mu_here (char* str,    char* check,    size_m i);
MUDEF muBool mu_where(wchar_m* str, wchar_m* check, size_m i);

MUDEF muBool mu_has (char* str,    char* find,    size_m beg, size_m end);
MUDEF muBool mu_whas(wchar_m* str, wchar_m* find, size_m beg, size_m end);

/* character conversion */

MUDEF int mu_wstring_to_string(char* dest,    wchar_m* src, size_m dest_len);
MUDEF int mu_string_to_wstring(wchar_m* dest, char* src,    size_m dest_len);

/* uppercase/lowercase */

MUDEF char    mu_char_to_lowercase (char c);
MUDEF wchar_m mu_wchar_to_lowercase(wchar_m c);

MUDEF char    mu_char_to_uppercase (char c);
MUDEF wchar_m mu_wchar_to_uppercase(wchar_m c);

MUDEF muBool mu_char_is_lowercase (char c);
MUDEF muBool mu_wchar_is_lowercase(wchar_m c);

MUDEF muBool mu_char_is_uppercase (char c);
MUDEF muBool mu_wchar_is_uppercase(wchar_m c);

/* numbers */

MUDEF int   mu_integer_to_string (int64_m n, char* dest,    size_m dest_len);
MUDEF int   mu_integer_to_wstring(int64_m n, wchar_m* dest, size_m dest_len);

MUDEF int   mu_float_to_string (double n, size_m max_decimals, char* dest,    size_m dest_len);
MUDEF int   mu_float_to_wstring(double n, size_m max_decimals, wchar_m* dest, size_m dest_len);

#ifdef __cplusplus
    }
#endif

#endif /* MUS_H */



#ifdef MUS_IMPLEMENTATION

#ifdef __cplusplus
    extern "C" {
#endif

#if !defined(mu_malloc)    || \
    !defined(mu_free)      || \
    !defined(mu_realloc)   || \
    !defined(mu_mbstowcs)  || \
    !defined(mu_mbsrtowcs) || \
    !defined(mu_wcstombs)  || \
    !defined(mu_wcsrtombs)

    #include <stdlib.h>
    #ifndef mu_malloc
        #define mu_malloc malloc
    #endif
    #ifndef mu_free
        #define mu_free free
    #endif
    #ifndef mu_realloc
        #define mu_realloc realloc
    #endif
    #ifndef mu_mbstowcs
        #define mu_mbstowcs mbstowcs
    #endif
    #ifndef mu_mbsrtowcs
        #define mu_mbsrtowcs mbsrtowcs
    #endif
    #ifndef mu_wcstombs
        #define mu_wcstombs wcstombs
    #endif
    #ifndef mu_wcsrtombs
        #define mu_wcsrtombs wcsrtombs
    #endif
#endif

#if !defined(mu_strlen) || !defined(mu_wstrlen)
    #ifndef mu_strlen
        #include <string.h>
        #define mu_strlen strlen
    #endif
    #ifndef mu_wstrlen
        #include <wchar.h>
        #define mu_wstrlen wcslen
    #endif
#endif

#if !defined(mu_snprintf)

    #include <stdio.h>

    #ifndef mu_snprintf
        #define mu_snprintf snprintf
    #endif

#endif

// not sure if multi-byte state being null is fully safe

MUDEF int mu_wstring_to_string(char* dest, wchar_m* src, size_m dest_len) {
    if (dest == MU_NULL) return (size_m)mu_wcsrtombs(MU_NULL, (const wchar_m**)&src, 1, MU_NULL);
    return (size_m)mu_wcstombs(dest, src, dest_len);
}
MUDEF int mu_string_to_wstring(wchar_m* dest, char* src, size_m dest_len) {
    if (dest == MU_NULL) return (size_m)mu_mbsrtowcs(MU_NULL, (const char**)&src, 1, MU_NULL);
    return (size_m)mu_mbstowcs(dest, src, dest_len);
}

MUDEF size_m mu_string_strlen(muString s) {
    return s.len;
}

MUDEF muString mu_string_create(char* s) {
    muString str;
    str.len = mu_strlen(s);
    str.size = (sizeof(char) * (str.len)) * 2;
    str.s = mu_malloc(str.size);
    str.ws = 0;
    str.type = MU_STRING_TYPE_CHAR;
    for (size_m i = 0; i < str.len; i++) {
        str.s[i] = s[i];
    }
    str.s[str.len] = '\0';
    return str;
}
MUDEF muString mu_wstring_create(wchar_m* ws) {
    muString str;
    str.len = mu_wstrlen(ws);
    str.size = (sizeof(wchar_m) * (str.len)) * 2;
    str.ws = mu_malloc(str.size);
    str.s = 0;
    str.type = MU_STRING_TYPE_WCHAR;
    for (size_m i = 0; i < str.len; i++) {
        str.ws[i] = ws[i];
    }
    str.ws[str.len] = '\0';
    return str;
}

MUDEF muString mu_string_destroy(muString str) {
    if (str.s) mu_free(str.s);
    str.s = 0;
    if (str.ws) mu_free(str.ws);
    str.ws = 0;
    return str;
}

MUDEF muString mu_string_size_check(muString str, size_m size) {
    muBool resized = MU_FALSE;
    while (str.size < size) {
        str.size *= 2;
        resized = MU_TRUE;
    }
    if (resized == MU_TRUE) {
        if (str.type == MU_STRING_TYPE_CHAR) {
            str.s = mu_realloc(str.s, str.size);
        } else {
            str.ws = mu_realloc(str.ws, str.size);
        }
    }
    return str;
}

MUDEF muBool mu_here(char* str, char* check, size_m i) {
    if (str[i] != check[0]) return MU_FALSE;
    size_m check_len = mu_strlen(check);
    size_m str_len = mu_strlen(str);
    for (size_m j = 0; j < check_len; j++) {
        if (i+j >= str_len || str[i+j] != check[j]) {
            return MU_FALSE;
        }
    }
    return MU_TRUE;
}
MUDEF muBool mu_where(wchar_m* str, wchar_m* check, size_m i) {
    if (str[i] != check[0]) return MU_FALSE;
    size_m check_len = mu_wstrlen(check);
    size_m str_len = mu_wstrlen(str);
    for (size_m j = 0; j < check_len; j++) {
        if (i+j >= str_len || str[i+j] != check[j]) {
            return MU_FALSE;
        }
    }
    return MU_TRUE;
}

MUDEF muBool mu_has(char* str, char* find, size_m beg, size_m end) {
    size_m find_len = mu_strlen(find);
    for (size_m i = beg; i + find_len < end; i++) {
        muBool found = MU_TRUE;
        for (size_m j = 0; j < find_len; j++) {
            if (str[i+j] != find[j]) found = MU_FALSE;
        }
        if (found == MU_TRUE) return MU_TRUE;
    }
    return MU_FALSE;
}
MUDEF muBool mu_whas(wchar_m* str, wchar_m* find, size_m beg, size_m end) {
    size_m find_len = mu_wstrlen(find);
    for (size_m i = beg; i + find_len < end; i++) {
        muBool found = MU_TRUE;
        for (size_m j = 0; j < find_len; j++) {
            if (str[i+j] != find[j]) found = MU_FALSE;
        }
        if (found == MU_TRUE) return MU_TRUE;
    }
    return MU_FALSE;
}

MUDEF muString mu_string_delete(muString str, size_m beg, size_m end) {
    for (size_m i = end; i < mu_string_strlen(str) + 1; i++) {
        if (str.type == MU_STRING_TYPE_CHAR) {
            str.s[i-(end-beg)] = str.s[i];
        } else {
            str.ws[i-(end-beg)] = str.ws[i];
        }
    }
    str.len -= (end - beg);
    return str;
}

MUDEF muString mu_string_insert(muString str, char* insert, size_m i) {
    if (str.type == MU_STRING_TYPE_WCHAR) {
        size_m size = mu_string_to_wstring(MU_NULL, insert, 0);
        wchar_m* insert_w = mu_malloc(size * sizeof(wchar_m));
        mu_string_to_wstring(insert_w, insert, size);
        str = mu_wstring_insert(str, insert_w, i);
        mu_free(insert_w);
        return str;
    }
    size_m insert_len = mu_strlen(insert);
    str = mu_string_size_check(str, sizeof(char) * (mu_string_strlen(str) + insert_len + 1));
    for (size_m j = mu_strlen(str.s); i < j+1; j--) {
        if (str.type == MU_STRING_TYPE_CHAR) {
            str.s[j+insert_len] = str.s[j];
        } else {
            str.ws[j+insert_len] = str.ws[j];
        }
    }
    for (size_m j = 0; j < insert_len; j++) {
        if (str.type == MU_STRING_TYPE_CHAR) {
            str.s[i+j] = insert[j];
        } else {
            str.ws[i+j] = insert[j];
        }
    }
    str.len += insert_len;
    return str;
}
MUDEF muString mu_wstring_insert(muString str, wchar_m* insert, size_m i) {
    if (str.type == MU_STRING_TYPE_CHAR) {
        size_m size = mu_wstring_to_string(MU_NULL, insert, 0);
        char* insert_c = mu_malloc(size * sizeof(char));
        mu_wstring_to_string(insert_c, insert, size);
        str = mu_string_insert(str, insert_c, i);
        mu_free(insert_c);
        return str;
    }
    size_m insert_len = mu_wstrlen(insert);
    str = mu_string_size_check(str, sizeof(wchar_m) * (mu_string_strlen(str) + insert_len + 1));
    for (size_m j = mu_string_strlen(str); i < j+1; j--) {
        if (str.type == MU_STRING_TYPE_CHAR) {
            str.s[j+insert_len] = str.s[j];
        } else {
            str.ws[j+insert_len] = str.ws[j];
        }
    }
    for (size_m j = 0; j < insert_len; j++) {
        if (str.type == MU_STRING_TYPE_CHAR) {
            str.s[i+j] = insert[j];
        } else {
            str.ws[i+j] = insert[j];
        }
    }
    str.len += insert_len;
    return str;
}

MUDEF muString mu_string_insert_integer(muString str, int64_m n, size_m i) {
    size_m len = mu_integer_to_string(n, MU_NULL, 0);
    len++;
    char* s = mu_malloc(len * sizeof(char));
    mu_integer_to_string(n, s, len);
    s[len-1] = 0;
    str = mu_string_insert(str, s, i);
    mu_free(s);
    return str;
}
MUDEF muString mu_wstring_insert_integer(muString str, int64_m n, size_m i) {
    size_m len = mu_integer_to_wstring(n, MU_NULL, 0);
    len++;
    wchar_m* s = mu_malloc(len * sizeof(wchar_m));
    mu_integer_to_wstring(n, s, len);
    s[len-1] = 0;
    str = mu_wstring_insert(str, s, i);
    mu_free(s);
    return str;
}

MUDEF muString mu_string_insert_float(muString str, double n, size_m max_decimals, size_m i) {
    size_m len = mu_float_to_string(n, max_decimals, MU_NULL, 0);
    len++;
    char* s = mu_malloc(len * sizeof(char));
    mu_float_to_string(n, max_decimals, s, len);
    s[len-1] = 0;
    str = mu_string_insert(str, s, i);
    mu_free(s);
    return str;
}
MUDEF muString mu_wstring_insert_float(muString str, double n, size_m max_decimals, size_m i) {
    size_m len = mu_float_to_wstring(n, max_decimals, MU_NULL, 0);
    len++;
    wchar_m* s = mu_malloc(len * sizeof(wchar_m));
    mu_float_to_wstring(n, max_decimals, s, len);
    s[len-1] = 0;
    str = mu_wstring_insert(str, s, i);
    mu_free(s);
    return str;
}

MUDEF muString mu_string_replace(muString str, char* find, char* replace, size_m beg, size_m end) {
    if (str.type == MU_STRING_TYPE_WCHAR) {
        size_m find_size = mu_string_to_wstring(MU_NULL, find, 0);
        size_m replace_size = mu_string_to_wstring(MU_NULL, replace, 0);
        wchar_m* find_w = mu_malloc(find_size * sizeof(wchar_m));
        wchar_m* replace_w = mu_malloc(replace_size * sizeof(wchar_m));
        mu_string_to_wstring(find_w, find, find_size);
        mu_string_to_wstring(replace_w, replace, replace_size);
        str = mu_wstring_replace(str, find_w, replace_w, beg, end);
        mu_free(find_w);
        mu_free(replace_w);
        return str;
    }
    size_m find_len = mu_strlen(find);
    size_m replace_len = mu_strlen(replace);
    size_m len_dif = find_len - replace_len;
    if (replace_len > find_len) len_dif = replace_len - find_len;
    for (size_m i = beg; i < end + 1 && i < mu_string_strlen(str); i++) {
        if (mu_here(str.s, find, i) == MU_TRUE) {
            str = mu_string_delete(str, i, i + find_len);
            if (replace_len > 0) {
                str = mu_string_insert(str, replace, i);
            }
            end -= len_dif;
        }
    }
    str.len = mu_strlen(str.s);
    return str;
}
MUDEF muString mu_wstring_replace(muString str, wchar_m* find, wchar_m* replace, size_m beg, size_m end) {
    if (str.type == MU_STRING_TYPE_CHAR) {
        size_m find_size = mu_wstring_to_string(MU_NULL, find, 0);
        size_m replace_size = mu_wstring_to_string(MU_NULL, replace, 0);
        char* find_c = mu_malloc(find_size * sizeof(char));
        char* replace_c = mu_malloc(replace_size * sizeof(char));
        mu_wstring_to_string(find_c, find, find_size);
        mu_wstring_to_string(replace_c, replace, replace_size);
        str = mu_string_replace(str, find_c, replace_c, beg, end);
        mu_free(find_c);
        mu_free(replace_c);
        return str;
    }
    size_m find_len = mu_wstrlen(find);
    size_m replace_len = mu_wstrlen(replace);
    size_m len_dif = find_len - replace_len;
    if (replace_len > find_len) len_dif = replace_len - find_len;
    for (size_m i = beg; i < end + 1 && i < mu_string_strlen(str); i++) {
        if (mu_where(str.ws, find, i) == MU_TRUE) {
            str = mu_string_delete(str, i, i + find_len);
            if (replace_len > 0) {
                str = mu_wstring_insert(str, replace, i);
            }
            end -= len_dif;
        }
    }
    str.len = mu_wstrlen(str.ws);
    return str;
}

MUDEF muString mu_string_to_lowercase(muString str, size_m beg, size_m end) {
    for (size_m i = beg; i < str.len && i < end + 1; i++) {
        if (str.type == MU_STRING_TYPE_CHAR) {
            str.s[i] = mu_char_to_lowercase(str.s[i]);
        } else {
            str.ws[i] = mu_wchar_to_lowercase(str.ws[i]);
        }
    }
    return str;
}
MUDEF muString mu_string_to_uppercase(muString str, size_m beg, size_m end) {
    for (size_m i = beg; i < str.len && i < end + 1; i++) {
        if (str.type == MU_STRING_TYPE_CHAR) {
            str.s[i] = mu_char_to_uppercase(str.s[i]);
        } else {
            str.ws[i] = mu_wchar_to_uppercase(str.ws[i]);
        }
    }
    return str;
}

// did this manually because C standards suck
// last updated 16 September 2023, covers 0 -> 65533
// https://en.wikipedia.org/wiki/List_of_Unicode_characters
// https://www.ssec.wisc.edu/~tomw/java/unicode.html

MUDEF char mu_char_to_lowercase(char c) {
    if (c >= 65 && c <= 90) c += 32;
    return c;
}
MUDEF wchar_m mu_wchar_to_lowercase(wchar_m c) {
    if (
    // latin alphabet
        (c >= 65 && c <= 90) ||
    // latin-1 supplement
        (c >= 192 && c <= 222 && c != 215) ||
    // greek and coptic
        (c >= 913 && c <= 939) ||
    // cyrillic
        (c >= 0x0410 && c <= 0x042F) ||
    // halfwidth and fullwidth forms
        (c >= 65313 && c <= 65338)
    ) {
        return c + 32;
    } else if (
    // latin extended-a
        (
            (c >= 256 && c <= 310 && c % 2 == 0) || 
            (c >= 313 && c <= 327 && c % 2 != 0) ||
            (c >= 330 && c <= 376 && c % 2 == 0) ||
            (c >= 377 && c <= 381 && c % 2 != 0)
        ) ||
    // latin extended-b
        (
            (c >= 461 && c <= 475 && c % 2 != 0) ||
            (c >= 478 && c <= 494 && c % 2 == 0) ||
            (c >= 504 && c <= 542 && c % 2 == 0) ||
            (c >= 546 && c <= 562 && c % 2 == 0) ||
            (c >= 582 && c <= 590 && c % 2 == 0)
        ) ||
    // greek and coptic
        (
            (c >= 984 && c <= 1006 && c % 2 == 0)
        ) ||
    // cyrillic
        (
            (c >= 0x0460 && c <= 0x0480 && c % 2 == 0) ||
            (c >= 0x048A && c <= 0x04BE && c % 2 == 0) ||
            (c >= 0x04C1 && c <= 0x04CD && c % 2 != 0) ||
            (c >= 0x04D0 && c <= 0x04FE && c % 2 == 0) ||
            // cyrillic supplement
            (c >= 0x0500 && c <= 0x052E && c % 2 == 0)
        ) ||
    // latin extended additional
        (
            (c >= 7680 && c <= 7828 && c % 2 == 0) ||
            (c >= 7840 && c <= 7928 && c % 2 == 0)
        )
    ) {
        return c + 1;
    } else if (
    // cyrillic
        (
            (c >= 0x0400 && c <= 0x040F)
        )
    ) {
        return c + 80;
    } else if (
    // armenian
        (
            (c >= 0x0530 && c <= 0x0558)
        ) ||
    // georgian
        (
            (c >= 0x10A0 && c <= 0x10CF)
        )
    ) {
        return c + 48;
    } else if (
    // greek extended
        (
            (c >= 7944 && c <= 7951) || (c >= 7960 && c <= 7965) || (c >= 7976 && c <= 7983) || (c >= 7992 && c <= 7999) ||
            (c >= 8008 && c <= 8013) || (c >= 8025 && c <= 8031) || (c >= 8040 && c <= 8047) || (c >= 8072 && c <= 8079) ||
            (c >= 8088 && c <= 8095) || (c >= 8104 && c <= 8111) || (c >= 8120 && c <= 8124)
        )
    ) {
        return c - 8;
    } else if (
    // enclosed alphanumerics
        (
            (c >= 9398 && c <= 9423)
        )
    ) {
        return c + 26;
    }
    switch (c) {
    default: break;
    // odd latin extended-b / ipa extensions
    case 386: case 388: case 391: case 395: case 401: case 408: case 416: case 418: case 420: case 423: case 428: case 431: 
    case 435: case 437: case 440: case 444: case 453: case 456: case 459: case 498: case 500: case 571: case 577: return c+1; break;
    case 384: return 579; break;
    case 385: return 595; break;
    case 390: return 596; break;
    case 393: return 598; break;
    case 394: return 599; break;
    case 398: return 600; break;
    case 399: return 601; break;
    case 400: return 603; break;
    case 403: return 608; break;
    case 404: return 611; break;
    case 406: return 617; break;
    case 407: return 616; break;
    case 412: return 623; break;
    case 413: return 626; break;
    case 425: return 643; break;
    case 430: return 648; break;
    case 433: return 650; break;
    case 434: return 641; break;
    case 439: return 658; break;
    case 452: return 454; break;
    case 455: return 457; break;
    case 458: return 460; break;
    case 497: return 499; break;
    case 544: return 414; break;
    case 573: return 410; break;
    case 579: return 384; break;
    case 580: return 649; break;
    case 581: return 652; break;

    // odd greek and coptic
    case 880: case 882: case 886: case 1015: case 1018: return c+1; break;
    case 895: return 1011; break;
    case 904: case 905: case 906: return c+37; break;
    case 908: case 910: case 911: return c+64; break;
    case 975: return 983; break;
    case 1012: return 977; break;
    case 1017: return 1010; break;
    case 1021: case 1022: case 1023: return c-130; break;

    // odd greek extended
    // this is so unsorted it makes my room look like a masterpiece
    case 8136: case 8137: case 8138: case 8139: return c-86; break;
    case 8140: return 8131; break;
    case 8152: return 8144; break;
    case 8153: return 8145; break;
    case 8154: case 8155: return c-100; break;
    case 8168: return 8160; break;
    case 8169: return 8161; break;
    case 8170: return 8058; break;
    case 8171: return 8059; break;
    case 8172: return 8165; break;
    case 8184: case 8185: return c-128; break;
    case 8187: return 8061; break;
    }
    return c;
}
MUDEF char mu_char_to_uppercase(char c) {
    if (c >= 97 && c <= 122) c -= 32;
    return c;
}
MUDEF wchar_m mu_wchar_to_uppercase(wchar_m c) {
    if (
    // latin alphabet
        (c >= 97 && c <= 122) ||
    // latin-1 supplement
        (c >= 224 && c <= 255 && c != 247) ||
    // greek and coptic
        (c >= 945 && c <= 971) ||
    // cyrillic
        (c >= (0x0410 + 32) && c <= (0x042F + 32)) ||
    // halfwidth and fullwidth forms
        (c >= (65313 + 32) && c <= (65338 + 32))
    ) {
        return c - 32;
    } else if (
    // latin extended-a
        (
            (c >= 257 && c <= 311 && c % 2 != 0) || 
            (c >= 312 && c <= 328 && c % 2 == 0) ||
            (c >= 329 && c <= 375 && c % 2 != 0) ||
            (c >= 378 && c <= 382 && c % 2 == 0)
        ) ||
    // latin extended-b
        (
            (c >= 462 && c <= 476 && c % 2 == 0) ||
            (c >= 479 && c <= 495 && c % 2 != 0) ||
            (c >= 505 && c <= 543 && c % 2 != 0) ||
            (c >= 547 && c <= 563 && c % 2 != 0) ||
            (c >= 583 && c <= 591 && c % 2 != 0)
        ) ||
    // greek and coptic
        (
            (c >= 985 && c <= 1007 && c % 2 != 0)
        ) ||
    // cyrillic
        (
            (c >= 0x0461 && c <= 0x0481 && c % 2 != 0) ||
            (c >= 0x048B && c <= 0x04BF && c % 2 != 0) ||
            (c >= 0x04C2 && c <= 0x04CE && c % 2 == 0) ||
            (c >= 0x04D1 && c <= 0x04FF && c % 2 != 0) ||
            // cyrillic supplement
            (c >= 0x0501 && c <= 0x052F && c % 2 != 0)
        ) ||
    // latin extended additional
        (
            (c >= 7681 && c <= 7829 && c % 2 != 0) ||
            (c >= 7841 && c <= 7929 && c % 2 != 0)
        )
    ) {
        return c - 1;
    } else if (
    // cyrillic
        (
            (c >= (0x0400+80) && c <= (0x040F+80))
        )
    ) {
        return c - 80;
    } else if (
    // armenian
        (
            (c >= (0x0530+48) && c <= (0x0558+48))
        ) ||
    // georgian
        (
            (c >= (0x10A0+48) && c <= (0x10CF+48))
        )
    ) {
        return c - 48;
    } else if (
    // greek extended
        (
            (c >= (7944-8) && c <= (7951-8)) || (c >= (7960-8) && c <= (7965-8)) || (c >= (7976-8) && c <= (7983-8)) || (c >= (7992-8) && c <= (7999-8)) ||
            (c >= (8008-8) && c <= (8013-8)) || (c >= (8025-8) && c <= (8031-8)) || (c >= (8040-8) && c <= (8047-8)) || (c >= (8072-8) && c <= (8079-8)) ||
            (c >= (8088-8) && c <= (8095-8)) || (c >= (8104-8) && c <= (8111-8)) || (c >= (8120-8) && c <= (8124-8))
        )
    ) {
        return c + 8;
    } else if (
    // enclosed alphanumerics
        (
            (c >= (9398+26) && c <= (9423+26))
        )
    ) {
        return c - 26;
    }
    switch (c) {
    default: break;
    // odd latin extended-b / ipa extensions
    case 387: case 389: case 392: case 396: case 402: case 409: case 417: case 419: case 421: case 424: case 429: case 432: 
    case 436: case 438: case 441: case 445: case 453: case 456: case 459: case 498: case 501: case 572: case 578: return c-1; break;
    case 579: return 384; break;
    case 595: return 385; break;
    case 596: return 390; break;
    case 598: return 393; break;
    case 599: return 394; break;
    case 600: return 398; break;
    case 601: return 399; break;
    case 603: return 400; break;
    case 608: return 403; break;
    case 611: return 404; break;
    case 617: return 406; break;
    case 616: return 407; break;
    case 623: return 412; break;
    case 626: return 413; break;
    case 643: return 425; break;
    case 648: return 430; break;
    case 650: return 433; break;
    case 641: return 434; break;
    case 658: return 439; break;
    case 454: return 452; break;
    case 457: return 455; break;
    case 460: return 458; break;
    case 499: return 497; break;
    case 414: return 544; break;
    case 410: return 573; break;
    case 384: return 579; break;
    case 649: return 580; break;
    case 652: return 581; break;

    // odd greek and coptic
    case 881: case 883: case 887: case 1016: case 1019: return c-1; break;
    case 1011: return 895; break;
    case 941: case 942: case 943: return c-37; break;
    case 972: case 974: case 975: return c-64; break;
    case 983: return 975; break;
    case 977: return 1012; break;
    case 1010: return 1017; break;
    case 891: case 892: case 893: return c+130; break;

    // odd greek extended
    // this is so unsorted it makes my room look like a masterpiece
    case 8050: case 8051: case 8052: case 8053: return c+86; break;
    case 8131: return 8140; break;
    case 8144: return 8152; break;
    case 8145: return 8153; break;
    case 8054: case 8055: return c+100; break;
    case 8160: return 8168; break;
    case 8161: return 8169; break;
    case 8058: return 8170; break;
    case 8059: return 8171; break;
    case 8165: return 8172; break;
    case 8056: case 8057: return c+128; break;
    case 8061: return 8187; break;
    }
    return c;
}

// these funcs aren't entirely necessary
MUDEF muBool mu_char_is_lowercase(char c) {
    return c != mu_char_to_uppercase(c);
}
MUDEF muBool mu_wchar_is_lowercase(wchar_m c) {
    return c != mu_wchar_to_uppercase(c);
}
MUDEF muBool mu_char_is_uppercase(char c) {
    return c != mu_char_to_lowercase(c);
}
MUDEF muBool mu_wchar_is_uppercase(wchar_m c) {
    return c != mu_wchar_to_lowercase(c);
}

MUDEF int mu_integer_to_string(int64_m n, char* dest, size_m dest_len) {
    if (dest == MU_NULL) return mu_snprintf(MU_NULL, 0, "%ld", (long)n);
    return mu_snprintf(dest, dest_len+1, "%ld", (long)n);
}

// would like to not have to convert here, but snwprintf isn't standard
MUDEF int mu_integer_to_wstring(int64_m n, wchar_m* dest, size_m dest_len) {
    if (dest == MU_NULL) return mu_snprintf(MU_NULL, 0, "%ld", (long)n);
    size_m len = mu_integer_to_string(n, MU_NULL, 0);
    char* s = mu_malloc(len * sizeof(char));
    mu_integer_to_string(n, s, len);
    for (size_m i = 0; i < dest_len; i++) {
        dest[i] = (wchar_m)s[i];
    }
    free(s);
    return 0;
}

MUDEF int mu_float_to_string(double n, size_m max_decimals, char* dest, size_m dest_len) {
    if (max_decimals >= 10) max_decimals = 9;
    char format[7] = "%0.0lf\0";
    format[3] = '0' + max_decimals;
    if (dest == MU_NULL) return mu_snprintf(MU_NULL, 0, format, n);
    return mu_snprintf(dest, dest_len+1, format, n);
}

MUDEF int mu_float_to_wstring(double n, size_m max_decimals, wchar_m* dest, size_m dest_len) {
    size_m len = mu_float_to_string(n, max_decimals, MU_NULL, 0);
    if (dest == MU_NULL) return len;
    char* s = mu_malloc(len * sizeof(char));
    mu_float_to_string(n, max_decimals, s, len);
    for (size_m i = 0; i < dest_len; i++) {
        dest[i] = (wchar_m)s[i];
    }
    mu_free(s);
    return 0;
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
Copyright (c) 2023 Hum
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
