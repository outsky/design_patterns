NOTICE
======
* only support linux
* using epoll

STRUCTURE
=========
pattern core
------------
* `event_t` : defines all supported event types
* `event_set `: holds all interested events as well as their handle and handler
* `event_handler `: interface for handling certain event on specified handle
* `reactor `:
    + registers event handlers and their associated handles
    + removes event handlers and their associated handles
    + manages an event_set
    + waits for events and calls the right handler

test case
---------
see test/README.md
