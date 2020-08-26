
### There is a number of "modern" return types proposals. 

The "other" proposals are for standardised error handling. They might be in the same category as valstat but are conceptually very different.
Core idea of the valstat is not yet another special type. It is the concept of metastate. 

Current situation with special return types is

- They are universally implemented as (very) complex types. Measuring thousands of lines of non trivial C++ code. Through years many of them are abandoned.
- std::exception is in the [curious state](https://www.reddit.com/r/cpp/comments/c75ipk/why_stdexpected_is_not_in_the_standard_yet_is_it/) of "forever close to adoption"
- 2020 Q3 probably the most advanced return type is **outcome**. Please see https://github.com/ned14/outcome. Its single header version measures 7223 lines (6951 sloc). In my humble opinion its usage is far from trivial.
- There is also a "final solution" in the form of [P0709](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0709r2.pdf) aka "deterministic exceptions"
    - basically the idea is keep the try/catch but dramatically change what is being thrown, thus leading to lighter and faster executable.
    - Author is in agreement with that idea. That is far reaching core language proposal. Alas, not to be implemented before C++23.
- Author claims the key problem with general concept of try/throw/catch is non locality of error handling. `catch` site can be "very far away" and that is the big source of subtle bugs. Both logical and technical.

`std::valstat` can not be compared to those. It is not a special type, it is just a metastate carrier. Very often callers do not even deal with instances of `std::valstat`

```cpp
// no special type involved
  auto [value, status] = metastate_returning_function () ;
```

**metastate** also helps to avoid try/throw/catch mechanism and to avoid imposing a "special return type" on more or less unwilling audience, after decades of trying. 
