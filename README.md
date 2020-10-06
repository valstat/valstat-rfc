# P2192

*As soon as we started programming, we found to our surprise that it wasn't as easy to get programs right as we had thought. Debugging had to be discovered. I can remember the exact instant when I realized that a large part of my life from then on was going to be spent in finding mistakes in my own programs.* —Maurice Wilkes discovers bugs, 1949 

Ultimately this is an effort to help eradicate programming errors. After several decades of being involved in all levels of computer programming, I might humbly add to Mr Wilkes observation: Very early in the process we have discovered that not every programmer knows exactly how to implement the requirement and is equally likely not fully able to do so.

This is not because programmers ( me included ) are less able to understand or learn. This is because programmers are very often supposed to implement what an ad-hoc software development lack of organization sends the programmers way.

Thus software diagnostics tools, testing concepts and programming idioms, are necessary to decouple from a constant and sometimes dramatic change in requirements. Or simply, lack of clear requirements.

## metastate

*"A paradigm is a standard, perspective, or set of ideas. A paradigm is a way of looking at something ... When you change paradigms, you're changing how you think about something..."* [vocabulary.com](https://www.vocabulary.com/dictionary/paradigm)

metastate is an software architecture paradigm. metastate is not yet another error return handling idiom. More precisely metastate is a paradigm about obtaining more information as a result of a function call. 

##  valstat

std::valstat is an metastate C++ implementation. std::valstat would be instrumental to the usability of metastate in C++ programs. 

> Together metastate and valstat are assembling an programming behaviour pattern.

## [Proposal P2192](P2192R2.md)

Next release to be published: [R2](P2192R2.md).

### Additional info

[This](https://github.com/DBJDBJ/metastate) is a repository hosting a development of the standard (ISO) C++ proposal ID: P2192 


- Here we have a folder with a number of [supporting documents](./supporting_documents/) 
- and some [supporting code](./code/) too. 
- Including a [folder with various pieces of information](./kb_documents/) ready to be included and reused in some larger documents.

### Contact

In case you hotly dislike this work, or if the situation is the opposite, or there is anything else you wish to communicate on the long scale in between these two extremes, please do not hesitate to contact the author:

(c) 2019, 2020 by [dbj at dbj dot org](mailto:dbj@dbj.org)

#### Content of this repository is licensed under the [License DBJ](./LICENSE.md)



