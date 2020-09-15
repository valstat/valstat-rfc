#pragma once

#include "std_valstat.h"
#ifdef __clang__
#include "../common/http_status_codes.h"
#endif

#define DBJ_USING_IOSTREAM
#include "../common/posix_codes_messages.h"

#include <optional>



// dbj imaginary company have adopted the metastate idea
namespace dbj {

	// declaring generic metastate carrier using std::optional
	// for quick easy and feasible occupancy state capturing
	template <typename T, typename S>
	using valstat = std::valstat<
		std::optional<T>, std::optional<S> 
	>;

	// depending on the context we can declare in advance
	// common alieases we know will be in wider use

	// declaring metastate carrier 
	// using posix error codes for status types
	template <typename T >
	using posix_valstat = dbj::valstat< T, dbj_errc >;

#ifdef __clang__
	// reality check tels us there are oceans of code
	// where http_status_code is predefined to exist as C code
	// so we simulate that relity here too
	// "lib/http_status_codes.h" is C code
	template <typename T >
	using http_valstat = dbj::valstat< T, http_status_code >;
#endif
}
