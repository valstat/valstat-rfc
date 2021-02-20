# P2192 -- a Function Return Protocol

## Why?

*As soon as we started programming, we found to our surprise that it wasn't as easy to get programs right as we had thought. Debugging had to be discovered. I can remember the exact instant when I realized that a large part of my life from then on was going to be spent in finding mistakes in my own programs.* —Maurice Wilkes discovers bugs, 1949 

 "... If you look at the source of your program, you'll notice routines that you call, but which you didn't implement..." -- Peter Van Der Linden

Ultimately this is an effort to help eradicate programming errors. After a lot of time spent being involved in all levels of computer programming, I might humbly add to Mr Wilkes observation: Very early in the process we have discovered that not every programmer knows exactly how to implement the requirement and is equally likely not fully able to do so. Why?

This is not because programmers are less able to understand or learn. This is because programmers are very often supposed to implement what an software development lack of organization sends the programmers way.

Software diagnostics tools, testing concepts and programming idioms, are necessary to decouple from a constant and sometimes dramatic change in project surroundings. Or simply, lack of clear requirements.

Also. Safe code is slow(er).Dangerous code is fast(er). valstat might help in that proverbial balancing act.Especially in modern distributed systems made up of legacy and modern components, developed in various languages or developed by various organizations.

## Paradigm

*"A paradigm is a standard, perspective, or set of ideas. A paradigm is a way of looking at something ... When you change paradigms, you're changing how you think about something..."* [vocabulary.com](https://www.vocabulary.com/dictionary/paradigm)

Valstat is a protocol and an software architecture paradigm. Not yet another error return handling idiom. More precisely valstat as a protocol, is a paradigm about obtaining more information as a result of a function call. 

> Together a protocol and its implementation are assembling an behaviour pattern.

# Documentation

valstat documentation is deliberately technical, short and to the point. Prose is left to be authored in future texts in response to criticizm, comments and clarification requirements.

> valstat protocol is designed to be language agnostic.

There is a root document describing the protocol and core concepts. Then there is a number of documents describing the implementation in few widely used programing languages. More to come.


1. valstat Protocol
2. c implementation
   1. planned ISO WG14 proposal 
3. c++ implementation
   1. ISO comittee WG21 (LEWG) [Proposal P2192](P2192R3.md)
4. javascript implementation
5. c# implementation
6. go implementation

## The Code

Is in separate repository [HERE](https://github.com/DBJDBJ/metastate_code).

### What is in here

[This](https://github.com/DBJDBJ/metastate) is a repository hosting a development of the standard (ISO) C++ proposal ID: P2192 


- Here we also have a folder with a number of [supporting documents](./supporting_documents/) 
- Including a [folder with various pieces of information](./kb_documents/) ready to be included and reused in some larger documents.

### Contact

In case you hotly dislike this work, or if the situation is the opposite, or there is anything else you wish to communicate on the long scale in between these two extremes, please do not hesitate to contact the author:

(c) 2019, 2020 by [dbj at dbj dot org](mailto:dbj@dbj.org)

#### Content of this repository is licensed under the [License DBJ](./LICENSE.md)



