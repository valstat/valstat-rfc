## 3. The Design

<cite>*"... behavioral design patterns are design patterns that identify common communication patterns among objects and realize these patterns. By doing so, these patterns increase flexibility in carrying out this communication."* <a href="https://en.wikipedia.org/wiki/Behavioral_pattern" >Source</a></cite>

Behavior of valstat metastate producers and consumers is actually more important than the type itself. `std::valstat` as a type is almost transparent. It is just a metastate carrier.

Both  producing and consuming code, are not faced with some special and heavy, return type. Consuming valstat, by using C++ structured binding, does not go against the concept. The concept is to observe together the state of occupancy of value and status returned. 

This proposal aim is to use standard C++ to do that elegantly, transparently and simply. On both consuming and returning sides of the function divide.