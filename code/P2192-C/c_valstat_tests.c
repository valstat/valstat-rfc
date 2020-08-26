
#include "c_valstat.h"
#include <stdio.h>

/* declare valstat_int type 

typedef struct valstat_int
{ 
int * value ; 
status_json status  ; 
} valstat_int

*/
valstat(int);

// valstat enabled C API 
#ifdef __clang__
	// due to usage of valstat we can make it a pure function
	// we do not use globals, or long jumps
	// or use anything but arguments
__attribute__((const))
#endif
// compare to add() to understand the simplicity
// of this valstat design
// also notice we do not use macros in here
// although we could and real life valstat 
// adopters probably will
valstat_int adder(int lhs, int rhs)
{
	if (lhs >= 0) {
		if (INT_MAX - lhs < rhs) {
			// creating valstat instance
			// with empty value option 
			// status option contains a string
			// in C this is called: "compound literall"
			return (valstat_int) { NULL, STATUS_MSG("integer overflow") };
			// signal the ERROR state
		}
	}
	else {
		if (rhs < INT_MIN - lhs) {
			// creating valstat instance
			// with empty value option 
			// status option contains a string
			return (valstat_int) { NULL, STATUS_MSG("integer overflow") };
			// signal the ERROR state
		}
	}
	// creating valstat instance
	// with value option set
	// status option remains empty
	//
	// NOTE! adress is anchored inside a int cache
	// otherwise we will get a dangling pointer
	return (valstat_int) { int_storage_store(lhs + rhs), status_json_empty() };
	// signal the OK state
} // adder()

/*
-------------------------------------------------------------------------------------------------
*/
static inline  void test_two_driver(int arg_1, int arg_2) {

	printf("\n\nadd( %d, %d )", arg_1, arg_2);

	valstat_int vs = adder(arg_1, arg_2);

	// to use decode() we must cast to a strong type
	// this is a good thing
	// note: decode() use is completely optional
	// meta_state ms = decode((Value) { vs.value }, (Status) { vs.status.data });
	// printf("\nmeta state name:%s, tag: %d", ms.name, ms.tag);

	if (vs.value) {
		printf("\nvalue: %d", *vs.value);
	}

	if (! status_json_is_empty( & vs.status) ) {
		printf("\nstatus:\n %s", vs.status.data );
	}
}

void test_two() {

	test_two_driver(INT_MAX, INT_MAX);
	test_two_driver(INT_MAX, 1);
	test_two_driver(42, 13);

}