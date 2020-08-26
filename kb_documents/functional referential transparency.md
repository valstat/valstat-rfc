#### Functional referential transparency

metastate adoption makes C++ functions with no side effects possible. Using valstat instance allows for much cleaner C++ functional programing.

Honorable readership of this paper is of course well aware, in pure functional languages functions cannot have side effects. That is, they are completely determined by what they return, and cannot do anything else. Most notably they cannot throw exceptions, or set globals. 

valstat passing a metastate out, allows for **referential transparency**: f(42) in purely functional code, can be evaluated once, and the value it returns can be replaced for every other instance of f(42) in the program. 

This has been done before and elsewhere, but not with the use of one unifying core idea.