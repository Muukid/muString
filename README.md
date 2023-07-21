# mus
mus, pronounced "moose", is a public domain simple single-file C library for easy string allocation and manipulation. To use it, simply download the `mus.h` file, add it to your include path, and include it like so:
```
#define MUS_IMPLEMENTATION
#include "mus.h"
```
Note that not defining `MUS_IMPLEMENTATION` will define the functions but not give an implementation for the functions, meaning that you can include it across multiple files and define `MUS_IMPLEMENTATION` once in your main file.

## Why single-file headers/public domain?
See the **FAQ** section of the stb repository: https://github.com/nothings/stb/blob/master/README.md I just needed a quick and easy-to-use drag-and-drop library to manipulate strings with few allocation calls, and I had a hard time finding one.

# Using mus

## Types and structures

### Size/Index
The type used to check a string's size or to refer to an index in a string is `size_m`, which matches the usage of `size_t`.

### String
A string in mus is called `mustring`, a struct defined below:
```
typedef struct {
	char* s;
	size_m size;
} mustring;
```
`char* s` actually holds the string data itself, and `size_m size` indicates the size allocated for the string.

### Boolean
The type used for boolean values is `MUS_BOOL`, defined below:
```
typedef enum { MUS_FALSE, MUS_TRUE } MUS_BOOL;
```

## Creating and destroying a string

### Creation
In order to create a string, use the function `mus_string_create`, defined below:
```
mustring mus_string_create(char* s);
```
The function returns a valid `mustring` struct with the data from `char* s` copied over to the struct in a newly allocated `char` array and `size` representing how much memory has been allocated for the string, which is usually double the amount of the length of the parameter `char* s` multiplied by the size of a `char`.

### Destruction
A string must be destroyed in order to free the data allocated for it, which is done via the function `mus_string_destroy`, defined below:
```
void mus_string_destroy(mustring str);
```
Once destroyed, the pointer `char* s` within the parameter `mustring str` is freed.

## Check string contents

### mus_strlen
The function `mus_strlen` can be used to see how long a string is, defined below:
```
size_m mus_strlen(char* s);
```

### mus_here
The function `mus_here` can be used to check whether or not a string's contents (`char* check`) is located at a given point (`size_m i`) in another string (`char* str`), defined below:
```
MUS_BOOL mus_here(char* str, char* check, size_m i);
```
### mus_has
The function `mus_has` can be used to check whether or not a given string of characters (`char* find`) can be found within a specified range (`size_m beg, size_m end`) of a given string (`char* str`), defined below:
```
MUS_BOOL mus_has(char* str, char* find, size_m beg, size_m end);
```

## Modify string contents

### mus_string_delete
The function `mus_string_delete` can be used to delete a given range (`size_m beg, size_m end`) of characters from a string (`mus_string str`), defined below:
```
mustring mus_string_delete(mustring str, size_m beg, size_m end);
```

### mus_string_insert
The function `mus_string_insert` can be used to insert a given string of characters (`char* insert`) into a given string (`mustring str`) at a given point (`size_m i`), defined below:
```
mustring mus_string_insert(mustring str, char* insert, size_m i);
```

### mus_string_replace
The function `mus_string_replace` can be used to find all instances of a given string of characters (`char* find`) in a given range (`size_m beg, size_m end`) of a given string (`mustring str`) and replace it with another given string of characters (`char* replace`), defined below:
```
mustring mus_string_replace(mustring str, char* find, char* replace, size_m beg, size_m end);
```
