/**
 *******************************************************************************
 *******************************************************************************
 *
 *	License :
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 
 *******************************************************************************
 *******************************************************************************
 *
 *
 *    @file   State.h
 *    @author gilou
 *    @date   30 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef STATE_H_
#define STATE_H_

class State {
protected:
	char m_name[10];	/**< state name initialize with the constructor */

public :
	State();

	virtual ~State();

	virtual void execute () = 0;

	virtual void print() = 0;

	bool isEqual(char *name)const;
};

class Idle : public State{

public :
	Idle();

	virtual ~Idle();

	virtual void execute ();

	virtual void print();

	bool isEqual(char *name)const;
};

/**
 *
 * @param a
 * @param b
 * @return
 */
bool operator==(State const& a, State const& b);

#endif /* STATE_H_ */
