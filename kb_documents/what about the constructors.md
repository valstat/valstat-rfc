## What about the constructors?

### Up to C++23

valstat is not a core language extension. Thus it can not solve the issue of constructors disabled to throw exceptions.

One way to offer some remedy is additional constructor argument as valstat reference.

But there is another theoretical possibility.

## callable constructors returning the metastate

### At or after  C++23

Core language proposal using the metastate paradigm.
```cpp
// the metastate constructor mechanism
struct person 
{
    // rule 1: 
    // must declare nested type using the std::valstat
    //
    // option: there can be one or more nested types following
    // the same rule
    // 
    using valstat = std::valstat< person , const char * > ;

    string name ;

   // rule 2: for constructor to be callable 
   // it must return by value instance of one 
   // of the the nested valstat types
   // constructor declaration rules are unchanged 
   person () : name('') {
     /*
     rule 3: optionally constructor can have 
     one or more return statements passing 
     the type based on std::valstat
     
     compiler infers the type of the return 
     from the first return statement
     following the  same rules as for 'auto' adorned
     function declarations
     */
       return 
       valstat{ this, "default construction: no problems" };
   }

   // rule 4: if constructor has no return statements
   // it can not be called as a function
   person person ( string new_name_) : name (new_name_) 
   {
      // no returns, 'traditional' constructor
      // can not be called as a function
   }

   // rule 5:
   // abstraction used as valstat value type must have 
   // an method named empty()
   // matching the logic and reasoning of std::empty()
   bool empty () const noexcept {  return name.empty() ;}
   bool empty ()       noexcept {  return name.empty() ;}

}; // eof person
```
To use the above "metastate enabled" class is easy, simple and natural. Traditional  construction.
```cpp
// using a constructor as ever before
person p{} ;
// or
return person{} ;
```
Alternatively one could capture the metastate as a result of a constructor call
```cpp
// calling the constructor
// NOTE: structured binding requires an rvalue
auto [ p , status ] = person() ;

if ( ! p.empty() ) {
    person p2 = *p ;
}
```
If constructor does not contain return statements is can not be called
```cpp
// compilation error
auto [ p , status ] = person("Mr Person") ;
```
This language extension would not brake any existing code.