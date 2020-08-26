### Restricted run time environments

Last but not the least, **valstat** makes coding without exceptions much easier. 
   
API using valstat is not throwing exceptions; consuming code is using simple local return handling.

For restricted runtime, users need non STL C++ valstat variant. Think gaming, IoT, medical instruments, etc. Non STL valstat option **might** look like this synopsis: 

```cpp
// valstat variant for 
// restricted environments
// still standard C++
template< typename T, typename S>
struct [[nodiscard]] valstat_light final 
{
   using type = valstat ;   
   using value_type = T;
   using status_type = S;   
   
   value_type * value{};
   status_type * status{};
}
```
Author is aware, one of the foundations of standard C++ is value and reference semantics. That C++ template is still valstat concept. But, without  the STL presence.

Consuming valstat_light has no conceptual difference vs consuming std::valstat. Of course proper care has to be taken to assure value or status are not dangling pointers.