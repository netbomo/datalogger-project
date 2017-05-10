#ifndef SLEEP__HPP
#define SLEEP__HPP

#include "../lib/State.h"

class Sleep : public State {
    // Operations
    public :
        Sleep();

        virtual ~Sleep();

        virtual void execute ();

        virtual void print(Usart &usart);
};

#endif
