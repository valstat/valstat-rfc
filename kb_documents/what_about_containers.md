#### What about containers?

The primary role and purpose of definitions of std::valstat<T,S> is to allow for transparent metastate passing idiom. `std::valstat` is not designed to act as possible element type of various std lib containers. 

If required by particular adopter, such a secondary role can be easily be achieved by the added layer of user code. An effort greatly simplified by using std::optional\<T\>, sometimes known as "container of one".
