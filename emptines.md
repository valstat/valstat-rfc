<h1>The Concept of Emptiness</h1>

| &nbsp;  | &nbsp;
|------------|-----------
| Author: `dbj@dbj.org` | License: CC BY SA 4.0
| Version: 0.2 | Date: 2022 JAN 14
| Version: 0.1 | Date: 2021 JUN 13

- [1. Motivation: The actual full picture](#1-motivation-the-actual-full-picture)
- [2. Formalization](#2-formalization)
- [3. Why](#3-why)
- [4. Appendix A: SQL](#4-appendix-a-sql)
- [5. Appendix B: Empty string](#5-appendix-b-empty-string)

## 1. Motivation: The actual full picture

Software element or an "entity" actually has primary and secondary state pairs. It is unfortunate that is not explained clearly and it thus sometimes unclear to some software developers.

In reality software entities (aka type instances) always exist, but can hold a value or be "empty". Close to all, example is perhaps a simple file. File can exist but can be empty or not. We can mandate the existence of a file, but leave its contents as optional.

Make an empty file. However you OS allows you to do that. It exists and is empty. That is its primary state.
```
exists.txt -- 0 bytes
```
Write to a file. Now it has a content. That is its primary secondary state.
```
exists.txt  -- 255 bytes
```
Another example is C++ `std::optional<T>` called also ["container of one"](https://en.cppreference.com/w/cpp/utility/optional).
```cpp
// holding exactly one element
std::optional<int> opt;
```
Primary state of `opt` is empty.
```cpp
// does not assert
assert( false == opt.has_value() ) ;
```
That container is empty. It has no value stored inside. That is what FIELD is. A container of a value.

Actually the primary state is always there:
```cpp
void * field_ {} ;
```
Primarily that `field_` pointer variable does exist. But it has no value.

## 2. Formalization

Software entity always exists and has a name; primarily it can be in two different "states of emptiness".

| emptiness state | description
|------------|-----------
| empty      | exist but has no value
| has_value  | exist and has value

Empty software entity can not exhibit secondary state. But, Non-empty entity can be in two secondary states:

| existence  | description
|------------|-----------
| `null`     | does not have a value
| NOT `null` | does have a value

`null` denotes the non-existence of some value. 

Example pseudo code
```
// primary two state
interface IEmpty {
    // true if empty
    bool is_empty () ;
}

// secondary two state
interface IExist {
    // true if null
    bool is_null () ;
}
```
Implement software entity that will always exist:
```
// persistent entity always exist
// just like a file on a disk, once created
type PersistentEntity :
implements IEmpty, IExist
{
// existence is implied
// it is never null
bool is_null () { false };

// depends on the concrete type
virtual bool is_empty () = 0 ;
};
```
And now implement a concrete type that has primary and secondary states
```cpp
type FileObject : inherits PersistentEntity 
{
    bool empty_ = false ;
    
    virtual bool is_empty () { return empty_ ; }

    FileObject () {
        // create empty file
        empty_ = true ;
    }

    void write( const char * new_line_ ){
        // write to it and change the primary state
        empty_ = false ;
    }

       // remove from the file all the text == content_
    void remove_if_inside ( const char * content_) {
       if ( this->file_is_empty())
         empty_ = true ;
    }
};
```


## 3. Why

It is somewhat hidden, but concept of emptiness does exist inside the core paradigm of types and values. 

I am making it prominent, to be used as a conceptual building block of the `valstat`.

## 4. Appendix A: SQL

For some a concept of emptiness is a  bit confusing. One practical example of "emptiness" concept clearly visible in the foundations is database and a database query result. Database is made of "records", and records are made of fields:

```
RECORD
   FIELD 1
   FIELD 2
   ...
   FIELD N
```
Field can contain a value or be empty. Confusingly for programmers, in DB theory `NULL` means empty. `NULL` in DB theory is generic value that means "that field has no value".  Any value can be `NULL` regardless of its value type.  Any DB field always exists. But it can be "empty" aka "null".

A non-empty query result always returns a `RECORD`, and it might contain empty `FIELDS`, if database was empty or if query returns one empty record. Example.

Table: `Customers` has two columns, one column is named `Address`:

| Address | Customer
|---------|---------
| Alley   | Alex
| Street  | Barry
| Park    | Chuck

[Example](https://www.w3schools.com/sql/trysql.asp?filename=trysql_is_not_null) query:
```sql
-- return the number of addresses starting with "W"
SELECT COUNT(Address)
FROM Customers
WHERE Address LIKE ("W*");
```
There is no address starting with 'W'. Database query result is returned and it does exist, but its one field contains value 0: 
```
Number of Records: 1
---------------------
COUNT(Address)
0
```
Something has been returned, thus we see "Number of Records: 1". 

(Although perhaps even more confusing) sometimes SQL query result will not exist if the whole result is empty. That is unrelated to field emptiness primary state. That is for pure technical and performance reasons, we do not want empty structure to be carried over from a database, jus to conclude it is empty.
```sql
SELECT CustomerName, ContactName, Address
FROM Customers
WHERE Address LIKE ("W*");
```
That query will return "No result". There are no records returned as there is no records where address field starts with "W*"

## 5. Appendix B: Empty string

The FIELD concept behind the variable, is unlike most programming languages where we do not care if variable is empty. Most just care if variable does exist or not. That is obvious with C/C++ native pointers.
```cpp
int * ip_ = null ;
```
We say: "ip_ does not exist, it is null".
But more precisely we should say: "ip_ does exist and its value is null". 

One naughty thorny detail that arises from that imprecise nomenclature is the famous "empty string" singularity:
```cpp
const char * thorny_ = "";
```
That `thorny_` for most programmers "does exist" because it is "clearly not null". But it is also empty. Thus one more check is necessary (to the annoyance of most):
```cpp
// thorny has arrived from GUI
// it is obviously OK?
if (thorny_ != null )
// but it can be empty
// so we need to check for that too
// by checking its first character 
// if it is a zero char
if ( thorny_[0] != '\0' ) 
{
    // thorny_ does exist and 
    // is not empty
}
```
Obviously to some that emptiness check is not most resilient and can lead to a false positive in a few nasty ways.