#ifndef SLEEP__HPP
#define SLEEP__HPP

#include "State.hpp"

class Sleep : public State {
    // Operations
    public :
        Sleep();

        virtual ~Sleep();

        virtual void execute ();
};

#endif
