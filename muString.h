/*
muString.h - Muukid
Public domain single-file C library for handling character encoding and string manipulation.
No warranty implied; use at your own risk.

Licensed under MIT License or public domain, whichever you prefer.
More explicit license information at the end of file.

@MENTION ASCII is with no extensions, only being valid from 0x00 to 0x7F. This is enforced.
@MENTION Only functions that take in or return 'muString' require mus to be initiated.
@MENTION Unicode surrogates are totally allowed.
@MENTION math.h
@MENTION Float/Double conversions for strings do very little rounding, if at all.
@MENTION Raw string -> int/float/double expect data[0] to data[data_size-1] to ONLY be number data.
This includes no spaces and only one '-' (and it must be at the beginning). Also, float/double
expect '.' to be in there at SOME point.

@TODO Hex/Binary string -> int.
*/

#ifndef MUS_H
	#define MUS_H
	
	/* muMemoryAllocator version 1.0.0 header */
	
		#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUMA_H) && \
			(MUMA_VERSION_MAJOR != 1 || MUMA_VERSION_MINOR != 0 || MUMA_VERSION_PATCH != 0)

			#pragma message("[MUS] muMemoryAllocator's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")

		#endif

		#ifndef MUMA_H
			#define MUMA_H
			
			/* muUtility version 1.0.0 header */
			
				#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUU_H) && \
					(MUU_VERSION_MAJOR != 1 || MUU_VERSION_MINOR != 0 || MUU_VERSION_PATCH != 0)

					#pragma message("[MUMA] muUtility's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")

				#endif

				#ifndef MUU_H
					#define MUU_H
					
					#if !defined(MU_SECURE_WARNINGS) && !defined(_CRT_SECURE_NO_WARNINGS)
						#define _CRT_SECURE_NO_WARNINGS
					#endif

					#ifdef __cplusplus
					extern "C" { // }
					#endif

					#define MUU_VERSION_MAJOR 1
					#define MUU_VERSION_MINOR 0
					#define MUU_VERSION_PATCH 0

					/* C standard library dependencies */

						#if !defined(int8_m)      || \
							!defined(uint8_m)     || \
							!defined(int16_m)     || \
							!defined(uint16_m)    || \
							!defined(int32_m)     || \
							!defined(uint32_m)    || \
							!defined(int64_m)     || \
							!defined(uint64_m)    || \
							!defined(MU_SIZE_MAX)

							#define __STDC_LIMIT_MACROS
							#define __STDC_CONSTANT_MACROS
							#include <stdint.h>

							#ifndef int8_m
								#ifdef INT8_MAX
									#define int8_m int8_t
								#else
									#define int8_m char
								#endif
							#endif

							#ifndef uint8_m
								#ifdef UINT8_MAX
									#define uint8_m uint8_t
								#else
									#define uint8_m unsigned char
								#endif
							#endif

							#ifndef int16_m
								#ifdef INT16_MAX
									#define int16_m int16_t
								#else
									#define int16_m short
								#endif
							#endif

							#ifndef uint16_m
								#ifdef UINT16_MAX
									#define uint16_m uint16_t
								#else
									#define uint16_m unsigned short
								#endif
							#endif

							#ifndef int32_m
								#ifdef INT32_MAX
									#define int32_m int32_t
								#else
									#define int32_m long
								#endif
							#endif

							#ifndef uint32_m
								#ifdef UINT32_MAX
									#define uint32_m uint32_t
								#else
									#define uint32_m unsigned long
								#endif
							#endif

							#ifndef int64_m
								#ifdef INT64_MAX
									#define int64_m int64_t
								#else
									#define int64_m long long
								#endif
							#endif

							#ifndef uint64_m
								#ifdef UINT64_MAX
									#define uint64_m uint64_t
								#else
									#define uint64_m unsigned long long
								#endif
							#endif

							#ifndef MU_SIZE_MAX
								#define MU_SIZE_MAX SIZE_MAX
							#endif

						#endif

						#if !defined(size_m)

							#include <stddef.h>

							#ifndef size_m
								#define size_m size_t
							#endif

						#endif

						#if !defined(muBool)   || \
							!defined(MU_TRUE)  || \
							!defined(MU_FALSE)

							#include <stdbool.h>

							#ifndef muBool
								#define muBool bool
							#endif

							#ifndef MU_TRUE
								#define MU_TRUE true
							#endif

							#ifndef MU_FALSE
								#define MU_FALSE false
							#endif

						#endif

					/* Useful macros */

						#ifndef MUDEF
							#ifdef MU_STATIC
								#define MUDEF static
							#else
								#define MUDEF extern
							#endif
						#endif

						#ifndef MU_ZERO_STRUCT
							#ifdef __cplusplus
								#define MU_ZERO_STRUCT(s) {}
							#else
								#define MU_ZERO_STRUCT(s) (s){0}
							#endif
						#endif

						#ifndef MU_ZERO_STRUCT_CONST
							#ifdef __cplusplus
								#define MU_ZERO_STRUCT_CONST(s) {}
							#else
								#define MU_ZERO_STRUCT_CONST(s) {0}
							#endif
						#endif

						#ifndef muByte
							#define muByte uint8_m
						#endif

						#ifndef mu_rle_uint8
							#define mu_rle_uint8(b) ((uint8_m)b[0] << 0)
						#endif
						#ifndef mu_rbe_uint8
							#define mu_rbe_uint8(b) ((uint8_m)b[0] << 0)
						#endif

						#ifndef mu_rle_uint16
							#define mu_rle_uint16(b) ((uint16_m)b[0] << 0 | (uint16_m)b[1] << 8)
						#endif
						#ifndef mu_rbe_uint16
							#define mu_rbe_uint16(b) ((uint16_m)b[1] << 0 | (uint16_m)b[0] << 8)
						#endif

						#ifndef mu_rle_uint32
							#define mu_rle_uint32(b) ((uint32_m)b[0] << 0 | (uint32_m)b[1] << 8 | (uint32_m)b[2] << 16 | (uint32_m)b[3] << 24)
						#endif
						#ifndef mu_rbe_uint32
							#define mu_rbe_uint32(b) ((uint32_m)b[3] << 0 | (uint32_m)b[2] << 8 | (uint32_m)b[1] << 16 | (uint32_m)b[0] << 24)
						#endif

						#ifndef mu_rle_uint64
							#define mu_rle_uint64(b) ((uint64_m)b[0] << 0 | (uint64_m)b[1] << 8 | (uint64_m)b[2] << 16 | (uint64_m)b[3] << 24 | (uint64_m)b[4] << 32 | (uint64_m)b[5] << 40 | (uint64_m)b[6] << 48 | (uint64_m)b[7] << 56)
						#endif
						#ifndef mu_rbe_uint64
							#define mu_rbe_uint64(b) ((uint64_m)b[7] << 0 | (uint64_m)b[6] << 8 | (uint64_m)b[5] << 16 | (uint64_m)b[4] << 24 | (uint64_m)b[3] << 32 | (uint64_m)b[2] << 40 | (uint64_m)b[1] << 48 | (uint64_m)b[0] << 56)
						#endif

						#ifndef mu_wle_uint8
							#define mu_wle_uint8(b, i) b[0] = (uint8_m)(i >> 0);
						#endif
						#ifndef mu_wbe_uint8
							#define mu_wbe_uint8(b, i) b[0] = (uint8_m)(i >> 0);
						#endif

						#ifndef mu_wle_uint16
							#define mu_wle_uint16(b, i) b[0] = (uint8_m)(i >> 0); b[1] = (uint8_m)(i >> 8);
						#endif
						#ifndef mu_wbe_uint16
							#define mu_wbe_uint16(b, i) b[1] = (uint8_m)(i >> 0); b[0] = (uint8_m)(i >> 8);
						#endif

						#ifndef mu_wle_uint32
							#define mu_wle_uint32(b, i) b[0] = (uint8_m)(i >> 0); b[1] = (uint8_m)(i >> 8); b[2] = (uint8_m)(i >> 16); b[3] = (uint8_m)(i >> 24);
						#endif
						#ifndef mu_wbe_uint32
							#define mu_wbe_uint32(b, i) b[3] = (uint8_m)(i >> 0); b[2] = (uint8_m)(i >> 8); b[1] = (uint8_m)(i >> 16); b[0] = (uint8_m)(i >> 24);
						#endif

						#ifndef mu_wle_uint64
							#define mu_wle_uint64(b, i) b[0] = (uint8_m)(i >> 0); b[1] = (uint8_m)(i >> 8); b[2] = (uint8_m)(i >> 16); b[3] = (uint8_m)(i >> 24); b[4] = (uint8_m)(i >> 32); b[5] = (uint8_m)(i >> 40); b[6] = (uint8_m)(i >> 48); b[7] = (uint8_m)(i >> 56);
						#endif
						#ifndef mu_wbe_uint64
							#define mu_wbe_uint64(b, i) b[7] = (uint8_m)(i >> 0); b[6] = (uint8_m)(i >> 8); b[5] = (uint8_m)(i >> 16); b[4] = (uint8_m)(i >> 24); b[3] = (uint8_m)(i >> 32); b[2] = (uint8_m)(i >> 40); b[1] = (uint8_m)(i >> 48); b[0] = (uint8_m)(i >> 56);
						#endif

						#ifndef MU_NULL_PTR
							#define MU_NULL_PTR 0
						#endif

						#ifndef MU_NULL
							#define MU_NULL 0
						#endif

						#ifndef MU_NONE
							#define MU_NONE MU_SIZE_MAX
						#endif

						#ifndef MU_SET_RESULT
							#define MU_SET_RESULT(res, val) if(res!=MU_NULL_PTR){*res=val;}
						#endif

						#ifndef MU_ASSERT
							#define MU_ASSERT(cond, res, val, after) if(!(cond)){MU_SET_RESULT(res, val) after}
						#endif

						#define MU_ENUM(name, ...) enum _##name{__VA_ARGS__};typedef enum _##name _##name; typedef size_m name;

						#if !defined(MU_WIN32) && !defined(MU_UNIX)
							#if defined(WIN32) || defined(_WIN32)
								#define MU_WIN32
							#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
								#define MU_UNIX
							#endif
						#endif

						#define MU_HRARRAY_DEFAULT_FUNC(name) \
							muBool name##_comp(name t0, name t1) { \
								return t0.active == t1.active; \
							} \
							\
							void name##_on_creation(name* p) { \
								if (p != MU_NULL_PTR) { \
									MU_LOCK_CREATE(p->lock, p->lock_active) \
								} \
							} \
							void name##_on_destruction(name* p) { \
								if (p != MU_NULL_PTR) { \
									MU_LOCK_DESTROY(p->lock, p->lock_active) \
								} \
							} \
							void name##_on_hold(name* p) { \
								if (p != MU_NULL_PTR) { \
									MU_LOCK_LOCK(p->lock, p->lock_active) \
								} \
							} \
							void name##_on_release(name* p) { \
								if (p != MU_NULL_PTR) { \
									MU_LOCK_UNLOCK(p->lock, p->lock_active) \
								} \
							} \
							\
							mu_dynamic_hrarray_declaration( \
								name##_array, name, name##_, name##_comp, \
								name##_on_creation, name##_on_destruction, name##_on_hold, name##_on_release \
							)

						#define MU_SAFEFUNC(result, lib_prefix, context, fail_return) \
							MU_SET_RESULT(result, lib_prefix##SUCCESS) \
							MU_ASSERT(context != MU_NULL_PTR, result, lib_prefix##NOT_YET_INITIALIZED, fail_return) \

						#define MU_HOLD(result, item, da, context, lib_prefix, fail_return, da_prefix) \
							MU_ASSERT(item < da.length, result, lib_prefix##INVALID_ID, fail_return) \
							da_prefix##hold_element(0, &da, item); \
							MU_ASSERT(da.data[item].active, result, lib_prefix##INVALID_ID, da_prefix##release_element(0, &da, item); fail_return)

						#define MU_RELEASE(da, item, da_prefix) \
							da_prefix##release_element(0, &da, item);

					#ifdef __cplusplus
					}
					#endif

				#endif /* MUU_H */
			
			#ifdef __cplusplus
				extern "C" {
			#endif

			#define MUMA_VERSION_MAJOR 1
			#define MUMA_VERSION_MINOR 0
			#define MUMA_VERSION_PATCH 0

			/* C standard library dependencies */

				#if !defined(mu_malloc)  || \
					!defined(mu_free)    || \
					!defined(mu_realloc)

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

				#endif

				#if !defined(mu_memset)  || \
					!defined(mu_memcpy)  || \
					!defined(mu_memmove)

					#include <string.h>

					#ifndef mu_memset
						#define mu_memset memset
					#endif

					#ifndef mu_memcpy
						#define mu_memcpy memcpy
					#endif

					#ifndef mu_memmove
						#define mu_memmove memmove
					#endif

				#endif

			/* Enums */

				MU_ENUM(mumaResult, 
					MUMA_SUCCESS,

					MUMA_FAILED_TO_ALLOCATE,
					MUMA_INVALID_INDEX,
					MUMA_INVALID_SHIFT_AMOUNT,
					MUMA_NOT_FOUND
				)

			/* Functions */

				#ifdef MUMA_NAMES
					MUDEF const char* muma_result_get_name(mumaResult result);
				#endif

			/* API macro functionality */

				#define mu_dynamic_array_declaration(struct_name, type, function_name_prefix, type_comparison_func) \
					\
					struct struct_name { \
						type* data; \
						size_m allocated_length; \
						size_m length; \
					}; typedef struct struct_name struct_name; \
					\
					struct_name function_name_prefix##create(mumaResult* result, size_m length) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						struct_name s = MU_ZERO_STRUCT(struct_name); \
						s.data = MU_NULL_PTR; \
						s.allocated_length = length; \
						s.length = length; \
						\
						if (s.length == 0) { \
							return s; \
						} \
						\
						s.data = (type*)mu_malloc(sizeof(type)*s.allocated_length); \
						if (s.data == 0) { \
							MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
							s.allocated_length = 0; \
							s.length = 0; \
							return s; \
						} \
						\
						mu_memset(s.data, 0, sizeof(type)*s.allocated_length);\
						return s;\
					} \
					\
					struct_name function_name_prefix##destroy(mumaResult* result, struct_name s) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						if (s.data != MU_NULL_PTR) { \
							mu_free(s.data); \
							s.data = MU_NULL_PTR; \
						} \
						\
						s.allocated_length = 0; \
						s.length = 0; \
						return s; \
					} \
					\
					struct_name function_name_prefix##resize(mumaResult* result, struct_name s, size_m length) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						if (s.length == length) { \
							return s; \
						} \
						\
						if (s.data == MU_NULL_PTR) { \
							if (length == 0) { \
								return s; \
							} \
							\
							mumaResult res = MUMA_SUCCESS; \
							s = function_name_prefix##create(&res, length); \
							if (res != MUMA_SUCCESS) { \
								MU_SET_RESULT(result, res) \
								return s; \
							} \
						} \
						\
						if (length == 0) { \
							s.length = 0; \
							mu_memset(s.data, 0, sizeof(type)*s.allocated_length); \
							return s; \
						} \
						\
						size_m old_length = s.length; \
						size_m old_allocated_length = s.allocated_length; \
						s.length = length; \
						\
						if ((s.length > s.allocated_length) || (s.length < s.allocated_length/2)) { \
							while (s.length > s.allocated_length) { \
								s.allocated_length *= 2; \
							} \
							while (s.length < s.allocated_length/2) { \
								s.allocated_length /= 2; \
							} \
							\
							type* new_data = (type*)mu_realloc(s.data, sizeof(type) * s.allocated_length); \
							if (new_data == 0) { \
								s.length = old_length; \
								s.allocated_length = old_allocated_length; \
								MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
								return s; \
							} \
							\
							s.data = new_data; \
						} \
						\
						if (old_length < s.length) { \
							mu_memset(&s.data[old_length], 0, sizeof(type)*(s.allocated_length-old_length)); \
						} \
						\
						return s; \
					} \
					\
					struct_name function_name_prefix##lshift(mumaResult* result, struct_name s, size_m index, size_m amount) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						if (index >= s.length) { \
							MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
							return s; \
						} \
						\
						if (amount == 0) { \
							return s; \
						} \
						\
						if (amount > index) { \
							MU_SET_RESULT(result, MUMA_INVALID_SHIFT_AMOUNT) \
							return s; \
						} \
						\
						mu_memmove(&s.data[index-amount], &s.data[index], sizeof(type)*(s.length-index)); \
						\
						mumaResult res = MUMA_SUCCESS; \
						s = function_name_prefix##resize(&res, s, s.length-amount); \
						if (res != MUMA_SUCCESS) { \
							MU_SET_RESULT(result, res) \
							return s; \
						} \
						\
						return s; \
					} \
					\
					struct_name function_name_prefix##rshift(mumaResult* result, struct_name s, size_m index, size_m amount) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						if (index >= s.length) { \
							MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
							return s; \
						} \
						\
						if (amount == 0) { \
							return s; \
						} \
						\
						mumaResult res = MUMA_SUCCESS; \
						s = function_name_prefix##resize(&res, s, s.length+amount); \
						if (res != MUMA_SUCCESS) { \
							MU_SET_RESULT(result, res) \
							return s; \
						} \
						\
						/* I have genuinely no idea why it needs to have a -1. It crashes if you don't. */\
						/* And lshift doesn't need one. Why?? */ \
						mu_memmove(&s.data[index+amount], &s.data[index], sizeof(type)*((s.length-index)-1)); \
						mu_memset(&s.data[index], 0, sizeof(type)*(amount)); \
						\
						return s; \
					} \
					\
					struct_name function_name_prefix##multiinsert(mumaResult* result, struct_name s, size_m index, type* insert, size_m count) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						mumaResult res = MUMA_SUCCESS; \
						s = function_name_prefix##rshift(&res, s, index, count); \
						if (res != MUMA_SUCCESS) { \
							MU_SET_RESULT(result, res) \
							return s; \
						} \
						\
						mu_memcpy(&s.data[index], insert, sizeof(type)*count); \
						return s; \
					} \
					\
					struct_name function_name_prefix##insert(mumaResult* result, struct_name s, size_m index, type insert) { \
						return function_name_prefix##multiinsert(result, s, index, &insert, 1); \
					} \
					\
					struct_name function_name_prefix##multierase(mumaResult* result, struct_name s, size_m index, size_m count) { \
						return function_name_prefix##lshift(result, s, index+count, count); \
					} \
					\
					struct_name function_name_prefix##erase(mumaResult* result, struct_name s, size_m index) { \
						return function_name_prefix##multierase(result, s, index, 1); \
					} \
					struct_name function_name_prefix##clear(mumaResult* result, struct_name s) { \
						return function_name_prefix##destroy(result, s); \
					} \
					\
					struct_name function_name_prefix##multipush(mumaResult* result, struct_name s, type* push, size_m count) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						mumaResult res = MUMA_SUCCESS; \
						s = function_name_prefix##resize(&res, s, s.length+count); \
						if (res != MUMA_SUCCESS) { \
							MU_SET_RESULT(result, res) \
							return s; \
						} \
						\
						mu_memcpy(&s.data[s.length-count], push, sizeof(type)*count); \
						return s; \
					} \
					\
					struct_name function_name_prefix##push(mumaResult* result, struct_name s, type push) { \
						return function_name_prefix##multipush(result, s, &push, 1); \
					} \
					\
					struct_name function_name_prefix##multipop(mumaResult* result, struct_name s, size_m count) { \
						return function_name_prefix##resize(result, s, s.length-count); \
					} \
					\
					struct_name function_name_prefix##pop(mumaResult* result, struct_name s) { \
						return function_name_prefix##multipop(result, s, 1); \
					} \
					\
					size_m function_name_prefix##find(mumaResult* result, struct_name s, type find) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						for (size_m i = 0; i < s.length; i++) { \
							if (type_comparison_func(find, s.data[i])) { \
								return i; \
							} \
						} \
						\
						MU_SET_RESULT(result, MUMA_NOT_FOUND) \
						return MU_NONE; \
					} \
					\
					struct_name function_name_prefix##find_push(mumaResult* result, struct_name s, type find, size_m* p_index) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						mumaResult res = MUMA_SUCCESS; \
						size_m index = function_name_prefix##find(&res, s, find); \
						if (index != MU_NONE) { \
							if (p_index != MU_NULL_PTR) { \
								*p_index = index; \
							} \
							return s; \
						} \
						if (res != MUMA_SUCCESS && res != MUMA_NOT_FOUND) { \
							if (p_index != MU_NULL_PTR) { \
								*p_index = MU_NONE; \
							} \
							MU_SET_RESULT(result, res) \
							return s; \
						} \
						\
						s = function_name_prefix##push(&res, s, find); \
						if (res != MUMA_SUCCESS) { \
							if (p_index != MU_NULL_PTR) { \
								*p_index = MU_NONE; \
							} \
							MU_SET_RESULT(result, res) \
							return s; \
						} \
						\
						if (p_index != MU_NULL_PTR) { \
							*p_index = s.length-1; \
						} \
						return s; \
					}

				#define mu_dynamic_hrarray_declaration( \
					struct_name, type, function_name_prefix, type_comparison_func, \
					on_creation, on_destruction, on_hold, on_release \
				) \
					\
					struct struct_name { \
						type* data; \
						size_m allocated_length; \
						size_m length; \
					}; typedef struct struct_name struct_name; \
					\
					struct_name function_name_prefix##create(mumaResult* result, size_m length) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						struct_name s = MU_ZERO_STRUCT(struct_name); \
						s.data = MU_NULL_PTR; \
						s.allocated_length = length; \
						s.length = length; \
						\
						if (s.length == 0) { \
							return s; \
						} \
						\
						s.data = (type*)mu_malloc(sizeof(type)*s.allocated_length); \
						if (s.data == 0) { \
							MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
							s.allocated_length = 0; \
							s.length = 0; \
							return s; \
						} \
						\
						mu_memset(s.data, 0, sizeof(type)*s.allocated_length); \
						\
						for (size_m i = 0; i < s.length; i++) { \
							on_creation(&s.data[i]); \
						} \
						return s;\
					} \
					\
					void function_name_prefix##hold_element(mumaResult* result, struct_name* s, size_m index) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						if (index >= s->length) { \
							MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
							return; \
						} \
						\
						on_hold(&s->data[index]); \
					} \
					\
					void function_name_prefix##release_element(mumaResult* result, struct_name* s, size_m index) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						if (index >= s->length) { \
							MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
							return; \
						} \
						\
						on_release(&s->data[index]); \
					} \
					\
					void function_name_prefix##destroy(mumaResult* result, struct_name* s) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						for (size_m i = 0; i < s->length; i++) { \
							function_name_prefix##hold_element(0, s, i); \
						} \
						\
						for (size_m i = 0; i < s->length; i++) { \
							on_destruction(&s->data[i]); \
						} \
						\
						if (s->data != MU_NULL_PTR) { \
							mu_free(s->data); \
							s->data = MU_NULL_PTR; \
						} \
						\
						s->allocated_length = 0; \
						s->length = 0; \
					} \
					\
					void function_name_prefix##inner_resize(mumaResult* result, struct_name* s, size_m length, muBool cd) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						if (s->length == length) { \
							return; \
						} \
						\
						if (s->data == MU_NULL_PTR) { \
							if (length == 0) { \
								return; \
							} \
							\
							mumaResult res = MUMA_SUCCESS; \
							*s = function_name_prefix##create(&res, length); \
							if (res != MUMA_SUCCESS) { \
								MU_SET_RESULT(result, res) \
								return; \
							} \
						} \
						\
						if (length == 0) { \
							for (size_m i = 0; i < s->length; i++) { \
								on_destruction(&s->data[i]); \
							} \
							s->length = 0; \
							mu_memset(s->data, 0, sizeof(type)*s->allocated_length); \
							return; \
						} \
						\
						if (cd) { \
							for (size_m i = 0; i < s->length; i++) { \
								function_name_prefix##hold_element(0, s, i); \
							} \
						} \
						\
						size_m old_length = s->length; \
						size_m old_allocated_length = s->allocated_length; \
						s->length = length; \
						\
						/* Note: this is really dangerous, because it's not guaranteed that the  */ \
						/* reallocation will follow through. If it doesn't, we've now called the */ \
						/* destroy function on a bunch of elements that still exist. I can't     */ \
						/* really think of a better way of doing it than this right now, though. */ \
						if (cd && old_length > s->length) { \
							for (size_m i = s->length; i < old_length; i++) { \
								on_destruction(&s->data[i]); \
							} \
						} \
						\
						if ((s->length > s->allocated_length) || (s->length < s->allocated_length/2)) { \
							while (s->length > s->allocated_length) { \
								s->allocated_length *= 2; \
							} \
							while (s->length < s->allocated_length/2) { \
								s->allocated_length /= 2; \
							} \
							\
							type* new_data = (type*)mu_realloc(s->data, sizeof(type) * s->allocated_length); \
							if (new_data == 0) { \
								s->length = old_length; \
								s->allocated_length = old_allocated_length; \
								if (cd) { \
									for (size_m i = 0; i < s->length; i++) { \
										function_name_prefix##release_element(0, s, i); \
									} \
								} \
								MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
								return; \
							} \
							\
							s->data = new_data; \
						} \
						\
						if (old_length < s->length) { \
							mu_memset(&s->data[old_length], 0, sizeof(type)*(s->allocated_length-old_length)); \
							\
							if (cd) { \
								for (size_m i = old_length; i < s->length; i++) { \
									on_creation(&s->data[i]); \
								} \
							} \
						} \
						\
						if (cd) { \
							for (size_m i = 0; i < s->length && i < old_length; i++) { \
								function_name_prefix##release_element(0, s, i); \
							} \
						} \
					} \
					\
					void function_name_prefix##resize(mumaResult* result, struct_name* s, size_m length) { \
						function_name_prefix##inner_resize(result, s, length, MU_TRUE); \
					} \
					\
					void function_name_prefix##inner_lshift(mumaResult* result, struct_name* s, size_m index, size_m amount, muBool cd) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						if (index >= s->length) { \
							MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
							return; \
						} \
						\
						if (amount == 0) { \
							return; \
						} \
						\
						if (amount > index) { \
							MU_SET_RESULT(result, MUMA_INVALID_SHIFT_AMOUNT) \
							return; \
						} \
						\
						if (cd) { \
							for (size_m i = 0; i < s->length; i++) { \
								function_name_prefix##hold_element(0, s, i); \
							} \
						} \
						\
						/* Dangerous; resize may fail after this */ \
						if (cd) { \
							for (size_m i = index-amount; i < index; i++) { \
								on_destruction(&s->data[i]); \
							} \
						} \
						\
						mu_memmove(&s->data[index-amount], &s->data[index], sizeof(type)*(s->length-index)); \
						\
						mumaResult res = MUMA_SUCCESS; \
						function_name_prefix##inner_resize(&res, s, s->length-amount, MU_FALSE); \
						\
						if (cd) { \
							for (size_m i = 0; i < s->length; i++) { \
								function_name_prefix##release_element(0, s, i); \
							} \
						} \
						\
						if (res != MUMA_SUCCESS) { \
							MU_SET_RESULT(result, res) \
							return; \
						} \
					} \
					\
					void function_name_prefix##lshift(mumaResult* result, struct_name* s, size_m index, size_m amount) { \
						function_name_prefix##inner_lshift(result, s, index, amount, MU_TRUE); \
					} \
					\
					void function_name_prefix##inner_rshift(mumaResult* result, struct_name* s, size_m index, size_m amount, muBool cd) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						if (index >= s->length) { \
							MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
							return; \
						} \
						\
						if (amount == 0) { \
							return; \
						} \
						\
						if (cd) { \
							for (size_m i = 0; i < s->length; i++) { \
								function_name_prefix##hold_element(0, s, i); \
							} \
						} \
						\
						mumaResult res = MUMA_SUCCESS; \
						function_name_prefix##inner_resize(&res, s, s->length+amount, MU_FALSE); \
						if (res != MUMA_SUCCESS) { \
							if (cd) { \
								for (size_m i = 0; i < s->length; i++) { \
									function_name_prefix##release_element(0, s, i); \
								} \
							} \
							MU_SET_RESULT(result, res) \
							return; \
						} \
						\
						mu_memmove(&s->data[index+amount], &s->data[index], sizeof(type)*((s->length-index)-1)); \
						\
						if (cd) { \
							for (size_m i = 0; i < index; i++) { \
								function_name_prefix##release_element(0, s, i); \
							} \
							for (size_m i = index+amount; i < s->length; i++) { \
								function_name_prefix##release_element(0, s, i); \
							} \
						} \
						\
						mu_memset(&s->data[index], 0, sizeof(type)*(amount)); \
						\
						if (cd) { \
							for (size_m i = index; i < index+amount; i++) { \
								on_creation(&s->data[i]); \
							} \
						} \
					} \
					\
					void function_name_prefix##rshift(mumaResult* result, struct_name* s, size_m index, size_m amount) { \
						function_name_prefix##inner_rshift(result, s, index, amount, MU_TRUE); \
					} \
					\
					void function_name_prefix##multiinsert(mumaResult* result, struct_name* s, size_m index, type* insert, size_m count) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						for (size_m i = 0; i < s->length; i++) { \
							function_name_prefix##hold_element(0, s, i); \
						} \
						\
						mumaResult res = MUMA_SUCCESS; \
						function_name_prefix##inner_rshift(&res, s, index, count, MU_FALSE); \
						if (res != MUMA_SUCCESS) { \
							for (size_m i = 0; i < s->length; i++) { \
								function_name_prefix##release_element(0, s, i); \
							} \
							MU_SET_RESULT(result, res) \
							return; \
						} \
						\
						mu_memcpy(&s->data[index], insert, sizeof(type)*count); \
						\
						for (size_m i = 0; i < index; i++) { \
							function_name_prefix##release_element(0, s, i); \
						} \
						for (size_m i = index+count; i < s->length; i++) { \
							function_name_prefix##release_element(0, s, i); \
						} \
						\
						for (size_m i = index; i < index+count; i++) { \
							on_creation(&s->data[i]); \
						} \
					} \
					\
					void function_name_prefix##insert(mumaResult* result, struct_name* s, size_m index, type insert) { \
						function_name_prefix##multiinsert(result, s, index, &insert, 1); \
					} \
					\
					void function_name_prefix##multierase(mumaResult* result, struct_name* s, size_m index, size_m count) { \
						function_name_prefix##lshift(result, s, index+count, count); \
					} \
					\
					void function_name_prefix##erase(mumaResult* result, struct_name* s, size_m index) { \
						function_name_prefix##multierase(result, s, index, 1); \
					} \
					void function_name_prefix##clear(mumaResult* result, struct_name* s) { \
						function_name_prefix##destroy(result, s); \
					} \
					\
					void function_name_prefix##multipush(mumaResult* result, struct_name* s, type* push, size_m count) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						size_m old_length = s->length; \
						for (size_m i = 0; i < s->length; i++) { \
							function_name_prefix##hold_element(0, s, i); \
						} \
						\
						mumaResult res = MUMA_SUCCESS; \
						function_name_prefix##inner_resize(&res, s, s->length+count, MU_FALSE); \
						if (res != MUMA_SUCCESS) { \
							for (size_m i = 0; i < s->length; i++) { \
								function_name_prefix##release_element(0, s, i); \
							} \
							MU_SET_RESULT(result, res) \
							return; \
						} \
						\
						mu_memcpy(&s->data[s->length-count], push, sizeof(type)*count); \
						\
						for (size_m i = 0; i < old_length; i++) { \
							function_name_prefix##release_element(0, s, i); \
						} \
						\
						for (size_m i = s->length-count; i < s->length; i++) { \
							on_creation(&s->data[i]); \
						} \
					} \
					\
					void function_name_prefix##push(mumaResult* result, struct_name* s, type push) { \
						function_name_prefix##multipush(result, s, &push, 1); \
					} \
					\
					void function_name_prefix##multipop(mumaResult* result, struct_name* s, size_m count) { \
						function_name_prefix##resize(result, s, s->length-count); \
					} \
					\
					void function_name_prefix##pop(mumaResult* result, struct_name* s) { \
						function_name_prefix##multipop(result, s, 1); \
					} \
					\
					size_m function_name_prefix##find(mumaResult* result, struct_name* s, type find) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						for (size_m i = 0; i < s->length; i++) { \
							function_name_prefix##hold_element(0, s, i); \
							if (type_comparison_func(find, s->data[i])) { \
								function_name_prefix##release_element(0, s, i); \
								return i; \
							} \
							function_name_prefix##release_element(0, s, i); \
						} \
						\
						MU_SET_RESULT(result, MUMA_NOT_FOUND) \
						return MU_NONE; \
					} \
					\
					void function_name_prefix##find_push(mumaResult* result, struct_name* s, type find, size_m* p_index) { \
						MU_SET_RESULT(result, MUMA_SUCCESS) \
						\
						mumaResult res = MUMA_SUCCESS; \
						size_m index = function_name_prefix##find(&res, s, find); \
						if (index != MU_NONE) { \
							if (p_index != MU_NULL_PTR) { \
								*p_index = index; \
							} \
							return; \
						} \
						if (res != MUMA_SUCCESS && res != MUMA_NOT_FOUND) { \
							if (p_index != MU_NULL_PTR) { \
								*p_index = MU_NONE; \
							} \
							MU_SET_RESULT(result, res) \
							return; \
						} \
						\
						function_name_prefix##push(&res, s, find); \
						if (res != MUMA_SUCCESS) { \
							if (p_index != MU_NULL_PTR) { \
								*p_index = MU_NONE; \
							} \
							MU_SET_RESULT(result, res) \
							return; \
						} \
						\
						if (p_index != MU_NULL_PTR) { \
							*p_index = s->length-1; \
						} \
					}

			#ifdef __cplusplus
				}
			#endif

		#endif /* MUMA_H */

	/* muMultithreading version 1.0.0 header (only if thread safe) */
	// CHANGES:
	// * Removed muma header inclusion because this library already has it.
	#ifdef MU_THREADSAFE
		#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUM_H) && \
			(MUM_VERSION_MAJOR != 1 || MUM_VERSION_MINOR != 0 || MUM_VERSION_PATCH != 0)

			#pragma message("[MUS] muMultithreading's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")

		#endif

		#ifndef MUM_H
			#define MUM_H

			#ifdef __cplusplus
			extern "C" { // }
			#endif

			#define MUM_VERSION_MAJOR 1
			#define MUM_VERSION_MINOR 0
			#define MUM_VERSION_PATCH 0

			/* Enums */

				MU_ENUM(mumResult,
					MUM_SUCCESS,

					MUM_MUMA_SUCCESS,
					MUM_MUMA_FAILED_TO_ALLOCATE,
					MUM_MUMA_INVALID_INDEX,
					MUM_MUMA_INVALID_SHIFT_AMOUNT,
					MUM_MUMA_NOT_FOUND,

					MUM_ALLOCATION_FAILED,

					MUM_ALREADY_INITIALIZED,
					MUM_ALREADY_TERMINATED,
					MUM_NOT_YET_INITIALIZED,

					MUM_CREATE_CALL_FAILED,
					MUM_DESTROY_CALL_FAILED,
					MUM_WAIT_CALL_FAILED,
					MUM_LOCK_CALL_FAILED,
					MUM_UNLOCK_CALL_FAILED,
					MUM_GET_RETURN_VALUE_CALL_FAILED,

					MUM_INVALID_ID,

					MUM_THREAD_TIMED_OUT,
					MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK
				)

			/* Macros */

				#define muThread size_m
				#define muMutex size_m
				#define muSpinlock size_m

				#ifdef MU_SPINLOCK
					#define muLock muSpinlock
					#define mu_lock_create mu_spinlock_create
					#define mu_lock_destroy mu_spinlock_destroy
					#define mu_lock_lock mu_spinlock_lock
					#define mu_lock_unlock mu_spinlock_unlock
				#else
					#define muLock muMutex
					#define mu_lock_create mu_mutex_create
					#define mu_lock_destroy mu_mutex_destroy
					#define mu_lock_lock mu_mutex_lock
					#define mu_lock_unlock mu_mutex_unlock
				#endif

			/* Incomplete types */

				typedef struct mumContext mumContext;

			/* Global variables */

				MUDEF mumContext* mum_global_context;

			/* Functions */

				/* Names */

					#ifdef MUM_NAMES
						MUDEF const char* mum_result_get_name(mumResult result);
					#endif

				/* Initiation/Termination */

					MUDEF void mum_init(mumResult* result);
					MUDEF void mum_term(mumResult* result);

				/* Thread */

					MUDEF muThread mu_thread_create(mumResult* result, void (*start)(void* args), void* args);
					MUDEF muThread mu_thread_destroy(mumResult* result, muThread thread);

					MUDEF void mu_thread_exit(void* ret);
					MUDEF void mu_thread_wait(mumResult* result, muThread thread);
					MUDEF void* mu_thread_get_return_value(mumResult* result, muThread thread);

				/* Mutex */

					MUDEF muMutex mu_mutex_create(mumResult* result);
					MUDEF muMutex mu_mutex_destroy(mumResult* result, muMutex mutex);

					MUDEF void mu_mutex_lock(mumResult* result, muMutex mutex);
					MUDEF void mu_mutex_unlock(mumResult* result, muMutex mutex);

				/* Spinlock */

					MUDEF muSpinlock mu_spinlock_create(mumResult* result);
					MUDEF muSpinlock mu_spinlock_destroy(mumResult* result, muSpinlock spinlock);

					MUDEF void mu_spinlock_lock(mumResult* result, muSpinlock spinlock);
					MUDEF void mu_spinlock_unlock(mumResult* result, muSpinlock spinlock);

			#ifdef __cplusplus
			}
			#endif

		#endif
	#endif

	#ifdef __cplusplus
	extern "C" { // }
	#endif

	#define MUS_VERSION_MAJOR 1
	#define MUS_VERSION_MINOR 0
	#define MUS_VERSION_PATCH 0

	/* C standard library dependencies */

		#if !defined(mu_log10) || \
			!defined(mu_floor) || \
			!defined(mu_pow)   || \
			!defined(mu_fmod)

			#include <math.h>

			#ifndef mu_log10
				#define mu_log10 log10
			#endif

			#ifndef mu_floor
				#define mu_floor floor
			#endif

			#ifndef mu_pow
				#define mu_pow pow
			#endif

			#ifndef mu_fmod
				#define mu_fmod fmod
			#endif

		#endif

	/* Incomplete types */

		typedef struct musContext musContext;

	/* Global variables */

		MUDEF musContext* mus_global_context;

	/* Macros */

		#define muCodePoint size_m
		#define muString size_m

	/* Enums */

		MU_ENUM(musResult,
			MUS_SUCCESS,

			MUS_ALREADY_INITIALIZED,
			MUS_ALLOCATION_FAILED,
			MUS_ALREADY_TERMINATED,
			MUS_NOT_YET_INITIALIZED,

			MUS_UNKNOWN_CHARACTER_ENCODING,

			MUS_CHARACTER_CANNOT_BE_ENCODED,

			MUS_INVALID_CHARACTER_ENCODING,
			MUS_INVALID_CODE_POINT_SIZE,
			MUS_INVALID_DATA_SIZE,
			MUS_INVALID_DATA_POINTER,
			MUS_INVALID_OFFSET,
			MUS_INVALID_ID,
			MUS_INVALID_DATA,

			MUS_INSUFFICIENT_DATA_SIZE,

			MUS_NONMATCHING_CHARACTER_ENCODING,

			MUS_OVERFLOW,

			MUS_MUMA_SUCCESS,
			MUS_MUMA_FAILED_TO_ALLOCATE,
			MUS_MUMA_INVALID_INDEX,
			MUS_MUMA_INVALID_SHIFT_AMOUNT,
			MUS_MUMA_NOT_FOUND
		)

		MU_ENUM(muCharacterEncoding,
			MU_UNKNOWN_CHARACTER_ENCODING,
			MU_ASCII,
			MU_UTF8,
		)

	/* Functions */

		/* Names */

			#ifdef MUS_NAMES
				MUDEF const char* mus_result_get_name(musResult result);
			#endif

		/* Initiation / Termination */

			MUDEF void mus_init(musResult* result);
			MUDEF void mus_term(musResult* result);

		/* String functions */

			/* Creation / Destruction */

				MUDEF muString mu_string_create(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);
				MUDEF muString mu_string_destroy(musResult* result, muString string);
				MUDEF muString mu_string_duplicate(musResult* result, muString string);

			/* Data */

				MUDEF muByte* mu_string_get_raw_data(musResult* result, muString string, size_m* size);

			/* Character encoding */

				MUDEF muCharacterEncoding mu_string_get_character_encoding(musResult* result, muString string);
				MUDEF void mu_string_convert_character_encoding(musResult* result, muString string, muCharacterEncoding desired_encoding);

			/* Code point */

				MUDEF size_m mu_string_get_code_point_length(musResult* result, muString string);

				MUDEF size_m mu_string_get_code_point_offset(musResult* result, muString string, size_m offset, size_m index);

				MUDEF muCodePoint mu_string_get_code_point(musResult* result, muString string, size_m offset);
				MUDEF void mu_string_set_code_point(musResult* result, muString string, muCodePoint code_point, size_m offset);

			/* Insert */

				MUDEF void mu_string_insert_raw_string(musResult* result, muString string, muCharacterEncoding encoding, muByte* data, size_m data_size, size_m offset);

				MUDEF void mu_string_insert_integer(musResult* result, muString string, int64_m i, size_m offset);
				MUDEF void mu_string_insert_float(musResult* result, muString string, float d, size_m decimal_places, size_m offset);
				MUDEF void mu_string_insert_double(musResult* result, muString string, double d, size_m decimal_places, size_m offset);

		/* Raw string functions */

			/* Conversion */

				MUDEF size_m mu_raw_string_get_character_encoding_conversion_size(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, muCharacterEncoding desired_encoding);
				MUDEF void mu_raw_string_convert_character_encoding(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, muCharacterEncoding output_encoding, muByte* output_data, size_m output_data_size);

			/* Integer/Float/Double conversion */

				MUDEF size_m mu_raw_string_get_integer_conversion_size(musResult* result, muCharacterEncoding encoding, int64_m i);
				MUDEF void mu_raw_string_convert_to_integer(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, int64_m i);
				MUDEF int64_m mu_raw_string_get_integer(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);

				MUDEF size_m mu_raw_string_get_float_conversion_size(musResult* result, muCharacterEncoding encoding, float d, size_m decimal_places);
				MUDEF void mu_raw_string_convert_to_float(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, float d, size_m decimal_places);
				MUDEF float mu_raw_string_get_float(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);

				MUDEF size_m mu_raw_string_get_double_conversion_size(musResult* result, muCharacterEncoding encoding, double d, size_m decimal_places);
				MUDEF void mu_raw_string_convert_to_double(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, double d, size_m decimal_places);
				MUDEF double mu_raw_string_get_double(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);

		/* Cross-encoding functions */

			MUDEF size_m mu_character_encoding_get_code_point_size(musResult* result, muCharacterEncoding encoding, muCodePoint code_point);
			MUDEF muCodePoint mu_character_encoding_get_code_point(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);
			MUDEF void mu_character_encoding_set_code_point(musResult* result, muCharacterEncoding encoding, muCodePoint code_point, muByte* data, size_m data_size);

			MUDEF muBool mu_character_encoding_verify_raw_string(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);
			MUDEF size_m mu_character_encoding_get_raw_string_code_point_length(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);
			MUDEF size_m mu_character_encoding_get_raw_string_code_point_offset(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, size_m offset, size_m index);

		/* Encoding-specific functions */

			/* ASCII */

				MUDEF size_m mu_ASCII_get_code_point_size(musResult* result, muCodePoint code_point);
				MUDEF muCodePoint mu_ASCII_get_code_point(musResult* result, muByte* data, size_m data_size);
				MUDEF void mu_ASCII_set_code_point(musResult* result, muCodePoint code_point, muByte* data, size_m data_size);

				MUDEF muBool mu_ASCII_verify_raw_string(musResult* result, muByte* data, size_m data_size);
				MUDEF size_m mu_ASCII_get_raw_string_first_code_point_index(musResult* result, muByte* data, size_m data_size);
				MUDEF size_m mu_ASCII_get_raw_string_code_point_length(musResult* result, muByte* data, size_m data_size);
				MUDEF size_m mu_ASCII_get_raw_string_code_point_offset(musResult* result, muByte* data, size_m data_size, size_m offset, size_m index);

			/* UTF-8 */

				MUDEF size_m mu_UTF8_get_code_point_size(musResult* result, muCodePoint code_point);
				MUDEF muCodePoint mu_UTF8_get_code_point(musResult* result, muByte* data, size_m data_size);
				MUDEF void mu_UTF8_set_code_point(musResult* result, muCodePoint code_point, muByte* data, size_m data_size);

				MUDEF muBool mu_UTF8_verify_raw_string(musResult* result, muByte* data, size_m data_size);
				MUDEF size_m mu_UTF8_get_raw_string_code_point_length(musResult* result, muByte* data, size_m data_size);
				MUDEF size_m mu_UTF8_get_raw_string_code_point_offset(musResult* result, muByte* data, size_m data_size, size_m offset, size_m index);

	#ifdef __cplusplus
	}
	#endif
