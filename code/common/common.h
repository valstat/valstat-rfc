#pragma once

/*
Here's a better C version (from Google's Chromium project):
It improves on the array[0] or *array version by using 0[array], 
which is equivalent to array[0] on plain arrays, 
but will fail to compile if array happens to be a C++ type that overloads operator[]().
*/
#undef  COUNT_OF
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

// mixing C and C++ in one header like this one is anti pattern
// even in one folder
// I am aware of that 
#ifdef __cplusplus
/*
(c) 2020 by dbj@dbj.org CC BY SA 3.0
*/
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <array>
#include <type_traits>
#include <typeinfo>

namespace {
	inline auto initialize_once = []() { std::cout << std::boolalpha; return true; } ();
}

// Show eXpression
// innocent loking but saves a lot of typing and rezults saving
#undef  SX
#define SX(x_) do { std::cout << "\n" #x_ " : " << (x_); } while(0)

#ifdef DBJ_USING_IOSTREAM

#include <iostream>

namespace dbj
{
    inline ::std::ostream& operator << (::std::ostream& os, dbj_errc ec_)
    {
        return os << dbj_errc_message(ec_);
    }
}

#endif // DBJ_USING_IOSTREAM

namespace dbj {

    using std::cout;

    // return false on empty
    inline auto show_option = [](const char* prompt_, auto opt_) ->bool
    {
        cout << "\n" << prompt_ << " : ";
        if (opt_) {
            cout << *opt_;
            return true;
        }
        cout << "Empty";
        return false;
    };

    inline auto show_metastate = []( const char * prompt_ , auto val_, auto stat_) {
        
        cout << "\n\n" << (prompt_ ? prompt_ : "");

        bool value_state = show_option("Value : ", val_);
        bool status_state = show_option("Status: ", stat_);

        cout << "\nmetastate captured as: ";
        if (value_state && status_state) { cout << "info"; }
        if (value_state && !status_state) { cout << "ok"; }
        if (!value_state && status_state) { cout << "error"; }
        if (!value_state && !status_state) { cout << "empty"; }
    };

} // dbj
#else // ! __cplusplus

#include <stdlib.h> // For random(), RAND_MAX

extern "C" {
    // https://stackoverflow.com/a/17554531
    inline unsigned int rand_interval(unsigned int min_, unsigned int max_)
    {
        int r;
        const unsigned int range = 1 + max_ - min_;
        const unsigned int buckets = RAND_MAX / range;
        const unsigned int limit = buckets * range;

        /* Create equal size buckets all in a row, then fire randomly towards
         * the buckets until you land in one of them. All buckets are equally
         * likely. If you land off the end of the line of buckets, try again. */
        do
        {
            r = rand();
        } while (r >= limit);

        return min_ + (r / buckets);
    }
} // "C"

#endif // ! __cplusplus
