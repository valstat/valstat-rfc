#pragma once

#ifdef __clang__
#pragma clang system_header
#else
/* might work under _MSC_VER too */
#endif

/*
(c) 2020 by dbj.org CC BY SA 4.0

some adopters a metastate paradigm "feel better" is they design abstractions actually representing 
metastates as types and instances of those types

here is one example, also written in C
*/

#include <stdlib.h> // NULL defined here
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	typedef struct meta_state_descriptor_type {
		unsigned tag;
		const char* const name;
	} meta_state_descriptor_type;

	static inline bool equal
	(meta_state_descriptor_type MS1, meta_state_descriptor_type MS2) {
		return MS1.tag == MS2.tag;
	}

	static inline const char* name
	(meta_state_descriptor_type MS1) {
		return MS1.name;
	}

/* an indicator */
#undef  THREAD_SAFE 
#define THREAD_SAFE 

/*
    process wide storage of metastate descriptors
*/
	THREAD_SAFE static const struct
	{
		meta_state_descriptor_type	OK;
		meta_state_descriptor_type	ERROR;
		meta_state_descriptor_type	INFO;
		meta_state_descriptor_type	EMPTY;
	}	meta_state_descriptors = {
		{ 0, "OK" },
		{ 1, "ERROR" },
		{ 2, "INFO" },
		{ 3, "EMPTY" }
	};
	/*
	if not coded like above those names are very likely to clash
	with macros and other names in real projects

	we design Values and Status as generic but strong types 
	thus no confusion when calling functions using these 
	as argument types
	*/
	typedef struct Value { const void* const instance; } Value;
	typedef struct Status { const void* const instance; } Status;

	THREAD_SAFE static const Value  empty_value = { NULL };
	THREAD_SAFE static const Status empty_status = { NULL };

	static inline bool is_empty_value(Value  v_) { return v_.instance == NULL; }
	static inline bool is_empty_status(Status s_) { return s_.instance == NULL; }

/*
metastate := occupancy AND occupancy

metastate capturing as an explicit operation
due to the Valstat and Status implementation, "anything" that can be cast 
to void * can be used 

	meta_state_descriptor_type msdt_ 
	  = metastate_capture
		( 
			(Value){ .instance = &some_lvalue } , 
			(Status){ .instance  = &some_other_lvalue } 
		);
*/

// pure function declaration
#if defined(__clang__) || defined(__GNUC__) 
	static
		meta_state_descriptor_type 
		capture(const Value value, const Status status) __attribute__((const));
#endif

	static inline
		meta_state_descriptor_type metastate_capture
		(const Value value, const Status status)
	{
		if (value.instance && status.instance) return meta_state_descriptors.INFO;
		if (value.instance && !status.instance) return meta_state_descriptors.OK;
		if (!value.instance && !status.instance) return meta_state_descriptors.EMPTY;
		/* if (!value.instance && status.instance) */ return meta_state_descriptors.ERROR;
	}

#ifdef __cplusplus
} //	extern "C" 
#endif // __cplusplus