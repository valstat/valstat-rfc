#pragma once
/*
these should not be part of the standard
these are seen by some as "best practices"
but not by everyone
*/
#ifdef DBJ_STL_VALSTAT_CONTROLLED_DEFINITION
/*
<type_traits> are optional, if required utilities used
can be quickly implemented in this header inside
dbj namespace too
*/
#include <type_traits>
namespace dbj::light {
    // controlled template definition
    template< typename T, typename S>
    struct valstat_definition final
    {
        using type = valstat<T, S>;

        static_assert(is_standard_layout_v < type >, "this valstat definition is not standard layout");
        static_assert(std::is_default_constructible_v    < type >, "this valstat definition is not default constructible");
        static_assert(std::is_copy_constructible_v     < type >, "this valstat definition is not copy constructible");
        static_assert(std::is_move_constructible_v      < type >, "this valstat definition is not move constructible");
        static_assert(std::is_swappable_v       < type >, "this valstat definition is not swappable ");

    }; // valstat_definition

    template< typename T, typename S>
    using valstat_definition_t = typename valstat_definition<T, S>::type;

}
#endif DBJ_STL_VALSTAT_CONTROLLED_DEFINITION