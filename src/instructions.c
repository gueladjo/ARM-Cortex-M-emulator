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
    if (((mem->reg[16] >> 31) & 1) ==  ((mem->reg[16] >> 28) & 1))
      return 1;
    else
      return 0;
    break;
  case 11:
    if (((mem->reg[16] >> 31) & 1) !=  ((mem->reg[16] >> 28) & 1))
      return 1;
    else
      return 0;
    break;
  case 12:
    if (!((mem->reg[16] >>30) & 1) && (((mem->reg[16] >> 31) & 1) ==  ((mem->reg[16] >> 28) & 1)))
      return 1;
    else
      return 0;
    break;
  case 13:
    if (((mem->reg[16] >>30) & 1) && (((mem->reg[16] >> 31) & 1) !=  ((mem->reg[16] >> 28) & 1)))
      return 1;
    else
      return 0;
    break;
  case 14:
    return 0;
  }
  return 1;
}

int addImm(int immediate, int* registers,  memory mem, char* encoding)
{
  if (strcmp(encoding, "T2") == 0) {
    mem->reg[registers[0]] = mem->reg[registers[0]] + immediate;
    return 0;      
  } 

  else {
    mem->reg[registers[0]] = mem->reg[registers[1]] + immediate;
    return 0;
  }
}

int ADD_Imm_T1(word binary, memory mem, int setflags)
{
  int imm = (binary & 0x01C0) >> 6;
  int regs[2];
  
  regs[1] = (binary & 0x0038) >> 3;
  regs[0] = (binary & 0x0007);

  return addImm(imm, regs, mem, "T1");
}

int ADD_Imm_T2(word binary, memory mem, int setflags)
{
  int imm = binary & 0x00FF;
  int regs[2];
  
  regs[0] = (binary & 0x0700) >> 8;

  return addImm(imm, regs, mem, "T2");
}

int ADD_Imm_T3(word binary, memory mem, int setflags)
{
  int imm = ThumbExpandImm((binary & 0x04000000) >> 26, (binary & 0x00008000) >> 12, binary & 0x000000FF);
  int regs[2];
  
  regs[1] = (binary & 0x000F0000) >> 16;
  regs[0] = (binary & 0x00000F00) >> 8;

  return addImm(imm, regs, mem, "T3");
}

int ADD_Imm_T4(word binary, memory mem, int setflags)
{
  int imm = ((binary & 0x04000000) >> 15) +((binary & 0x00008000) >> 4) + (binary & 0x000000FF);
  int regs[2];
  
  regs[1] = (binary & 0x000F0000) >> 16;
  regs[0] = (binary & 0x00000F00) >> 8;

  return addImm(imm, regs, mem, "T4");
}

int addReg(int* registers, memory mem, char* encoding)
{
  if (strcmp(encoding, "T2") == 0) {
    mem->reg[registers[0]] = mem->reg[registers[0]] + mem->reg[registers[1]];   
    return 0;
  }

  if (strcmp(encoding, "T1") == 0) {
    mem->reg[registers[0]] = mem->reg[registers[1]] + mem->reg[registers[2]];
    return 0;   
  }

  else {
    // ajouter gestion du shift pour T3 ici
    mem->reg[registers[0]] = mem->reg[registers[1]] + mem->reg[registers[2]];
    return 0;
  }
}

int ADD_Reg_T1(word binary, memory mem, int setflags)
{
  int regs[3];
  
  regs[2] = (binary & 0x01C00) >> 6;
  regs[1] = (binary & 0x0038) >> 3;
  regs[0] = (binary & 0x0007);

  return addReg(regs, mem, "T1");
}

int ADD_Reg_T2(word binary, memory mem, int setflags)
{
  int regs[2];
  
  regs[1] = (binary & 0x0078) >> 3;
  regs[0] = (binary & 0x0007);

  return addReg(regs, mem, "T2");
}

int ADD_Reg_T3(word binary, memory mem, int setflags)
{
//A Faire: gérer shift
  return 0;
}

