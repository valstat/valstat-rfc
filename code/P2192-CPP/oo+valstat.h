#pragma once
#undef NDEBUG
#define DEBUG 1
#include "../common/dbj_dbg.h"

#include "std_valstat.h"

#include <stdlib.h>
#include <string.h>

/// ------------------------------------------------------------
struct person final
{
    using string = const char*;

    // by value
    using valstat = std::valstat< person, const char* >;
    // by reference
    using valstat_r = std::valstat< person&, const char* >;
    // as a 'this' pointer
    using valstat_p = std::valstat< person const*, const char* >;

    // default ctor makes empty person
    constexpr person() noexcept : name_() { }
    constexpr person(string new_name_) noexcept : name_(new_name_) { }

    // ad hoc valstat ... perfectly usable
    std::valstat<string, string > name() const noexcept {
        if (empty())
            // ERROR metastate
            return { {},"empty person means empty name" };
        // OK metastate
        return { name_ , {} };
    }

    // did it in a pedestrian way 
    static bool  is_empty_string( const char * str_ ) {
        if (!str_)
            return true;
        if (str_[0] == char(0))
            return true;
        return false;
    }

    bool empty() const noexcept { return is_empty_string(name_); }
    bool empty()       noexcept { return is_empty_string(name_); }

    // in this instance can provide these too
    // so we can use this type in generic situations
    // together with std::optiponal 
    // who does not have empty() method for some reason
    operator bool () const noexcept { return  ! is_empty_string(name_); }
    operator bool ()       noexcept { return  ! is_empty_string(name_); }

    static inline void print(person const& pson_)
    {
        if (pson_.empty()) {
            dbj_debug("%s", "can not print empty person");
            return;
        }
        else
            dbj_debug("\n\n%s\n", "Person");

        auto name_meta_state = pson_.name();

        //if (name_meta_state.value)
        //    dbj_debug("Name: %s", name_meta_state.value);

        //if (name_meta_state.status)
        //    dbj_debug("Name could not be obtained. Reason: %s", name_meta_state.status);

        dbj::show_metastate( "(" __FILE__ ") Person name" , name_meta_state.value , name_meta_state.status );
    }

private:
    string name_{};
};
/// --------------------------------------------------------------------
constexpr inline
person::valstat make_person(person::string name_) noexcept {
    return { person(name_), "Made in factory" };
}

void oo_valstat_test ()
{
    // default person is empty
    person::print(person{});

    // warning: decomposition declarations are a C++17 extension
    if (auto [person_a, status] = make_person("Stack"); !person_a.empty()) {
        person::print(person_a);
    }

    if (auto [person_a, status] =  person::valstat_p { new person("Heap"), {} };
        // person is a pointer here
        !person_a->empty())
    {
        person::print(*person_a);
    }
}

/// --------------------------------------------------------------
/// STL + metastate sample
/// --------------------------------------------------------------
#include <optional>
// we do not use std::errc
// so we do not include system_error machinery
// #include <errno.h>

namespace dbj {

    /*
    metastate emiting sequence type
    */
    template<typename T, size_t N>
    struct sequence {

        using type = sequence;
        using value_type = T;


    // emebeded valstat alias
    using valstat = std::valstat< std::optional<T>, std::optional<dbj_errc> >;

    // does not require deduction guide
    explicit sequence(const T(&arr_)[N]) noexcept
    {
        for (size_t k = 0; k < N; ++k)
        {
            data_[k] = arr_[k];
        }
    }

    // elegant solution to the index out of bounds
    // no exceptions and no special return value
    valstat operator [] ( size_t idx_ ) noexcept
    {
        if ((size_ - 1) < idx_ )
            /* ERROR metastate */
            return { {}, dbj::dbj_errc(invalid_argument) };
        /* OK metastate */
        return { data_[idx_] , {} };
    }

    private:
        size_t size_{N};
        T data_[N]{};
    }; // sequence

} // dbj

#include <iostream>

inline void stl_valstat_test() {

    int arr[] = {1,2,3};
    dbj::sequence sq( arr );

    {
        // error
        auto [value, status] = sq[3];
        dbj::show_metastate("(" __FILE__ ") dbj::sequence" ,  value, status);
    }
    {
        // ok
        auto [value, status] = sq[2];
        dbj::show_metastate("(" __FILE__ ") dbj::sequence", value, status);
    }
}

