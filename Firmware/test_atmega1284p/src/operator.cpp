/**
 *******************************************************************************
 *******************************************************************************
 *
 *	Licence :
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
 *    @file   operator.cpp
 *    @Author gilou
 *    @date   20 avr. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */



#include <stdlib.h>

extern "C" void __cxa_pure_virtual() { while (1); }

// wrapper functions for malloc and free so that the c++ operators new and
// delete work, these functions will not throw an exception


void* operator new(size_t __s)
{
	return malloc(__s);
}

void* operator new[](size_t __s)
{
	return malloc(__s);
}

void operator delete(void* __p)
{
	if (__p) free(__p);
}

void operator delete[](void* __p)
{
	if (__p) free(__p);
}