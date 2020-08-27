#pragma once

#ifndef __dbj__dbg_h
#define __dbj__dbg_h

#include <stdio.h>
// WARNING! Cludge ahead -- strerror is not thread safe, is ancient etc
// using POSIX C errno and strerror
#include <errno.h>
#include <string.h>

// _DEBUG is MSVCRT only
#ifdef _DEBUG 

#ifdef NDEBUG
#undef NDEBUG
#endif

#else // _DEBUG not defined 

#ifndef NDEBUG
#define NDEBUG
#endif

#endif // ! _DEBUG

/*
dbj reminder: stderr is buffered by default, stdout is not
dbj reminder: NDEBUG is in standard, DEBUG is not

macros bellow do require dbj_error_label: and are using goto

example

Buffer *
buffer_alloc(int initial_size)
{
    Buffer *buf = malloc(sizeof(Buffer));
    char   *tmp = calloc(1, initial_size * sizeof(char));

    jump_to_error_if(buf == NULL || tmp == NULL);

    return buf;
dbj_error_label:
    if (buf) { buffer_free(buf); }
    if (tmp) { free(tmp); }

    return NULL;
}
*/

#ifdef NDEBUG // release
#define dbj_debug(M, ...)
#else // dbj_debug
#define dbj_debug(M, ...) fprintf(stderr, "DEBUG %s (in function '%s'):%d:  " M "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

// dbj added 
// show the expression and the value it yields to
#ifdef NDEBUG
#define DBG(FMT_, X_)
#else  
#define DBG(FMT_,X_) dbj_debug( "%s = " FMT_, #X_, (X_) )
#endif

// NOTE! stays in release mode 
#define dbj_jump_unless(A) if (!(A)) { goto dbj_error_label ; }

#ifdef NDEBUG
#define dbj_error_unless(A, M, ...) (void)0
#else
#define dbj_error_unless(A, M, ...) if (!(A)) { fprintf(stderr, M "\n", ##__VA_ARGS__); goto dbj_error_label ; }
#endif

#define dbj_clean_errno() (errno == 0 ? "None" :  strerror(errno))

// NOTE! stays in release mode 
#define dbj_log_err(M, ...) fprintf(stderr, "[ERROR] (%s (in function '%s'):%d: errno: %s) " M "\n", __FILE__, __FUNCTION__, __LINE__, dbj_clean_errno(), ##__VA_ARGS__)

#define dbj_log_log(M, ...) fprintf(stderr, "[LOG  ] (%s (in function '%s'):%d ) " M "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define dbj_jump_to_error_if(A) if (A) { goto dbj_error_label ; }
#define dbj_jump_to_error_unless(A) if (!(A)) { goto dbj_error_label ; }

#endif // __dbj__dbg_h
