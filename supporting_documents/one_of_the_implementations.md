#  valstat
## Implementation matters, concept is the key

Same as its concept, architecture and design, **valstat** implementation must be simple, resilient and feasible. And performant. 

There are many possible valstat implementations. Each conforming to the concept and architecture. But also developed inside the scope of the particular project. Here are the just a few.

#### Most Restricted Run-Time

Probably the most low-level valstat shape. 

```cpp
struct valstat {
 void * val ;
 void * stat;
} ;
```
It is easy to imagine "wall of macros" to alow for normal usage of that.  But it certainly will work and will be "close to the metal". And, important for those run-times, that  will allow for integration with (home grown) C API-s. 

#### Speed Concerned C++ Requirement

C++ projects where space and speed are of an essence.  Think gaming, no `std::` lib, and [similar environments](https://www.perforce.com/resources/qac/misra-c-cpp).

```cpp
template <typename T, typename S> 
struct valstat_template final {
   using type = valstat_template ;
   using value_type = T ;
  using status_type = S;
  T * val ;
  S  * stat ;
};
```
This valstat variant, does not mean heap allocation of course. This is an "[Trivial Type](https://en.cppreference.com/w/cpp/types/is_trivial)". As a such it is compile time friendly. Usage is decently comfortable.
```cpp
enum class my_status_code {
  SUCCESS = 0,
  HAS_MORE, // more data
   . . .
};
   // my specific valstat 
    template<typename T>
     using my_valstat = valstat_template<T, my_status_code > ;

   // POSIX valstat 
  //  https://en.cppreference.com/w/cpp/error/errc
    template<typename T>
     using posix_valstat = valstat_template<T, std::errc > ;

const char * status_to_message ( my_status * ) ;
const char * status_to_message ( std::errc  *) ;
```

Hopefully most of the time, teams will be not severely bounded with run-time limitations and would like to use `std::valstat` in its standard form. Here is one longer example from an imaginary company and its C++ team.

## JSON as the  Status -- Implementation

It is envisaged C++ projects/components/libraries in many organizations will be/are written in standard C++ and are mostly server side components. They are server side because most (all) production code in the front and middle tier is not C++ written. Business logic is in the middle tier. That is the domain of languages like Java, and run-times like Node.js.  

As a such the system tier, C++ components, will need to utilise simple message passing mechanism between internal and external (perhaps other organizations) remote or local components. Think Cloud Computing.

For that purpose, the globally adopted default is [JSON](http://json.org/) data interchange format. 

JSON is also used on the application (code) level. For Node.JS code , that means there are no language specific transformations to/from JSON on the component boundaries. JSON is JavaScript.

> For the real life *`valstat`* implementation, one can do far worse then implement status as  json formatted string. 

There is abundance of quality json coding/decoding [c++ libraries](https://github.com/nlohmann/json) too, to be used on both intra and extra component levels.

## Implementation summary

Thus far, the dbj implementation might be using the `std::valstat` in the following manner: 
```cpp
// std::valstat used by the fictitious company: dbj
// (c) 2019/2020 by dbj@dbj.org
// Licence CC BY SA 4.0
namespace dbj {
    template< typename T >
	using valstat = std::valstat<T, std::string >;
} // dbj
```
`dbj::valstat` is a template alias, using `std::valstat`. where the status type is resolved to be always a `std::string` type.

In this scenario valstat template depends only on the value type. Or rather: it creates types defined by it's value type only.

dbj have decided to keep the name `valstat`, thus they have to be careful with possible name clashes with `std::valstat`.

Template type definitions are concrete types. Here is some concrete usage.
```cpp
namespace dbj {
	//  concrete valstat types for intrinsics
	using valstat_int  = valstat<int> ;
	using valstat_bool = valstat<bool> ;
} 
```
One can easily imagine all the valstat types pre-declared for all the intrinsic C++ types.

#### Status as a JSON, implementation 

`dbj::valstat` status type is `std::string`. That is the scope of this valstat design. `std::string`, might be an overkill. In reality, any character buffer might be a less complex solution. 

Dbj developers, could design and implement much more elaborate json infused concept. But, they have decided that would bring complexity with no obvious benefits. Perhaps it is best to wait for std::json type in some future C++.

"Status as a string" allows for total decoupling from the status type. Which in turn allows for pick-n-mix of any kind of statuses, which in turn means status choice can be decided at the very moment of preparing a valstat to be returned, not before. DBJ team  do not need to pre-declare various elaborate valstat hierarchies, for different status types.

With this design, dbj developers, can (depending on the context) use very simple or very elaborate statuses when creating dbj::valstat instances. 

For example: If required they might create some complex status logic, as json messages, to be  passed between server side remote components.  Components likely implemented in different languages and for different runtime environments. Even residing on different clouds.

For development of their components, dbj will re-use just one very simple json formated status, actually passing all the information required, in vast majority of development uses cases. And they have decided they will do exactly that on the intra-component level. (inside their components). 

```cpp
//
// one function used dbj components to build the valstat status for any kind of valstat type. 
//
std::string make_status(const char* file, long line, const char * time_stamp, const char* msg);
// example of return
// "{ "message" : "division with zero", "file" : "math.h", "line" : 12475, "timestamp" : "Nov 20 11:35:23 2019" }",
```
Same as in decades before, but in different format, required by todays cloud computing infrastructure, micro services, and such.

## Samples

First the functionality for creating messages from `std::errc` codes.
```cpp
namespace posix {
	inline std::string errc_to_message(std::errc posix_err_code) ;
} 
```
Note: dbj do use `std::string` just for a simpler documentation. Any char buffer type will do. Anything that users do prefer.

**Example**

Armed with this design dbj developers have implemented (and still do) some usefull changes. Here are some of them.

`std::from_chars()` is one of the recent, very good additions to std lib. But, it could be said for some users, it has one perhaps ["peculiar" interface](https://en.cppreference.com/w/cpp/utility/from_chars). Let's wrap that up using `valstat`.

```cpp
using namespace std;
template< typename T>
inline dbj::valstat<T> convert(string_view sv) noexcept(true)
{
		static_assert( is_arithmetic_v<T> ) ;
	T rezult;
	if (auto [p, e] = from_chars(sv.data(), sv.data() + sv.size(), rezult);
		e == std::errc() 
		)
	{
		// pass back valstat OK state
		// no "special macro" necessary
		return { {rezult}, {  } };
	}
	else {
		// make and pass valstat error state
		// status is message in json format.
		return { {}, { make_status
			( __FILE__, __LINE__, __TIMESTAMP__, 
				posix::errc_to_message().c_str( e ) ) 
			} };
	}
}
```
Consuming logic is demonstrably simpler vs "naked" `std::from_chars`. 

The key point: This valstat enabled API is not a surprise. Even if you have never seen it before. Even if it would have no comments.
```cpp
   	using namespace std;
// we require int type for a return value
// stat type is always json formated string
auto [val,stat] = convert<int>("42"sv);

// cover all four possible states
// consuming logic is application specific
	cout << "Value: "
	if (val ) 
    	 cout << *val ;
	else
    	 cout << "Empty" ;

	cout << endl << "Status: "
	if (stat) 
		cout << *stat ;
	else
    	 cout << "Empty" ;
```
#### Macros, what macros?

Notice, how macros are not necessary here. Nothing is stopping users to invent and use them. But, valstat is certainly not forcing them to use macros.

Example. In some situations, users will just "pass further" the `valstat` instances. It seems there is  no "need" to use the macros here too.
```cpp
auto valstat_ = convert<int>("42"sv);
// Show why is the valstat instance returned
if ( ! valstat_.value ) 
      	return valstat_ ;
```
We can "pass around" the valstat instances easily and by value.

###	PARADIGM SHIFTING

In practice valstat does bring some surprising benefits.

For example, [] opearator returning valstat delivers a simple solution "for bad index problem". It is a problem because today, `std::` lib throws [std::out_of_range](https://en.cppreference.com/w/cpp/error/out_of_range) exception from indexing operators. 

```cpp
namespace dbj {
        // simple arrays that does not throw exceptions
	template< size_t N>	struct arry final
	{
	array<char, N> buff_{ };

	// valstat return does not require
	// exception thinking fame of mind
	dbj::valstat<char> operator [] (size_t idx_) const noexcept
	{
		if (idx_ >= buff_.size())
			return { {}, { make_status(__FILE__, __LINE__, __TIMESTAMP__,"Index too big") } };

		return { { buff_[idx_] } , {} };
	}
	}; // arry
}
```
The usage. In normal, index within bounds, situations, `val` is used as ever before.

```cpp
arry<0xFF> xarr{{"0123456ABCDEFGH"}};
// OK
auto [val, stat] = charr[7];
// 7 is ok index, no error state
if (val) cout << *val ;
```
Bad index, is easily  reported, and application proceeds.
```cpp
// ERROR state return
auto [val, stat] = charr[42];
if (stat) cout << stat ;
```
What also matters here is local handling of error state. In this particular case we are not pushed all of a sudden, and far away, to where some "bad index" exception might be caught. Provided we know where is that location.

Using valstat, users simply and locally handle the error situation. And then proceed in sync with the application logic.

### Trade offs and comparisons

`std::valstat` is based on the qualities and abilities of  `std::optional<>`

### Forms of structured binding

References to valstat elements can also be obtained:
```cpp
auto & [val, stat] = f();
... or ...
const auto& [val, stat] = f();
```
That is an epitomy of an "C++ foot-gun". Possible to code but "blows your leg off". Thus: do not do it.

## Conclusion

Using `std::valstat`, beside other already mentioned benefits, delivers consistent API's where adoption barrier is lowered. Consuming states are never a "surprise". 

The API author does not have to invent API specific return concept. Users already understand the `std::valstat` concept. Even if they have never seen it before.

Types returned are API specific. In case of the dbj company, status is always a json message. How is that handled depends on the API and consuming application context. 

**valstat** is a familiar concept. Its value raises with its  ubiquity. 

#### Ubiquity requires interoperability.

Let's start with C interoperability.

### [One of the possible C interops](./one_of_the_possible_C_interops.md)
