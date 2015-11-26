#include "instructions.h"

void set_apsr(memory mem, int n, int z, int c, int v) {
  word apsr;
  apsr = (n << 31) + (z << 30) + (c << 29) + (v << 28);
  mem->reg[16] = apsr;
  return;
}

int conditionPassed(memory mem, int cond) {
  switch (cond) {
  case 0: //Equal
    if ((mem->reg[16] >> 30) & 1) //Si Z == 1
      return 1;
    else
      return 0;
    break;
  case 1: //Not equal
    if (!((mem->reg[16] >> 30) & 1)) //Si Z == 0
      return 1;
    else
      return 0;
    break;
  case 2: //Carry set (greater than)
    if ((mem->reg[16] >> 29) & 1) //Si C == 1
      return 1;
    else
      return 0;
    break;
  case 3:
    if (!((mem->reg[16] >> 29) & 1))
      return 1;
    else
      return 0;
    break;
  case 4:
    if ((mem->reg[16] >> 31) & 1)
      return 1;
    else
      return 0;
    break;
  case 5:
    if (!((mem->reg[16] >> 31) & 1))
      return 1;
    else
      return 0;
    break;
  case 6:
    if (!(mem->reg[16] >> 28) & 1)
      return 1;
    else
      return 0;
    break;
  case 7:
    if (!((mem->reg[16] >> 28) & 1))
      return 1;
    else
      return 0;
    break;
  case 8:
    if ((mem->reg[16] >> 29) & 1 && !(((mem->reg[16]) >>  30) & 1))
      return 1;
    else
      return 0;
    break;
  case 9:
    if (!((mem->reg[16] >> 29) & 1) || (mem->reg[16]) >> 30)
      return 1;
    else
      return 0;
    break;
  case 10:
    if ((mem->reg[16] >> 31) & 1 ==  (mem->reg[16] >> 28) & 1)
      return 1;
    else
      return 0;
    break;
  case 11:
    if ((mem->reg[16] >> 31) & 1 !=  (mem->reg[16] >> 28) & 1)
      return 1;
    else
      return 0;
    break;
  case 12:
    if (!((mem->reg[16] >>30) & 1) && ((mem->reg[16] >> 31) & 1 ==  (mem->reg[16] >> 28) & 1))
      return 1;
    else
      return 0;
    break;
  case 13:
    if (((mem->reg[16] >>30) & 1) && ((mem->reg[16] >> 31) & 1 !=  (mem->reg[16] >> 28) & 1))
      return 1;
    else
      return 0;
    break;
  case 14:
    return 0;
  }
}

int ADD_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {
  word imm;
  int rd, rn;
  if (*in_it <= 0) { //Case not in IT Block : setflags
    imm = (binary & 0x1C0) >> 6;
    rd = (binary & 0x7);
    rn = (binary & 0x38) >> 3;
    mem->reg[rd] = mem->reg[rn] + imm;
    set_apsr(mem, (mem->reg[rd] & 0x80000000) >> 31, !mem->reg[rd], 0, 0); //Pas de retenue ni d'overflow dans cet encodage
  }
  
  else {
    int temp;
    temp = (*it_state & 0xF) << 1; //Prêt pour décalage mask vers condition
    if (conditionPassed(mem, (*it_state >> 4))) { //Si les conditions sont remplies
      imm = (binary & 0x1C0) >> 6;
      rd = (binary & 0x7);
      rn = (binary & 0x38) >> 3;
      mem->reg[rd] = mem->reg[rn] + imm;
    }
    *it_state = (*it_state & 0xE0) + temp; //Mise à jour de it_state
  }

  return 0;
}

int ADD_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int ADD_Imm_T3(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int ADD_Imm_T4(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int ADD_Reg_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int ADD_Reg_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int ADD_Reg_T3(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int B_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int B_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int B_T3(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int B_T4(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int BL_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int BX_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int CMP_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int CMP_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int CMP_Reg_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int CMP_Reg_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int CMP_Reg_T3(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int IT_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int LDR_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int LDR_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int LDR_Imm_T3(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int LDR_Imm_T4(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int LDR_Lit_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int LDR_Lit_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int MOV_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int MOV_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int MOV_Imm_T3(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int MOV_Reg_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int MOV_Reg_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int MOV_Reg_T3(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int MOVT_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int MUL_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int MUL_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int POP_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int POP_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int POP_T3(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int PUSH_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int PUSH_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int PUSH_T3(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int STR_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int STR_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int STR_Imm_T3(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int STR_Imm_T4(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int STR_Reg_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int STR_Reg_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int SUB_Imm_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int SUB_Imm_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int SUB_Imm_T3(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int SUB_Imm_T4(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int SUB_Reg_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int SUB_Reg_T2(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}
int SVC_T1(word binary, memory mem, int* in_it, unsigned int* it_state) {return 0;}