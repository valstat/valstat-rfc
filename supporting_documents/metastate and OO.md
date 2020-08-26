## Is metastate usable with OO ?

We think it is, please see the slightly longer example bellow. 
```cpp
// (c) 2019/2020 by dbj@dbj.org CC BY SA 4.0
// https://wandbox.org/permlink/GUAC6DfgL5v2JY8b
#undef NDEBUG
#define DEBUG 1
#include "dbg.h"

#include <stdlib.h>
#include <string.h>

/*
// <valstat>
namespace std 
{
 template< typename T, typename S>
	struct [[nodiscard]] valstat
 {
    // both types must be able to 
    // simply and resiliently
    // exhibit state of occupancy
    // "empty" or "has value"
		using value_type = T ;
		using status_type = S ;

    // metastate is captured by AND-ing
    // state of occupancy of these two 
		value_type   value;
		status_type  status;
 };
} // std
*/
// 
class person final
{
public:
    using string = const char*;

```
First detail to notice is we use std::valstat in tis original form. No template aliases and no home grown valstats.

Using valstat in the context of a class. Which variety is required is up to the designer.
```cpp   
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

    // a pedestrian way 
    // to avoid potential optimized-away problem
    static bool  is_empty_string( const char * str_ ) {
        if (!str_)
            return true;
        if (str_[0] == char(0))
            return true;
        return false;
    }

    bool empty() const noexcept { return is_empty_string(name_); }
    bool empty()       noexcept { return is_empty_string(name_); }

    static void print(person const& pson_)
    {
        if (pson_.empty()) {
            dbj_debug("%s", "can not print empty person");
            return;
        }
        else
            dbj_debug("\n\n%s\n", "Person");

        auto name_meta_state = pson_.name();

        if (name_meta_state.value)
            dbj_debug("Name: %s", name_meta_state.value);

        if (name_meta_state.status)
            dbj_debug("Name could not be obtained. Reason: %s", name_meta_state.status);
    }

private:
    string name_{};
};

```
Constructor can not return valstat (yet?), ditto we use factory function. 
```cpp
    constexpr inline 
       person::valstat make_person( string name_ ) noexcept {
        // INFO metastate
    return { person(name_), "Made in a factory" };
    }
```
Notice above we have achieved both compile time and pure function. Also enabled thanks to using valstat to return the metastate. 
Note we could use that at compile time but not as structured return due to the C++ rules.
```cpp
int main()
{
    // default person is empty
    person::print(person{});

    // warning: decomposition declarations are a C++17 extension
    if (auto [person_a, status] = 
        make_person("Stack"); 
        // an value
        !person_a.empty()) 
    {
        person::print(person_a);
    }
```
Arbitrary usage of another nested valstat type.
```cpp    
    if (auto [person_a, status] =  
        person::valstat_p { new person("Heap"), {} };
        // person is a pointer here
        !person_a->empty())
    {
        person::print(*person_a);
    }
}

```
Obviously OO practitioners who are also metastate adopters might have other very different, but equally valid designs,

