#ifndef OBSERVER_H
#define OBSERVER_H

class subject;
class observer {
    public:
        observer(subject* s);
        virtual ~observer(void);

        void unregister(void);
        virtual void update(void) = 0;

    private:
        subject* sub;
};

#endif
