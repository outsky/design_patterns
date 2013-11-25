SUMMARY
=======
Test this pattern by implement the example `logging service` in POSA(v2).

- Server listens and accepts clients connections;
- Client sends info and err messages to server;
- Server outputs and records those messages;

STRUCTURE
=========
Only the server uses reactor pattern.
Two concrete handlers are driven from `event_handler` interface:
- accept_handler :
    + accepts new connections
    + registers new connections with `log_handlers`
- log_handler :
    + receives messages from client
    + outputs and records those messages
