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
 *     *    This code is inspire by the article :
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
 *    @file   ringbufs.h
 *    @author gilou
 *    @date   12 sept. 2017
 *    @brief  Brief description of file.
 *
 *    Detailed description of file.
 */
#ifndef LIB_RINGBUFS_H_
#define LIB_RINGBUFS_H_
#define RBUF_SIZE 256

typedef struct ringBufS
{
	char buf[RBUF_SIZE];	/*!< This is the managed buffer. The size of this buffer is set by the RBUF_SIZE*/
	int head;				/*!< This is the head pointer. The coming bytes get written to the managed buffer using this index. The arithmetic for this index is in lmdulo-256. */
	int tail;				/*!< This index is used to receive the oldest data in the queue. this index also follows the same modulus arithmetic as the HEAD index case. */
	int count;				/*!< This field is used to keep track of the total number of elements currently in the queue. the maximun value od this field is set by the RBUF_SIZE macro. */
}ringBufS;

#ifdef __cplusplus
	extern "C"{
#endif

	/**
	 * Initialize the queue
	 * @param _this pointer on a buffer structure ringBufS
	 */
	void	ringBufS_init	(ringBufS *_this);

	/**
	 * Determine whether or not the queue is empty
	 * @param _this pointer on a buffer structure ringBufS
	 * @return
	 */
	int		ringBufS_empty	(ringBufS *_this);

	/**
	 * Determine whether or not the queue is full
	 * @param _this pointer on a buffer structure ringBufS
	 * @return
	 */
	int		ringBufS_full	(ringBufS *_this);

	/**
	 * Get a byte from the queue, (TAIL)
	 * @param _this pointer on a buffer structure ringBufS
	 * @return a int value from the char or 256 is the queue is empty
	 */
	int		ringBufS_get	(ringBufS *_this);

	/**
	 * Put a byte to the queue, (HEAD)
	 * @param _this pointer on a buffer structure ringBufS
	 * @param c
	 */
	void	ringBufS_put	(ringBufS *_this, const char c);

	/**
	 * Flush the queue and optionally clear the buffer bytes to 0.
	 * @param _this
	 * @param clearBuffer
	 */
	void 	ringBufS_flush	(ringBufS *_this, const unsigned char clearBuffer);

#ifdef __cplusplus
	}
#endif

#endif /* LIB_RINGBUFS_H_ */
