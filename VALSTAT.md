<h1>valstat - returns handling protocol</h1><!-- omit in toc -->
 
| &nbsp;           | &nbsp;                                                |
| ---------------- | ----------------------------------------------------- |
| Document Number: | **DBJ0001**                                           |
| Date             | 2021-01-01                                            |
| Audience         |                       |
| Author         | Dusan B. Jovanovic ( [dbj@dbj.org](mailto:dbj@dbj.org) ) |
<h4>
Simplicity is the ultimate sophistication -- Leonardo Da Vinci
</h4>
<!--
There are two ways of constructing a software design: One way is to make it so simple that there are obviously no deficiencies, and the other way is to make it so complicated that there are no obvious deficiencies. -- [C.A.R. Hoare](https://en.wikiquote.org/wiki/C._A._R._Hoare) -->

## Table of Contents<!-- omit in toc -->

- [1. Abstract](#1-abstract)
- [2. Motivation](#2-motivation)
- [3. The Protocol](#3-the-protocol)
- [4. Conceptual building blocks](#4-conceptual-building-blocks)
  - [4.1. VALSTAT structure](#41-valstat-structure)
  - [4.2. Field](#42-field)
    - [4.2.1. Occupancy states](#421-occupancy-states)
  - [4.3. VALSTAT State](#43-valstat-state)
  - [4.4. The VALSTAT Responder](#44-the-valstat-responder)
- [5. Why the "two"](#5-why-the-two)
- [6. VALSTAT and the global IT landscape?](#6-valstat-and-the-global-it-landscape)
- [7. Conclusions](#7-conclusions)
- [8. References](#8-references)
- [9. Appendix: Cross Domains Requirements](#9-appendix-cross-domains-requirements)
  - [9.1. Appendix: Climate impact](#91-appendix-climate-impact)
- [10. Appendix: VALSTAT state BNF definition](#10-appendix-valstat-state-bnf-definition)

## Revision history<!-- omit in toc -->

R1: created

<!-- div class="page"/ -->

## 1. Abstract

This is a proposal about logical, feasible, lightweight and effective protocol for handling the call/response activity in an both platform and language agnostic manner.

This paper describes an software protocol, not language specific implementations or platform specific solutions.

<span id="motivation" />

## 2. Motivation 

Context: Integration is a common recurring theme in a software development. It is complex and difficult to predict in a feasible level of detail.

Levels of Integration

1. universal
1. domain
1. system
1. application
1. component
2. code

A call/response activity exists an all levels.

Calling, creating a response, returning a response and handling the response an all levels. 

Resilience requires a common and simple guidance. A standard. A protocol.

<!--
### 2.1. Narrow scope 

**Universal returns handling protocol is missing**

Valstat idea is dealing with general call/response activity in an satisfactory universally applicable manner. Lack of that inevitably has contributed to a rising technical debt. 

Lack of a common and ubiquitous non trivial returns handling protocol is raising the level of complexity for all levels of software architectures.

Narrow scope, closed logic, returns handling, are already designed and deployed on micro levels inside thousands of components. Also exhibiting even more API's. That fact inevitably present an hard requirement for code development. 

Common "Returns Protocol" is the next step in evolution of call response handling. Need for a common protocol is the true picture of a real life requirements.  

`VALSTAT` protocol is firmly opposed to a simple "error or no error", call/response logic. 

### 3. Wider Scope

Each and every development effort, eventually meets the integration requirements, yet there is very little advice on how to approach them, before they become a problem. Large scale integration efforts are external to any programming language, of course. But directly shaping the logic and style of system architecture in very large portion of today's projects. 

Valstat is that little protocol designed to aid integration between architectures

Wider scope motivation is to develop an paradigm to aid solving following three core categories, affected by existence of running software systems:

1. Run-Time
2. Interoperability
3. Energy
-->
## 3. The Protocol

A call/response, software activity guided by a protocol is a paradigm shift.

*"A paradigm is a standard, perspective, or set of ideas. A paradigm is a way of looking at something ... When you change paradigms, you're changing how you think about something..."* [vocabulary.com](https://www.vocabulary.com/dictionary/paradigm)

## 4. Conceptual building blocks

VALSTAT protocol is based on a structure returned as a consequence of a call.

- `CALLER` is software entity calling the local or remote function or component. 
- `RESPONDER` is a software entity creating a VALSTAT structure to be returned to a `CALLER`

```
CALLER --> (local/remote call) --> RESPONDER

CALLER <-- (VALSTAT structure) <-- RESPONDER
```
<font size=2>(We use BNF. [Backus–Naur form or Backus normal form (BNF)](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form) is a metasyntax notation for context-free grammars.)</font>
```
--> ::= { remote | local } {"call" | "data flow"}
```

IMPORTANT: `CALLER` or `RESPONDER` can reside inside the same or different, application or system domains. Local or remote to each other.

**Response using logic is divided in two steps**

As part of response handling activity, the two steps logic is always present: 

- step one -- Is something returned? 
- step two -- Can I use it?   

Conceptually valstat protocol is the "two steps" returns processing:

1. use the structure returned to determine the step two
    - not using the type system or actual values returned
2. use the content returned
    - using the type system and values returned

That is a two step logic, that can be applied across many software development platforms and run-time domains.

Going further, the VALSTAT structure is made of two fields. 

### 4.1. VALSTAT structure

VALSTAT structure is an record made of two fields:

- VALSTAT record
  - `VALUE`
  - `STATUS`
<!--
Language agnostic aka pseudo code

```
begin record VALSTAT  
     declare field VALUE
     declare field STATUS
eof record
```
-->
### 4.2. Field

VALSTAT "field" is analogous to the database field. <!-- The name "field" is the name for an "single piece of information" -->

In database theory also "field" is: [*"a particular piece of data encapsulated within a class or object"*](https://en.wikipedia.org/wiki/Field_(computer_science)).

Key attributes of a Valstat field concept are:

1. field always exists
2. field can be empty or "contain a value"

#### 4.2.1. Occupancy states

Field can be in two "occupancy states"  (authors term) . We will call them : "empty" and "occupied". For example, it is a well known and adopted fact: database field always exist, but it can be empty.

![field two occupancy states](media/field%20occupancy%20states.png)

In software development terms field is an object **potentially** holding only one value. It might be tested if it is "empty" , or occupied aka "holding a value".

### 4.3. VALSTAT State 

VALSTAT State is an boolean result of AND combination of occupancy states of two VALSTAT structure fields. 

Combination of two fields value *and* status occupancies is giving four possible states. 

We will name them for further use.

| Meta State Tag | Value occupancy  | op | Status occupancy  |
| ----- | ------- | ------- | --- |
| **Info**  | Has value | AND | Has value |
| **OK**    | Has value | AND | Empty     |
| **Error** | Empty     | AND | Has value |
| **Empty** | Empty     | AND | Empty     |

VALSTAT state tags are just that: tags. Not mandating but just indicating the behavior. 

In adopting the returned VALSTAT structure two step handling we do not immediately inspect returned values. In step one we inspect the relationship of two fields returned first.

In the most (all?) programing languages, there is no need for existence of some dedicated VALSTAT types.  Returned structure handling, step one is only about the relationship between two fields states of occupancy.

VALSTAT state capturing is the act of decoding the relationship between occupancy states of its two fields.

Following is synopsis of capturing the four possible VALSTAT states:

```cpp
// pseudo code
// two fields are input into the step one:
// capturing one of four possible VALSTAT states
// In step one types or values of the
// content returned are not used
// they are irrelevant in step one
// 
  if (   is_empty( value ) &&   is_empty( status )  { /* state: info */ }
  if (   is_empty( value ) && ! is_empty( status )  { /* state: ok   */ }
  if ( ! is_empty( value ) &&   is_empty( status )  { /* state: error*/ }
  if ( ! is_empty( value ) && ! is_empty( status )  { /* state: empty*/ }
```
That synopsis can be implemented, almost as it is, in many languages: C, JavaScript, Python, GO, Java etc. 

NOTE: in particular language definition of the VALSTAT we very often do not need `is_empty()` function implemented. 

Here is canonical C++ example.

```cpp
// calling
auto [value, status] = valstat_enabled_function ();

// step one: capturing the one
// of four possible states
  if (   value &&   status )  { /* info */ }
  if (   value && ! status )  { /* ok   */ }
  if ( ! value &&   status )  { /* error*/ }
  if ( ! value && ! status )  { /* empty*/ }
```

Protocol return handling code is neatly divided in steps. VALSTAT handling logic serves well for arriving to cleaner idioms for complex call consumption code. 

The aded benefits are immediate applicability and ability in addressing the requirements across domains.

### 4.4. The VALSTAT Responder

From the callable entity VALSTAT Responder is "signaling back" following the internal logic and creating and returning the appropriate VALSTAT structure.

The VALSTAT structure declaration.
```
begin record VALSTAT  
     declare field VALUE
     declare field STATUS
eof record
```
RESPONDER signaling the "OK" state
```
// pseudo code
return VALSTAT  
     VALUE = (value), STATUS = (NULL)
eof return
```
Both `value` and `NULL` are meaningful as defined by the application domain. Or a system domain. On the higher level there are inter system formats like JSON or XML.

RESPONDER signaling the "INFO" state
```
return VALSTAT  
     VALUE = (value), STATUS = (information)
eof return
```
RESPONDER signaling the "ERROR" state
```
return VALSTAT  
     VALUE = (NULL), STATUS = (information)
eof return
```
RESPONDER signaling the "EMPTY" state
```
return VALSTAT  
     VALUE = (NULL), STATUS = (NULL)
eof return
```

## 5. Why the "two"

Context depend on the domain.

As ever in the information systems, the meaning of the information is context specific.

VALSTAT is a protocol for light **and** efficient information passing. 

VALSTAT is not a messaging protocol. Contrast it to some protocol using (for example) XML (or JSON) documents for functions calling and function return values. Author is not aware of such a protocol. But anything between that and record of two fields is not as light and not as efficient as VALSTAT structure.

And going bellow the two, degenerates back to the "error value return" anti-pattern.


## 6. VALSTAT and the global IT landscape?

VALSTAT protocol value lies in it's deliberate simplicity, aiding in solving the strict operational requirements and interoperability issues.

Modern software architectures are clusters of inter-operating but separated components and even sub-systems. One thorny big issue, is solving universally applicable returns handling, across language and system  barriers.  And this is where VALSTAT as a protocol might help. (think JSON format for VALSTAT protocol) 

Universal adoption of the VALSTAT requires no changes in any of the software development languages. It just has ot be universally used.

<!-- div class="page"/ -->

## 7. Conclusions

Hopefully proving the benefits of evolution of error code handling into returns handling protocol does not need much convincing. 

There are many real situations where the VALSTAT paradigm can be successfully used. From a micro to the macro level. 

As an common returns protocol, VALSTAT state requires to be ubiquitously adopted to become truly an instrumental to widespread interoperability. From micro to macro levels. From inside the code to inter component calls. From inside an project to inter systems situations.

VALSTAT protocol is multilingual in nature. Thus adopters from any imperative language are free to implement it in any way they wish too. The key requirement is: interoperability. 

Authors primary aim is to achieve widespread adoption of this paradigm. As shown VALSTAT is more than just solving the "error-signalling problem". It is an paradigm, instrumental in solving the often hard and orthogonal set of run-time requirements.

It works on the micro level but VALSTAT aims high. And it's proposed scope is rather wide. But it is a humble protocol. It is just an simple and effective way of building bridges over one deeply fragmented part of the vast global IT territory. While imposing extremely little on adopters implementations and leaving the non-adopters to "proceed as before" if the wish too.

Obstacles to VALSTAT paradigm adoption are far from just technical. But here is at least an immediately usable attempt to chart the way out.

----------------------

<!-- <div class="page"/> -->

## 8. References

- <a id="ref1">[1]</a> B. Stroustrup (2018) **P0976: The Evils of Paradigms Or Beware of one-solution-fits-all thinking**, https://www.stroustrup.com/P0976-the-evils-of-paradigms.pdf

- <a id="ref2">[2]</a> Wikipedia **Empty String**, https://en.wikipedia.org/wiki/Empty_string

- <a id="ref3">[3]</a> "Your Dictionary" **Definition of empty**,  https://www.yourdictionary.com/empty

- <a id="ref4">[4]</a> Joel Spolsky, **Joel On Software -- 13: Exceptions**, https://www.joelonsoftware.com/2003/10/13/13/

<!-- div class="page"/ -->

## 9. Appendix: Cross Domains Requirements

**Run-Time **

One motivation of this paper is to try and offer an "over arching", but simple enough, returns handling paradigm applicable across the IT landscape. Including across a growing number of programing dialects, fragmenting away the industry and markets relying on existence of the standards.

**Interoperability**

Each solution to often strict run-time requirements is one nail in the coffin of interoperability. In danger of sounding like offering an panacea,  author will also draw the attention to the malleability of the VALSTAT paradigm to be implemented with wide variety of languages used in developing components of an modern distributed system. 

Usability of an API is measured on all levels: from the code level, to the distributed system level. In order to design an API to be **feasibly** usable it has to be interoperable. That leads to three core requirements of

Interoperable API core requirements (to start with)

1. no "error code" as return value
   - Complexity arises from "special" error codes multiplied with different types multiplied with different context
   - In turn increasing the learning curve for every function in every API
   - How to decode the error code, becomes a prominent issue
     - Think Windows: `NTSTATUS`, `HRESULT`, `GetCode()`, `errno`
2. no "return arguments" aka "reference arguments" where applicable.
   - language specific mutable argument solutions are definitely not interoperable.  
3. no special globals
   - Think C errno legacy 
   - pure functions can not use globals

Some of the designed-in, simplicity in this paper is an result of deliberate attempt to increase the interoperability (also with other run-time environments and languages). 

It is important to understand there are inter domain interoperability requirements. Examples: [WASM](https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm), [Node.JS](https://nodejs.org/api/addons.html), [Android](https://developer.android.com/studio/projects/add-native-code) and such. 

### 9.1. Appendix: Climate impact

This is operational environment requirement. Operational environment can be satellite in an orbit circling the earth, or a data center on Island. 

Side info: it is not just hardware energy consumption, it is also energy required to [cool that data-center hardware](https://en.wikipedia.org/wiki/Dennard_scaling) down.

Solving data centers energy spending has become an imperative. Most of the server side software is written in C/C++ . Pressure is on, to design and develop with energy consumption as an primary requirement. 

[This one is not a "simple" requirement](https://hal.archives-ouvertes.fr/hal-01496266/document). [Somewhat paradoxically](https://youtu.be/koTf7u0v41o) this category of requirements requires less and less code and more and more performance in the same time. Smaller executables means less energy spent on that executable running and less energy for cooling the CPU running it.

## 10. Appendix: VALSTAT state BNF definition

```
; occupancy states
empty     ::= true | false
has_value ::= true | false
occupancy_state ::== empty | has_value

; field is made of
; occupancy state and value
field ::=  occupancy_state AND value

; 'value' and 'status' are fields
value  ::= field
status ::= field

; VALSTAT state is AND combination of 
; two fields occupancy states
; combination of occupancy_states of two fields
VALSTAT state ::= is_empty(value) AND is_empty(status)

; valstat is a record made of two fields
valsat ::= value AND status
``` 

[Backus–Naur form or Backus normal form (BNF)](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form) is a metasyntax notation for context-free grammars.