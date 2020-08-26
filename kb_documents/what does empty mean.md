## What does empty mean?

*The definition of empty is: having or containing nothing, or having no worth, meaning or value.*[[2]](#ref2)

In the context of an IT system empty means: missing information. But empty has to have some representation is software and hardware.

To explain what is empty we will explain why do we need **null**. The necessity of having both empty[[1]](#ref1) and null concepts is probably best described in the context of a database.

Database field can be empty or occupied with a value. But the field certainly and always exists. How do we know it is empty? Because it contains a **null**. In fact in the database theory  the preferred way is to say "The field contains a null", instead of saying "The field is empty"[[3]](#ref3) 

**"EMPTY" metastate**

What is it for? It might be used by adopters operating on the abstraction exhibiting the "empty or not" capabilities. Like database field is. If they operate on persistent data they might use the metastate concept to pass back to the caller full information after the field reading operation. For example:

```cpp
// field_information() API is using valstat to pass back 
// one of the metastates 
// for obtaining full information after 
// the attempted field value retrieval
auto [ value, status ] = database.full_field_info( field_name ) ;

// canonical metastates capturing idiom 
// capturing all four possible metastates 
// objective is callers comprehension of 
// a full information passed out of the function

if (   value &&   status )  { /* metastate: info */ 
   // for example some significant value has been retrieved
   std::cout << "\nSignificant value found: " << *value ;
  }

if (   value && ! status )  { /* metastate: ok   */ 
   std::cout << "\nRetrieved value: " << *value ;
  }

if ( ! value &&   status )  { /* metastate: error */ 
   // in this scenario status contains an error message
   std::cout << "\nRead error: " << *status ;
  }

if ( ! value && ! status )  { /* metastate: empty */ 
   std::cout << "\nField is empty." ;
  }

```
How adopters will use the metastate and code the solution, is entirely up to them. As long as they stay true to the paradigm.

**C++ type system and "emptiness"**

Fundamental C++ types can not have occupancy state tested, that is they can not be "empty" or not. Consider this:
```cpp
// default bool is initialized to false
// by the system
bool k ; 
// k is never "empty" it can be only in two states: true or false
```
Fundamental types can not exist without a value assigned to them by system or a programmer. Instances of fundamental types can not be in an "empty" or "not empty" state of occupancy.

Compound types can aid implementing the meaning of empty operation on them. For an example see [std::empty](https://en.cppreference.com/w/cpp/iterator/empty).
```cpp
// C is some std lib Container
template <class C> 
constexpr auto empty(const C& c) noexcept -> bool
{
    return c.size() == 0 ;
}
```
Thus for example std::string can be in "empty" or "not empty" state.

--- 

-- Wikipedia -- *Empty String*, <span id="ref1">[1]</span> https://en.wikipedia.org/wiki/Empty_string

-- "Your Dictionary" -- *Definition of empty*, <span id="ref2">[2]</span> https://www.yourdictionary.com/empty

-- Mark Whitehorn 2007 -- *What does null mean?*,<span id="ref3">[3]</span> https://link.springer.com/chapter/10.1007%2F978-1-84628-687-2_31