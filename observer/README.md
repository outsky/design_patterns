SUMMARY
=======
Implement the `observer pattern` in the `pull` way, which means, the `observers` should `pull` states from `subject` using methods like `get_xxx`.

STRUCTURE
=========
There are two base classes: `subject` and `observer`.
* `subject` is defined in subject.h, which managers observers and notifies observers when state changes.
    A class should driven from this base class to become a `subject`. The only thing the class should do is call `update_observers` method when it needs to.
* `observer` is defined in observer.h, which react to `subject` by calling `update` when interesting thing happens.
    A class should driven from this base class to become a `observer`. 
    * The class must pass a `subject*` to the base class constructor so that it could be added to the subject.
    * The class must implements the `update` interface, usually, gets states here and do logic based on the states.

EXAMPLE
=======
An example is given in the test folder.
In the example, three observers(bar, num, text) observe a subject(hp).
It means, show the health condition in three different ways:
* text: prints `dead`, `low`, `high` depending on the hp value.
* number: prints current hp, max hp and percentage value.
* bar: draws a bar to visualize current hp value.

