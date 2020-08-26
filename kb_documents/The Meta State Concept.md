## The Meta State Concept

- Each function will return two instance of two types. Types are: Value and Status.
- Both types have two occupancy states: Empty or Has value
- AND Combination of their occupancy states is called "Meta State"
```
   occupancy_state := { empty | has_value }
   meta state := occupancy_state AND occupancy_state 
```
Thus Value and State types only role is to exhibit one of the two occupancy state.  Pseudo code.
```
Value  value   := { Empty | Has Value  }
Status status  := { Empty | Has Value  }
```
The valstat enabled function caller is to make decisions by observing the "Meta State" returned. Not the values of any of the two returns, value and state.

metastate combination of value *and* status occupancies is giving four possible metastates. 

| Meta State | Value state  | op | Status state  |
| ----- | ------- | ------- | --- |
| **Info**  | Present | AND | Present |
| **OK**    | Present | AND | NIL     |
| **Error** | NIL     | AND | Present |
| **Empty** | NIL     | AND | NIL     |

metastate names are descriptive, but are not implying any kind of required behavior from both callers or producers. Final metastate meaning is defined by valstat adopters.

Let's postulate the existence of the meta_state function. It receives two arguments: Value and Status instances. It observes their occupancy states and returns the resulting meta state.
```
  valstat = meta_state ( value, status ) ;
```
Where meta_state is one of

   1. **OK** 
      1. Value instance occupancy is: "Has Value", Status occupancy is: "Empty". Function returned a value expected. Status is considered as irrelevant.
   1. **ERROR** 
      1. Value instance occupancy is: "Empty", Status occupancy is: "Has Value". Function has **not** returned a value expected. Status instance contains the relevant reason.
   1. **INFO** 
      1. Value occupancy is "Has Value", Status occupancy is: "Has Value". Function returned a value expected. There is also a status value present.
   1. **EMPTY** 
      1. Value occupancy is: "Empty", Status occupancy is: "Empty". The "empty return"

It is important to realize, the metastate meaning should be observed in the context of both caller and API called.

```
 // function meta_state pseudo code
  valstat meta_state ( value, status ) 
  {
     if ( has_value(value) AND is_empty( status) )
        return OK_META_STATE ;

     if ( is_empty(value) AND has_value( status) )
        return ERROR_META_STATE ;

     if ( has_value(value) AND has_value( status) )
        return INFO_META_STATE ;

     if ( is_empty(value) AND is_empty( status) )
        return EMPTY_META_STATE ;
  }
```

### Meta state existence is implied. 
  There is no meta_state function. It is just an illustration of how is metastate made. Adopters of the concept, and its implementation in the shape of `std::valstat` are using the occupancy states of Value and Status to pass the metastate to the caller. The existence of the "Meta State" abstraction is required but it is not implemented, it is just an abstraction. It is implied. 
  
  Finally,metastate has the full meaning in the context of the API producing it and in the context of a caller using it.