<h1>metastate for callable constructors</h1><!-- omit in toc -->
 
| &nbsp;           | &nbsp;                                                |
| ---------------- | ----------------------------------------------------- |
| Document Number: | **PXXXX**                                           |
| Date             | 2020-09-21                                            |
| Audience          | WG21       |
| Author         | Dusan B. Jovanovic ( [dbj@dbj.org](mailto:dbj@dbj.org) ) |

<h2>&nbsp;</h2>

- [1. Abstract](#1-abstract)
- [2. Motivation](#2-motivation)
- [3. Callable constructors returning the metastate](#3-callable-constructors-returning-the-metastate)
- [4. Synopsis](#4-synopsis)
  - [4.1. noexcept](#41-noexcept)
  - [4.2. Heap construction](#42-heap-construction)
  - [4.3. Unfinished instances](#43-unfinished-instances)
- [5. The usage](#5-the-usage)
  - [5.1. What about new](#51-what-about-new)
- [6. Summary](#6-summary)

<h2>&nbsp;</h2>

"There are two ways of constructing a software design: One way is to make it so simple that there are obviously no deficiencies, and the other way is to make it so complicated that there are no obvious deficiencies. The first method is far more difficult." -- [C.A.R. Hoare](https://en.wikiquote.org/wiki/C._A._R._Hoare)

<h2>&nbsp;</h2>

## 1. Abstract

This paper describes an very simple callable constructors mechanism, by imposing a core language change that is fundamental but in the same time non breaking.

## 2. Motivation

In the constructor the runtime uses type data to determine how much space is needed to store an object instance in memory. After this space is allocated, the constructor is called as an internal part of the instantiation and initialization process to initialize the contents of the instance.

Then the constructor exits, the runtime returns the newly-created instance. So the reason the constructor doesn't return a value is because it's not called directly, it's called by the memory allocation and object initialization code.

C++ constructors only way to signal the construction outcome, is to throw an exception. Projects where exception are mandated to be non existent are using factory methods to create class instances. That has effectively created another C++ dialect and fragmented the community.

Using metastates carrier return type we can design a simple way around that limitation of constructors. 

## 3. Callable constructors returning the metastate

First we will mandate an constructor return type signature. That is a type which instance is returned from a constructor of type T.

```cpp
 /* determine value of type T, occupancy state */ 
    template<typename T>
       bool is_empty( T const & ) ;

/*  mandated constructor valstat signature
    definition of a template is a type
    this is the signature compiler will require 
    for a type to be returned from a callable class
*/
namespace my {
  template<typename T *, typename S> struct valstat 
  {
    // constructed instance pointer or a nullptr
    T * value; 
    // type whose state of occupancy can be determined
    S status;

    /* NOT MANDATED methods!
        determine the occupancy state of both
      bool is_empty ( value ) { ... }
      bool is_empty ( status ) { ... }
    */
  };
} // my
```
Basically it is a two type parameter template made of two members of those same two types. We call it `valstat` as an abbreviation of "value and status". 

Instances of definitions of that template are used to "capture the four metastates" as described in [P2192](https://github.com/DBJDBJ/metastate).

Combination of value *and* status occupancies is giving four possible metastates. 

| Meta State Label | Value occupancy  | op | Status occupancy  |
| ----- | ------- | ------- | --- |
| **Info**  | Has value | AND | Has value |
| **OK**    | Has value | AND | Empty     |
| **Error** | Empty     | AND | Has value |
| **Empty** | Empty     | AND | Empty     |

<h2>&nbsp;</h2>

**Valstat is the metastates carrier.**

<h2>&nbsp;</h2>

## 4. Synopsis

 Now we shall present the synopsis of the callable constructor paradigm. Class can have a mixture of callable and non-callable (aka "normal") constructors.

```cpp
// synopsis of a class having callable constructors
struct person 
{
    // rule 1: 
    // constructor return type must be nested valstat template in the same class
    // to which constructor belongs to.
    // such valstat must be having a mandated signature
    // first argument must be the pointer to the ise defined type being constructed
    // that type state of occupancy is determined by using the `bool empty()` method 
    // on the same type
    // second type is any type to which state of occupancy can be determined
    //
    // first template argument must equal the reference to the type inside which 
    // the valstat is nested
    //
    // option 1.1 : there can be one or more nested types following
    // the rule 1
    // 
    // first arg must be a person and it must be a reference
    // second arg must allow to determine its state of occupancy
    using valstat = my::valstat< person * , const char * > ;

    std::string name ;

   // rule 2: callable constructor has the same signature as 
   // any other non callable constructor and
   // all the other language rules for constructors do apply
   person () noexcept : name("") 
   {
     /*
     option 2.1: constructor can have 
     one or more returns returning 
     the nested valstat instances
     
     constructor return type rules are
     the same rules as for any other function

     bellow we return person::valstat
     */
       return 
       valstat{ *this, "default person constructed" };
   }

   // Non-rule: if constructor has no return type
   // in its signature it can not be called as a function
   // RULE 3: callable constructors can not be declared explicit
   explicit person person ( string new_name_) : name (new_name_) 
   {
   }

   // non-rule :
   // for some user defined types is is "good thing" to be able to
   // answer to the message "are you empty?"
   bool empty () const noexcept {  return name.empty() ;}
   bool empty ()       noexcept {  return name.empty() ;}

   // destructors were always callable
   ~person () { if (! empty() ) name = ""; }

   // the standard assignments are unchanged
   person & operator = ( person const & ) noexcept = default ;
   person & operator = ( person && ) noexcept = default ;

   // but nothing is stopping us to return valstat<person *, const char *>
   // from copy or move constructors

    // callable copy constructor
    // NOTE: if this is not 'called' instance copying 
    // rules are unchanged and working as ever before
    auto person ( person const & another_ ) noexcept
    {
             if ( another_.empty() )
             {
               return { nullptr, "refused to copy an empty person" };
             }

            // OK metastate return
             return { this, nullptr } ;
    }

    // callable move constructor
    // NOTE: if this is not 'called' instance moving 
    // rules are unchanged and working as ever before
    auto person ( person && another_ ) noexcept
    {
             if ( another_.empty() )
             {
               return { nullptr, "refused to move an empty person" };
             }

            // OK metastate return
             return { this, nullptr } ;
    }

}; // eof person
```
### 4.1. noexcept

Until now noexcept constructors have been a best guess. In this scenario noexcept might be a truly no exceptions thrown mark. At least when callable constructors are concerned.

### 4.2. Heap construction

RULE 4: Constructors can not be called if class is made on heap. 

### 4.3. Unfinished instances

In case of returning a valstat prematurely i.e.from an unfinished object, `this` will be a nullptr.

## 5. The usage

To use the above "callable constructors enabled" class might be called: easy, simple and natural. 

Traditional  construction.
```cpp
// old object instantiation syntax
// using a constructor as ever before
// compiler ignores any returns from the constructor
person p() ;
person * pp = new person();
return person() ;
```
Remember: class with callable constructor will be usable as all the "legacy" classes before.
Alternatively one could capture the metastate as a result of a constructor call
```cpp
// calling the constructor
// return type is: person::valstat< person *, const char *>
// pp is a person instance pointer
// status type is "const char *"
auto [ pp , status ] = person() ;

// using the metastates
if ( pp ) {
    std::cout << "new person is created" ;
} else {
  std::cout << "person default constructor has failed." ;
}

if ( status ) {
    std::cout << "status is: " << status ;
} else
    std::cout << "status is empty" ;
} 

```
Notice in this example, we do not need to implement or use `is_empty()` methods because both value and status are pointers.

If constructor does not contain return statements, it can not be called
```cpp
// compilation error -- non callable constructor
auto [ p , status ] = person("Mr Person") ;
```

### 5.1. What about new 

It is very simple. Constructor can not be called if class is made on heap. But placement new can be used.

```cpp
 // example placement new for a default constructed person
 void *operator new(size_t s, person::valstat< person*, const char *> & pv_ )   
      { 
         // create on stack and save person() instance
         // return the valstat result
          auto valstat_ = person_allocator::allocate () ;
          // copy the valstat
          pv_ = valstat_ ;
          // can be person pointer or nullptr
           return valstat_.value ;
      }
   // usage
     person::valstat pv ;
        person * pp = new (pv) person() ;

    // using the metastates
if ( pv.value ) {
    std::cout << "new person is created" ;
} else {
  std::cout << "person default constructor has failed." ;
}

if ( pv.status ) {
    std::cout << "status is: " << pv.status ;
} else
    std::cout << "status is empty" ;
} 
  // delete as ever
  delete pv.value ;

```

## 6. Summary
Can this mechanism be abused? Anything in C++ can be abused. Standard constructor paradigm can be abused. Callable constructors can be abused too.

This language extension would not brake any existing code. It will make it possible for strictly-no-exceptions project to design and use classes too. 
