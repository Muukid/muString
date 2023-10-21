
# muString
muString, abbreviated as "mus" (in which case is pronounced 'moose') (which is also what this guide will refer to it as for the sake of simplicity), is a public domain simple single-file C library for easy string allocation and manipulation. To use it, simply download the `muString.h` file, add it to your include path, and include it like so:
```
#define MUS_IMPLEMENTATION
#include "muString.h"
```
Note that *not* defining `MUS_IMPLEMENTATION` will define the functions and structs but will not give an implementation for the functions, meaning that you can include it across multiple files and define `MUS_IMPLEMENTATION` once in your main file.

# Licensing
mus is licensed under public domain or MIT, whichever you prefer. More information is provided in the accompanying file `license.md` and at the bottom of `muString.h`.

# Macros

## Version
Near the top of the file, the version of mus is defined with `MUS_VERSION_MAJOR`, `MUS_VERSION_MINOR`, and `MUS_VERSION_PATCH`. These versions match the releases on the GitHub page and can be used to check if the correct version of mus is running in a program.

## Static/Extern defines
Defining `MU_STATIC` before including `muString.h` will set all the definitions of the functions to `static`. Otherwise, it defaults to `extern`. Additionally, you can define `MUDEF` directly to have full control over its definition.

## Defines
There are a few defines that mus uses that rely on the C standard library to define themselves, all of which can be defined by the user before defining `MUS_IMPLEMENTATION` if they wish to customize them. These are:

`int64_m`: equivalent to `int64_t`, uses `stdint.h` for definition.

`size_m` : equivalent to `size_t`, uses `stddef.h` for definition.

`wchar_m`: equivalent to `wchar_t`, uses `wchar.h` for definition.

`mu_malloc`: equivalent to `malloc`, uses `stdlib.h` for definition.

`mu_free`: equivalent to `free`, uses `stdlib.h` for definition.

`mu_realloc`: equivalent to `realloc`, uses `stdlib.h` for definition.

`mu_mbstowcs`: equivalent to `mbstowcs`, uses `stdlib.h` for definition.

`mu_mbsrtowcs`: equivalent to `mbsrtowcs`, uses `stdlib.h` for definition.

`mu_wcstombs`: equivalent to `wcstombs`, uses `stdlib.h` for definition.

`mu_wcsrtombs`: equivalent to `wcsrtombs`, uses `stdlib.h` for definition.

`mu_strlen`: equivalent to `strlen`, uses `string.h` for definition.

`mu_wstrlen`: equivalent to `wcslen`, uses `wchar.h` for definition.

`mu_snprintf`: equivalent to `snprintf`, uses `stdio.h` for definition.

Note that if all definitions for a C standard header file are defined before `MUS_IMPLEMENTATION`, it will not be included. For example, if `mu_snprintf` was defined beforehand, `stdio.h` will not be included, as no definitions would be reliant on `stdio.h` being included.

# Enumerators

## `muBool`
The enumerator `muBool` is used as a generic boolean type, defined below in `MUS_H`:

```
typedef enum { MU_FALSE, MU_TRUE } muBool;
```

## `muStringType`
The enumerator `muStringType` is used to define whether or not a string is using the data type `char` or `wchar_m`, defined below in `MUS_H`:

```
typedef enum { MU_STRING_TYPE_CHAR, MU_STRING_TYPE_WCHAR } muStringType;
```

# Structs

## muString
The struct `muString` is the struct that most of the functionality of the mus API is built upon, defined below in `MUS_H`:

```
typedef struct {
    char* s;
    wchar_m* ws;
    muStringType type;
    size_m size;
    size_m len;
} muString;
```

`char* s` is used to hold the pointer to the `char` text that the struct is holding if the value of `type` is `MU_STRING_TYPE_CHAR`.
`wchar_m* ws` is used to hold the pointer to the `wchar_m` text that the struct is holding if the value of `type` is `MU_STRING_TYPE_WCHAR`.
`size` is used to hold the allocated amount of data for the string that it's holding.
`len` is used to hold the amount of characters in use for the string that it's holding.

Note that mus allocates twice the amount of space than requested and then doubles it every time that it needs to reallocate more data, meaning that `size` and `len` never match and cannot be used interchangeably.

# muString functions

## String length
There are three functions to check the length of a string. If you're using a `muString` struct, you should always use the function `mu_string_strlen`, defined below in `MUS_H`:

```
MUDEF size_m mu_string_strlen(muString s);
```

All it does is return `s.len`, because `muString` internally stores the length of the string, meaning that you can access it immediately without having to scan the string. You can also just access the variable `len` in the `muString` struct. To be clear, ***use `mu_string_strlen` whenever you can.*** This is because other methods, like using the `strlen` function, can differ heavily in performance on certain operating systems, particularly on Windows. It is also agnostic as to whether or not a string is using `char`s or `wchar_m`s.

If you have a raw `char` pointer string to check the length of, you can use `mu_strlen`, which is simply a define for `strlen` provided in `string.h` unless defined otherwise by the user.

If you have a raw `wchar_m` pointer string to check the length of, you can use `mu_wstrlen`, which is simply a define for `wcslen` provided in `wchar.h` unless defined otherwise by the user.

