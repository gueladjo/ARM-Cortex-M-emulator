
/**
 * @file bits.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Nov  2 18:48:44 2013
 * @brief Bit twiddling.
 *
 * Bit twiddling.
 */

#ifndef _BITS_H_
#define _BITS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common/types.h"

unsigned int get_host_endianness( void );
byte *__flip_endianness( byte * block, uint sz );


#define FLIP_ENDIANNESS( x ) __flip_endianness( (byte*)&(x), sizeof(x) )


#define CLR_BIT( b, n ) ( b &= 0xFF-(1<<(n)) )
#define SET_BIT( b, n ) ( b |= 1<<(n) )
#define GET_BIT( b, n ) ( ( b & 1<<(n) ) >> (n) )

#define GET_BITS( b, i, j ) ( b & (((1 << (j-i)) - 1) << i ) )


#ifdef __cplusplus
}
#endif

#endif /* _BITS_H_ */
