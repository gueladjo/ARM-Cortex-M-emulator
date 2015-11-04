/**
 * @file disassembly.h
 * @author Mickael Albisser 
 * @brief Disassembly functions - header
 */

#ifndef _DISASSEMBLY
#define _DISASSEMBLY

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

typedef struct dico dico;

struct dico
{
char id_debug[16];
char mnemo[16];
int size; //Size of instruction in bits (16 or 32)
size_t sig;
size_t mask;
int nb_op; //Number of operands
char registers_index[16]; //Index of registers
char immediate_index[16]; //Index of immediate
};

void extract_dico(char* dico_file, dico* dico);
#endif
