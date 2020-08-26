### 2.2. Motivation

*Divergent error handling has fractured the C++ community into incompatible dialects, because of long-standing
unresolved problems (in C++ exception handling)* -- [Herb Sutter, P0709R4](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0709r4.pdf)

Current state of affairs in the area of "error handling" and lack of standard and modern mechanism is what `std::valstat` might solve, that and urgency of common solution is the main motivation of proposing metastate concept and `std::valstat`. Some of the problems C++ users at large are faced with are:

- Currently (2020 Q3) there is no usable, simple and resilient standard C++ returns concept, or std type. And there is no consensus around such a thing. No thing, no consensus. Time lines for the past possible solutions are measured in years or even decades. Inevitably, sooner or later, each and every project, 
designs and implements some organization, company, project or library specific concept to handle the non-trivial logic of communicating, from some home grown API. Those efforts are instantly lowering the interoperability between different C++ components, projects and systems, because of using different return types and returns handling concepts. It might be the time for WG21 to publish the official concept and its implementation. 

- Detrimental effects of exceptions is well documented. [[6]](#ref6). But, today (2020Q3) exceptions are alive and well. Majority of exceptions based solutions are often designed by "simply" creating specific `std::exception` hierarchy. Or by using "special" "throwable" types. Or even worse, by mixing the two. Usually the amount of complex code to implement these solutions is many thousands of lines of complex C++.

- For the lack of better, primordial C artefact **errno**, is still in a widespread use in modern C++ today  ( std::errc ).

- `<system_error>` is legacy but in the standard. For std::error_code and adjacent problems please look no further than [Summary of SG14 discussion on <system_error>](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0824r1.html)

---

## References

- Craig Ben,   **Error size benchmarking: Redux** , <span id="ref1">[1]</span> http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1640r1.html