int ADD_SP_T1(word binary, memory mem, int setflags)
{
  int imm = binary & 0x00FF;
  int regs[2];
  
  regs[1] = 13;
  regs[0] = (binary & 0x0700) >> 8;

  return addImm(imm, regs, mem, "SP"); 
}

int ADD_SP_T2(word binary, memory mem, int setflags)
{
  int imm = binary & 0x007F;
  int regs[1];
  
  regs[0] = 13;

  return addImm(imm, regs, mem, "T2");  
}

int ADD_SP_T3(word binary, memory mem, int setflags)
{
  int imm = ThumbExpandImm((binary & 0x04000000) >> 26, (binary & 0x00007000) >> 12, binary & 0x000000FF);
  int regs[2];
  
  regs[1] = 13;
  regs[0] = (binary & 0x00000F00) >> 8;

  return addImm(imm, regs, mem, "SP");
}

int ADD_SP_T4(word binary, memory mem, int setflags)
{
  int imm = ((binary & 0x04000000) >> 15) + ((binary & 0x00007000) >> 4) + (binary & 0x000000FF);
  int regs[2];
  
  regs[1] = 13;
  regs[0] = (binary & 0x00000F00) >> 8;

  return addImm(imm, regs, mem, "SP");

}

int cmpImm(int registr, int immediate, memory mem)
{
  int result = registr - immediate;
  
  if (result == 0) {
   // mise a jour de Z
    mem->reg[16] = mem->reg[16] | 0x40000000;  
  }

  else {
    mem->reg[16] = mem->reg[16] & 0xBFFFFFFF;  
  }

  if (result < 0) {
    // mise à jour de C et de N
    mem->reg[16] = mem->reg[16] & 0xDFFFFFFF;
    mem->reg[16] = mem->reg[16] & 0x7FFFFFFF;
  } 

  else {
    mem->reg[16] = mem->reg[16] | 0x20000000;  
    mem->reg[16] = mem->reg[16] | 0x80000000;  
  }

  // faire signed overflow (V flag)
  return 0;
}

int CMP_Imm_T1(word binary, memory mem, int setflags)
{
  int registr = (binary & 0x0700) >> 8;
  int imm = binary & 0x00FF;

  return cmpImm(registr, imm, mem);
}

int CMP_Imm_T2(word binary, memory mem, int setflags)
{
  int registr = (binary & 0x000F0000) >> 16;
  int imm = ThumbExpandImm((binary & 0x04000000) >> 26, (binary & 0x00007000) >> 12, binary & 0x000000FF);


  return cmpImm(registr, imm, mem);

}


int cmpReg(int* registers, memory mem, char* encoding)
{
  int result = mem->reg[registers[0]] - mem->reg[registers[1]];

  if (strcmp(encoding, "T3") == 0) {
    //gérer le shift
  }
 
  if (result == 0) {
   // mise a jour de Z
    mem->reg[16] = mem->reg[16] | 0x40000000;  
  }

  else {
    mem->reg[16] = mem->reg[16] & 0xBFFFFFFF;  
  }

  if (result < 0) {
    // mise à jour de C et de N
    mem->reg[16] = mem->reg[16] & 0xDFFFFFFF;
    mem->reg[16] = mem->reg[16] & 0x7FFFFFFF;
  } 

  else {
    mem->reg[16] = mem->reg[16] | 0x20000000;  
    mem->reg[16] = mem->reg[16] | 0x80000000;  
  }

  // faire signed overflow (V flag)
  return 0;
}

int CMP_Reg_T1(word binary, memory mem, int setflagse)
{
  int regs[2];
  
  regs[1] = (binary & 0x0038) >> 3;
  regs[0] = binary & 0x0007;

  return cmpReg(regs, mem, "T1");
}

int CMP_Reg_T2(word binary, memory mem, int setflags)
{
  int regs[2];
  
  regs[1] = (binary & 0x0078) >> 3;
  regs[0] = binary & 0x0007;

  return cmpReg(regs, mem, "T2");
}

