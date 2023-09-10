
# mus
mus, pronounced "moose", is a public domain simple single-file C library for easy string allocation and manipulation. To use it, simply download the `mus.h` file, add it to your include path, and include it like so:
```
#define MUS_IMPLEMENTATION
#include "mus.h"
```
Note that *not* defining `MUS_IMPLEMENTATION` will define the functions and structs but will not give an implementation for the functions, meaning that you can include it across multiple files and define `MUS_IMPLEMENTATION` once in your main file.

# Licensing
mus is licensed under public domain or MIT, whichever you prefer. More information is provided in the accompanying file `license.md` and at the bottom of `mus.h`.

# Why single-file headers/public domain?
See the **Why single-file headers?** section within the **FAQ** section of the stb repository: https://github.com/nothings/stb/blob/master/README.md Really, I just needed a quick and easy-to-use drag-and-drop library to manipulate strings with few allocation calls, and I had a hard time finding one. Also linking libraries sucks.

# Macros

## Version
Near the top of the file, the version of mus is defined with `MUS_VERSION_MAJOR`, `MUS_VERSION_MINOR`, and `MUS_VERSION_PATCH`. These versions match the releases on the GitHub page and can be used to check if the correct version of mus is running in a program.

## Static/Extern defines
Whether or not mus defines its functions as static or extern can be controlled by defining `MUSDEF` before including the file.

## Defines
There are a few defines that mus uses that rely on the C standard library to define themselves, all of which can be defined by the user before including the file if they wish to customize them. These are:

`size_m` : equivalent to `size_t`, uses `stddef.h` for definition.
`wchar_m`: equivalent to `wchar_t`, uses `wchar.h` for definition.
`mus_malloc`: equivalent to `malloc`, uses `stdlib.h` for definition.
`mus_free`: equivalent to `free`, uses `stdlib.h` for definition.
`mus_realloc`: equivalent to `realloc`, uses `stdlib.h` for definition.
`mus_strlen`: equivalent to `strlen`, uses `string.h` for definition.
`mus_wstrlen`: equivalent to `wcslen`, uses `wchar.h` for definition.

# Enumerators

## `MUS_BOOL`
The enumerator `MUS_BOOL` is used as a generic boolean type, defined below in `MUS_H`:

```
typedef enum { MUS_FALSE, MUS_TRUE } MUS_BOOL;
```

## `MUS_STRING_TYPE`
The enumerator `MUS_STRING_TYPE` is used to define whether or not a string is using the data type `char` or `wchar_m`, defined below in `MUS_H`:

```
typedef enum { MUS_STRING_TYPE_CHAR, MUS_STRING_TYPE_WCHAR } MUS_STRING_TYPE;
```

# Structs

## mustring
The struct `mustring` is the struct that most of the functionality of the mus API is built upon, defined below in `MUS_H`:

```
typedef struct {
    char* s;
    wchar_m* ws;
    MUS_STRING_TYPE type;
    size_m size;
    size_m len;
} mustring;
```

`char* s` is used to hold the pointer to the `char` text that the struct is holding if the value of `type` is `MUS_STRING_TYPE_CHAR`.
`wchar_m* ws` is used to hold the pointer to the `wchar_m` text that the struct is holding if the value of `type` is `MUS_STRING_TYPE_WCHAR`.
`size` is used to hold the allocated amount of data for the string that it's holding.
`len` is used to hold the amount of characters in use for the string that it's holding.

Note that mus allocates twice the amount of space than requested and then doubles it every time that it needs to reallocate more data, meaning that `size` and `len` never match and cannot be used interchangeably.

# Functions

## String length
There are three functions to check the length of a string. If you're using a `mustring` struct, you should always use the function `mus_string_strlen`, defined below in `MUS_H`:

```
MUSDEF size_m mus_string_strlen(mustring s);
```

All it does is return `s.len`, because `mustring` internally stores the length of the string, meaning that you can access it immediately without having to scan the string. You can also just access the variable `len` in the mustring `struct`. To be clear, ***use `mus_string_strlen` whenever you can.*** This is because other methods, like using the `strlen` function, can differ heavily in performance on certain operating systems, particularly on Windows. It is also agnostic as to whether or not a string is using `char`s or `wchar_m`s.

If you have a raw `char` pointer string to check the length of, you can use `mus_strlen`, which is simply a define for `strlen` provided in `string.h` unless defined otherwise by the user.

If you have a raw `wchar_m` pointer string to check the length of, you can use `mus_wstrlen`, which is simply a define for `wcslen` provided in `wchar_m.h` unless defined otherwise by the user.

