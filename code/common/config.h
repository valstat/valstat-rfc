#pragma once

#ifdef __cplusplus

#ifdef __clang__
#pragma clang system_header
#endif

#if __GNUC__
#define DBJ_PURE_FUN   __attribute__((const))
#else
#define DBJ_PURE_FUN   
#endif
/*

Copyright (c) Microsoft Corporation.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

portions (c) 2020 by dbj.org CC BY SA 4.0

*/

#if !defined(DBJ_HAS_CXX17) && !defined(DBJ_HAS_CXX20)
#if defined(_MSVC_LANG)
#define DBJ__STL_LANG _MSVC_LANG
#else
#define DBJ__STL_LANG __cplusplus
#endif

#if DBJ__STL_LANG > 201703L
#define DBJ_HAS_CXX17 1
#define DBJ_HAS_CXX20 1
#elif DBJ__STL_LANG > 201402L
#define DBJ_HAS_CXX17 1
#define DBJ_HAS_CXX20 0
#else // DBJ__STL_LANG <= 201402L
#define DBJ_HAS_CXX17 0
#define DBJ_HAS_CXX20 0
#endif // Use the value of DBJ__STL_LANG to define DBJ_HAS_CXX17 and DBJ_HAS_CXX20

#undef DBJ__STL_LANG
#endif // !defined(DBJ_HAS_CXX17) && !defined(DBJ_HAS_CXX20)

#endif // __cplusplus
