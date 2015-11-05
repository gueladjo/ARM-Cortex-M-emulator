/**
 * @file disassembly.h
 * @author Mickael Albisser 
 * @brief Disassembly functions - header
 */

#ifndef _DISASSEMBLY
#define _DISASSEMBLY

#include "memory.h"
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
unsigned int sig;
unsigned int mask;
int nb_op; //Number of operands
char registers_index[20];
char immediate_index[20];
int it;
};

int is_16bits(byte* header);
int it_condition(unsigned int firstcond, char* condition);
int create_mask(unsigned int a, unsigned int b);
int search_instruction(int binary, dico* dictionary, dico* instruction, int is_short);
int print_instruction(int binary, dico instruction, byte* header, dico* dictionary, memory mem);
int read_instruction(byte* header);
void extract_dico(char* dico_file, dico* dico);

#endif
