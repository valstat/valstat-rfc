
### 4.2. Is valstat an original concept?

There are seemingly similar mechanisms and types in few other programming languages. As far as author is concerned none of them is based on the using the "Meta State" concept.

Most notably [GO](https://blog.golang.org/go1.13-errors) and [Rust](https://doc.rust-lang.org/book/ch09-02-recoverable-errors-with-result.html) are using the dedicated types to deliver non-mandated returns handling concept. (GO online doc: "..This form can be used to improve on a couple of clumsy idioms in C programs: in-band error returns such as -1 for EOF and modifying an argument passed by address.." )

In contrast to valstat metastate, all are making the key conceptual difference of handling "error or no error", single state. Having only 'error' in the vocabulary. There is no richer return signaling as it is made possible by  the concept of metastate. There is no semantic richness of metastate.