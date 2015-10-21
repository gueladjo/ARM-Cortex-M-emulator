
/**
 * @file types.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Oct 31 19:09:31 2013
 * @brief Types.h
 *
 * Types.
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

typedef uint32_t uint, word;
typedef uint8_t  byte;

typedef uint32_t vaddr32;
typedef uint64_t vaddr64;

typedef union {
    vaddr32 _32;
    vaddr64 _64;
} vaddr, addr;


#define STRLEN 256

typedef char string[STRLEN+1];

#define init_string(s) memset( s, '\0', STRLEN+1 )


typedef enum { UNDEF, LSB, MSB } endianness;

#ifdef __cplusplus
}
#endif

#endif /* _TYPES_H_ */