int CMP_Reg_T3(word binary, memory mem, int setflags)
{
//Implémenter shift  
  return 0;
}

int IT_T1(word binary, memory mem, int setflags, unsigned* it_state)
{
  *it_state = binary & 0x00FF; 

  return 0;
}

int movImm(int registr, int immediate, memory mem) 
{
  // gérer set flags
  mem->reg[registr] = immediate;

  return 0; 
}

int MOV_Imm_T1(word binary, memory mem, int setflags)
{
  int registr = (binary & 0x0700) >> 8;
  int imm = binary & 0x00FF;

  return movImm(registr, imm, mem);
}

int MOV_Imm_T2(word binary, memory mem, int setflags)
{
  int registr = (binary & 0x00000F00) >> 8;
  int imm = ThumbExpandImm((binary & 0x04000000) >> 26, (binary & 0x00007000) >> 12, binary & 0x000000FF); //ThumbExpandImm_C
 //int imm = 3;
  return movImm(registr, imm, mem); 
}

int MOV_Imm_T3(word binary, memory mem, int setflags)
{
  int registr = (binary & 0x000F0000) >> 8;
  int imm = ((binary & 0x000F0000) >> 4) + ((binary & 0x04000000) >> 14) + ((binary & 0x00007000) >> 4) + (binary & 0x000000FF);

  return movImm(registr, imm, mem);
}

int movReg(int* registers, memory mem)
{
  mem->reg[registers[0]] = mem->reg[registers[1]];
  return 0;
}


int MOV_Reg_T1(word binary, memory mem, int setflags)
{
  int regs[2];
  
  regs[1] = (binary & 0x0078) >> 3;
  regs[0] = binary & 0x0007;

  return movReg(regs, mem);
}

int MOV_Reg_T2(word binary, memory mem, int setflags)
{
  int regs[2];
  
  regs[1] = (binary & 0x0038) >> 3;
  regs[0] = binary & 0x0007;

  return movReg(regs, mem);
}

int MOV_Reg_T3(word binary, memory mem, int setflags)
{
  int regs[2];
  
  regs[1] = (binary & 0x00000F00) >> 8;
  regs[0] = binary & 0x0000000F;

  return movReg(regs, mem); 
}


int B_T1(word binary, memory mem, int setflags)
{
  int cond = (binary & 0xF00) >> 8;
  if (conditionPassed(mem, cond)) { 
    int8_t imm8 = binary & 0xFF;
    int32_t offset;
    if (imm8 >= 0)
      offset = imm8 << 1;
    
    else {
      offset = ((imm8 << 1) | 0xFFFFFE00);
    }

    mem->reg[15] = mem->reg[15] + offset + 2; //Ajouter la taille de l'instruction
   }
  return 0;
}

int B_T2(word binary, memory mem, int setflags)
{
  int32_t imm11 = binary & 0x07FF;
  int32_t offset;
  if ((imm11 << 21 ) >= 0)
    offset = imm11 << 1;
    
  else {
    offset = ((imm11 << 1) | 0xFFFFF000);
  }

  mem->reg[15] = mem->reg[15] + offset + 2;

  return 0;
}


int B_T3(word binary, memory mem, int setflags) {
  int cond = (binary & 0x03C00000) >> 22;
  if (conditionPassed(mem, cond)) { 
    int32_t imm11 = binary & 0x7FF;
    int32_t offset;
    if ((imm11 << 21) >= 0)
      offset = imm11 << 1;
    
    else {
      offset = ((imm11 << 1) | 0xFFFFF000);
    }

    mem->reg[15] = mem->reg[15] + offset + 4;
   }
  return 0;
}

