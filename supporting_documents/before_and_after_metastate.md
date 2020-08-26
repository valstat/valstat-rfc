# valstat use-cases<!-- omit in toc -->

## Contents<!-- omit in toc -->

- [1. Abstract](#1-abstract)
  - [1.1. DBJ, the Imagined Company, has adopted metastate paradigm and std::valstat is the core of its implementation.](#11-dbj-the-imagined-company-has-adopted-metastate-paradigm-and-stdvalstat-is-the-core-of-its-implementation)
- [2. functions returning void, with internal errors](#2-functions-returning-void-with-internal-errors)
- [3. Consuming index operator returns](#3-consuming-index-operator-returns)
- [4. User defined literals returning errors](#4-user-defined-literals-returning-errors)
- [5. what about references?](#5-what-about-references)
- [6. std::from_chars](#6-stdfrom_chars)

## 1. Abstract 

*"..be clever just if you absolutely must..."* -- Bjarne Stroustrup key note, [CppCon 2019](https://youtu.be/u_ij0YNkFUs)

Reality in this document is happening in a parallel universe where standard C++ library contains `std::valstat` too.

### 1.1. DBJ, the Imagined Company, has adopted metastate paradigm and std::valstat is the core of its implementation.

After considerable investment in [Node.JS N-API](https://nodejs.org/api/n-api.html), DBJ are now looking into C++ [WASM](https://webassembly.org/docs/c-and-c++/) modules too. 

To be able to pass more information from their API's, DBJ developers have decided to adopt the std::valstat concept. They have decide to define valstat status field, always as a string type. 

This also provides for valstat simpler for the initial consumption of the concept.

<table style="table_layout:fixed;  width:100%;" ><tr>
<th style="width: 50%" >std</th>
<th style="width: 50%" >dbj</th></tr>
<tr><td style="width: 50%" valign="top" >

Standard valstat shape.

```cpp
namespace std
{
template< typename T, typename S>
struct [[nodiscard]] valstat
{
	using value_type = T;
	using status_type = S;
	value_type value;
	status_type status;
};
}
```
</td><td style="width: 50%" valign="top" >

DBJ valstat is an template alias.

```cpp
namespace dbj
{
template< typename T>
using valstat = 
   std::valstat< std::optional<T>, std::optional<dbj::status_code> >;
}
```
</td></tr>
</table>

For the DBJ team, added benefit is that `dbj::valstat` type is simpler to use because it is differentiated by only the value type. There is no need for additional scaffolding for handling different status types. Internally DBJ team has an mature and elaborate code to message subsystem which they will re-use.

That makes the overall DBJ code landscape, even more standardized. And that improves the resilience and has a lot of economic sense.

Because all the functions on the DBJ API's using valstat are designed to return metastates, components interoperability is improved and that in turn shortens the time investment required.

Here are some of DBJ Imaginary Company, findings and experiences, after deciding to start with limited refactoring by gradually switching to the metastate paradigm.

## 2. functions returning void, with internal errors

This is a particular problem when converting a lot of legacy C/C++ code. Good and valuable algorithms, but mostly written by assuming server side, specific OS environment. As an example, N-API code is running "inside" N-API. There are no OS services. There is no console or stdio.h

N-API code is "compile once run everywhere". Almost.
<table style="table_layout:fixed;  width:100%;" ><tr><th style="width: 50%">Before</th>
<th style="width: 50%" >After</th></tr>
<tr><td style="width: 50%" valign="top" >

```cpp
// legacy code
void utf8_print_code_points(FILE * file_ptr_ , uint8_t * s) 
{
	uint32_t codepoint;
	uint32_t state = 0;

	for (; *s; ++s)
    if (!utf8_decode(&state, &codepoint, *s))
 {
	 if (state != UTF8_ACCEPT) {
     // perror()? Obviously there is a work to be done.
     // this module has no console available
 perror("\nThe string is not well-formed: ");
	return;
 }
	fprintf(file_ptr_, "U+%04X", codepoint);
 }
}
// usage, is crude, return type is void
// there is no stdout available
// there is no way to know if function has failed
// using errno might have helped, but using globals 
// is not functional programing friendly
utf8_print_code_points(stderr, (uint8_t*)SPECIMEN);

```

</td><td style="width: 50%" valign="top" >

```cpp
// valstat return type 
dbj::valstat<bool> utf8_print_code_points(FILE * file_ptr_ , uint8_t * s) 
{
	uint32_t codepoint;
	uint32_t state = 0;

	for (; *s; ++s)
		if (!utf8_decode(&state, &codepoint, *s))
		{
			if (state != UTF8_ACCEPT) {
        // ERROR metastate
				return { {/*empty*/}, dbj::code::the_string_is_not_well_formed };
			}
			fprintf(file_ptr_, "U+%04X", codepoint);
		}
    // OK metastate
	return { true, dbj::code::ok };
}
// usage
// before valstat the return was 'void' 
// and `perror()` was used internally
auto [val,stat] = 
   dbj::utf8_print_code_points(file_pointer, (uint8_t*)SPECIMEN);

// is it here or not here
// is all we care about
  if (!val && stat ) 
		// signal the error to the N-API caller
  if ( val )    
    // return the *val to the N-API caller
```
</td></tr>
</table>
The above after part is paradigm shift. special values are not required any more. Usage site is more complex but way more safe and resilient vs the legacy.

There are no exceptions in sight. That code is exception free. That has rendered smaller and faster binaries.

Please see: Craig Ben,   **Error size benchmarking: Redux** , http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1640r1.html

## 3. Consuming index operator returns

With localized error handling, no asserts and no exceptions are thrown. That is a big deal for DBJ N-API functions which must not exit().

<table style="table_layout:fixed;  width:100%;" ><tr><th style="width: 50%">Before</th><th style="width: 50%" >After</th></tr>
<tr><td style="width: 50%" valign="top" >
    
```cpp
std::array alfa = {'A', 'B', 'C', 'D'};

// ok index
auto a0 = alfa[0];

// bad index + index operator = crash?
auto a7 = alfa[7] ; 

// bad index + at = exception in flight
// landing site unknown
auto a8 = alfa.at(8) ;

```
This project is built with exceptions. Bad index provokes app termination or throw. Whatever is the OS in use, compiler in use and app design applied.
</td><td style="width: 50%" valign="top" >

  dbj have written valstat compliant array. Index operator returns dbj valstat. That is a crucial detail.

  ```cpp
  // synopsis
   struct array_vstat {
    dbj::valstat<T> operator [] (int idx_) ;
   } ;
```
Usage is simple and resilient with fully localized error handling.
```cpp

array_vstat alfa_vt = {'A', 'B', 'C', 'D'};

// index operator returns valstat
auto [val, stat] = alfa_vt[index_] ;

if (val) {
    // use_the_value 
    auto rez = *val  ;
}

if (stat) {
    // use the status
    // deal with error, if there is one
    auto status = *stat ;
}

```
</td></tr>
</table>

The "after" phase indexing operator is certainly more complex to use. But infinitely safer and resilient. Without throwing the exceptions.

## 4. User defined literals returning errors

Previously DBJ have written few crucial UDL's. How do we return error from UDL? Until valstat came on the scene, using exceptions or some elaborate return type. 

DBJ, have found using UDL's with valstat as a lighter and simpler, exception free solution. with almost no overhead. Here is an real life example.

<table style="table_layout:fixed;  width:100%;" ><tr><th style="width: 50%">Before</th>
<th style="width: 50%" >After</th></tr>
<tr><td style="width: 50%" valign="top" >

Transform, number to std::array of chars, at compile time.
```cpp
template< char ... Chs >
inline constexpr decltype(auto) 
  operator"" _charay()
{
  // max length is 42
 if (sizeof...(Chs) > 42 )
 // classical exception based approach
    throw std::runtime_error
       ("Never go over 42!") ;
// append '\0'
 return  std::array{ Chs..., char(0) };
}
```
</td><td style="width: 50%" valign="top" >

```cpp
// valstat returned from UDL 
template< char ... Chs >
inline constexpr decltype(auto) 
  operator"" _charray_valstat()
{
 using val_type = 
    std::array<char,1 + sizeof...(Chs)>;
 using vstt = dbj::valstat<val_type>;

// ERR state, no exception
  if (sizeof...(Chs) > 42 )
  return vstt{{}, "Never go over 42!"};

// OK state, no exception
  return vstt{ 
    val_type{ Chs..., char(0) } , {/*empty*/} 
  };
}
```
Usage is safe and resilient.
```cpp
auto [val, stat] = 123_charray_valstat;

// value
// *val is std::array<char,3> of
// three elements: '1','2','3'
if (val) cout << *val;

// status
// *stat is std::string
if (stat) cout << *stat;
```
</td></tr>
</table>

## 5. what about references?

std::valstat is using std::optional. Hence there is no danger coming from dangling pointers or dangling references, as std::optional contains the value handled.

But, faced with some legacy C++ API's, DBJ have met a real need to handle references. This is `std::valstat` issue because it is based on `std::optional`.  

Solution is simple. In case they need to pass references using valstat, DBJ decided to use `std::reference_wrapper`

```cpp
// valstat carrying reference
dbj::valstat<reference_wrapper<int> > ref_signal(int& input_ref_)
{
    if ( input_ref_ == 0 ) {
     input_ref_ = SIG_ATOMIC_MAX;
     return { {input_ref_} , {} };
    }
return { {} , "input must be zero" };
}

// usage
int arg = 0;
// valstat call
auto [val, stat] = ref_signal(arg);

#if _DEBUG
if (val)
   assert(*val == SIG_ATOMIC_MAX);
#endif
```
Above is somewhat controversial code. Standard C++ is value semantics based language. Thus solving references related  issues might be somewhat futile effort. Answer is simple: avoid references. If possible, that is.

## 6. std::from_chars 

Very good and performant addition to std:: lib. But using specific return type not seen anywhere else in the std::lib, perhaps a primordial form of valstat. Thus requires yet another special handling when calling.

Instead DBJ company have written a simple wrap-up, using the valstat. Idea is to use valstat everywhere, to standardize the code as much as possible.

<table style="table_layout:fixed;  width:100%;" ><tr>
<th style="width: 50%" valign="top" >Before</th>
<th style="width: 50%" valign="top" >After</th></tr>
<tr><td style="width: 50%" valign="top" >

```cpp
std::array<char, 10> str{"42"};
 int result;
 // not valstat concept
 // ec always represents an error
 // not a status
if(auto [p, ec] = 
   std::from_chars(
       str.data(), 
       str.data()+str.size(),
        result);
  // creates 0 error code
  // which is controversial        
   ec == std::errc()
   ) 
   {
     // ec is 0 here
  std::cout << result;
   } else {
// error
// ec is std::errc enum
// caller decides 
// what to do here
}
```
</td><td style="width: 50%" valign="top" >

```cpp
template< typename T>
inline dbj::valstat<T> 
  convert(std::string_view sv) 
{
 T rezult;
 if (auto [p, e] = std::from_chars(
     sv.data(), 
     sv.data() + sv.size(), 
     rezult);
	 e == std::errc())
	{
// valstat OK state
	return { rezult, {  } };
   }
else // valstat ERR state
   return { {}, dbj_errc_to_msg( e ) };
}
// usage
auto [val,stat] = convert<int>("42"sv);

if (val) 
// use_the_value 
    auto rez = *val  ;

if (stat)
    // use the status
    auto status = *stat ;
```
</td></tr>
</table>

As DBJ developers progress, we will add in here few more interesting use-cases proving the utility of valstat.

<table style="table_layout:fixed;  width:100%;" ><tr>
<thstyle="width: 50%" valign="top" >&nbsp;</th>
<thstyle="width: 50%" valign="top" >&nbsp;</th>
</tr>
<tr><tdstyle="width: 50%" valign="top" >

```cpp
```
</td><tdstyle="width: 50%" valign="top"  >

```cpp
```
</td></tr>
</table>
