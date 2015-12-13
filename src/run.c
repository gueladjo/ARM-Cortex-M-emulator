#include "run.h"

int run(dico* dictionary, memory mem) {
  dico* instruction = malloc(sizeof(*instruction));
  unsigned int* it_state = malloc(sizeof(*it_state)); //condition + mask
  int is_short;
  word binary;
  byte* header;
  int ret;

  while (mem->reg[15] <= mem->txt->vaddr + mem->txt->size) {
    header = mem->txt->raddr + mem->reg[15] - mem->txt->vaddr;
    is_short = is_16bits(header+1);
    binary = 0;

    if (mem->break_list[mem->reg[15]-mem->txt->vaddr] == 1) { //On atteint le breakpoint, on change sa valeur pour passer par dessus ensuite
      printf("Breakpoint reached at address %x\n", mem->reg[15]);
      mem->break_list[mem->reg[15]-mem->txt->vaddr] = 3;
      return 0;
    }
    
    if (mem->break_list[mem->reg[15]-mem->txt->vaddr] == 2) {
      mem->break_list[mem->reg[15]-mem->txt->vaddr] = 0;
      return 0;
    }
    
    if (mem->break_list[mem->reg[15]-mem->txt->vaddr] == 3) { 
      mem->break_list[mem->reg[15]-mem->txt->vaddr] = 1;
    }
      
    if (is_short) {
      mem->reg[15] = mem->reg[15] + 2;
      memcpy(&binary, header, 2);
      search_instruction(binary, dictionary, instruction, is_short);
      DEBUG_MSG("Instruction : %s", instruction->id_debug);
      ret = execute_instruction(binary, instruction, it_state, mem);
      if (ret == 1)
	return 0;
    }
      else {
      word temp = 0;
      mem->reg[15] = mem->reg[15] + 4;
      memcpy(&binary, header, 2);
      memcpy(&temp, header+2, 2);
      binary = (binary << 16) + temp;
      search_instruction(binary, dictionary, instruction, is_short);
      DEBUG_MSG("Instruction : %s", instruction->id_debug);
      ret = execute_instruction(binary, instruction, it_state, mem);
      if (ret == 1)
	return 0;
    }
  }
  free(instruction);
  return 0;
}