int B_T4(word binary, memory mem, int setflags)
{
  int32_t imm11 = binary & 0x07FF;
  int32_t imm10 = (binary & 0x03FF0000) >> 16;
  int32_t S = (binary & 0x04000000) >> 26;
  int32_t I1 = ~(((binary & 0x00002000) >> 13) ^ S);
  int32_t I2 = ~(((binary & 0x00000800) >> 11) ^ S);

  int32_t imm = (imm11 << 1) + (imm10 << 12) + (I2 << 22) + (I1 << 23) + (S << 24); 
  int32_t offset;
  if ((imm << 7)  >= 0)
    offset = imm;
    
  else {
    offset = (imm | 0xFE000000);
  }

  mem->reg[15] = mem->reg[15] + offset + 4;

  return 0;
}

int BL_T1(word binary, memory mem, int setflags)
{
  int32_t imm11 = binary & 0x07FF;
  int32_t imm10 = (binary & 0x03FF0000) >> 16;
  int32_t S = (binary & 0x04000000) >> 26;
  int32_t I1 = ~(((binary & 0x00002000) >> 13) ^ S);
  int32_t I2 = ~(((binary & 0x00000800) >> 11) ^ S);

  int32_t imm = (imm11 << 1) + (imm10 << 12) + (I2 << 22) + (I1 << 23) + (S << 24); 

  int32_t offset;
  if ((imm << 7)  >= 0)
    offset = imm;
    
  else {
    offset = (imm | 0xFE000000);
  }

  mem->reg[14] = mem->reg[15] | 0x1;
  mem->reg[15] = mem->reg[15] + offset + 4;

  return 0;
}

int BX_T1(word binary, memory mem, int setflags)
{
  int32_t registr = (binary & 0x0078) >> 3;
  int offset = mem->reg[registr];
  mem->reg[15] = mem->reg[15] + offset + 2;

  return 0;
}

int LDR_Imm_T1(word binary, memory mem, int setflags)
{
  int32_t imm5 = (binary & 0x07C0) >> 6;
  int32_t registrn = (binary & 0x0038) >> 3;
  int32_t registrt = (binary & 0x0007);

  int32_t imm32 = (imm5 << 2);
  int adress = mem->reg[registrn] + imm32;

  int bValue[4];

  int i =0;
  if (mem->endianness == LSB) {
    for (i=0;i<=3;i++) {
      bValue[i] = read_memory_value(adress + i, mem);    
    }
  }
  else if (mem->endianness == MSB) {
    for (i=0;i<=3;i++) {
      bValue[3-i] = read_memory_value(adress + i, mem);
    }
  }

  int value = (bValue[0] << 24) + (bValue[1] << 16) + (bValue[2] << 8) + bValue[3]; 
  mem->reg[registrt] = value; 
  return 0;
}

int LDR_Imm_T2(word binary, memory mem, int setflags)
{
  int32_t imm8 = (binary & 0x00FF);
  int32_t registrt = (binary & 0x0700) >> 8;

  int32_t imm32 = (imm8 << 2);
  int adress = mem->reg[13] + imm32;

  int bValue[4];
  int i =0;
  if (mem->endianness == LSB) {
    for (i=0;i<=3;i++) {
      bValue[i] = read_memory_value(adress + i, mem);    
    }
  }
  else if (mem->endianness == MSB) {
    for (i=0;i<=3;i++) {
      bValue[3-i] = read_memory_value(adress + i, mem);
    }
  }

  int value = (bValue[0] << 24) + (bValue[1] << 16) + (bValue[2] << 8) + bValue[3]; 

  mem->reg[registrt] = value; 

  return 0;
}

int LDR_Imm_T3(word binary, memory mem, int setflags)
{
  int32_t imm12 = binary & 0x0FFF;
  int32_t registrn = (binary & 0x000F0000) >> 16;
  int32_t registrt = (binary & 0xF000) >> 12;

  int32_t imm32 = imm12;
  int adress = mem->reg[registrn] + imm32;

  int bValue[4];
  int i =0;
  if (mem->endianness == LSB) {
    for (i=0;i<=3;i++) {
      bValue[i] = read_memory_value(adress + i, mem);    
    }
  }
  else if (mem->endianness == MSB) {
    for (i=0;i<=3;i++) {
      bValue[3-i] = read_memory_value(adress + i, mem);
    }
  }

  int value = (bValue[0] << 24) + (bValue[1] << 16) + (bValue[2] << 8) + bValue[3]; 

  mem->reg[registrt] = value; 

  return 0;
}

