/**
 * @file run.h
 * @author Mickael Albisser 
 * @brief Run functions - header
 */

#ifndef _RUN
#define _RUN

#include "memory.h"
#include "disassembly.h"
#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

int run(dico* dictionary, memory mem);
int execute_instruction(word binary, dico* instruction, int* in_it, unsigned int* it_state, memory mem);
int step(dico* dictionary, memory mem);
int step_into(dico* dictionary, memory mem);

#endif
