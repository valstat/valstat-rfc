#pragma once
/*
(c) 2020 by dbj@dbj.org CC BY SA 4.0

Poor Mans Storage -- Why? 
Because pointers from here are not dangling pointers
Because this centralizes allocation 
Because this allows for heap alloc avoidance

C valstat is based on two pointers thus what the pointer points to is a major issue
for int's we use this poor mans int storage from which we deliver addresses of T instances

NOTE: in real life we use "uthash" hash table implementation to keep all non-local values
      thus we use handles, not pointers
	  http://troydhanson.github.io/uthash/userguide.html
*/
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// default capacity
#undef  DEFAULT_STORAGE_CAPACITY 
#define DEFAULT_STORAGE_CAPACITY 0xFF

#undef CONCAT_
#undef CONCAT
#define CONCAT_(a,b) a##b
#define CONCAT(a,b) CONCAT_(a,b)

#undef DECLARE_STORAGE
#define DECLARE_STORAGE(T, CAPACITY ) \
	static T   CONCAT(T,_storage) [CAPACITY] = { 0 }; \
	static int CONCAT(T,_storage_end) = CAPACITY; \
	static int CONCAT(T,_storage_level) = 0; \
	static inline T * CONCAT(T,_storage_store) (T new_ ) \
	{ \
		assert( CONCAT(T, _storage_level) < CONCAT( T, _storage_end) ); \
		CONCAT(T,_storage) [ CONCAT(T, _storage_level) ] = new_; \
		return &( CONCAT(T, _storage) [  CONCAT(T, _storage_level) ]); \
		CONCAT( T, _storage_level) += 1; \
	}

	/*
	here we declare storages for fundamental types
	for example:

	DECLARE_STORAGE(int, 0xFF) makes:

	static int int_storage[STORAGE_CAPACITY] = { 0 };
	static int int_storage_end = STORAGE_CAPACITY;
	static int int_storage_level = 0;
	static inline int* int_storage_store(int new_int)
	{
		assert(int_storage_level < int_storage_end);
		int_storage[int_storage_level] = new_int;
		return &(int_storage[int_storage_level]);
		int_storage_level += 1;
	}
	*/
	DECLARE_STORAGE(int, 0xFF)

	// storage of bool-eans
	//
	// bool * bool_storage_store ( bool new_bool )

	DECLARE_STORAGE(bool, 0xFF)

/* 
have to use a typedef as typename is used to generate function names, for example
bellow generates

 const_char_star * const_char_star_storage_store ( const_char_star new_) 
 {
 . . . 
 }
 */
	typedef const char*  const_char_star;
	DECLARE_STORAGE(const_char_star, 0xFF)

#undef CONCAT_
#undef CONCAT
#undef DECLARE_STORAGE

#ifdef __cplusplus
} // extern "C" 
#endif // __cplusplus