int LDR_Imm_T4(word binary, memory mem, int setflags) {return 0;}

int LDR_Lit_T1(word binary, memory mem, int setflags)
{
  int32_t imm8 = (binary & 0x00FF);
  int32_t registrt = (binary & 0x0700) >> 8;

  int32_t imm32 = (imm8 << 2);
  int adress = mem->reg[15] + imm32;

  int bValue[4];
  int i =0;
  if (mem->endianness == LSB) {
    for (i=0;i<=3;i++) {
      bValue[i] = read_memory_value(adress + i, mem);    
    }
  }
  else if (mem->endianness == MSB) {
    for (i=0;i<=3;i++) {
      bValue[3-i] = read_memory_value(adress + i, mem);
    }
  }

  int value = (bValue[0] << 24) + (bValue[1] << 16) + (bValue[2] << 8) + bValue[3];
  mem->reg[registrt] = value; 

  return 0;
}

int LDR_Lit_T2(word binary, memory mem, int setflags)
{
  int32_t imm12 = (binary & 0x0FFF);
  int32_t registrt = (binary & 0xF000) >> 12;
  int U = (binary & 0x00800000) >> 23;

  int32_t imm32 = imm12;
  int adress;
  if (U == 0) 
    adress = mem->reg[15] - imm32;

  else
    adress = mem->reg[15] + imm32;

  int bValue[4];
  int i =0;
  if (mem->endianness == LSB) {
    for (i=0;i<=3;i++) {
      bValue[i] = read_memory_value(adress + i, mem);    
    }
  }
  else if (mem->endianness == MSB) {
    for (i=0;i<=3;i++) {
      bValue[3-i] = read_memory_value(adress + i, mem);
    }
  }

  int value = (bValue[0] << 24) + (bValue[1] << 16) + (bValue[2] << 8) + bValue[3]; 

  mem->reg[registrt] = value; 

  return 0;
}

int MOVT_T1(word binary, memory mem, int setflags)
{
  int registr = (binary & 0x00000F00) >> 8;
  int imm = ((binary & 0x000F0000) >> 4 ) + ((binary & 0x04000000) >> 15) + ((binary & 0x00007000) >> 4) + (binary & 0x000000FF);
  imm = (imm << 16);
  mem->reg[registr] = imm + (mem->reg[registr] & 0x0000FFFF);

  return 0;   
}

int MUL_T1(word binary, memory mem, int setflags)
{
  int destination = (binary & 0x0007);
  int registr = (binary & 0x0038) >> 3;

  int64_t mult = mem->reg[destination] * mem->reg[registr];
  int result = mult & 0x00000000FFFFFFFF;

  mem->reg[destination] = result; 

  return 0;
}

int MUL_T2(word binary, memory mem, int setflags)
{
  int destination = (binary & 0x00000F00) >> 8 ;
  int registr1 = (binary & 0x000F0000) >> 16;
  int registr2 = (binary & 0x0000000F); 

  int64_t mult = mem->reg[registr1] * mem->reg[registr2];
  int result = mult & 0x00000000FFFFFFFF;

  mem->reg[destination] = result; 

  return 0;
}

int POP_T1(word binary, memory mem, int setflags)
{
  if (mem->reg[13] == mem->stack->vaddr + mem->stack->size)
    return 0; //Rien à sortir
  int reglist = (binary & 0x00FF);
  int P = (binary & 0x0100) >> 8;
  int registers = reglist + ( P << 15);

  int i = 0;
  int bit;
  for(i = 0; i < 15; i++) {
    bit = (registers & (1 << i)) >> i;
    if (bit == 1) {
      mem->reg[13] = mem->reg[13] + 4;
      mem->reg[i] = read_word(mem->reg[13], mem); 
    }
  } 

  return 0;
}