## String creation
There are two functions to create a string. If you'd like to create a string that uses `char` for its datatype, use `mu_string_create`, defined below in `MUS_H`:

```
MUDEF muString mu_string_create (char* s);
```

The function takes in a preexisting `char` pointer string and returns a valid `muString` struct which can then be used with other functions.

If you'd like to create a string that uses `wchar_m` for its datatype, use `mu_wstring_create`, defined below in `MUS_H`:

```
MUDEF muString mu_wstring_create(wchar_m* ws);
```

The function takes in a preexisting `wchar_m` pointer and returns a valid `muString` struct which can then be used with other functions.

Note that if you use `mu_string_create`, the variable `s` of type `char*` is used to store the string, whilst if you use `mu_wstring_create`, the variable `ws` of type `wchar_m*` is used.

## String destruction
To destroy a string, the function `mu_string_destroy` is used, defined below in `MUS_H`:

```
MUDEF muString mu_string_destroy(muString str);
```

This function is agnostic to whether or not the string uses `char` or `wchar_m`. It also can be safely called multiple times as long as the return value is not ignored.

Note that this function ***must*** be called once a `muString` is done being used, or else the memory will not be freed and *will* lead to memory leaks.

## Size checking
Size checking is mostly used internally within the mus functions, but can also be used by the user. The function used is `mu_string_size_check`, and its purpose is to reallocate more size for a `muString` when necessary, defined below in `MUS_H`:

```
MUDEF muString mu_string_size_check(muString str, size_m size);
```

## Deleting section of string
To delete a certain section of a string, use the function `mu_string_delete`, defined below in `MUS_H`:

```
MUDEF muString mu_string_delete(muString str, size_m beg, size_m end);
```

Deletes all characters starting at index `beg` and ending at index `end`. This function can be used for both `char` and `wchar_m` strings.

## Inserting a string into another
There are two functions used for inserting one string into another, `mu_string_insert` for `char` strings and `mu_wstring_insert` for `wchar_m` strings, both defined below in `MUS_H`:

```
MUDEF muString mu_string_insert (muString str, char* insert,    size_m i);
MUDEF muString mu_wstring_insert(muString str, wchar_m* insert, size_m i);
```

Inserts `insert` into the string contents of `str` at index `i`.

Note that `str` doesn't need to match the character type of insertion, meaning that you can use `mu_string_insert` on a wide-character string and vice versa, but it's not recommended, as it can be slow.

## Inserting an integer into a string
There are two functions used for inserting an integer into a string, `mu_string_insert_integer` for `char` strings and `mu_wstring_insert_integer` for `wchar_m` strings, both defined below in `MUS_H`:

```
MUDEF muString mu_string_insert_integer (muString str, int64_m n, size_m i);
MUDEF muString mu_wstring_insert_integer(muString str, int64_m n, size_m i);
```

Converts `n` to a string and inserts it into the string contents of `str` at index `i`.

Note that these functions are dependent on `mu_integer_to_string` and `mu_integer_to_wstring`, so limitations on `n` for those functions apply.

## Inserting a floating point number into a string
There are two functions used for inserting a floating point number into a string, `mu_string_insert_float` for `char` strings and `mu_wstring_insert_float` for `wchar_m` strings, both defined below in `MUS_H`:

```
MUDEF muString mu_string_insert_float (muString str, double n, size_m max_decimals, size_m i);
MUDEF muString mu_wstring_insert_float(muString str, double n, size_m max_decimals, size_m i);
```

Converts `n` to a string, rounding it to `max_decimals` decimals, and inserts it into the string contents of `str` at index `i`.

Note that these functions are dependent on `mu_float_to_string` and `mu_float_to_wstring`, so limitations on `n` and `max_decimals` for those functions apply.

## Replacing all instances of a string with another
There are two functions for replacing all instances of a string with another, `mu_string_replace` for `char` strings and `mu_wstring_replace` for `wchar_m` strings, both defined below in `MUS_H`:

```
MUDEF muString mu_string_replace (muString str, char* find,    char* replace,    size_m beg, size_m end);
MUDEF muString mu_wstring_replace(muString str, wchar_m* find, wchar_m* replace, size_m beg, size_m end);
```

Finds all instances of `find` within the string contents of `str` and replaces it with `replace`, scanning from index `beg` to `end` within the string contents of `str`.

Note that `str` doesn't need to match the character type of insertion, meaning that you can use `mu_string_replace` on a wide-character string and vice versa, but it's not recommended, as it can be slow.

## Lowercase/Uppercase string conversion
There are two functions for converting a string to lowercase or uppercase, `mu_string_to_lowercase` and `mu_string_to_uppercase`, both defined below in `MUS_H`:

```
MUDEF muString mu_string_to_lowercase(muString str, size_m beg, size_m end);
MUDEF muString mu_string_to_uppercase(muString str, size_m beg, size_m end);
```

Converts all characters from index `beg` to index `end` to lowercase/uppercase.

Note that these functions can be called with any type of `muString`.

