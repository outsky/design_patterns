#ifndef SUBJECT_H
#define SUBJECT_H

#include <list>
#include "observer.h"

class subject {
    public:
        subject(void);
        virtual ~subject(void);

        void add_observer(observer* ob);
        void remove_observer(const observer* ob);
        void notify_observers(void);

    private:
        bool is_registered(const observer* ob);
        void add(observer* ob);
        std::list<observer*>::iterator get(const observer* ob);
        void rm(const observer* ob);

        std::list<observer*> obs;
};

#endif