#endif /* MUS_H */


#ifdef MUS_IMPLEMENTATION

	/* muMemoryAllocator version 1.0.0 implementation */

		#ifndef MUMA_IMPLEMENTATION
			#define MUMA_IMPLEMENTATION

			#ifdef MUMA_IMPLEMENTATION

				/* muUtility version 1.0.0 implementation */

					#ifndef MUU_IMPLEMENTATION
						#define MUU_IMPLEMENTATION

						#ifdef MUU_IMPLEMENTATION
				
							// ...

						#endif /* MUU_IMPLEMENTATION */
					#endif

				#ifdef __cplusplus
					extern "C" {
				#endif

				#ifdef MUMA_NAMES
					MUDEF const char* muma_result_get_name(mumaResult result) {
						switch (result) {
							default: return "MUMA_UNKNOWN"; break;
							case MUMA_SUCCESS: return "MUMA_SUCCESS"; break;
							case MUMA_FAILED_TO_ALLOCATE: return "MUMA_FAILED_TO_ALLOCATE"; break;
							case MUMA_INVALID_INDEX: return "MUMA_INVALID_INDEX"; break;
							case MUMA_INVALID_SHIFT_AMOUNT: return "MUMA_INVALID_SHIFT_AMOUNT"; break;
							case MUMA_NOT_FOUND: return "MUMA_NOT_FOUND"; break;
						}
					}
				#endif

				#ifdef __cplusplus
					}
				#endif

			#endif /* MUMA_IMPLEMENTATION */
		#endif

	/* muMultithreading version 1.0.0 implementation (only if thread safe) */
	// CHANGES:
	// * Removed muma implementation inclusion because this library already has it.
	// * Added alt MU_LOCK definitions for non-thread safe code.

	#ifdef MU_THREADSAFE

		#ifndef MUM_IMPLEMENTATION
			#define MUM_IMPLEMENTATION

			#ifdef MUM_IMPLEMENTATION

				#ifdef __cplusplus
				extern "C" { // }
				#endif

				/* Shared functions */

					/* Names */

						#ifdef MUM_NAMES
							MUDEF const char* mum_result_get_name(mumResult result) {
								switch (result) {
									default: return "MUM_UNKNOWN"; break;
									case MUM_SUCCESS: return "MUM_SUCCESS"; break;
									case MUM_MUMA_SUCCESS: return "MUM_MUMA_SUCCESS"; break;
									case MUM_MUMA_FAILED_TO_ALLOCATE: return "MUM_MUMA_FAILED_TO_ALLOCATE"; break;
									case MUM_MUMA_INVALID_INDEX: return "MUM_MUMA_INVALID_INDEX"; break;
									case MUM_MUMA_INVALID_SHIFT_AMOUNT: return "MUM_MUMA_INVALID_SHIFT_AMOUNT"; break;
									case MUM_MUMA_NOT_FOUND: return "MUM_MUMA_NOT_FOUND"; break;
									case MUM_ALLOCATION_FAILED: return "MUM_ALLOCATION_FAILED"; break;
									case MUM_ALREADY_INITIALIZED: return "MUM_ALREADY_INITIALIZED"; break;
									case MUM_ALREADY_TERMINATED: return "MUM_ALREADY_TERMINATED"; break;
									case MUM_NOT_YET_INITIALIZED: return "MUM_NOT_YET_INITIALIZED"; break;
									case MUM_CREATE_CALL_FAILED: return "MUM_CREATE_CALL_FAILED"; break;
									case MUM_DESTROY_CALL_FAILED: return "MUM_DESTROY_CALL_FAILED"; break;
									case MUM_WAIT_CALL_FAILED: return "MUM_WAIT_CALL_FAILED"; break;
									case MUM_LOCK_CALL_FAILED: return "MUM_LOCK_CALL_FAILED"; break;
									case MUM_UNLOCK_CALL_FAILED: return "MUM_UNLOCK_CALL_FAILED"; break;
									case MUM_GET_RETURN_VALUE_CALL_FAILED: return "MUM_GET_RETURN_VALUE_CALL_FAILED"; break;
									case MUM_INVALID_ID: return "MUM_INVALID_ID"; break;
									case MUM_THREAD_TIMED_OUT: return "MUM_THREAD_TIMED_OUT"; break;
									case MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK: return "MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK"; break;
								}
							}
						#endif

					/* Get result */

						mumResult muma_result_to_mum_result(mumaResult res) {
							switch (res) {
								default: case MUMA_SUCCESS: return MUM_MUMA_SUCCESS; break;
								case MUMA_FAILED_TO_ALLOCATE: return MUM_MUMA_FAILED_TO_ALLOCATE; break;
								case MUMA_INVALID_INDEX: return MUM_MUMA_INVALID_INDEX; break;
								case MUMA_INVALID_SHIFT_AMOUNT: return MUM_MUMA_INVALID_SHIFT_AMOUNT; break;
								case MUMA_NOT_FOUND: return MUM_MUMA_NOT_FOUND; break;
							}
						}

				/* Unix */

				#ifdef MU_UNIX

					#include <pthread.h>

					/* Useful things */

						// These two functions are essentially stolen from:
						// https://github.com/stepancheg/no-mutex-c

						static inline muBool mum_atomic_compare_exchange(int* ptr, int compare, int exchange) {
							return __atomic_compare_exchange_n(ptr, &compare, exchange, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
						}

						static inline void mum_atomic_store(int* ptr, int value) {
							if (value) {}
							__atomic_store_n(ptr, 0, __ATOMIC_SEQ_CST);
						}

						#ifdef MU_THREADSAFE

							#define MU_LOCK pthread_mutex_t lock; muBool lock_active;

							#define MU_LOCK_CREATE(lock, lock_active) \
								if (pthread_mutex_init(&lock, 0) == 0) { \
									lock_active = MU_TRUE; \
								} else { \
									lock_active = MU_FALSE; \
								}

							#define MU_LOCK_DESTROY(lock, lock_active) \
								if (lock_active) { \
									if (pthread_mutex_destroy(&lock) == 0) { \
										lock_active = MU_FALSE; \
									} \
								}

							#define MU_LOCK_LOCK(lock, lock_active) \
								if (lock_active) { \
									pthread_mutex_lock(&lock); \
								}

							#define MU_LOCK_UNLOCK(lock, lock_active) \
								if (lock_active) { \
									pthread_mutex_unlock(&lock); \
								}

						#else

							#define MU_LOCK
							#define MU_LOCK_CREATE(lock, active)
							#define MU_LOCK_DESTROY(lock, active)
							#define MU_LOCK_LOCK(lock, active)
							#define MU_LOCK_UNLOCK(lock, active)

						#endif

					/* Arrays */

						/* Thread */

							struct mu_unix_thread {
								muBool active;
								pthread_t handle;
								void* ret;

								MU_LOCK
							};
							typedef struct mu_unix_thread mu_unix_thread;

							MU_HRARRAY_DEFAULT_FUNC(mu_unix_thread)

						/* Mutex */

							struct mu_unix_mutex {
								muBool active;
								pthread_mutex_t handle;

								MU_LOCK
							};
							typedef struct mu_unix_mutex mu_unix_mutex;

							MU_HRARRAY_DEFAULT_FUNC(mu_unix_mutex)

						/* Spinlock */

							struct mu_unix_spinlock {
								muBool active;
								int locked;

								MU_LOCK
							};
							typedef struct mu_unix_spinlock mu_unix_spinlock;

							MU_HRARRAY_DEFAULT_FUNC(mu_unix_spinlock)

					/* Initiation / Termination */

						/* Setup */

							struct mumContext {
								mu_unix_thread_array threads;
								#define MUM_GTHREADS mum_global_context->threads
								mu_unix_mutex_array mutexes;
								#define MUM_GMUTEXES mum_global_context->mutexes
								mu_unix_spinlock_array spinlocks;
								#define MUM_GSPINLOCKS mum_global_context->spinlocks
							};

						/* API-level */

							mumContext* mum_global_context = MU_NULL_PTR;

							MUDEF void mum_init(mumResult* result) {
								MU_SET_RESULT(result, MUM_SUCCESS)

								MU_ASSERT(mum_global_context == MU_NULL_PTR, result, MUM_ALREADY_INITIALIZED, return;)

								mum_global_context = (mumContext*)mu_malloc(sizeof(mumContext));
								MU_ASSERT(mum_global_context != 0, result, MUM_ALLOCATION_FAILED, return;)

								MUM_GTHREADS = MU_ZERO_STRUCT(mu_unix_thread_array);
								MUM_GMUTEXES = MU_ZERO_STRUCT(mu_unix_mutex_array);
								MUM_GSPINLOCKS = MU_ZERO_STRUCT(mu_unix_spinlock_array);
							}

							MUDEF void mum_term(mumResult* result) {
								MU_SET_RESULT(result, MUM_SUCCESS)

								MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_ALREADY_TERMINATED, return;)

								for (size_m i = 0; i < MUM_GTHREADS.length; i++) {
									mu_thread_destroy(0, i);
								}
								mu_unix_thread_destroy(0, &MUM_GTHREADS);

								for (size_m i = 0; i < MUM_GMUTEXES.length; i++) {
									mu_mutex_destroy(0, i);
								}
								mu_unix_mutex_destroy(0, &MUM_GMUTEXES);

								for (size_m i = 0; i < MUM_GSPINLOCKS.length; i++) {
									mu_spinlock_destroy(0, i);
								}
								mu_unix_spinlock_destroy(0, &MUM_GSPINLOCKS);

								mu_free(mum_global_context);
								mum_global_context = MU_NULL_PTR;
							}

					/* Threads */

						/* API-level */

							MUDEF muThread mu_thread_create(mumResult* result, void (*start)(void* args), void* args) {
								MU_SET_RESULT(result, MUM_SUCCESS)

								MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

								size_m thread = MU_NONE;
								mumaResult muma_res = MUMA_SUCCESS;
								mu_unix_thread_find_push(&muma_res, &MUM_GTHREADS, MU_ZERO_STRUCT(mu_unix_thread), &thread);
								MU_ASSERT(muma_res == MUMA_SUCCESS && thread != MU_NONE, result, muma_result_to_mum_result(muma_res), 
									return MU_NONE;
								)

								mu_unix_thread_hold_element(0, &MUM_GTHREADS, thread);
								void* (*func)(void*);
								mu_memcpy(&func, &start, sizeof(void*));
								if (pthread_create(&MUM_GTHREADS.data[thread].handle, 0, func, args) != 0) {
									MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
									MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
									return MU_NONE;
								}

								MUM_GTHREADS.data[thread].active = MU_TRUE;
								MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
								return thread;
							}

							MUDEF muThread mu_thread_destroy(mumResult* result, muThread thread) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return thread;)
								MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return thread;, mu_unix_thread_)

								MU_ASSERT(pthread_cancel(MUM_GTHREADS.data[thread].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
									MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_) return thread;
								)
								MUM_GTHREADS.data[thread].active = MU_FALSE;

								MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
								return MU_NONE;
							}

							MUDEF void mu_thread_exit(void* ret) {
								pthread_exit(ret);
							}

							MUDEF void mu_thread_wait(mumResult* result, muThread thread) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
								MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return;, mu_unix_thread_)

								MU_ASSERT(pthread_join(MUM_GTHREADS.data[thread].handle, &MUM_GTHREADS.data[thread].ret) == 0, result, MUM_WAIT_CALL_FAILED,
									MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_) return;
								)
								MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
							}

							MUDEF void* mu_thread_get_return_value(mumResult* result, muThread thread) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return MU_NULL_PTR;)
								MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return MU_NULL_PTR;, mu_unix_thread_)

								void* ret = MUM_GTHREADS.data[thread].ret;
								MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
								return ret;
							}

					/* Mutexes */

						/* API-level */

							MUDEF muMutex mu_mutex_create(mumResult* result) {
								MU_SET_RESULT(result, MUM_SUCCESS)
								MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

								size_m mutex = MU_NONE;
								mumaResult muma_res = MUMA_SUCCESS;
								mu_unix_mutex_find_push(&muma_res, &MUM_GMUTEXES, MU_ZERO_STRUCT(mu_unix_mutex), &mutex);
								MU_ASSERT(muma_res == MUMA_SUCCESS && mutex != MU_NONE, result, muma_result_to_mum_result(muma_res),
									return MU_NONE;
								)

								mu_unix_mutex_hold_element(0, &MUM_GMUTEXES, mutex);
								if (pthread_mutex_init(&MUM_GMUTEXES.data[mutex].handle, 0) != 0) {
									MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
									MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
									return MU_NONE;
								}

								MUM_GMUTEXES.data[mutex].active = MU_TRUE;
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
								return mutex;
							}

							MUDEF muMutex mu_mutex_destroy(mumResult* result, muMutex mutex) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return mutex;)
								MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return mutex;, mu_unix_mutex_)

								MU_ASSERT(pthread_mutex_destroy(&MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
									MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_) return mutex;
								)
								MUM_GMUTEXES.data[mutex].active = MU_FALSE;
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
								return MU_NONE;
							}

							// This poses an odd structural challenge. If we make both the lock & unlock
							// function hold the mutex, you reach a deadlock pretty easily, ie:

							// * Thread 1 locks mutex, quickly holding and releasing it.
							// * Thread 2 attempts to lock mutex, holding it and waiting for it to be unlocked,
							// * Thread 1 is done, and attempts to unlock the mutex, so it tries to hold it,
							// but it's still being held by thread 2 who is attempting to lock it, so you reach
							// a deadlock.

							// This can be solved by:
							// A) Making the 'lock mutex' function release the function early, allowing it to
							// wait to lock it without holding it during that wait time.
							// B) Make the 'unlock mutex' function not hold/release the mutex.

							// I've chosen A because the only way it *doesn't* work well is if the user deletes
							// a mutex while a thread is running with it locked AND another one is waiting on
							// it, which is already a horrible situation to be in. Pretty sure deleting a mutex
							// with a thread having it locked is already undefined behavior anyway.

							// Note that this same logic applies to the other locks.

							MUDEF void mu_mutex_lock(mumResult* result, muMutex mutex) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
								MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_unix_mutex_)

								MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
								MU_ASSERT(pthread_mutex_lock(&MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_LOCK_CALL_FAILED, 
									return;
								)
							}

							MUDEF void mu_mutex_unlock(mumResult* result, muMutex mutex) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
								MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_unix_mutex_)

								MU_ASSERT(pthread_mutex_unlock(&MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_UNLOCK_CALL_FAILED, 
									MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_) return;
								)
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
							}

					/* Spinlocks */

						/* API-level */

							MUDEF muSpinlock mu_spinlock_create(mumResult* result) {
								MU_SET_RESULT(result, MUM_SUCCESS)
								MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

								size_m spinlock = MU_NONE;
								mumaResult muma_res = MUMA_SUCCESS;
								mu_unix_spinlock_find_push(&muma_res, &MUM_GSPINLOCKS, MU_ZERO_STRUCT(mu_unix_spinlock), &spinlock);
								MU_ASSERT(muma_res == MUMA_SUCCESS && spinlock != MU_NONE, result, muma_result_to_mum_result(muma_res),
									return MU_NONE;
								)

								mu_unix_spinlock_hold_element(0, &MUM_GSPINLOCKS, spinlock);
								MUM_GSPINLOCKS.data[spinlock].locked = 0;

								MUM_GSPINLOCKS.data[spinlock].active = MU_TRUE;
								MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
								return spinlock;
							}

							MUDEF muSpinlock mu_spinlock_destroy(mumResult* result, muSpinlock spinlock) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return spinlock;)
								MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return spinlock;, mu_unix_spinlock_)

								MUM_GSPINLOCKS.data[spinlock].locked = 0;
								MUM_GSPINLOCKS.data[spinlock].active = MU_FALSE;
								MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
								return MU_NONE;
							}

							MUDEF void mu_spinlock_lock(mumResult* result, muSpinlock spinlock) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
								MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_unix_spinlock_)

								MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
								// Note that this is a bit less safe than mutexes, but there's little I can do
								// in this position.
								while (!mum_atomic_compare_exchange(&MUM_GSPINLOCKS.data[spinlock].locked, 0, 1)) {}
							}

							MUDEF void mu_spinlock_unlock(mumResult* result, muSpinlock spinlock) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
								MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_unix_spinlock_)

								mum_atomic_store(&MUM_GSPINLOCKS.data[spinlock].locked, 0);
								MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
							}

				#endif /* MU_UNIX */

				#ifdef MU_WIN32

					#include <windows.h>

					/* Useful things */

						static inline muBool mum_atomic_compare_exchange(LONG volatile* ptr, LONG compare, LONG exchange) {
							return InterlockedCompareExchange(ptr, exchange, compare) != exchange;
						}

						// Kind of a hack
						static inline void mum_atomic_store(LONG volatile* ptr, long value) {
							if (value == 0) {
								_interlockedbittestandreset(ptr, 0);
							} else {
								_interlockedbittestandset(ptr, 0);
							}
						}

						#ifdef MU_THREADSAFE

							#define MU_LOCK HANDLE lock; muBool lock_active;

							#define MU_LOCK_CREATE(lock, lock_active) \
								lock = CreateMutex(0, MU_FALSE, 0); \
								if (lock != 0) { \
									lock_active = MU_TRUE; \
								} else { \
									lock_active = MU_FALSE; \
								}

							#define MU_LOCK_DESTROY(lock, lock_active) \
								if (lock_active) { \
									if (CloseHandle(lock) != 0) { \
										lock_active = MU_FALSE; \
									} \
								}

							#define MU_LOCK_LOCK(lock, lock_active) \
								if (lock_active) { \
									WaitForSingleObject(lock, INFINITE); \
								}

							#define MU_LOCK_UNLOCK(lock, lock_active) \
								if (lock_active) { \
									ReleaseMutex(lock); \
								}

						#else

							#define MU_LOCK
							#define MU_LOCK_CREATE(lock, active)
							#define MU_LOCK_DESTROY(lock, active)
							#define MU_LOCK_LOCK(lock, active)
							#define MU_LOCK_UNLOCK(lock, active)

						#endif

					/* Arrays */

						/* Thread */

							struct mu_win32_thread {
								muBool active;
								HANDLE handle;

								MU_LOCK
							};
							typedef struct mu_win32_thread mu_win32_thread;

							MU_HRARRAY_DEFAULT_FUNC(mu_win32_thread)

						/* Mutex */

							struct mu_win32_mutex {
								muBool active;
								HANDLE handle;

								MU_LOCK
							};
							typedef struct mu_win32_mutex mu_win32_mutex;

							MU_HRARRAY_DEFAULT_FUNC(mu_win32_mutex)

						/* Spinlock */

							struct mu_win32_spinlock {
								muBool active;
								LONG volatile locked;

								MU_LOCK
							};
							typedef struct mu_win32_spinlock mu_win32_spinlock;

							MU_HRARRAY_DEFAULT_FUNC(mu_win32_spinlock)

					/* Initiation / Termination */

						/* Setup */

							struct mumContext {
								mu_win32_thread_array threads;
								#define MUM_GTHREADS mum_global_context->threads
								mu_win32_mutex_array mutexes;
								#define MUM_GMUTEXES mum_global_context->mutexes
								mu_win32_spinlock_array spinlocks;
								#define MUM_GSPINLOCKS mum_global_context->spinlocks
							};

						/* API-level */

							mumContext* mum_global_context = MU_NULL_PTR;

							MUDEF void mum_init(mumResult* result) {
								MU_SET_RESULT(result, MUM_SUCCESS)

								MU_ASSERT(mum_global_context == MU_NULL_PTR, result, MUM_ALREADY_INITIALIZED, return;)

								mum_global_context = (mumContext*)mu_malloc(sizeof(mumContext));
								MU_ASSERT(mum_global_context != 0, result, MUM_ALLOCATION_FAILED, return;)

								MUM_GTHREADS = MU_ZERO_STRUCT(mu_win32_thread_array);
								MUM_GMUTEXES = MU_ZERO_STRUCT(mu_win32_mutex_array);
								MUM_GSPINLOCKS = MU_ZERO_STRUCT(mu_win32_spinlock_array);
							}

							MUDEF void mum_term(mumResult* result) {
								MU_SET_RESULT(result, MUM_SUCCESS)

								MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_ALREADY_TERMINATED, return;)

								for (size_m i = 0; i < MUM_GTHREADS.length; i++) {
									mu_thread_destroy(0, i);
								}
								mu_win32_thread_destroy(0, &MUM_GTHREADS);

								for (size_m i = 0; i < MUM_GMUTEXES.length; i++) {
									mu_mutex_destroy(0, i);
								}
								mu_win32_mutex_destroy(0, &MUM_GMUTEXES);

								for (size_m i = 0; i < MUM_GSPINLOCKS.length; i++) {
									mu_spinlock_destroy(0, i);
								}
								mu_win32_spinlock_destroy(0, &MUM_GSPINLOCKS);

								mu_free(mum_global_context);
								mum_global_context = MU_NULL_PTR;
							}

					/* Threads */

						/* API-level */

							MUDEF muThread mu_thread_create(mumResult* result, void (*start)(void* args), void* args) {
								MU_SET_RESULT(result, MUM_SUCCESS)

								MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

								size_m thread = MU_NONE;
								mumaResult muma_res = MUMA_SUCCESS;
								mu_win32_thread_find_push(&muma_res, &MUM_GTHREADS, MU_ZERO_STRUCT(mu_win32_thread), &thread);
								MU_ASSERT(muma_res == MUMA_SUCCESS && thread != MU_NONE, result, muma_result_to_mum_result(muma_res), 
									return MU_NONE;
								)

								mu_win32_thread_hold_element(0, &MUM_GTHREADS, thread);
								LPTHREAD_START_ROUTINE lp_start;
								mu_memcpy(&lp_start, &start, sizeof(void*));
								DWORD id;
								MUM_GTHREADS.data[thread].handle = CreateThread(0, 0, lp_start, args, 0, &id);
								if (MUM_GTHREADS.data[thread].handle == 0) {
									MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_);
									MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
									return MU_NONE;
								}

								MUM_GTHREADS.data[thread].active = MU_TRUE;
								MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
								return thread;
							}

							MUDEF muThread mu_thread_destroy(mumResult* result, muThread thread) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return thread;)
								MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return thread;, mu_win32_thread_)

								MU_ASSERT(CloseHandle(MUM_GTHREADS.data[thread].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
									MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_) return thread;
								)
								MUM_GTHREADS.data[thread].active = MU_FALSE;

								MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
								return MU_NONE;
							}

							MUDEF void mu_thread_exit(void* ret) {
								DWORD d;
								mu_memcpy(&d, &ret, sizeof(DWORD));
								ExitThread(d);
							}

							MUDEF void mu_thread_wait(mumResult* result, muThread thread) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
								MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return;, mu_win32_thread_)

								DWORD wait_result = WaitForSingleObject(MUM_GTHREADS.data[thread].handle, INFINITE);

								switch (wait_result) {
									case WAIT_TIMEOUT: {
										MU_SET_RESULT(result, MUM_THREAD_TIMED_OUT)
									} break;

									case WAIT_FAILED: {
										MU_SET_RESULT(result, MUM_WAIT_CALL_FAILED)
									} break;
								}

								MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
							}

							MUDEF void* mu_thread_get_return_value(mumResult* result, muThread thread) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return MU_NULL_PTR;)
								MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return MU_NULL_PTR;, mu_win32_thread_)

								DWORD exit_code = 0;
								MU_ASSERT(GetExitCodeThread(MUM_GTHREADS.data[thread].handle, &exit_code) != 0, result, MUM_GET_RETURN_VALUE_CALL_FAILED, 
									MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_) return MU_NULL_PTR;
								)

								void* p;
								mu_memcpy(&p, &exit_code, sizeof(DWORD));

								MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
								return p;
							}

					/* Mutexes */

						/* API-level */

							MUDEF muMutex mu_mutex_create(mumResult* result) {
								MU_SET_RESULT(result, MUM_SUCCESS)
								MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

								size_m mutex = MU_NONE;
								mumaResult muma_res = MUMA_SUCCESS;
								mu_win32_mutex_find_push(&muma_res, &MUM_GMUTEXES, MU_ZERO_STRUCT(mu_win32_mutex), &mutex);
								MU_ASSERT(muma_res == MUMA_SUCCESS && mutex != MU_NONE, result, muma_result_to_mum_result(muma_res),
									return MU_NONE;
								)

								mu_win32_mutex_hold_element(0, &MUM_GMUTEXES, mutex);
								MUM_GMUTEXES.data[mutex].handle = CreateMutex(0, MU_FALSE, 0);
								if (MUM_GMUTEXES.data[mutex].handle == 0) {
									MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
									MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
									return MU_NONE;
								}

								MUM_GMUTEXES.data[mutex].active = MU_TRUE;
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
								return mutex;
							}

							MUDEF muMutex mu_mutex_destroy(mumResult* result, muMutex mutex) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return mutex;)
								MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return mutex;, mu_win32_mutex_)

								MU_ASSERT(CloseHandle(MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
									MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_) return mutex;
								)
								MUM_GMUTEXES.data[mutex].active = MU_FALSE;
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
								return MU_NONE;
							}

							MUDEF void mu_mutex_lock(mumResult* result, muMutex mutex) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
								MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_win32_mutex_)

								MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
								DWORD wait_result = WaitForSingleObject(MUM_GMUTEXES.data[mutex].handle, INFINITE);

								switch (wait_result) {
									// The mutex has most likely been closed. This should pretty much never happen with
									// the way mum is set up, but if it has, that's really bad. Most likely, rather mum
									// is not working as intended at all, or the user has modified things that they
									// shouldn't.
									case WAIT_FAILED: {
										MU_SET_RESULT(result, MUM_INVALID_ID)
									} break;

									// The thread holding the mutex has died. This can be due to a few things:
									// * The thread crashed or otherwise imploded in on itself.
									// * I forgot to put an unlock call on an error return case.
									// * The user has fiddled around with values they shouldn't.
									// Either way, this is REALLY, REALLY bad, and will lead to sporadic random bugs &
									// crashes.
									// Note: we still have ownership due to this, but ehhhhh.
									// https://devblogs.microsoft.com/oldnewthing/20050912-14/?p=34253
									// (Raymond Chen is awesome btw)
									case WAIT_ABANDONED: {
										MU_SET_RESULT(result, MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK)
									} break;
								}
							}

							MUDEF void mu_mutex_unlock(mumResult* result, muMutex mutex) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
								MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_win32_mutex_)

								MU_ASSERT(ReleaseMutex(MUM_GMUTEXES.data[mutex].handle), result, MUM_UNLOCK_CALL_FAILED, 
									MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_) return;
								)
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
							}

					/* Spinlocks */

						/* API-level */

							MUDEF muSpinlock mu_spinlock_create(mumResult* result) {
								MU_SET_RESULT(result, MUM_SUCCESS)
								MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

								size_m spinlock = MU_NONE;
								mumaResult muma_res = MUMA_SUCCESS;
								mu_win32_spinlock_find_push(&muma_res, &MUM_GSPINLOCKS, MU_ZERO_STRUCT(mu_win32_spinlock), &spinlock);
								MU_ASSERT(muma_res == MUMA_SUCCESS && spinlock != MU_NONE, result, muma_result_to_mum_result(muma_res),
									return MU_NONE;
								)

								mu_win32_spinlock_hold_element(0, &MUM_GSPINLOCKS, spinlock);
								MUM_GSPINLOCKS.data[spinlock].locked = 0;

								MUM_GSPINLOCKS.data[spinlock].active = MU_TRUE;
								MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
								return spinlock;
							}

							MUDEF muSpinlock mu_spinlock_destroy(mumResult* result, muSpinlock spinlock) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return spinlock;)
								MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return spinlock;, mu_win32_spinlock_)

								MUM_GSPINLOCKS.data[spinlock].locked = 0;
								MUM_GSPINLOCKS.data[spinlock].active = MU_FALSE;
								MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
								return MU_NONE;
							}

							MUDEF void mu_spinlock_lock(mumResult* result, muSpinlock spinlock) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
								MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_win32_spinlock_)

								MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
								while (!mum_atomic_compare_exchange(&MUM_GSPINLOCKS.data[spinlock].locked, 0, 1)) {}
							}

							MUDEF void mu_spinlock_unlock(mumResult* result, muSpinlock spinlock) {
								MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
								MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_win32_spinlock_)

								mum_atomic_store(&MUM_GSPINLOCKS.data[spinlock].locked, 0);
								MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
							}

				#endif /* MU_WIN32 */

				#ifdef __cplusplus
				}
				#endif

			#endif /* MUM_IMPLEMENTATION */
		#endif

	#else

		#define MU_LOCK
		#define MU_LOCK_CREATE(...)
		#define MU_LOCK_DESTROY(...)
		#define MU_LOCK_LOCK(...)
		#define MU_LOCK_UNLOCK(...)

	#endif

	#ifdef __cplusplus
	extern "C" { // }
	#endif

	/* Result conversion */

		musResult muma_result_to_mus_result(mumaResult res) {
			switch (res) {
				default: case MUMA_SUCCESS: return MUS_MUMA_SUCCESS; break;
				case MUMA_FAILED_TO_ALLOCATE: return MUS_MUMA_FAILED_TO_ALLOCATE; break;
				case MUMA_INVALID_INDEX: return MUS_MUMA_INVALID_INDEX; break;
				case MUMA_INVALID_SHIFT_AMOUNT: return MUS_MUMA_INVALID_SHIFT_AMOUNT; break;
				case MUMA_NOT_FOUND: return MUS_MUMA_NOT_FOUND; break;
			}
		}

	/* Functions */

		/* Names */

			#ifdef MUS_NAMES
				MUDEF const char* mus_result_get_name(musResult result) {
					switch (result) {
						default: return "MUS_UNKNOWN"; break;
						case MUS_SUCCESS: return "MUS_SUCCESS"; break;
						case MUS_ALREADY_INITIALIZED: return "MUS_ALREADY_INITIALIZED"; break;
						case MUS_ALLOCATION_FAILED: return "MUS_ALLOCATION_FAILED"; break;
						case MUS_ALREADY_TERMINATED: return "MUS_ALREADY_TERMINATED"; break;
						case MUS_NOT_YET_INITIALIZED: return "MUS_NOT_YET_INITIALIZED"; break;
						case MUS_UNKNOWN_CHARACTER_ENCODING: return "MUS_UNKNOWN_CHARACTER_ENCODING"; break;
						case MUS_CHARACTER_CANNOT_BE_ENCODED: return "MUS_CHARACTER_CANNOT_BE_ENCODED"; break;
						case MUS_INVALID_CHARACTER_ENCODING: return "MUS_INVALID_CHARACTER_ENCODING"; break;
						case MUS_INVALID_CODE_POINT_SIZE: return "MUS_INVALID_CODE_POINT_SIZE"; break;
						case MUS_INVALID_DATA_SIZE: return "MUS_INVALID_DATA_SIZE"; break;
						case MUS_INVALID_DATA_POINTER: return "MUS_INVALID_DATA_POINTER"; break;
						case MUS_INVALID_OFFSET: return "MUS_INVALID_OFFSET"; break;
						case MUS_INVALID_ID: return "MUS_INVALID_ID"; break;
						case MUS_INVALID_DATA: return "MUS_INVALID_DATA"; break;
						case MUS_INSUFFICIENT_DATA_SIZE: return "MUS_INSUFFICIENT_DATA_SIZE"; break;
						case MUS_NONMATCHING_CHARACTER_ENCODING: return "MUS_NONMATCHING_CHARACTER_ENCODING"; break;
						case MUS_MUMA_FAILED_TO_ALLOCATE: return "MUS_MUMA_FAILED_TO_ALLOCATE"; break;
						case MUS_MUMA_INVALID_INDEX: return "MUS_MUMA_INVALID_INDEX"; break;
						case MUS_MUMA_INVALID_SHIFT_AMOUNT: return "MUS_MUMA_INVALID_SHIFT_AMOUNT"; break;
						case MUS_MUMA_NOT_FOUND: return "MUS_MUMA_NOT_FOUND"; break;
					}
				}
			#endif

		/* Structs */

			muBool mu_bstring_comp(muByte b0, muByte b1) {
				return b0 == b1;
			}

			mu_dynamic_array_declaration(mus_byte_string, muByte, mus_bstring_, mu_bstring_comp)

			struct mus_string {
				muBool active;

				muCharacterEncoding encoding;
				mus_byte_string bytes;
				size_m code_point_length;

				MU_LOCK
			};
			typedef struct mus_string mus_string;

			MU_HRARRAY_DEFAULT_FUNC(mus_string)

		/* Initiation / Termination */

			struct musContext {
				mus_string_array strings;
				#define MUS_GSTRINGS mus_global_context->strings
			};

			musContext* mus_global_context = MU_NULL_PTR;

			MUDEF void mus_init(musResult* result) {
				MU_SET_RESULT(result, MUS_SUCCESS)

				MU_ASSERT(mus_global_context == MU_NULL_PTR, result, MUS_ALREADY_INITIALIZED, return;)

				mus_global_context = (musContext*)mu_malloc(sizeof(musContext));
				MU_ASSERT(mus_global_context != 0, result, MUS_ALLOCATION_FAILED, return;)

				MUS_GSTRINGS = MU_ZERO_STRUCT(mus_string_array);
			}

			MUDEF void mus_term(musResult* result) {
				MU_SET_RESULT(result, MUS_SUCCESS)
				MU_ASSERT(mus_global_context != MU_NULL_PTR, result, MUS_ALREADY_TERMINATED, return;)

				for (size_m i = 0; i < MUS_GSTRINGS.length; i++) {
					mu_string_destroy(0, i);
				}
				mus_string_destroy(0, &MUS_GSTRINGS);

				mu_free(mus_global_context);
				mus_global_context = MU_NULL_PTR;
			}

		/* String functions */

			/* Creation/Destruction */

				MUDEF muString mu_string_create(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return MU_NONE;)
					mumaResult muma_res = MUMA_SUCCESS;
					musResult res = MUS_SUCCESS;
					
					MU_ASSERT(mu_character_encoding_verify_raw_string(&res, encoding, data, data_size) == MU_TRUE, result, MUS_INVALID_CHARACTER_ENCODING, return MU_NONE;)
					MU_ASSERT(res == MUS_SUCCESS, result, res, return MU_NONE;)

					mus_string s_string = MU_ZERO_STRUCT(mus_string);
					s_string.active = MU_TRUE;

					s_string.encoding = encoding;
					s_string.code_point_length = mu_character_encoding_get_raw_string_code_point_length(&res, encoding, data, data_size);
					MU_ASSERT(res == MUS_SUCCESS, result, res, return MU_NONE;)
					
					s_string.bytes = mus_bstring_create(&muma_res, data_size);
					MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), return MU_NONE;)
					mu_memcpy(s_string.bytes.data, data, data_size);

					size_m string = MU_NONE;
					mus_string_find_push(&muma_res, &MUS_GSTRINGS, MU_ZERO_STRUCT(mus_string), &string);
					MU_ASSERT(muma_res == MUMA_SUCCESS && string != MU_NONE, result, muma_result_to_mus_result(muma_res), 
						mus_bstring_destroy(0, s_string.bytes);
					)

					mus_string_hold_element(0, &MUS_GSTRINGS, string);
					MUS_GSTRINGS.data[string] = s_string;
					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
					return string;
				}

				MUDEF muString mu_string_destroy(musResult* result, muString string) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return string;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return string;, mus_string_)
					mumaResult muma_res = MUMA_SUCCESS;

					mus_bstring_destroy(&muma_res, MUS_GSTRINGS.data[string].bytes);
					MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return string;)
					MUS_GSTRINGS.data[string].active = MU_FALSE;

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
					return MU_NONE;
				}

				MUDEF muString mu_string_duplicate(musResult* result, muString string) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return MU_NONE;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return MU_NONE;, mus_string_)
					musResult mus_res = MUS_SUCCESS;

					muString dupe = mu_string_create(&mus_res, 
						MUS_GSTRINGS.data[string].encoding,
						MUS_GSTRINGS.data[string].bytes.data,
						MUS_GSTRINGS.data[string].bytes.length
					);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return MU_NONE;)

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
					return dupe;
				}

			/* Data */

				MUDEF muByte* mu_string_get_raw_data(musResult* result, muString string, size_m* size) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return MU_NULL_PTR;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return MU_NULL_PTR;, mus_string_)

					muByte* data = MUS_GSTRINGS.data[string].bytes.data;
					MU_SET_RESULT(size, MUS_GSTRINGS.data[string].bytes.length)

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
					return data;
				}

			/* Character encoding */

				MUDEF muCharacterEncoding mu_string_get_character_encoding(musResult* result, muString string) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return MU_UNKNOWN_CHARACTER_ENCODING;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return MU_UNKNOWN_CHARACTER_ENCODING;, mus_string_)

					muCharacterEncoding encoding = MUS_GSTRINGS.data[string].encoding;

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
					return encoding;
				}

				MUDEF void mu_string_convert_character_encoding(musResult* result, muString string, muCharacterEncoding desired_encoding) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)
					musResult mus_res = MUS_SUCCESS;
					mumaResult muma_res = MUMA_SUCCESS;

					size_m new_size = mu_raw_string_get_character_encoding_conversion_size(&mus_res, 
						MUS_GSTRINGS.data[string].encoding,
						MUS_GSTRINGS.data[string].bytes.data,
						MUS_GSTRINGS.data[string].bytes.length,
						desired_encoding
					);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)
					if (MUS_GSTRINGS.data[string].encoding == desired_encoding) {
						return;
					}

					mus_byte_string new_bytes = mus_bstring_create(&muma_res, new_size);
					MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					mu_raw_string_convert_character_encoding(&mus_res, 
						MUS_GSTRINGS.data[string].encoding,
						MUS_GSTRINGS.data[string].bytes.data,
						MUS_GSTRINGS.data[string].bytes.length,
						desired_encoding,
						new_bytes.data,
						new_bytes.length
					);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, mus_bstring_destroy(0, new_bytes); MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					size_m new_code_point_length = mu_character_encoding_get_raw_string_code_point_length(&mus_res,
						desired_encoding, new_bytes.data, new_bytes.length
					);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, mus_bstring_destroy(0, new_bytes); MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					mus_bstring_destroy(0, MUS_GSTRINGS.data[string].bytes);
					MUS_GSTRINGS.data[string].bytes = new_bytes;
					MUS_GSTRINGS.data[string].encoding = desired_encoding;
					MUS_GSTRINGS.data[string].code_point_length = new_code_point_length;

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
				}

			/* Code point */

				MUDEF size_m mu_string_get_code_point_length(musResult* result, muString string) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return 0;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return 0;, mus_string_)

					size_m code_point_length = MUS_GSTRINGS.data[string].code_point_length;

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
					return code_point_length;
				}

				MUDEF size_m mu_string_get_code_point_offset(musResult* result, muString string, size_m offset, size_m index) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return 0;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return 0;, mus_string_)
					musResult mus_res = MUS_SUCCESS;

					size_m ret_offset = mu_character_encoding_get_raw_string_code_point_offset(&mus_res,
						MUS_GSTRINGS.data[string].encoding,
						MUS_GSTRINGS.data[string].bytes.data,
						MUS_GSTRINGS.data[string].bytes.length,
						offset, index
					);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return 0;)

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
					return ret_offset;
				}

				MUDEF muCodePoint mu_string_get_code_point(musResult* result, muString string, size_m offset) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return 0;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return 0;, mus_string_)
					musResult mus_res = MUS_SUCCESS;

					MU_ASSERT(offset < MUS_GSTRINGS.data[string].bytes.length, result, MUS_INVALID_OFFSET, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return 0;)

					muCodePoint cp = mu_character_encoding_get_code_point(&mus_res, 
						MUS_GSTRINGS.data[string].encoding,
						&MUS_GSTRINGS.data[string].bytes.data[offset],
						MUS_GSTRINGS.data[string].bytes.length-offset
					);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return 0;)

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
					return cp;
				}

				MUDEF void mu_string_set_code_point(musResult* result, muString string, muCodePoint code_point, size_m offset) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)
					musResult mus_res = MUS_SUCCESS;
					mumaResult muma_res = MUMA_SUCCESS;

					MU_ASSERT(offset < MUS_GSTRINGS.data[string].bytes.length, result, MUS_INVALID_OFFSET, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					// Get sizes of current code point and the code point to be set

					muCodePoint prev_cp = mu_character_encoding_get_code_point(&mus_res, 
						MUS_GSTRINGS.data[string].encoding,
						&MUS_GSTRINGS.data[string].bytes.data[offset],
						MUS_GSTRINGS.data[string].bytes.length-offset
					);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					size_m prev_cp_size = mu_character_encoding_get_code_point_size(&mus_res, MUS_GSTRINGS.data[string].encoding, prev_cp);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)
					size_m cp_size = mu_character_encoding_get_code_point_size(&mus_res, MUS_GSTRINGS.data[string].encoding, code_point);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					// Compare sizes and shift data accordingly

					if (prev_cp_size > cp_size) {
						MUS_GSTRINGS.data[string].bytes = mus_bstring_lshift(&muma_res, 
							MUS_GSTRINGS.data[string].bytes,
							offset + prev_cp_size,
							prev_cp_size - cp_size
						);
						MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)
					} else if (prev_cp_size < cp_size) {
						MUS_GSTRINGS.data[string].bytes = mus_bstring_rshift(&muma_res,
							MUS_GSTRINGS.data[string].bytes,
							offset + prev_cp_size,
							cp_size - prev_cp_size
						);
						MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)
					}

					// Set code point

					mu_character_encoding_set_code_point(&mus_res, 
						MUS_GSTRINGS.data[string].encoding,
						code_point,
						&MUS_GSTRINGS.data[string].bytes.data[offset],
						MUS_GSTRINGS.data[string].bytes.length-offset
					);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
				}

			/* Insert */

				MUDEF void mu_string_insert_raw_string(musResult* result, muString string, muCharacterEncoding encoding, muByte* data, size_m data_size, size_m offset) {
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
					MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)

					mumaResult muma_res = MUMA_SUCCESS;
					musResult mus_res = MUS_SUCCESS;

					MU_ASSERT(encoding == MUS_GSTRINGS.data[string].encoding, result, MUS_NONMATCHING_CHARACTER_ENCODING, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					size_m first_offset = mu_character_encoding_get_raw_string_code_point_offset(&mus_res, encoding, data, data_size, 0, 0);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					MUS_GSTRINGS.data[string].bytes = mus_bstring_multiinsert(&muma_res, 
						MUS_GSTRINGS.data[string].bytes, offset,
						&data[first_offset], data_size-first_offset
					);
					MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
				}

				MUDEF void mu_string_insert_integer(musResult* result, muString string, int64_m i, size_m offset) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)

					mumaResult muma_res = MUMA_SUCCESS;
					musResult mus_res = MUS_SUCCESS;

					size_m int_size = mu_raw_string_get_integer_conversion_size(&mus_res, MUS_GSTRINGS.data[string].encoding, i);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					MUS_GSTRINGS.data[string].bytes = mus_bstring_rshift(&muma_res,
						MUS_GSTRINGS.data[string].bytes, offset, int_size
					);
					MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					mu_raw_string_convert_to_integer(&mus_res, 
						MUS_GSTRINGS.data[string].encoding,
						&MUS_GSTRINGS.data[string].bytes.data[offset],
						MUS_GSTRINGS.data[string].bytes.length-offset,
						i
					);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res,
						MUS_GSTRINGS.data[string].bytes = mus_bstring_lshift(0, MUS_GSTRINGS.data[string].bytes, offset+int_size, int_size);
						MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
						return;
					)

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
				}

				MUDEF void mu_string_insert_float(musResult* result, muString string, float d, size_m decimal_places, size_m offset) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)

					mumaResult muma_res = MUMA_SUCCESS;
					musResult mus_res = MUS_SUCCESS;

					size_m int_size = mu_raw_string_get_float_conversion_size(&mus_res, MUS_GSTRINGS.data[string].encoding, d, decimal_places);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					MUS_GSTRINGS.data[string].bytes = mus_bstring_rshift(&muma_res,
						MUS_GSTRINGS.data[string].bytes, offset, int_size
					);
					MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					mu_raw_string_convert_to_float(&mus_res, 
						MUS_GSTRINGS.data[string].encoding,
						&MUS_GSTRINGS.data[string].bytes.data[offset],
						MUS_GSTRINGS.data[string].bytes.length-offset,
						d, decimal_places
					);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res,
						MUS_GSTRINGS.data[string].bytes = mus_bstring_lshift(0, MUS_GSTRINGS.data[string].bytes, offset+int_size, int_size);
						MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
						return;
					)

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
				}

				MUDEF void mu_string_insert_double(musResult* result, muString string, double d, size_m decimal_places, size_m offset) {
					MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
					MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)

					mumaResult muma_res = MUMA_SUCCESS;
					musResult mus_res = MUS_SUCCESS;

					size_m int_size = mu_raw_string_get_double_conversion_size(&mus_res, MUS_GSTRINGS.data[string].encoding, d, decimal_places);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					MUS_GSTRINGS.data[string].bytes = mus_bstring_rshift(&muma_res,
						MUS_GSTRINGS.data[string].bytes, offset, int_size
					);
					MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

					mu_raw_string_convert_to_double(&mus_res, 
						MUS_GSTRINGS.data[string].encoding,
						&MUS_GSTRINGS.data[string].bytes.data[offset],
						MUS_GSTRINGS.data[string].bytes.length-offset,
						d, decimal_places
					);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res,
						MUS_GSTRINGS.data[string].bytes = mus_bstring_lshift(0, MUS_GSTRINGS.data[string].bytes, offset+int_size, int_size);
						MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
						return;
					)

					MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
				}

		/* Raw string functions */

			/* Conversion */

				MUDEF size_m mu_raw_string_get_character_encoding_conversion_size(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, muCharacterEncoding desired_encoding) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)
					musResult res = MUS_SUCCESS;

					size_m i = mu_character_encoding_get_raw_string_code_point_offset(&res, encoding, data, data_size, 0, 0);
					MU_ASSERT(res == MUS_SUCCESS, result, res, return 0;)

					size_m output_size = 0;

					for (; i < data_size;) {
						muCodePoint cp = mu_character_encoding_get_code_point(&res, encoding, &data[i], data_size-i);
						MU_ASSERT(res == MUS_SUCCESS, result, res, return 0;)

						size_m cp_size = mu_character_encoding_get_code_point_size(&res, desired_encoding, cp);
						MU_ASSERT(res == MUS_SUCCESS, result, res, return 0;)
						output_size += cp_size;

						cp_size = mu_character_encoding_get_code_point_size(&res, encoding, cp);
						MU_ASSERT(res == MUS_SUCCESS, result, res, return 0;)

						i += cp_size;
					}

					return output_size;
				}

				MUDEF void mu_raw_string_convert_character_encoding(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, muCharacterEncoding output_encoding, muByte* output_data, size_m output_data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
					MU_ASSERT(output_data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
					MU_ASSERT(output_data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
					musResult res = MUS_SUCCESS;

					size_m input_i = mu_character_encoding_get_raw_string_code_point_offset(&res, encoding, data, data_size, 0, 0);
					MU_ASSERT(res == MUS_SUCCESS, result, res, return;)

					size_m output_i = 0;

					for (; input_i < data_size;) {
						muCodePoint input_cp = mu_character_encoding_get_code_point(&res, encoding, &data[input_i], data_size-input_i);
						MU_ASSERT(res == MUS_SUCCESS, result, res, return;)

						size_m input_cp_size = mu_character_encoding_get_code_point_size(&res, encoding, input_cp);
						MU_ASSERT(res == MUS_SUCCESS, result, res, return;)

						size_m output_cp_size = mu_character_encoding_get_code_point_size(&res, output_encoding, input_cp);
						MU_ASSERT(res == MUS_SUCCESS, result, res, return;)

						input_i += input_cp_size;
						MU_ASSERT((output_data_size - output_i) >= output_cp_size, result, MUS_INSUFFICIENT_DATA_SIZE, return;)

						mu_character_encoding_set_code_point(&res, output_encoding, input_cp, &output_data[output_i], output_data_size-output_i);
						MU_ASSERT(res == MUS_SUCCESS, result, res, return;)
						output_i += output_cp_size;
					}
				}

			/* Integer/Float/Double conversion */

				// For these functions, ASCII/UTF-8 is assumed. As of the time of writing this,
				// these are the only encoding formats supported by muString, and their
				// implementation for the characters concerned (0-9 and .) are the same, storing 1
				// byte per character.

				#define MUS_MAX_FLOAT_DECIMAL_POINTS 7
				#define MUS_MAX_DOUBLE_DECIMAL_POINTS 16

				MUDEF size_m mu_raw_string_get_integer_conversion_size(musResult* result, muCharacterEncoding encoding, int64_m i) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					if (encoding) {}

					uint8_m add = 0;
					uint64_m ui;
					if (i >= 0) {
						ui = (uint64_m)i;
					} else {
						add = 1;
						ui = (uint64_m)((int64_m)(-i));
					}

					// https://stackoverflow.com/a/14564888
					uint8_m digits = ((uint8_m)(mu_log10((double)ui))) + 1;
					return (size_m)(digits + add);
				}

				MUDEF void mu_raw_string_convert_to_integer(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, int64_m i) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)

					musResult mus_res = MUS_SUCCESS;
					size_m isize = mu_raw_string_get_integer_conversion_size(&mus_res, encoding, i);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, return;)
					MU_ASSERT(data_size >= isize, result, MUS_INSUFFICIENT_DATA_SIZE, return;)

					uint8_m neg = 0;
					if (i < 0) {
						neg = 1;
					}

					// https://stackoverflow.com/a/14564888
					for (int8_m isizemod = isize-1; isizemod >= 0; isizemod -= 1, i /= 10) {
						if (i < 0) {
							data[isizemod] = ((-i) % 10) + 48;
						} else {
							data[isizemod] = (i % 10) + 48;
						}
					}

					if (neg > 0) {
						data[0] = 45;
					}
				}

				MUDEF int64_m mu_raw_string_get_integer(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)
					if (encoding) {}

					size_m beg_i = 0;
					if (data[0] == 45) {
						beg_i = 1;
					}

					int64_m ret = 0;
					for (size_m i = data_size; i != beg_i;) {
						i -= 1;
						int64_m bef_ret = ret;

						MU_ASSERT(data[i] >= 48 && data[i] <= 57, result, MUS_INVALID_DATA, return 0;)
						ret += mu_pow(10, (data_size-i)-1) * (data[i]-48);
						if (ret < bef_ret) {
							MU_SET_RESULT(result, MUS_OVERFLOW)
							return 0;
						}
					}

					if (beg_i == 1) {
						return -ret;
					}
					return ret;
				}

				MUDEF size_m mu_raw_string_get_float_conversion_size(musResult* result, muCharacterEncoding encoding, float d, size_m decimal_places) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					if (encoding) {}
					if (decimal_places > MUS_MAX_FLOAT_DECIMAL_POINTS) {
						decimal_places = MUS_MAX_FLOAT_DECIMAL_POINTS;
					}

					uint8_m add = 0;
					float ud;
					if (d >= 0.f) {
						ud = d;
					} else {
						add = 1;
						ud = -d;
					}

					uint8_m digits = ((uint8_m)(mu_log10((double)ud))) + 1;
					if (decimal_places == 0) {
						return (size_m)(digits + add);
					}
					return ((size_m)(digits + add)) + decimal_places + 1;
				}

				void mus_write_double_int_only(size_m intsize, double floored_d, muByte* data) {
					uint8_m neg = 0;
					if (floored_d < 0.f) {
						neg = 1;
					}

					size_m count = 0;

					for (int64_m isizemod = intsize-1; isizemod >= 0; isizemod -= 1, count += 1) {
						double i = floored_d / mu_pow(10.f, count);
						if (i < 0) {
							data[isizemod] = (muByte)mu_fmod(-i, 10.f) + 48;
						} else {
							data[isizemod] = (muByte)mu_fmod(i, 10.f) + 48;
						}
					}

					if (neg > 0) {
						data[0] = 45;
					}
				}

				MUDEF void mu_raw_string_convert_to_float(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, float d, size_m decimal_places) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
					if (decimal_places > MUS_MAX_FLOAT_DECIMAL_POINTS) {
						decimal_places = MUS_MAX_FLOAT_DECIMAL_POINTS;
					}

					musResult mus_res = MUS_SUCCESS;
					size_m dsize = mu_raw_string_get_float_conversion_size(&mus_res, encoding, d, decimal_places);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, return;)
					MU_ASSERT(data_size >= dsize, result, MUS_INSUFFICIENT_DATA_SIZE, return;)

					size_m intsize = (dsize - decimal_places);
					if (decimal_places != 0) {
						intsize -= 1;
					}

					// Write int digits

					double floored_d;
					if (d >= 0.f) {
						floored_d = mu_floor((double)d);
					} else {
						floored_d = -mu_floor((double)-d);
					}

					mus_write_double_int_only(intsize, floored_d, data);

					// Write decimal digits

					if (decimal_places > 0) {
						data[intsize] = 46;

						if (d < 0.f) { d = -d; }
						if (floored_d < 0.f) { floored_d = -floored_d; }

						double d_decimals = (double)(d)-floored_d;
						if (d_decimals != 0.f) {
							d_decimals = (((double)(d_decimals)) * (mu_pow(10.f, (double)(decimal_places))));
						} else {
							d_decimals = (((double)(floored_d)) * (mu_pow(10.f, (double)(decimal_places))));
						}

						mus_write_double_int_only(decimal_places, d_decimals, &data[intsize+1]);
					}
				}

				MUDEF float mu_raw_string_get_float(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0.f;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0.f;)
					if (encoding) {}

					size_m beg_i = 0;
					if (data[0] == 45) {
						beg_i = 1;
					}

					size_m decimal = MU_NONE;
					for (size_m i = 0; i < data_size; i++) {
						if (data[i] == 46) {
							decimal = i;
							break;
						}
					}
					MU_ASSERT(decimal != MU_NONE, result, MUS_INVALID_DATA, return 0.f;)

					float ret = 0.f;
					for (size_m i = beg_i; i < data_size; i++) {
						if (i == decimal) {
							continue;
						}
						MU_ASSERT(data[i] >= 48 && data[i] <= 57, result, MUS_INVALID_DATA, return 0;)

						if (i < decimal) {
							ret += (float)(mu_pow(10.f, (decimal-i)-1) * (double)(data[i]-48));
						} else {
							ret += (float)(mu_pow(10.f, -(double)((i-decimal))) * (double)(data[i]-48));
						}
					}

					if (beg_i == 1) {
						return -ret;
					}
					return ret;
				}

				MUDEF size_m mu_raw_string_get_double_conversion_size(musResult* result, muCharacterEncoding encoding, double d, size_m decimal_places) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					if (encoding) {}
					if (decimal_places > MUS_MAX_DOUBLE_DECIMAL_POINTS) {
						decimal_places = MUS_MAX_DOUBLE_DECIMAL_POINTS;
					}

					uint8_m add = 0;
					double ud;
					if (d >= 0.f) {
						ud = d;
					} else {
						add = 1;
						ud = -d;
					}

					uint8_m digits = ((uint8_m)(mu_log10(ud))) + 1;
					if (decimal_places == 0) {
						return (size_m)(digits + add);
					}
					return ((size_m)(digits + add)) + decimal_places + 1;
				}

				MUDEF void mu_raw_string_convert_to_double(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, double d, size_m decimal_places) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
					if (decimal_places > MUS_MAX_DOUBLE_DECIMAL_POINTS) {
						decimal_places = MUS_MAX_DOUBLE_DECIMAL_POINTS;
					}

					musResult mus_res = MUS_SUCCESS;
					size_m dsize = mu_raw_string_get_double_conversion_size(&mus_res, encoding, d, decimal_places);
					MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, return;)
					MU_ASSERT(data_size >= dsize, result, MUS_INSUFFICIENT_DATA_SIZE, return;)

					size_m intsize = (dsize - decimal_places);
					if (decimal_places != 0) {
						intsize -= 1;
					}

					// Write int digits

					double floored_d;
					if (d >= 0.f) {
						floored_d = mu_floor((double)d);
					} else {
						floored_d = -mu_floor((double)-d);
					}

					mus_write_double_int_only(intsize, floored_d, data);

					// Write decimal digits

					if (decimal_places > 0) {
						data[intsize] = 46;

						if (d < 0.f) { d = -d; }
						if (floored_d < 0.f) { floored_d = -floored_d; }

						double d_decimals = (double)(d)-floored_d;
						if (d_decimals != 0.f) {
							d_decimals = (((double)(d_decimals)) * (mu_pow(10.f, (double)(decimal_places))));
						} else {
							d_decimals = (((double)(floored_d)) * (mu_pow(10.f, (double)(decimal_places))));
						}

						mus_write_double_int_only(decimal_places, d_decimals, &data[intsize+1]);
					}
				}

				MUDEF double mu_raw_string_get_double(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0.f;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0.f;)
					if (encoding) {}

					size_m beg_i = 0;
					if (data[0] == 45) {
						beg_i = 1;
					}

					size_m decimal = MU_NONE;
					for (size_m i = 0; i < data_size; i++) {
						if (data[i] == 46) {
							decimal = i;
							break;
						}
					}
					MU_ASSERT(decimal != MU_NONE, result, MUS_INVALID_DATA, return 0.f;)

					double ret = 0.f;
					for (size_m i = beg_i; i < data_size; i++) {
						if (i == decimal) {
							continue;
						}
						MU_ASSERT(data[i] >= 48 && data[i] <= 57, result, MUS_INVALID_DATA, return 0;)

						if (i < decimal) {
							ret += mu_pow(10.f, (decimal-i)-1) * (double)(data[i]-48);
						} else {
							ret += mu_pow(10.f, -(double)((i-decimal))) * (double)(data[i]-48);
						}
					}

					if (beg_i == 1) {
						return -ret;
					}
					return ret;
				}

		/* Cross-encoding functions */

			MUDEF size_m mu_character_encoding_get_code_point_size(musResult* result, muCharacterEncoding encoding, muCodePoint code_point) {
				switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return 0; break;
					case MU_ASCII: return mu_ASCII_get_code_point_size(result, code_point); break;
					case MU_UTF8: return mu_UTF8_get_code_point_size(result, code_point); break;
				}
			}

			MUDEF muCodePoint mu_character_encoding_get_code_point(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
				switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return 0; break;
					case MU_ASCII: return mu_ASCII_get_code_point(result, data, data_size); break;
					case MU_UTF8: return mu_UTF8_get_code_point(result, data, data_size); break;
				}
			}

			MUDEF void mu_character_encoding_set_code_point(musResult* result, muCharacterEncoding encoding, muCodePoint code_point, muByte* data, size_m data_size) {
				switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return; break;
					case MU_ASCII: mu_ASCII_set_code_point(result, code_point, data, data_size); break;
					case MU_UTF8: mu_UTF8_set_code_point(result, code_point, data, data_size); break;
				}
			}

			MUDEF muBool mu_character_encoding_verify_raw_string(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
				switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return MU_FALSE; break;
					case MU_ASCII: return mu_ASCII_verify_raw_string(result, data, data_size); break;
					case MU_UTF8: return mu_UTF8_verify_raw_string(result, data, data_size); break;
				}
			}

			MUDEF size_m mu_character_encoding_get_raw_string_code_point_length(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
				switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return 0; break;
					case MU_ASCII: return mu_ASCII_get_raw_string_code_point_length(result, data, data_size); break;
					case MU_UTF8: return mu_UTF8_get_raw_string_code_point_length(result, data, data_size); break;
				}
			}

			MUDEF size_m mu_character_encoding_get_raw_string_code_point_offset(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, size_m offset, size_m index) {
				switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return 0; break;
					case MU_ASCII: return mu_ASCII_get_raw_string_code_point_offset(result, data, data_size, offset, index); break;
					case MU_UTF8: return mu_UTF8_get_raw_string_code_point_offset(result, data, data_size, offset, index); break;
				}
			}

		/* Encoding-specific functions */

			/* ASCII */

				MUDEF size_m mu_ASCII_get_code_point_size(musResult* result, muCodePoint code_point) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(code_point <= 0x7F, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)

					return 1;
				}

				MUDEF muCodePoint mu_ASCII_get_code_point(musResult* result, muByte* data, size_m data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)
					MU_ASSERT(data[0] <= 0x7F, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)

					return data[0];
				}

				MUDEF void mu_ASCII_set_code_point(musResult* result, muCodePoint code_point, muByte* data, size_m data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
					MU_ASSERT(code_point <= 0x7F, result, MUS_CHARACTER_CANNOT_BE_ENCODED, return;)

					data[0] = code_point;
				}

				MUDEF muBool mu_ASCII_verify_raw_string(musResult* result, muByte* data, size_m data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

					for (size_m i = 0; i < data_size; i++) {
						if (data[i] > 0x7F) {
							return MU_FALSE;
						}
					}

					return MU_TRUE;
				}

				MUDEF size_m mu_ASCII_get_raw_string_code_point_length(musResult* result, muByte* data, size_m data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

					return data_size;
				}

				MUDEF size_m mu_ASCII_get_raw_string_code_point_offset(musResult* result, muByte* data, size_m data_size, size_m offset, size_m index) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return offset;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return offset;)
					MU_ASSERT(offset+index < data_size, result, MUS_INVALID_OFFSET, return offset;)

					return offset+index;
				}

			/* UTF-8 */

				muByte mus_UTF8_get_code_point_size_from_byte(muByte byte) {
					// 00000000 <= n <= 01111111
					if (/*byte >= 0 && */byte <= 127) {
						return 1;
					// 11000000 <= n < 11100000
					} else if (byte >= 192 && byte < 224) {
						return 2;
					// 11100000 <= n < 11110000
					} else if (byte >= 224 && byte < 240) {
						return 3;
					// 11110000 <= n <= 11110111
					} else if (byte >= 240 && byte <= 247) {
						return 4;
					} else {
						// Value is 10xxxxxx or 11111xxx; invalid.
						return 0;
					}
				}

				MUDEF size_m mu_UTF8_get_code_point_size(musResult* result, muCodePoint code_point) {
					MU_SET_RESULT(result, MUS_SUCCESS)

					// Storable in 7 bits = 1 byte
					if (code_point < 128) {
						return 1;
					}
					// Storable in 11 bits = 2 bytes
					if (code_point < 2048) {
						return 2;
					}
					// Storable in 16 bits = 3 bytes
					if (code_point < 65536) {
						return 3;
					}
					// Storable in 21 bits = 4 bytes
					if (code_point < 2097152) {
						return 4;
					}
					// Anything larger cannot be stored.
					MU_SET_RESULT(result, MUS_CHARACTER_CANNOT_BE_ENCODED)
					return 0;
				}

				MUDEF muCodePoint mu_UTF8_get_code_point(musResult* result, muByte* data, size_m data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

					muByte code_point_size = mus_UTF8_get_code_point_size_from_byte(data[0]);
					MU_ASSERT(code_point_size != 0, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)

					for (size_m i = 1; i < code_point_size; i++) {
						// 10000000 <= n <= 10111111
						if (!(data[i] >= 128 && data[i] <= 191)) {
							MU_SET_RESULT(result, MUS_INVALID_CHARACTER_ENCODING)
							return 0;
						}
					}

					switch (code_point_size) { default: MU_SET_RESULT(result, MUS_INVALID_CHARACTER_ENCODING) return 0; break;
						case 1: {
							return data[0];
						} break;

						case 2: {
							MU_ASSERT(data_size >= 2, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
							return
							// 110xxxxx
							(muCodePoint)(data[0] & 31) << 6 |
							// 10xxxxxx
							(muCodePoint)(data[1] & 63);
						} break;

						case 3: {
							MU_ASSERT(data_size >= 3, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
							return
							// 1110xxxx
							(muCodePoint)(data[0] & 15) << 12 |
							// 10xxxxxx
							(muCodePoint)(data[1] & 63) << 6 |
							// 10xxxxxx
							(muCodePoint)(data[2] & 63);
						} break;

						case 4: {
							MU_ASSERT(data_size >= 4, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
							return
							// 1110xxxx
							(muCodePoint)(data[0] & 7) << 18 |
							// 10xxxxxx
							(muCodePoint)(data[1] & 63) << 12 |
							// 10xxxxxx
							(muCodePoint)(data[2] & 63) << 6 |
							// 10xxxxxx
							(muCodePoint)(data[3] & 63);
						} break;
					}
				}

				MUDEF void mu_UTF8_set_code_point(musResult* result, muCodePoint code_point, muByte* data, size_m data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)

					musResult res = MUS_SUCCESS;
					size_m code_point_size = mu_UTF8_get_code_point_size(&res, code_point);
					MU_ASSERT(res == MUS_SUCCESS, result, res, return;)

					MU_ASSERT(code_point_size <= data_size, result, MUS_INVALID_DATA_SIZE, return;)

					switch (code_point_size) { default: MU_SET_RESULT(result, MUS_CHARACTER_CANNOT_BE_ENCODED) return; break;
						case 1: {
							data[0] = code_point;
						} break;

						// mask = Get what you want
						// add  = Add bits
						case 2: {
							// 110xxxxx                           mask    add
							data[0] = (muByte)(((code_point >> 6) & 31) | 192);
							// 10xxxxxx                      mask
							data[1] = (muByte)((code_point & 63) | 128);
						} break;

						case 3: {
							// 1110xxxx
							data[0] = (muByte)(((code_point >> 12) & 15) | 224);
							// 10xxxxxx
							data[1] = (muByte)(((code_point >> 6) & 63) | 128);
							// 10xxxxxx
							data[2] = (muByte)((code_point & 63) | 128);
						} break;

						case 4: {
							// 11110xxx
							data[0] = (muByte)(((code_point >> 18) & 7) | 240);
							// 10xxxxxx
							data[1] = (muByte)(((code_point >> 12) & 63) | 128);
							// 10xxxxxx
							data[2] = (muByte)(((code_point >> 6) & 63) | 128);
							// 10xxxxxx
							data[3] = (muByte)((code_point & 63) | 128);
						} break;
					}
				}

				MUDEF muBool mu_UTF8_verify_raw_string(musResult* result, muByte* data, size_m data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

					musResult res = MUS_SUCCESS;
					size_m i = 0;
					// BOM
					if (data_size >= 3) {
						if (data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) {
							i = 3;
						}
					}

					for (; i < data_size;) {
						muByte code_point_size = mus_UTF8_get_code_point_size_from_byte(data[i]);
						if (code_point_size == 0) {
							return MU_FALSE;
						}
						if (!((i+code_point_size < data_size) || (i+code_point_size == data_size))) {
							return MU_FALSE;
						}

						mu_UTF8_get_code_point(&res, &data[i], data_size-i);
						if (res != MUS_SUCCESS) {
							return MU_FALSE;
						}

						i += code_point_size;
					}
					return MU_TRUE;
				}

				MUDEF size_m mu_UTF8_get_raw_string_code_point_length(musResult* result, muByte* data, size_m data_size) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

					size_m i = 0;
					// BOM
					if (data_size >= 3) {
						if (data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) {
							i = 3;
						}
					}

					size_m code_point_length = 0;

					for (; i < data_size;) {
						muByte code_point_size = mus_UTF8_get_code_point_size_from_byte(data[i]);
						MU_ASSERT(code_point_size != 0, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
						MU_ASSERT((i+code_point_size < data_size) || (i+code_point_size == data_size), result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
						code_point_length += 1;

						i += code_point_size;
					}
					return code_point_length;
				}

				MUDEF size_m mu_UTF8_get_raw_string_code_point_offset(musResult* result, muByte* data, size_m data_size, size_m offset, size_m index) {
					MU_SET_RESULT(result, MUS_SUCCESS)
					MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
					MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

					size_m i = 0;
					if (offset == 0) {
						// BOM
						if (data_size >= 3) {
							if (data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) {
								i = 3;
							}
						}
					}

					size_m count_index = 0;

					for (; i < data_size && count_index != index;) {
						muByte code_point_size = mus_UTF8_get_code_point_size_from_byte(data[0]);
						MU_ASSERT(code_point_size != 0, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
						MU_ASSERT((i+code_point_size < data_size) || (i+code_point_size == data_size), result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
						count_index += 1;

						i += code_point_size;
					}

					MU_ASSERT(count_index == index, result, MUS_INVALID_OFFSET, return 0;)
					return i;
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
Copyright (c) 2024 Hum
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

