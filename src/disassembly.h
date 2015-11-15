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

#define DICO_SIZE 53

typedef struct dico dico;

struct dico
{
  char id_debug[16];
  char mnemo[16];
  int size; //Size of instruction in bits (16 or 32)
  unsigned int sig;
  unsigned int mask;
  int nb_op; //Number of operands
  char registers_index[32];
  char immediate_index[32];
  int it;
  int treatImm;
  int treatReg;
};

int is_16bits(byte* header);
int it_condition(unsigned int it_state, char* condition);
int it_XYZ(unsigned int it_state, char* XYZ);
int search_instruction(word binary, dico* dictionary, dico* instruction, int is_short);
int decode_instruction(int binary, dico* instruction, int* in_it, unsigned int* it_state, int print);
word create_mask(unsigned int a, unsigned int b);
void extract_dico(char* dico_file, dico* dico);
int disasm(size_t startadress, size_t endadress, dico* dictionary, memory mem);
int it_condition(unsigned int firstcond, char* condition);
word ThumbExpandImm(unsigned int i, unsigned int imm3, unsigned int imm8);
void DecodeImmShift(unsigned int imm3, unsigned int imm2, unsigned int type, char* ret);
word SignExtend16(unsigned int imm, unsigned int size);
word SignExtend32(unsigned int s, unsigned int j1, unsigned int j2, unsigned int imm10, unsigned int imm11);

#endif
