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

void set_apsr(memory mem, int n, int z, int c, int v);
int conditionPassed(memory mem, int cond);

int ADD_Imm_T1(word binary, memory mem, int setflags);
int ADD_Imm_T2(word binary, memory mem, int setflags);
int ADD_Imm_T3(word binary, memory mem, int setflags);
int ADD_Imm_T4(word binary, memory mem, int setflags);
int ADD_Reg_T1(word binary, memory mem, int setflags);
int ADD_Reg_T2(word binary, memory mem, int setflags);
int ADD_Reg_T3(word binary, memory mem, int setflags);
int B_T1(word binary, memory mem, int setflags);
int B_T2(word binary, memory mem, int setflags);
int B_T3(word binary, memory mem, int setflags);
int B_T4(word binary, memory mem, int setflags);
int BL_T1(word binary, memory mem, int setflags);
int BX_T1(word binary, memory mem, int setflags);
int CMP_Imm_T1(word binary, memory mem, int setflags);
int CMP_Imm_T2(word binary, memory mem, int setflags);
int CMP_Reg_T1(word binary, memory mem, int setflags);
int CMP_Reg_T2(word binary, memory mem, int setflags);
int CMP_Reg_T3(word binary, memory mem, int setflags);
int IT_T1(word binary, memory mem, int setflags, unsigned int* it_state);
int LDR_Imm_T1(word binary, memory mem, int setflags);
int LDR_Imm_T2(word binary, memory mem, int setflags);
int LDR_Imm_T3(word binary, memory mem, int setflags);
int LDR_Imm_T4(word binary, memory mem, int setflags);
int LDR_Lit_T1(word binary, memory mem, int setflags);
int LDR_Lit_T2(word binary, memory mem, int setflags);
int MOV_Imm_T1(word binary, memory mem, int setflags);
int MOV_Imm_T2(word binary, memory mem, int setflags);
int MOV_Imm_T3(word binary, memory mem, int setflags);
int MOV_Reg_T1(word binary, memory mem, int setflags);
int MOV_Reg_T2(word binary, memory mem, int setflags);
int MOV_Reg_T3(word binary, memory mem, int setflags);
int MOVT_T1(word binary, memory mem, int setflags);
int MUL_T1(word binary, memory mem, int setflags);
int MUL_T2(word binary, memory mem, int setflags);
int POP_T1(word binary, memory mem, int setflags);
int POP_T2(word binary, memory mem, int setflags);
int POP_T3(word binary, memory mem, int setflags);
int PUSH_T1(word binary, memory mem, int setflags);
int PUSH_T2(word binary, memory mem, int setflags);
int PUSH_T3(word binary, memory mem, int setflags);
int STR_Imm_T1(word binary, memory mem, int setflags);
int STR_Imm_T2(word binary, memory mem, int setflags);
int STR_Imm_T3(word binary, memory mem, int setflags);
int STR_Imm_T4(word binary, memory mem, int setflags);
int STR_Reg_T1(word binary, memory mem, int setflags);
int STR_Reg_T2(word binary, memory mem, int setflags);
int SUB_Imm_T1(word binary, memory mem, int setflags);
int SUB_Imm_T2(word binary, memory mem, int setflags);
int SUB_Imm_T3(word binary, memory mem, int setflags);
int SUB_Imm_T4(word binary, memory mem, int setflags);
int SUB_Reg_T1(word binary, memory mem, int setflags);
int SUB_Reg_T2(word binary, memory mem, int setflags);
int SVC_T1(word binary, memory mem, int setflags);


#endif
