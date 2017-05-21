#ifndef SLEEP__HPP
#define SLEEP__HPP

#include "../lib/State.h"

class Sleep : public State {
private:
	char m_name[10];	/**< state name initialize with the constructor */

	// Operations
public :
	Sleep();

	virtual ~Sleep();

	virtual void execute ();

	virtual void print();

	bool isEqual(char *name)const;
};

#endif
