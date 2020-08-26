
# valstat myth buster<!-- omit in toc -->


## Table of contents<!-- omit in toc -->

- [Introduction](#introduction)
- [Claim: valstat is too simple](#claim-valstat-is-too-simple)
- [Claim: "simple" function do not benefit from valstat](#claim-simple-function-do-not-benefit-from-valstat)
- [Claim: recursion is "not good for valstat"](#claim-recursion-is-not-good-for-valstat)
- [Claim: valstat is not good for compile time](#claim-valstat-is-not-good-for-compile-time)
- [Claim: valstat computing, is not trivial](#claim-valstat-computing-is-not-trivial)
- [Conclusion: valstat is not standing in the way](#conclusion-valstat-is-not-standing-in-the-way)

# Introduction

 With `valstat` algorithms consume and use two kinds of information in the same time: the state and values returned. 4 states available, define the consuming algorithms.

  - Empty 
    - `return {{}, {}}`
  - OK 
    - `return { value, {}}`
  - Error 
    - ` return { {}, status }`
  - Info 
    - `return { value, status}`

API implementers are free to choose if they will use and return them all, one,two or three states.

Lets try and convince the readership of valstat benefits, with the following "Q&A"/"mith busters" session.

# Claim: valstat is too simple 

Answer: **False**

What is P0939R4?

P0939R4 is very important ISO C++ related document, produced by WG21 ISO C++ overarching authority. Which due to WG21 policies and politics, is not an "overarching authority". But still it is an unofficial roadmap to the better and simpler C++.

This author does hope, the valstat is seen as in support of the following ( P0939R4 -- "[Direction for ISO C++](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0939r4.pdf)", Page 5): 

*"... we badly need to simplify use of
C++ and that leaves us with three alternatives:

- Provide simpler alternatives for simple uses
- Provide simplifying generalizations
- Provide alternatives to complicated and/or error-prone features.
  
Often, a significant improvement involves a combination of those three..."

Perhaps a bold claim, but `valstat` conforms, to all the three "requirements" above.

# Claim: "simple" function do not benefit from valstat

**Answer: False**

Consider the [canonical example](https://stackoverflow.com/a/3947943) of an seemingly "simple function", made for avoiding integer overflow on addition.

```cpp
int add(int lhs, int rhs)
{
 if (lhs >= 0) {
  if (INT_MAX - lhs < rhs) {
   /* would overflow */
   abort();
  }
 }
 else {
  if (rhs < INT_MIN - lhs) {
   /* would overflow */
   abort();
  }
 }
 return lhs + rhs;
}
```

Conceptually safe and sound. But, it aborts. Not an option across many application domains. Thus, very often, C++ coders solution is to throw an exception. Usually, with the added "help" of "wall of macros". One might think this is legacy issue. But. Author has seen and is still seeing, a lot of code similar to the following.

```cpp
// gold plated, industrial strength C++? 
int add(int lhs, int rhs)
{
 if (lhs >= 0) {
  if (INT_MAX - lhs < rhs) {
   /* throw on overflow */
   VERY_SAFE_AND_CLEVER_MACRO_THAT_LOGS_AND_THROWS( 
      USE_PROPER_TYPE_FROM_WELL_DESIGNED_EXCEPTION_HIERARCHY, 
      SELECT_FROM_CLEVER_STORAGE_OF_MESSAGES 
      ) ;
  }
 }
 else {
  if (rhs < INT_MIN - lhs) {
   /* would overflow */
   VERY_SAFE_AND_CLEVER_MACRO_THAT_LOGS_AND_THROWS( 
      USE_PROPER_TYPE_FROM_WELL_DESIGNED_EXCEPTION_HIERARCHY, 
      SELECT_FROM_CLEVER_STORAGE_OF_MESSAGES 
      ) ;
  }
 }
 return lhs + rhs;
}
```
Beside immense complexity (aka over engineering) of any code similar to the code above, there is a [cold truth about the cost of using exceptions](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1640r1.html). 
Exceptions in flight, landing site unknown
. What could possibly go wrong? 

This and other reasons of not designing and coding like above, are making a long list.

Now. Please consider the same "simple function" with the valstat applied.

```cpp
// use the std::valstat where status is a string
template<typename T>
using valstat = std::valstat<T, std::string >;

valstat<int> add(int lhs, int rhs) noexcept
{
 if (lhs >= 0) {
  if (INT_MAX - lhs < rhs) {
   return {{}, "integer overflow" }; 
   // signal the ERROR state
  }
 }
 else {
  if (rhs < INT_MIN - lhs) {
   return {{}, "integer overflow" }; 
   // signal the ERROR state
  }
 }
 return { lhs + rhs, {}} ; 
 // signal the OK state
}
```

Consuming the returned state and values from this function are allowing returned value handling and local error handling, in a simple, clear and logical code.

```cpp
auto [val, state] = add( int_1, int_2 ) ;

if (val) //  OK
    use_the_val ( *val );
else { 
   // if no val is returned
   // state is returned
    process_the_error(*state);
}
```
Above code is not using all the four possile valstat states.

And there is mo exceptions. And mo special heavey return types, [found in some solutions](https://github.com/ned14/outcome/blob/develop/single-header/outcome.hpp) with thousands of lines log, single header.

# Claim: recursion is "not good for valstat"

Answer: **True**

True, because author wants to discourage the recursion as much as possible. 

> Just say no, to the recursion.

Consider the epitome of recursive functions, since the dawn of computing. The famous "fibonacci" is this:

```cpp
// canonical 
int fibonacci(int n) {
   if(n == 0){
      return 0;
   } else if(n == 1) {
      return 1;
   } else {
      return (fibonacci(n-1) + fibonacci(n-2));
   }
}
```

Nice, simple and unusable. Assumption is readers know why: parameter `n` can be too large for `int` result, or even process crash can occur because of recursion too deep. Provided processes are well isolated on the underlying OS, etc. 

Lets present the simple solution in the spirit of [P0939](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0939r4.pdf), adorned with `valstat`.

```cpp
// we declare the type used to store the results
 using fibo_type = int64_t;
// declare the valstat type
// status is POSIX error code
using valstat_fibo = dbj_std::valstat< fibo_type, std::errc >;

// int64_t can store fibonacci results for N <= 93
// we pre-compute and store them in the compile time table
constexpr auto fibo_table_size = 94;

constexpr int64_t fibo_table_64[ fibo_table_size ]{
   // pre compute values here
} ;
```

The outcome is very safe and simple, compile time fibonacci:

```cpp
constexpr inline valstat_fibo fibo(fibo_type n) noexcept
{
   // on invalid argument
   // signal the error and return the posix code
   if (n < 0 || n > fibo_table_size)
      return { {} , std::errc::invalid_argument };

   // return the precomputed value
   return { fibo_table_64[n], {} }; // signal OK
}
```

# Claim: valstat is not good for compile time

Answer: **False**

Reader has already noticed `constexpr` statements above. Let us show how simple the `valstat` makes compile time situations.

```cpp
// 
// structured binding can not be constexpr
// thus we take the valstat struct
constexpr auto vstat_ = fibo(93);
// and compile time assert check
static_assert( vstat_.value  , "OK: fibo(N), max N == 93 ");

// or in a one-liner style
// albeit not recommended
static_assert( fibo(93).value  , "OK: fibo(N), max N == 93 ");
```

And now, back to the "normal", runtime usage of our `valstat_fibo`

```cpp
// run time usage
 if (auto [val, stat] = fibo(42); val)
 {
  // val is retuned, OK state is signaled
  ::printf("\nfibo(42) ==  %ld ", *val);  
 }
else {
  // no val returned thus the error state is signalled
  ::printf("\nPOSIX errno: %d " , *stat);
}
```

# Claim: valstat computing, is not trivial

Answer: **True**

We do not engage in "trivial computing". We negage in safe computing. 

Consider the `valstat` consumption algorithms. They certainly are convincing but sometimes can be a bit complicating series of if's and else's. 

The remedies are numerous. Each does depend on the narrower context of the usage. For example. `valstat` goes well together with  the ["new" if()](https://en.cppreference.com/w/cpp/language/if) syntax, available as of C++17. Instead of:

```cpp
auto [val , stat ] = fun() ;
if ( val )
 // use val
 if ( stat )
 // use stat
```

It is simpler to use the new if() 

```cpp
if ( auto [val,stat] = fun(); stat) 
   report_error(*stat); // state no empty
else 
   use_(*val); // val not empty

```
Key point above: `val` and `stat` are not available outside of if() scope.

> Sequence of calls 

to valstat enabled functions can be non trivial. Although better expression might be: "Not simple enough".

Let's just present the clean and correct code using the new if() syntax:

```cpp
// let's try a sequence of valstat calls
 if (auto [val, stat] = fibo(41); stat) printf("error: %d", *stat);
   else // proceed
   if (auto [val, stat] = fibo(42); stat) printf("error: %d", *stat);
      else
      if (auto [val, stat] = fibo(43); stat) printf("error: %d", *stat);
         else
            printf("And the final fibo(43) == %I64d", *val);
```

It is hard to imagine the context in which the above snippet might be useful. What would you code if you would need results of both `fibo(41)` and `fibo(42)`?

For sequence of calls, in the same scope, I actually would recommend structured binding, Example:

```cpp
// using valstat_fibo = dbj_std::valstat< fibo_type, std::errc >;
valstat_fibo fibo_rezult{} ;

 fibo_rezult = fibo(41); 
 if (! fibo_rezult.val ) { printf("error: %d", fibo_rezult->stat); return; }
 printf("fibo(41) == %I64d", fibo_rezult->val);

 fibo_rezult = fibo(42); 
 if (! fibo_rezult.val ) { printf("error: %d", fibo_rezult->stat); return; }
 printf("fibo(42) == %I64d", fibo_rezult->val);

fibo_rezult = fibo(43); 
 if (! fibo_rezult.val ) { printf("error: %d", fibo_rezult->stat); return; }
 printf("fibo(43) == %I64d", fibo_rezult->val);
```

Much simpler and cleaner, vs complex if/else cascading.

# Conclusion: valstat is not standing in the way

Readers are encouraged to try these or numerous better coding idioms. Leading to simpler code. And that leads to resilient code. 

> valstat is certainly not standing in the way.
