<h1>valstat - call/response <!-- returns handling--> protocol</h1><!-- omit in toc -->
 
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
  - [3.1. Conceptual building blocks](#31-conceptual-building-blocks)
  - [3.2. VALSTAT structure](#32-valstat-structure)
  - [3.3. Field](#33-field)
    - [3.3.1. Occupancy states](#331-occupancy-states)
  - [3.4. VALSTAT State](#34-valstat-state)
  - [3.5. The VALSTAT Responder Behavior](#35-the-valstat-responder-behavior)
- [4. Why the "two"?](#4-why-the-two)
- [5. VALSTAT and the IT landscape?](#5-valstat-and-the-it-landscape)
- [6. Conclusions](#6-conclusions)
- [7. References](#7-references)
- [8. Appendix: VALSTAT BNF definition](#8-appendix-valstat-bnf-definition)

## Revision history<!-- omit in toc -->

R1: created

<!-- div class="page"/ -->

## 1. Abstract

This is a proposal about logical, feasible, lightweight and effective protocol for handling the call/response activity in an both platform and language agnostic manner.

This paper describes an software protocol, not language specific implementations or platform specific solutions.

<span id="motivation" />

## 2. Motivation 

Context: Integration is a common recurring theme in a software development. It is complex and difficult to predict in a feasible level of detail.

Levels of Integration, ordered by the scope width

| scope | level
|-------| -------
| widest | universal
|       | domain
|       | system
|       | application
|       | component
| narrowest      | code

Calling, creating a response, returning a response and handling the response, is activity present an all levels. 

Feasibility of the integration requires a high level of resilience. Integration resilience requires a common and simple guidance. An standard protocol.

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

### 3.1. Conceptual building blocks

VALSTAT protocol is based on a structure returned as a consequence of a call.

- `CALLER` is software entity calling the local or remote function or component. 
- `RESPONDER` is a software entity creating a VALSTAT structure to be returned to a `CALLER`

```
CALLER --> (local/remote call) --> RESPONDER

CALLER <-- (VALSTAT structure) <-- RESPONDER
```
Locality: `CALLER` or `RESPONDER` can reside inside the same or different, application or system domains. Local or remote to each other.

**Response using logic is divided in two steps**

As part of response handling activity, regardless of platform or language, the two steps logic is always present: 

- step one -- Is something returned? 
- step two -- Can I use it?   

Conceptually valstat protocol is the "two steps" return processing:

1. use the structure returned to determine the step two
    - not using the type system or actual values returned
2. use the content returned
    - using the type system and values returned

That is a two step logic, that can be applied across many (if not all) software development platforms and run-time domains.

### 3.2. VALSTAT structure

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
### 3.3. Field

VALSTAT "field" is analogous to the database field. "field" is the name for an "single piece of information".

In database theory "field" is: [*"a particular piece of data encapsulated within a class or object"*](https://en.wikipedia.org/wiki/Field_(computer_science)).

Key attributes of a field are:

1. field always exists
2. field can be empty or "contain a value"

#### 3.3.1. Occupancy states

Field can be in two "occupancy states"  (authors term) . We will call them : "empty" and "occupied". 

![field two occupancy states](media/field%20occupancy%20states.png)

In software development terms field is an object **potentially** holding only one value. It might be tested if it is "empty" , or occupied aka "holding a value".

### 3.4. VALSTAT State 

VALSTAT State is an boolean result of AND combination of occupancy states of two VALSTAT structure fields. 

Combination of two fields value *and* status occupancies is giving four possible states.

![valstat record state decoding](media/VALSTAT%20Record%20States.png)
Valstat Record state decoding

| State Tag | Value occupancy  | op | Status occupancy  |
| ----- | ------- | ------- | --- |
| **Info**  | Has value | AND | Has value |
| **OK**    | Has value | AND | Empty     |
| **Error** | Empty     | AND | Has value |
| **Empty** | Empty     | AND | Empty     |

VALSTAT state tags are just that: tags. Not mandating but just indicating the behavior. 

In adopting the returned VALSTAT structure two step handling we do not immediately inspect returned values. In step one we inspect the relationship of two fields returned first.

In the most (all?) programing languages, there is no need for existence of some dedicated VALSTAT types.  Returned structure handling, step one is only about the relationship between two fields states of occupancy.

VALSTAT state decoding is the act of decoding the relationship between occupancy states of its two fields.

Following is synopsis of decoding one of the four possible VALSTAT states:

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
That synopsis can be implemented, almost as it is, in many languages: C, JavaScript, Python, GO, Java, C# etc. 

NOTE: in particular language definition of the VALSTAT we very often do not need `is_empty()` function implemented. Here is canonical C++ as an example.

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

VALSTAT Protocol return handling code is neatly divided in two steps. VALSTAT handling logic serves well for arriving to cleaner idioms for complex call consumption code. 

### 3.5. The VALSTAT Responder Behavior

From the callable entity, an VALSTAT Responder is "signaling back" following the encapsulated application logic; creating and returning the appropriate VALSTAT structure.

The VALSTAT structure pseudo code declaration.
```
begin record VALSTAT  
     field VALUE
     field STATUS
eof record
```
RESPONDER behavior consists of creating the appropriate VALSTAT record. RESPONDER signaling the "OK" state:
```
// pseudo code
return VALSTAT  
// occupied value field and empty status field 
     VALUE = (value), STATUS = (NULL)
eof return
```
Both `value` and `NULL` are meaningful software entities, as defined by their application domain. RESPONDER signaling the "INFO" state
```
return VALSTAT  
// occupied value field and occupied status field 
     VALUE = (value), STATUS = (information)
eof return
```
RESPONDER signaling the "ERROR" state
```
return VALSTAT  
// empty value field and occupied status field 
     VALUE = (NULL), STATUS = (information)
eof return
```
RESPONDER signaling the "EMPTY" state
```
return VALSTAT  
// empty value field and empty status field 
     VALUE = (NULL), STATUS = (NULL)
eof return
```

## 4. Why the "two"?

Context depend on the domain. As ever in the information systems, the meaning of the information is context specific.

VALSTAT is a protocol for light **and** efficient information passing. 

VALSTAT is not a messaging protocol. Contrast it to some protocol using (for example) XML (or JSON) documents for functions calling and function return values. Author is not aware of such a protocol. But anything between that and VALSTAT record of two fields, is not as light and not as efficient as VALSTAT structure.

And going bellow the two, degenerates back to the "single error value return" anti-pattern.


## 5. VALSTAT and the IT landscape?

VALSTAT protocol value lies in it's deliberate simplicity, still capable aiding in solving the software operational and interoperability issues.

Modern software architectures are clusters of inter-operating but separated components and even sub-systems. One thorny big infrastructure issue, is solving universally applicable returns handling, across language and system  barriers.  And this is where VALSTAT as a protocol might help. (think JSON format for VALSTAT protocol) 

Universal adoption of the VALSTAT requires no changes in any of the software development languages. It just has to be universally adopted and used.

<!-- div class="page"/ -->

## 6. Conclusions

Hopefully proving the benefits of evolution of error code handling into returns handling protocol does not need much convincing. 

There are many real situations where the VALSTAT protocol can be successfully used. From a micro to the macro level. 

As an common returns protocol, VALSTAT state requires to be ubiquitously adopted to become truly an instrumental to widespread interoperability. From micro to macro levels. From inside the code to inter component calls. From inside an project to inter systems situations.

VALSTAT protocol is multilingual in nature. Thus adopters from any imperative language are free to implement it in any way they wish too. For non coherent groups of adopters, the key requirement is: interoperability. 

Authors primary aim is to suggest widespread adoption of this paradigm. As shown VALSTAT is more than just solving the "error-signalling problem". It is an paradigm, instrumental in solving the often hard and orthogonal set of run-time operational requirements.

VALSTAT aims high. And it's proposed scope is rather wide. But it is a humble protocol. It is just an simple and effective way of building bridges over deeply fragmented parts of the vast global IT infrastructure. While in the same time imposing extremely little on adopters implementations and leaving the non-adopters to "proceed as before" if they wish too.

Obstacles to VALSTAT adoption are far from just technical. But here is at least an immediately usable attempt to chart the way out.

----------------------

<!-- <div class="page"/> -->

## 7. References

- <a id="ref1">[1]</a> B. Stroustrup (2018) **P0976: The Evils of Paradigms Or Beware of one-solution-fits-all thinking**, https://www.stroustrup.com/P0976-the-evils-of-paradigms.pdf

- <a id="ref2">[2]</a> Wikipedia **Empty String**, https://en.wikipedia.org/wiki/Empty_string

- <a id="ref3">[3]</a> "Your Dictionary" **Definition of empty**,  https://www.yourdictionary.com/empty

- <a id="ref4">[4]</a> Joel Spolsky, **Joel On Software -- 13: Exceptions**, https://www.joelonsoftware.com/2003/10/13/13/

<!-- div class="page"/ -->

<!--
## 8. Appendix: Cross Domains Requirements

**Run-Time **

One motivation of this paper is to try and offer an "over arching", but simple enough, returns handling paradigm applicable across the IT landscape. Including across a growing number of programing dialects, fragmenting away the industry and markets relying on existence of the standards.

**Interoperability**

Each solution to often strict run-time requirements is one nail in the coffin of interoperability. In danger of sounding like offering an panacea,  author will also draw the attention to the malleability of the VALSTAT paradigm to be implemented with wide variety of languages used in developing components of an modern distributed system.

Usability of an API is measured on all levels: from the code level, to the distributed system level. In order to design an API to be **feasibly** usable it has to be **interoperable**. That leads to three core requirements of

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

### 8.1. Appendix: Climate impact

This is operational environment requirement. Operational environment can be satellite in an orbit circling the earth, or a data center on Island. 

Side info: it is not just hardware energy consumption, it is also energy required to [cool that data-center hardware](https://en.wikipedia.org/wiki/Dennard_scaling) down.

Solving data centers energy spending has become an imperative. Most of the server side software is written in C/C++ . Pressure is on, to design and develop with energy consumption as an primary requirement. 

[This one is not a "simple" requirement](https://hal.archives-ouvertes.fr/hal-01496266/document). [Somewhat paradoxically](https://youtu.be/koTf7u0v41o) this category of requirements requires less and less code and more and more performance in the same time. Smaller executables means less energy spent on that executable running and less energy for cooling the CPU running it.
-->

## 8. Appendix: VALSTAT BNF definition

```
; occupancy states
empty     ::= true | false
occupied  ::= true | false
occupancy_state ::== empty | occupied

; field is logically made of
; occupancy state and value
field ::=  { occupancy_state , value }

; 'value' and 'status' are fields
value  ::= field
status ::= field

; VALSTAT state is AND combination of 
; two fields occupancy states
; combination of occupancy_states of two fields
VALSTAT state ::= is_empty(value) AND is_empty(status)

; valstat is a record made of two fields
valsat ::= { value , status }
``` 

[Backus–Naur form or Backus normal form (BNF)](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form) is a metasyntax notation for context-free grammars.