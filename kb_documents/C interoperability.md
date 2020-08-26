## C interoperability

It is entirely possible to use the metastate concept with the C code. And then to transparently consume such a code from C++. Here is a quick example of one of several possible designs.

```cpp
/* C code */
typedef struct int_valstat {
    int * value ;
    const char * status ;
} int_valstat ;

int_valstat valstat_compliant_c_api () 
{
   static int v42 = 42;

   /* OK state return */
   if ( time_is_right() )
    return (int_valstat){ & v42, NULL };

   /* ERROR state return */
    return (int_valstat){ NULL, "The time was not right" };
} ;
```
C consumers point of view
```cpp
/* C code consuming C valstat */
int_valstat valstat = valstat_compliant_c_api () ;

   if ( valstat.value ) {
      return *valstat.value ;
   } 
   if ( valstat.state ) 
   {
      async_log ( *valstat.state );
   }

```
For the C++ consumers, usage idiom is completely unchanged, vs calling some other valstat enabled C++ API.
```cpp
// cpp code consuming C function
auto [value, status] = valstat_compliant_c_api () ;

   if ( value ) {
      return *value ;
   } 
   if ( state ) 
   {
      async_log ( *state );
   }

```
If the called valstat enabled function is C or C++, that is entirely transparent to the C++ caller.

Added benefit is in having richer signaling from legacy C API's, vs single state "error or no error" legacy concept.

From entirely different context but very interestingly fitting sentence:

*"..This form can be used to improve on a couple of clumsy idioms in C programs: in-band error returns such as -1 for EOF and modifying an argument passed by address.."* -- (online documentation about GO errors package)*