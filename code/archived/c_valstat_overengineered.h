#pragma once

#ifdef __clang__
#pragma clang system_header
#else
#error Please use clang
#endif

// moved to archive --> #include "explicit_meta_state.h"
#include "pm_storage.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/*
	This is overengineered design resulting in non trivial implementation and usage.

	Here we have C valstat based on strong types based on void pointers.

	C version using naked void pointers would loose the roles of the Value and Status
	Simplest solution to rectify that is to use "strong type concept"

	Using "strong type" is a good thing also beacuse void pointers have no role (aka type)

    not so simple typeless valstat 
	*/
	typedef struct Valstat {
		/* Value and Status are strong types */
		Value	value;
		Status	status;
	} Valstat;


	/*
	 ********************************************************************
	*/
	static inline  void test_zero() {

		static const Status just_a_status = { "I am a status" };
		static const int i1 = 42;
		static const Value value = { &i1 };

		assert(
			equal(meta_state_descriptors.EMPTY, metastate_capture(empty_value, empty_status))
		);

		assert(
			equal(meta_state_descriptors.ERROR, metastate_capture(empty_value, just_a_status))
		);

		assert(
			equal(meta_state_descriptors.INFO, metastate_capture(value, just_a_status))
		);

		assert(
			equal(meta_state_descriptors.OK, metastate_capture(value, empty_status))
		);
	}

	/*
	 ********************************************************************
	*/
	// valstat enabled C API 
#ifdef __clang__
	// due to usage of valstat we can make this a pure function
	// we do not use globals, or long jumps
	// or use anything but arguments
	__attribute__((const))
#endif
		Valstat add(int lhs, int rhs)
	{
		if (lhs >= 0) {
			if (INT_MAX - lhs < rhs) {
				// creating valstat instance
				// with empty value option 
				// status option contains a string
				// in C this is called: "compound literall"
				return (Valstat) { {}, { "integer overflow" } };
				// signal the ERROR state
			}
		}
		else {
			if (rhs < INT_MIN - lhs) {
				// creating valstat instance
				// with empty value option 
				// status option contains a string
				return (Valstat) { {}, { "integer overflow" } };
				// signal the ERROR state
			}
		}
		// creating valstat instance
		// with value option set
		// status option remains empty
		//
		// NOTE! adress is anchored inside a int cache
		// otherwise we will get a dangling pointer
		return (Valstat) { {  int_storage_store(lhs + rhs) }, {} };
		// signal the OK state
	} // add()
	/*
	-------------------------------------------------------------------------------------------------
	*/
	static inline  void test_one_driver(int arg_1, int arg_2 ) {

		printf("\n\nadd( %d, %d )", arg_1, arg_2);

		Valstat vs = add(arg_1, arg_2);

		 meta_state_descriptor_type ms = metastate_capture(vs.value, vs.status);

		 printf("\nmeta state name:%s, tag: %d", ms.name, ms.tag);

		 if (vs.value.instance ) {
			 printf("\nvalue: %d", *(int*)vs.value.instance);
		 }

		 if (vs.status.instance ) {
			 printf("\nstatus: %s", (const char* const)vs.status.instance);
		 }
	}

	static inline  void test_one() {

		test_one_driver(INT_MAX, INT_MAX);
		test_one_driver(INT_MAX, 1);
		test_one_driver(42, 13);

	}

#ifdef __cplusplus
} // extern "C" 
#endif // __cplusplus
