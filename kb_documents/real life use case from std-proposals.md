
### "Dmitry Dmitry"  "requirement

TL;DR: What do you think about creating a new flavour of control-flow
statements (in particular "return") that could return from outer/parent
scope?

In the thread "*Initialisers in ternary operators*" we had this example:
```cpp
 std::optional<std::string> FindUsersCity(bool non_default) {
 std::optional<ContactsServer> contacts =
 GetOrOpenContactsServerConnection();
 if(!contacts)
 return std::nullopt;
 std::optional<UserId> uid = contacts->GetUserId();
 if(!uid)
 return std::nullopt;
 std::optional<GeoServer> geo = GetOrOpenGeoServerConnection();
 if(!geo)
 return std::nullopt;
 std::optional<Location> uloc = geo->GetLocation(*uid);
 if(!uloc)
 return std::nullopt;
 return uloc->GetCityName();
 }
```

If conceptually it is fine, then what do you think about implementing the
idea by creating another flavour of return statements that can return not
only from its own scope, but also from outer/parent scope?
If you wish, this new flavour can be viewed as a middle ground between (1)
regular return's (which often requires boilerplate for handling the pattern
above) and (2) exceptions (which are too non-local and implicit and can
"jump" at an arbitrary place).

The code would look like this:

```cpp
std::optional<std::string> FindUsersCity() 
{
    std::optional<ContactsServer>   contacts =GetOrOpenContactsServerConnection();
    std::optional<UserId>           uid = contacts.GetOrReturnNullOpt()->GetUserId();
    std::optional<GeoServer>        geo = GetOrOpenGeoServerConnection();
    std::optional<Location>         uloc = geo.GetOrReturnNullOpt()->GetLocation(*uid);
return uloc.GetOrReturnNullOpt()->GetCityName();
}
```
where `GetOrReturnNullOpt()` is a method that (1) either returns a value
if std::optional is not empty, (2) or returns from FindUsersCity() if
std::optional is empty.

### Bary Revzin solution

As I said in https://lists.isocpp.org/std-proposals/2020/09/1811.php, this
already exists in C++20 in the form of coroutines... you just need to opt
in support for optional<T> in the only sensible way and then you can write:
```cpp
std::optional<std::string> FindUsersCity(bool non_default) {
    ConstactsSeriver cs = co_await GetOrOpenContatsServerConnection();
    UserId uid = co_await cs.GetUserId();
    GeoServer gs = co_await GetOrOpenGeoServerConnection();
    Location uloc = co_await gw.GetLocationId(uid);
    return uloc.GetCityName();
}
```
co_await here is effectively syntax sugar for and_then() (in the same way
in Haskell do-notation is syntax sugar for bind). It's just that the
continuation gets written as the next statement in the function rather than
the body of a lambda that gets passed as an argument.

Barry Revzin

## DBJ metastate solution

```cpp
// ignore the value, check it there is a status
#define check_return(VS) if ( int(VS.status) > 0) return {0, VS.status}
// call the valstat returning function and return if there is a status
#define call( VS, F ) auto VS = F ;    check_return(VS)

        valstat<const char> FindUsersCity() noexcept
        {
            call( contacts , GetOrOpenContactsServerConnection() );
            call( uid , contacts.value->GetUserId() );
            call( geo , GetOrOpenGeoServerConnection() ) ;
            call( uloc , geo.value->GetLocation(uid.value) );
            return uloc.value ->GetCityName();
        }

#undef call
#undef check_return
```

I think one *big* selling point for above is no std lib, no exceptions. 
And "as of Monday" that code can go into your pacemaker software straight away.

https://godbolt.org/z/G4qKGq