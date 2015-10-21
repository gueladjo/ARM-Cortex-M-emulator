/**
 * @file section.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Nov  5 14:54:23 2013
 * @brief Section handling.
 *
 * Section handling.
 */

#ifndef _SECTION_H_
#define _SECTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common/types.h"

typedef union {
    uint32_t  _32;
    uint64_t _64;
} implantation_addr;

typedef struct {
    implantation_addr   addr;
    char   *name;
} scn;

typedef struct {
    uint   size;
    scn   *scn;
} scntab;

scntab new_scntab( uint size );
void   del_scntab( scntab scntab );
void   print_scntab( scntab scntab );

#ifdef __cplusplus
}
#endif

#endif /* _SECTION_H_ */