Note that these functions have reliance on `mu_char_to_lowercase`, `mu_wchar_to_lowercase`, `mu_char_to_uppercase`, and `mu_wchar_to_uppercase`, meaning that the uppercase/lowercase conversion logic is done using said functions, and restrictions on ASCII/Unicode standards apply according to said functions.

# String functions

## Here check
To check if a given string's contents is at a given point in another string, you can use the two functions `mu_here` and `mu_where` for `char` strings and `wchar_m` strings respectively, both defined below in `MUS_H`:

```
MUDEF muBool  mu_here (char* str,    char* check,    size_m i);
MUDEF muBool  mu_where(wchar_m* str, wchar_m* check, size_m i);
```

Returns `MU_TRUE` if all characters at index `i` in `str` match the characters contained in `check`.

## Has check
To check if a given string's contents are contained somewhere within another string, you can use the two functions `mu_has` and `mu_whas` for `char` and `wchar_m` strings respectively, both defined below in `MUS_H`:

```
MUDEF muBool mu_has (char* str,    char* find,    size_m beg, size_m end);
MUDEF muBool mu_whas(wchar_m* str, wchar_m* find, size_m beg, size_m end);
```

Returns `MU_TRUE` if the contents of `find` are contained somewhere within `str` from index `beg` to index `end`.

# Character conversion

## `char` to `wchar_m` and vice versa
There are two functions for converting `char` strings to `wchar_m` strings and `wchar_m` strings to `char` strings, `mu_wstring_to_string` and `mu_string_to_wstring` respectively, both defined below in `MUS_H`:

```
MUDEF int mu_wstring_to_string(char* dest,    wchar_m* src, size_m dest_len);
MUDEF int mu_string_to_wstring(wchar_m* dest, char* src,    size_m dest_len);
```

Converts `src` to opposite character type and stores the result in `dest` if `dest_len` is long enough to store it. Returns -1 if conversion failed or returns the minimum length necessary to fully convert `src` if `dest` is `MU_NULL`.

# Lowercase/Uppercase functions

## Lowercase/Uppercase character conversion
There are four functions for converting a character to lowercase or uppercase, `mu_char_to_lowercase`, `mu_wchar_to_lowercase`, `mu_char_to_uppercase`, and `mu_wchar_to_uppercase`, all four defined below in `MUS_H`:

```
MUDEF char    mu_char_to_lowercase (char c);
MUDEF wchar_m mu_wchar_to_lowercase(wchar_m c);

MUDEF char    mu_char_to_uppercase (char c);
MUDEF wchar_m mu_wchar_to_uppercase(wchar_m c);
```

Converts the given character to lowercase/uppercase and returns the result.

Note that `mu_char_to_lowercase` and `mu_char_to_uppercase` uses normal ASCII logic (-128 to 127), but `mu_wchar_to_lowercase` and `mu_wchar_to_uppercase` use the Unicode standard and cover code points 0 to 65,533.

## Lowercase/Uppercase character check
There are four functions to see if a character is lowercase or uppercase, `mu_char_is_lowercase`, `mu_wchar_is_lowercase`, `mu_char_is_uppercase`, and `mu_wchar_is_uppercase`, all four defined below in `MUS_H`:

```
MUDEF muBool  mu_char_is_lowercase (char c);
MUDEF muBool  mu_wchar_is_lowercase(wchar_m c);

MUDEF muBool  mu_char_is_uppercase (char c);
MUDEF muBool  mu_wchar_is_uppercase(wchar_m c);
```

Note that each function will return `MU_FALSE` if the given character is neither uppercase nor lowercase.

Note that these functions have reliance on `mu_char_is_lowercase`, `mu_wchar_is_lowercase`, `mu_char_is_uppercase`, and `mu_wchar_is_uppercase`, meaning that the uppercase/lowercase conversion logic is done using said functions, and restrictions on ASCII/Unicode standards apply according to said functions.

# Number to string functions

## Integer to string conversion
There are two functions to convert an integer to a string, `mu_integer_to_string` for `char` strings and `mu_integer_to_wstring` for `wchar_m` strings, both defined below in `MUS_H`:

```
MUDEF int   mu_integer_to_string (int64_m n, char* dest,    size_m dest_len);
MUDEF int   mu_integer_to_wstring(int64_m n, wchar_m* dest, size_m dest_len);
```

Converts `n` to a string and stores the result in `dest` if `dest_len` is long enough to store it. Returns -1 if conversion failed or returns the minimum length necessary to fully convert `n` if `dest` is `MU_NULL`.

## Floating point number to string conversion
There are two functions to convert a floating point number to a string, `mu_float_to_string` for `char` strings and `mu_float_to_wstring` for `wchar_m` strings, both defined below in `MUS_H`:

```
MUDEF int   mu_float_to_string (double n, size_m max_decimals, char* dest,    size_m dest_len);
MUDEF int   mu_float_to_wstring(double n, size_m max_decimals, wchar_m* dest, size_m dest_len);
```

Converts `n` to a string and stores the result in `dest` with less decimals than `max_decimals` if `dest_len` is long enough to store it. Returns -1 if conversion failed or returns the minimum length necessary to fully convert `n` if `dest` is `MU_NULL`.
