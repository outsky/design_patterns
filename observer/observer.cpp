#include "observer.h"
#include "subject.h"

observer::observer(subject* s) : sub(s) {
    sub->add_observer(this);
}

observer::~observer(void) {
    unregister();
}

void observer::unregister(void) {
    sub->remove_observer(this);
}
