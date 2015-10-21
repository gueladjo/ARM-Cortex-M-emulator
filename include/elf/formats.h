/**
 * @file formats.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Nov  1 00:31:06 2013
 * @brief Common binary formats.
 *
 * Common binary formats.
 */

#ifndef _FORMATS_H_
#define _FORMATS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/* Forward declaration */
struct process_t;

#include "common/types.h"
#include "elf/elf.h"

#define MACH_T_UNDEF    0x00
#define MACH_T_MIPS     0x01
#define MACH_T_M68K     0x02
#define MACH_T_SPARC    0x03
#define MACH_T_I386     0x04
#define MACH_T_ARM      0x40

#define MACH_T_UNDEF_S  "undef"
#define MACH_T_MIPS_S   "mips"
#define MACH_T_M68K_S   "m68k"
#define MACH_T_SPARC_S  "sparc"
#define MACH_T_I386_S   "i386"
#define MACH_T_ARM_S    "arm"

#define MACH_W_UNDEF    0
#define MACH_W_32       32
#define MACH_W_64       64

typedef enum {undef, elf} binfmt;
#ifdef __cplusplus
}
#endif

#endif /* _FORMATS_H_ */