int POP_T2(word binary, memory mem, int setflags)
{
  if (mem->reg[13] == mem->stack->vaddr + mem->stack->size)
    return 0; //Rien à sortir
  int reglist = (binary & 0x1FFF);
  int P = (binary & 0x8000) >> 15;
  int M = (binary & 0x4000) >> 14;
  int registers = reglist + (M << 14) + ( P << 15);

  int i = 0;
  int bit;
  for(i = 0; i < 15; i++) {
    bit = (registers & (1 << i)) >> i;
    if (bit == 1) {
      mem->reg[13] = mem->reg[13] + 4;
      mem->reg[i] = read_word(mem->reg[13], mem); 
    }
  } 
  return 0;
}

int POP_T3(word binary, memory mem, int setflags)
{
  if (mem->reg[13] == mem->stack->vaddr + mem->stack->size)
    return 0; //Rien à sortir
  int registrt = (binary & 0xF000) >> 12;
  mem->reg[13] = mem->reg[13] + 4;
  mem->reg[registrt] = read_word(mem->reg[13], mem); 
  return 0;
}

int PUSH_T1(word binary, memory mem, int setflags)
{
  int reglist = (binary & 0x00FF);
  int M = (binary & 0x0100) >> 8;
  int registers = reglist + ( M << 14);

  int i = 0;
  int bit;
  for(i = 0; i < 15; i++) {
    bit = (registers & (1 << (15 - i))) >> (15 - i);
    if (bit == 1) {
      write_word(mem->reg[13], mem->reg[15 - i], mem);
      mem->reg[13] = mem->reg[13] - 4; //Decrement apres ajout
    }
  } 
  return 0;
}

int PUSH_T2(word binary, memory mem, int setflags)
{
  int reglist = (binary & 0xEFFF);
  int M = (binary & 0x04000) >> 14;
  int registers = reglist + ( M << 14);

  int i = 0;
  int bit;
  for(i = 0; i < 15; i++) {
    bit = (registers & (1 << (15 - i))) >> (15 - i);
    if (bit == 1) {
      write_word(mem->reg[13], mem->reg[15 - i], mem);
      mem->reg[13] = mem->reg[13] - 4;
    }
  } 
  return 0;
}

int PUSH_T3(word binary, memory mem, int setflags)
{
  int registrt = (binary & 0xF000) >> 12;
  write_word(mem->reg[13], mem->reg[registrt], mem);
  mem->reg[13] = mem->reg[13] - 4;

  return 0;
}

int STR_Imm_T1(word binary, memory mem, int setflags)
{ 
  int32_t imm5 = (binary & 0x07C0) >> 6;
  int32_t registrn = (binary & 0x0038) >> 3;
  int32_t registrt = (binary & 0x0007);

  int32_t imm32 = (imm5 << 2);
  int adress = mem->reg[registrn] + imm32;
  int value = mem->reg[registrt];

  byte bValue[4];
  bValue[0] = (byte)((value & 0xff000000) >> 24);
  bValue[1] = (byte)((value & 0x00ff0000) >> 16);
  bValue[2] = (byte)((value & 0x0000ff00) >> 8);
  bValue[3] = (byte)((value & 0x000000ff));

  int i =0;
  if (mem->endianness == LSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[i], mem)) {
        WARNING_MSG("address is not valid %s\n", "STR_IMM_T1");
        return 1;
      }
    }
  }
  else if (mem->endianness == MSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[3-i], mem)) {
        WARNING_MSG("address is not valid %s\n", "STR_IMM_T1");
        return 1;
      }
    }
  }

  return 0;
}

