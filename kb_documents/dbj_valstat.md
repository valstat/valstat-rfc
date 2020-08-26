
## dbj valstat

Probably the simplest but fully functional std::valstat implementation is this:

```cpp
namespace dbj {
    template< typename T, typename S >
    using valstat 
      = std::valstat< std::optional<T>, std::optional<S> > ;
}
```

Why std::optional\<T\>? Standard "vocabulary" type `std::optional` is used to implement the: "value is there or not there", what we call  the "occupancy state". It also encapsulates the solution of the issues of carrying the value, not reference or pointer. standard C++ is value semantics based language. Types handled by std::valstat must be usable in the context of std::optional<\T\>  

The only other **feasible** way would be to use smart or native pointers. Hopefully there is no need to explain, the increase of complexity in that scenario, although native pointers, is exactly the design option author uses for constrained environments. 

In this canonical implementation, `std::valstat`  type behavior is fully determined by features and capabilities of `std::optional`.

The only valstat requirements for the value and status types, are the ones imposed by the std::option type.

Over-elaborating by using some special status or value types made specifically to be used with valstat is certainly possible but not recommended.

Author has not yet encountered a real need to go beyond strings or fundamental types used as status type.

Remember: Four valstat meta-states do not depend on the actual value or actual status values. Value type can be almost "anything", std::optional allowing.

If  concrete valstat template definition (aka type) can be moved, copied, assigned, swapped etc, that entirely depends on std::optional embedded.