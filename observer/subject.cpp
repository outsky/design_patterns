#include "subject.h"

subject::subject(void) {
}

subject::~subject(void) {
    obs.clear();
}

void subject::add_observer(observer* ob) {
    if( is_registered(ob) )
        return;
    add(ob);
}

void subject::remove_observer(const observer* ob) {
    rm(ob);
}

void subject::notify_observers(void) {
    for(std::list<observer*>::const_iterator it=obs.begin();
            it!=obs.end(); ++it) {
        (*it)->update();
    }
}

bool subject::is_registered(const observer* ob) {
    return get(ob) != obs.end();
}

void subject::add(observer* ob) {
    obs.push_front(ob);
}

std::list<observer*>::iterator subject::get(const observer* ob) {
    for(std::list<observer*>::iterator it=obs.begin();
            it!=obs.end(); ++it) {
        if(*it == ob)
            return it;
    }
    return obs.end();
}

void subject::rm(const observer* ob) {
    std::list<observer*>::iterator it = get(ob);
    if(it == obs.end())
        return;
    obs.erase(it);
}