int STR_Imm_T2(word binary, memory mem, int setflags)
{
  int32_t imm8 = (binary & 0xFF);
  int32_t registrt = (binary & 0x0700) >> 8;

  int32_t imm32 = (imm8 << 2);
  int adress = mem->reg[13] + imm32;
  int value = mem->reg[registrt];

  byte bValue[4];
  bValue[0] = (byte)((value & 0xff000000) >> 24);
  bValue[1] = (byte)((value & 0x00ff0000) >> 16);
  bValue[2] = (byte)((value & 0x0000ff00) >> 8);
  bValue[3] = (byte)((value & 0x000000ff));

  int i =0;
  if (mem->endianness == LSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[i], mem)) {
        WARNING_MSG("address is not valid %s\n", "STR_IMM_T2");
        return 1;
      }
    }
  }
  else if (mem->endianness == MSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[3-i], mem)) {
        WARNING_MSG("address is not valid %s\n", "STR_IMM_T2");
        return 1;
      }
    }
  }

  return 0;
}

int STR_Imm_T3(word binary, memory mem, int setflags)
{
  int32_t imm12 = binary & 0x0FFF;
  int32_t registrn = (binary & 0x000F0000) >> 16;
  int32_t registrt = (binary & 0xF000) >> 12;

  int32_t imm32 = imm12;
  int adress = mem->reg[registrn] + imm32;
  int value = mem->reg[registrt];

  byte bValue[4];
  bValue[0] = (byte)((value & 0xff000000) >> 24);
  bValue[1] = (byte)((value & 0x00ff0000) >> 16);
  bValue[2] = (byte)((value & 0x0000ff00) >> 8);
  bValue[3] = (byte)((value & 0x000000ff));

  int i =0;
  if (mem->endianness == LSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[i], mem)) {
        WARNING_MSG("address is not valid %s\n", "STR_IMM_T1");
        return 1;
      }
    }
  }
  else if (mem->endianness == MSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[3-i], mem)) {
        WARNING_MSG("address is not valid %s\n", "STR_IMM_T1");
        return 1;
      }
    }
  }
  return 0;
}

int STR_Imm_T4(word binary, memory mem, int setflags) {return 0;}

int STR_Reg_T1(word binary, memory mem, int setflags)
{
  int32_t registrm = (binary & 0x01C0) >> 6;
  int32_t registrn = (binary & 0x0038) >> 3;
  int32_t registrt = (binary & 0x0007);

  int adress = mem->reg[registrn] + mem->reg[registrm];
  int value = mem->reg[registrt];

  byte bValue[4];
  bValue[0] = (byte)((value & 0xff000000) >> 24);
  bValue[1] = (byte)((value & 0x00ff0000) >> 16);
  bValue[2] = (byte)((value & 0x0000ff00) >> 8);
  bValue[3] = (byte)((value & 0x000000ff));

  int i =0;
  if (mem->endianness == LSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[i], mem)) {
        WARNING_MSG("address is not valid %s\n", "STR_IMM_T1");
        return 1;
      }
    }
  }
  else if (mem->endianness == MSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[3-i], mem)) {
        WARNING_MSG("address is not valid %s\n", "STR_IMM_T1");
        return 1;
      }
    }
  }
  return 0;
}

int STR_Reg_T2(word binary, memory mem, int setflags)
{
  int32_t shift = (binary & 0x0030) >> 4;
  int32_t registrn = (binary & 0x000F0000) >> 16;
  int32_t registrt = (binary & 0xF000) >> 12;
  int32_t registrm = (binary & 0x000F);

  int adress = mem->reg[registrn] + (mem->reg[registrm] << shift);
  int value = mem->reg[registrt];

  byte bValue[4];
  bValue[0] = (byte)((value & 0xff000000) >> 24);
  bValue[1] = (byte)((value & 0x00ff0000) >> 16);
  bValue[2] = (byte)((value & 0x0000ff00) >> 8);
  bValue[3] = (byte)((value & 0x000000ff));

  int i =0;
  if (mem->endianness == LSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[i], mem)) {
        WARNING_MSG("address is not valid %s\n", "STR_IMM_T1");
        return 1;
      }
    }
  }
  else if (mem->endianness == MSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[3-i], mem)) {
        WARNING_MSG("address is not valid %s\n", "STR_IMM_T1");
        return 1;
      }
    }
  }
  return 0;
}

