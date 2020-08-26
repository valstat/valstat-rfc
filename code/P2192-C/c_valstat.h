#pragma once
#ifdef __clang__
#pragma clang system_header
#else
/* might work under _MSC_VER too */
#endif

#include "pm_storage.h"
// moved to archive --> #include "explicit_meta_state.h"
#include <stdio.h>
#include <limits.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/* 
	we always use json formatted string as status message 
	this design cuts dependancy on some external string storage
	and makes no use of heap
	*/
#define STATUS_JSON_BUFER_SIZE 1024
	typedef struct status_json { 
		char data[STATUS_JSON_BUFER_SIZE]; 
	} status_json;

	static inline status_json status_json_empty() { return (status_json) { {0} }; }

	static inline bool status_json_is_empty(status_json* sj) { return sj->data[0] == 0; }

	static inline status_json status_json_msg(const char* file_, const long line_, const char* msg_, const char* timestamp_) 
	{
		status_json sj_ = status_json_empty() ;
		int rez = snprintf(sj_.data, STATUS_JSON_BUFER_SIZE,
			"{ \"file\" : \"%s\"  , \"line\" : %ld , \"message\" : \"%s\" , \"timestamp\" : \"%s\"  }", file_, line_, msg_, timestamp_);
		assert( rez > 0 );
		return sj_;
	}

	/*
	(c) 2020 by dbj@dbj.org CC BY SA 4.0

	This valstat design option, is as pragmatic as possible
	The whole of C valstat is in the two macros bellow
	NOTE: status is always a message
	in a json format
	*/
#define valstat_( T ) typedef struct valstat_##T \
{ \
T * value ; \
status_json status  ; \
} valstat_##T

#define valstat( T ) valstat_( T )

/* use this to declare valstat type */
#define valstat_decl( T ) valstat_##T 

#undef STATUS_MSG
#define STATUS_MSG(M_) status_json_msg( __FILE__, __LINE__ , M_, __TIMESTAMP__ ) 

#ifdef __cplusplus
} // extern "C" 
#endif // __cplusplus