int execute_instruction(word binary, dico* instruction, unsigned int* it_state, memory mem) {
  int setflags = 0;
  if ((*it_state & 0xf) != 0) { //If in IT Block
    int temp = (*it_state & 0xF) << 1;
    if (!conditionPassed(mem, (*it_state >> 4))) { //Conditions non remplies
      *it_state = (*it_state & 0xE0) + temp; //Mise à jour de it_state
      return 0;
    }
    setflags = 1;
    *it_state = (*it_state & 0xE0) + temp; //Mise à jour de it_state
  }
  
  if (!strcmp(instruction->id_debug, "ADD_Imm_T1")) {
    return ADD_Imm_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "ADD_Imm_T2")) {
    return ADD_Imm_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "ADD_Imm_T3")) {
    return ADD_Imm_T3(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "ADD_Imm_T4")) {
    return ADD_Imm_T4(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "ADD_Reg_T1")) {
    return ADD_Reg_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "ADD_Reg_T2")) {
    return ADD_Reg_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "ADD_Reg_T3")) {
    return ADD_Reg_T3(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "B_T1")) {
    return B_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "B_T2")) {
    return B_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "B_T3")) {
    return B_T3(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "B_T4")) {
    return B_T4(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "BL_T1")) {
    return BL_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "BX_T1")) {
    return BX_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "CMP_Imm_T1")) {
    return CMP_Imm_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "CMP_Imm_T2")) {
    return CMP_Imm_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "CMP_Reg_T1")) {
    return CMP_Reg_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "CMP_Reg_T2")) {
    return CMP_Reg_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "CMP_Reg_T3")) {
    return CMP_Reg_T3(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "IT_T1")) {
    return IT_T1(binary, mem, setflags, it_state);
  }  
  else if (!strcmp(instruction->id_debug, "LDR_Imm_T1")) {
    return LDR_Imm_T1(binary, mem, setflags);
  }  
  else if (!strcmp(instruction->id_debug, "LDR_Imm_T2")) {
    return LDR_Imm_T2(binary, mem, setflags);
  }  
  else if (!strcmp(instruction->id_debug, "LDR_Imm_T3")) {
    return LDR_Imm_T3(binary, mem, setflags);
  }  
  else if (!strcmp(instruction->id_debug, "LDR_Imm_T4")) {
    return LDR_Imm_T4(binary, mem, setflags);
  }  
  else if (!strcmp(instruction->id_debug, "LDR_Lit_T1")) {
    return LDR_Lit_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "LDR_Lit_T2")) {
    return LDR_Lit_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "MOV_Imm_T1")) {
    return MOV_Imm_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "MOV_Imm_T2")) {
    return MOV_Imm_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "MOV_Imm_T3")) {
    return MOV_Imm_T3(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "MOV_Reg_T1")) {
    return MOV_Reg_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "MOV_Reg_T2")) {
    return MOV_Reg_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "MOV_Reg_T3")) {
    return MOV_Reg_T3(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "MOVT_T1")) {
    return MOVT_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "MUL_T1")) {
    return MUL_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "MUL_T2")) {
    return MUL_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "POP_T1")) {
    return POP_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "POP_T2")) {
    return POP_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "POP_T3")) {
    return POP_T3(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "PUSH_T1")) {
    return PUSH_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "PUSH_T2")) {
    return PUSH_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "PUSH_T3")) {
    return PUSH_T3(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "STR_Imm_T1")) {
    return STR_Imm_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "STR_Imm_T2")) {
    return STR_Imm_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "STR_Imm_T3")) {
    return STR_Imm_T3(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "STR_Imm_T4")) {
    return STR_Imm_T4(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "STR_Reg_T1")) {
    return STR_Reg_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "STR_Reg_T2")) {
    return STR_Reg_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "SUB_Imm_T1")) {
    return SUB_Imm_T1(binary, mem, setflags);
    }
  else if (!strcmp(instruction->id_debug, "SUB_Imm_T2")) {
    return SUB_Imm_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "SUB_Imm_T3")) {
    return SUB_Imm_T3(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "SUB_Imm_T4")) {
    return SUB_Imm_T4(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "SUB_Reg_T1")) {
    return SUB_Reg_T1(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "SUB_Reg_T2")) {
    return SUB_Reg_T2(binary, mem, setflags);
  }
  else if (!strcmp(instruction->id_debug, "SVC_T1")) {
    return SVC_T1(binary, mem, setflags);
  }  
  else {
    WARNING_MSG("Warning : function not found\n");
    return 1;
  }
}

int step(dico* dictionary, memory mem) {
  if (mem->reg[15] == 0)
    mem->reg[15] = mem->txt->vaddr;
  int is_short = is_16bits(mem->txt->raddr + mem->reg[15] - mem->txt->vaddr +1);
  if (is_short)
    mem->break_list[mem->reg[15]-mem->txt->vaddr + 2] = 2;
  else
    mem->break_list[mem->reg[15]-mem->txt->vaddr + 4] = 2;
  return run(dictionary, mem);
}

int step_into(dico* dictionary, memory mem) {
  dico* instruction = malloc(sizeof(*instruction));
  unsigned int* it_state = malloc(sizeof(*it_state)); //condition + mask
  int is_short;
  word binary;
  byte* header;
  
  if (mem->reg[15] == 0)
    mem->reg[15] = mem->txt->vaddr;
  header = mem->txt->raddr + mem->reg[15] - mem->txt->vaddr;
  is_short = is_16bits(header+1);
  binary = 0;
  
  if (is_short) {
    mem->reg[15] = mem->reg[15] + 2;
    memcpy(&binary, header, 2);
    search_instruction(binary, dictionary, instruction, is_short);
    execute_instruction(binary, instruction, it_state, mem);
  }
  else {
    word temp = 0;
    mem->reg[15] = mem->reg[15] + 4;
    memcpy(&binary, header, 2);
    memcpy(&temp, header+2, 2);
    binary = (binary << 16) + temp;
    search_instruction(binary, dictionary, instruction, is_short);
    execute_instruction(binary, instruction, it_state, mem);
  }
  free(instruction);
  return 0;
}
