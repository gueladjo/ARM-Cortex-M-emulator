/**
 * @file instructions.h
 * @author Mickael Albisser 
 * @brief Run instructions - header
 */

#ifndef _INSTRUCTIONS
#define _INSTRUCTIONS

#include "memory.h"
#include "disassembly.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

int ADD_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int ADD_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int ADD_Imm_T3(word binary, memory mem, int* in_it, unsigned int* it_state);
int ADD_Imm_T4(word binary, memory mem, int* in_it, unsigned int* it_state);
int ADD_Reg_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int ADD_Reg_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int ADD_Reg_T3(word binary, memory mem, int* in_it, unsigned int* it_state);
int B_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int B_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int B_T3(word binary, memory mem, int* in_it, unsigned int* it_state);
int B_T4(word binary, memory mem, int* in_it, unsigned int* it_state);
int BL_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int BX_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int CMP_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int CMP_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int CMP_Reg_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int CMP_Reg_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int CMP_Reg_T3(word binary, memory mem, int* in_it, unsigned int* it_state);
int IT_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int LDR_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int LDR_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int LDR_Imm_T3(word binary, memory mem, int* in_it, unsigned int* it_state);
int LDR_Imm_T4(word binary, memory mem, int* in_it, unsigned int* it_state);
int LDR_Lit_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int LDR_Lit_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int MOV_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int MOV_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int MOV_Imm_T3(word binary, memory mem, int* in_it, unsigned int* it_state);
int MOV_Reg_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int MOV_Reg_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int MOV_Reg_T3(word binary, memory mem, int* in_it, unsigned int* it_state);
int MOVT_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int MUL_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int MUL_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int POP_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int POP_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int POP_T3(word binary, memory mem, int* in_it, unsigned int* it_state);
int PUSH_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int PUSH_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int PUSH_T3(word binary, memory mem, int* in_it, unsigned int* it_state);
int STR_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int STR_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int STR_Imm_T3(word binary, memory mem, int* in_it, unsigned int* it_state);
int STR_Imm_T4(word binary, memory mem, int* in_it, unsigned int* it_state);
int STR_Reg_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int STR_Reg_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int SUB_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int SUB_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int SUB_Imm_T3(word binary, memory mem, int* in_it, unsigned int* it_state);
int SUB_Imm_T4(word binary, memory mem, int* in_it, unsigned int* it_state);
int SUB_Reg_T1(word binary, memory mem, int* in_it, unsigned int* it_state);
int SUB_Reg_T2(word binary, memory mem, int* in_it, unsigned int* it_state);
int SVC_T1(word binary, memory mem, int* in_it, unsigned int* it_state);


#endif