int subImm(int immediate, int* registers,  memory mem, char* encoding)
{
  if (strcmp(encoding, "T2") == 0) {
    mem->reg[registers[0]] = mem->reg[registers[0]] - immediate;
    return 0;      
  } 

  else {
    mem->reg[registers[0]] = mem->reg[registers[1]] -  immediate;
    return 0;
  }
}

int SUB_Imm_T1(word binary, memory mem, int setflags)
{
  int imm = (binary & 0x01C0) >> 6;
  int regs[2];
  
  regs[1] = (binary & 0x0038) >> 3;
  regs[0] = (binary & 0x0007);

  return subImm(imm, regs, mem, "T1");
}

int SUB_Imm_T2(word binary, memory mem, int setflags)
{
  int imm = binary & 0x00FF;
  int regs[2];
  
  regs[0] = (binary & 0x0700) >> 8;

  return subImm(imm, regs, mem, "T2");
}

int SUB_Imm_T3(word binary, memory mem, int setflags)
{
  int imm = ThumbExpandImm((binary & 0x04000000) >> 26, (binary & 0x00007000) >> 12, binary & 0x000000FF);
  int regs[2];
  
  regs[1] = (binary & 0x000F0000) >> 16;
  regs[0] = (binary & 0x00000F00) >> 8;

  return subImm(imm, regs, mem, "T3");
}

int SUB_Imm_T4(word binary, memory mem, int setflags)
{
  int imm = ((binary & 0x04000000) >> 15) +((binary & 0x00007000) >> 4) + (binary & 0x000000FF);
  int regs[2];
  
  regs[1] = (binary & 0x000F0000) >> 16;
  regs[0] = (binary & 0x00000F00) >> 8;

  return subImm(imm, regs, mem, "T4");
}

int subReg(int* registers, memory mem, char* encoding)
{
  if (strcmp(encoding, "T2") == 0) {
    // gérer shift
    mem->reg[registers[0]] = mem->reg[registers[1]] -  mem->reg[registers[2]];
    return 0;
  }

  if (strcmp(encoding, "T1") == 0) {
    mem->reg[registers[0]] = mem->reg[registers[1]] -  mem->reg[registers[2]];
    return 0;   
  }

  return 1;
}

int SUB_Reg_T1(word binary, memory mem, int setflags)
{
  int regs[3];
  
  regs[2] = (binary & 0x01C00) >> 6;
  regs[1] = (binary & 0x0038) >> 3;
  regs[0] = (binary & 0x0007);

  return subReg(regs, mem, "T1");
}

int SUB_Reg_T2(word binary, memory mem, int setflags)
{
  int regs[2];
  regs[2] = (binary & 0x0000000F);
  regs[1] = (binary & 0x000F0000) >> 16;
  regs[0] = (binary & 0x00000F00) >> 8;

  return subReg(regs, mem, "T2");
}

int SUB_SP_T1(word binary, memory mem, int setflags)
{
  int imm7 = binary & 0x007F;
  int imm32 = imm7 << 2;

  mem->reg[13] = mem->reg[13] - imm32;

  return 0;
}

int SUB_SP_T2(word binary, memory mem, int setflags)
{ 
  int imm = ThumbExpandImm((binary & 0x04000000) >> 26, (binary & 0x00007000) >> 12, binary & 0x000000FF);
  
  int registrd = (binary & 0x0F00) >> 8;
  mem->reg[registrd] = mem->reg[13] - imm;

  return 0;
}

int SUB_SP_T3(word binary, memory mem, int setflags)
{
  int imm = ((binary & 0x04000000) >> 15) + ((binary & 0x00007000) >> 4) + (binary & 0x000000FF);
  
  int registrd = (binary & 0x00000F00) >> 8;
  mem->reg[registrd] = mem->reg[13] - imm;

  return 0;
}

int SVC_T1(word binary, memory mem, int setflags) {return 0;}

