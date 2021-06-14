<h1>The Concept of Emptiness</h1>

| &nbsp;  | &nbsp;
|------------|-----------
| Author: `dbj@dbj.org` | License: CC BY SA 4.0
| Version: 0.1 | Date: 2021 JUN 13

- [1. Introduction](#1-introduction)
- [2. Where and When](#2-where-and-when)
- [3. Why](#3-why)
- [Appendix A: SQL](#appendix-a-sql)
## 1. Introduction

Any entity can be in two different states

| existence  | description
|------------|-----------
| `null`     | does not exist
| NOT `null` | does exist

`null` denotes the non-existence of some entity. Entity that exists can be in two different states of emptiness.

| emptiness  | description
|------------|-----------
| empty      | exist but has no value
| has_value  | exist and has value

Non existant entity can not exhibit any other state. 

Pseudo code
```
interface IEmpty {
    // true if empty
    bool is_empty () ;
}

interface IExist {
    // true if null
    bool is_null () ;
}
```

Implement entity that will always exist:

```
type PersistentEntity :
implements IEmpty, IExist
{
// existence is implied
bool is_null () { false };

// depends on the implementation 
bool is_empty () ;

}
```
## 2. Where and When

Some entities always exist, but can hold a value or be "empty". Best example is perhaps a simple file. File can exist but can be empty or not.

We can mandate the existence of a file, but leave its contents as optional.

Make an empty file. It exists and is empty.
```
exists.txt -- 0 bytes
```
Write to a file. Exists and has value.
```
exists.txt  -- 255 bytes
```
Another example is C++ `std::optional<T>` called also ["container of one"](https://en.cppreference.com/w/cpp/utility/optional).
```cpp
// holding exactly one element
std::optional<int> opt;
// returns true on empty element
bool empty = ! opt.has_value() ;
```
Above, container is not empty. Its one element has no value.

## 3. Why

Concept of emptiness does exist inside the paradigm of types and values. 

I am making it prominent, to be used as a conceptual building block of the `valstat`.

## Appendix A: SQL

A bit confusing but practical example is database and a database query result. Database is made of "records", and records are made of fields:

```
RECORD
   FIELD 1
   FIELD 2
   ...
   FIELD N
```
Field can contain a value or be empty. Confusingly (for programmers) in DB theory `NULL` means empty. `NULL` in DB theory is generic value that means "that field has no value".  Any value can be `NULL` regardless of its type. 

> SQL result will not exist if the whole result is empty.

We usually make a decision what do we mean by "empty" when using SQL.

Database query result is also made of records. Query result fields can contain values or be empty. But result fields always do exist. Even if empty. A query always returns a `RECORD`, and it might contain empty `FIELDS`, if database is empty or if query returns one empty record.

Table: `Customers` has two columns, one column is named `Address`:

| Address | Customer
|---------|---------
| Alley   | Alex
| Street  | Barry
| Park    | Chuck

[Example](https://www.w3schools.com/sql/trysql.asp?filename=trysql_is_not_null) query:


```sql
SELECT COUNT(Address)
FROM Customers
WHERE Address LIKE ("W*");
```
There is no address starting with 'W'. Database query result always does exist, but its one field contains value 0: 
```
Number of Records: 1
---------------------
COUNT(Address)
0
```
Something has been returned, but it's value means to us the result is empty. That is just because we decided it will mean 'empty'.