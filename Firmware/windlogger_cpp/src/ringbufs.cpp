/**
 *******************************************************************************
 *******************************************************************************
 *
  *	License :
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    This code is inspire by the article :
 *    http://www.embedded.com/electronics-blogs/embedded-round-table/4419407/1/The-ring-buffer
 *
 *    coded   BY:   Ken Wada
 *            Aurium Technologies Inc.
 *            15-July-2013
 *
 * 
 *******************************************************************************
 *******************************************************************************
 *
 *
 *    @file   ringbufs.cpp
 *    @author gilou
 *    @date   12 sept. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef SRC_RINGBUFS_CPP_
#define SRC_RINGBUFS_CPP_

#include <string.h>
#include "../lib/ringbufs.h"

void ringBufS_init(ringBufS *_this){
	/*****
	 * the following clears:
	 * 	-> buf
	 * 	-> head
	 * 	->tail
	 * 	-> count
	 * and sets head = tail
	 */
	memset(_this, 0, sizeof(*_this));
}


int ringBufS_empty(ringBufS *_this){
	return (_this->count==0);
}

int ringBufS_full(ringBufS *_this){
	return (_this->count>=RBUF_SIZE);
}

int ringBufS_get (ringBufS *_this){
	int c;
	if(_this->count>0)
	{
		c	= _this->buf[_this->tail];
		_this->tail = (_this->tail+1)%RBUF_SIZE;
		--_this->count;
	}
	else {
		c = 256;
	}
	return c;
}

void ringBufS_put (ringBufS *_this, const char c){
	if(_this->count < RBUF_SIZE)
	{
		_this->buf[_this->head] = c;
		_this->head = (_this->head+1)%RBUF_SIZE;
		++_this->count;
	}
}

void ringBufS_flush(ringBufS *_this, const unsigned char clearBuffer){
	_this->count = 0;
	_this->head = 0;
	_this->tail = 0;
	if(clearBuffer){
		memset(_this->buf,0, sizeof(_this->buf));
	}
}
#endif /* SRC_RINGBUFS_CPP_ */
