SUMMARY
=======
Implement `strategy pattern` by an example.

Strategy
========
Define a family of algorithms, encapsulate each one, and make them interchangeable.
Strategy lets the algorithm vary independently from clients that use it.

EXAMPLE
=======
The example reads a string inputed by user, and then process the string according to the user's choice.
The algorithms user can choose are:
* to uppercase
* to lowercase
* remove spaces

STRUCTURE
=========
`stralg` : strategy
    * declares an interface common to all supported algorithms
    * `str_processor` uses this interface to call the algorithm defined by a concrete `stralg`

`stralg_lower` `stralg_upper` `stralg_trim` : concrete strategy
    * implements the algorithm using the `stralg` interface

`str_processor`: context
    * is configured with a concrete `stralg` object
    * maintains a reference to a `stralg` object
