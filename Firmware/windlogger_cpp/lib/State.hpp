#ifndef STATE__HPP
#define STATE__HPP

class State {
    // Operations
    public :
        State ();

        virtual ~State();

        virtual void execute () = 0;
};

class Idle : public State{
    // Operations
    public :
        Idle();

        virtual ~Idle();

        virtual void execute ();
};

#endif