## String creation
There are two functions to create a string. If you'd like to create a string that uses `char` for its datatype, use `mus_string_create`, defined below in `MUS_H`:

```
MUSDEF mustring mus_string_create(char* s);
```

The function takes in a preexisting `char` pointer string and returns a valid `mustring` struct which can then be used with other functions.

If you'd like to create a string that uses `wchar_m` for its datatype, use `mus_wide_string_create`, defined below in `MUS_H`:

```
MUSDEF mustring mus_wide_string_create(wchar_m* ws);
```

The function takes in a preexisting `wchar_m` pointer and returns a valid `mustring` struct which can then be used with other functions.

Note that if you use `mus_string_create`, the variable `s` of type `char*` is used to store the string, whilst if you use `mus_wide_string_create`, the variable `ws` of type `wchar_m*` is used.

## String destruction
To destroy a string, the function `mus_string_destroy` is used, defined below in `MUS_H`:

```
MUSDEF mustring mus_string_destroy(mustring str);
```

This function is agnostic to whether or not the string uses `char` or `wchar_m`. It also can be safely called multiple times as long as the return value is not ignored.

Note that this function ***must*** be called once a `mustring` is done being used, or else the memory will not be freed and *will* lead to memory leaks.

## Size checking
Size checking is mostly used internally within the mus functions, but can also be used by the user. The function used is `mus_string_size_check`, and its purpose is to reallocate more size for a `mustring` when necessary, defined below in `MUS_H`:

```
MUSDEF mustring mus_string_size_check(mustring str, size_m size);
```

## Here check
To check if a given string's contents is at a given point in another string, you can use the two functions `mus_here` and `mus_w_here` for `char` strings and `wchar_m` strings respectively, both defined below in `MUS_H`:

```
MUSDEF MUS_BOOL mus_here(char* str, char* check, size_m i);
MUSDEF MUS_BOOL mus_w_here(wchar_m* str, wchar_m* check, size_m i);
```

Returns `MUS_TRUE` if all characters at index `i` in `str` match the characters contained in `check`.

## Has check
To check if a given string's contents are contained somewhere within another string, you can use the two functions `mus_has` and `mus_w_has` for `char` and `wchar_m` strings respectively, both defined below in `MUS_H`:

```
MUSDEF MUS_BOOL mus_has(char* str, char* find, size_m beg, size_m end);
MUSDEF MUS_BOOL mus_w_has(wchar_m* str, wchar_m* find, size_m beg, size_m end);
```

Returns `MUS_TRUE` if the contents of `find` are contained somewhere within `str` from index `beg` to index `end`.

## Deleting section of string
To delete a certain section of a string, use the function `mus_string_delete`, defined below in `MUS_H`:

```
MUSDEF mustring mus_string_delete(mustring str, size_m beg, size_m end);
```

Deletes all characters starting at index `beg` and ending at index `end`. This function can be used for both `char` and `wchar_m` strings.

## Inserting a string into another
There are two functions used for inserting one string into another, `mus_string_insert` for `char` strings and `mus_string_w_insert` for `wchar_m` strings, both defined below in `MUS_H`:

```
MUSDEF mustring mus_string_insert(mustring str, char* insert, size_m i);
MUSDEF mustring mus_string_w_insert(mustring str, wchar_m* insert, size_m i);
```

Inserts `insert` into the string contents of `str` at index `i`.

Note that `str` *must* match the character type of insertion, aka `mus_string_insert` must only be used with `mustring`s whose `type` is equal to `MUS_STRING_TYPE_CHAR`, and `mus_string_w_insert` must only be used with `mustring`s whose `type` is equal to `MUS_STRING_TYPE_WCHAR`.

## Replacing all instances of a string with another
There are two functions for replacing all instances of a string with another, `mus_string_replace` for `char` strings and `mus_string_w_replace` for `wchar_m` strings, both defined below in `MUS_H`:

```
MUSDEF mustring mus_string_replace(mustring str, char* find, char* replace, size_m beg, size_m end);
MUSDEF mustring mus_string_w_replace(mustring str, wchar_m* find, wchar_m* replace, size_m beg, size_m end);
```

Finds all instances of `find` within the string contents of `str` and replaces it with `replace`, scanning from index `beg` to `end` within the string contents of `str`.

Note that `str` *must* match the character type of insertion, aka `mus_string_replace` must only be used with `mustring`s whose `type` is equal to `MUS_STRING_TYPE_CHAR`, and `mus_string_w_replace` must only be used with `mustring`s whose `type` is equal to `MUS_STRING_TYPE_WCHAR`.
