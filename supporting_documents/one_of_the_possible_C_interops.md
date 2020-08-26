# std::valstat -- transparent return type
## One of the possible implementations of C interoperability

The ubiquity of a valstat is important. An possible implementation of interoperability with C is presented here.

The C language was always able to return structures from functions. Using standard C++ such a structure being returned from a C function, can also be consumed with C++ structured binding.
```
/* this is C code */
/*
The whole of valstat C interop is in the two macros bellow
*/
#define valstat_interop_( T ) typedef struct interop_valstat_##T \
{ \
T * value ; \
const char * status  ; \
} interop_valstat_##T

#define valstat_interop( T ) valstat_interop_( T )

/* use this to declare valstat interop type */
#define valstat_interop_decl_( T ) interop_valstat_##T 

```
In C the concept of "value is there or not there", can be most naturally implemented with native pointers. The usage.
```cpp
/* create
    struct interop_valstat_int { int * value; const char * status; } 
    */
valstat_interop(int);
/* create 
    struct interop_valstat_uint64_t  { uint64_t * value; const char * status; } 
    */
valstat_interop(uint64_t );
```
How would one valstat compliant function developed in C, now look like? The declaration.
```cpp
	extern "C" valstat_interop_decl(uint64_t)
		divider(uint64_t dividend, uint64_t	divisor);
```
(We assume `#ifdef __cplusplus` protectors need not be shown here.)

The implementation.
```cpp
/* we like to use json formatted strings as status */
#define FLT(M) \
"{ \"file\": \"" __FILE__  "\", " \
 " \"line\": \""  STRINGIZE(__LINE__) "\", "  "\"message\": \"" M "\", \"timestamp\": \"" __TIMESTAMP__  "\" }" 

/* this is C code */
inline valstat_interop_decl(uint64_t)
  divider (uint64_t	dividend, uint64_t	divisor)
{
	if (0 == divisor)
		return	(interop_valstat_uint64_t){	NULL, FLT("invalid_argument: zero divisor") };

	if ((dividend) >= UINT16_MAX)
		return (interop_valstat_uint64_t) { NULL, FLT("dividend too large") };

	if ((divisor) >= UINT16_MAX)
		return (interop_valstat_uint64_t) { NULL, FLT("divisor too large") };

	// quick and dirty cludge
	static uint64_t rezult_anchor = 0;
		rezult_anchor = (dividend / divisor);
	// valstat OK state
	return (interop_valstat_uint64_t) { & rezult_anchor , NULL };
}
```
We can use the above C function, from the cpp code, while enjoying all the benefits of valstat.
```cpp
// structured binding of a result from a C function
auto [value, status] = divider( 13 , 42 );

// covering all four states
cout << "\nvalue:\t";
if (value)
    cout <<  *value ;
else
    cout <<  "{ empty }" ;

cout << " / status:";
if (status)
    cout << *status ;
else
    cout <<  "{ empty }" ;

cout << endl;
```
Very non intrusive, fully implemented consuming logic. Using C / C++ , valstat interop.
