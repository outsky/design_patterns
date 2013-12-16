NOTICE
======
* only support linux
* using epoll

STRUCTURE
=========
pattern core
------------
* `event_set `: holds all interested events as well as their handler
* `event_handler `: interface for handling certain event on specified handle
* `reactor `:
    + registers event handlers and their associated handles
    + removes event handlers and their associated handles
    + manages an event_set
    + waits for events and calls the right handler

test case
---------
see [./test/README.md](https://github.com/outsky/design_patterns/blob/master/reactor/cpp/test/README.md "doc for reactor test case")

