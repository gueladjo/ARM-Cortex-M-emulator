#include "run.h"

int run(dico* dictionary, memory mem) {
  dico* instruction = malloc(sizeof(*instruction));
  unsigned int* it_state = malloc(sizeof(*it_state)); //condition + mask
  int is_short;
  word binary;
  byte* header;

  while (mem->reg[15] <= mem->txt->vaddr + mem->txt->size) {
    header = mem->txt->raddr + mem->reg[15] - mem->txt->vaddr;
    is_short = is_16bits(header+1);
    binary = 0;

    if (mem->break_list[mem->reg[15]-mem->txt->vaddr] == 1) {
      printf("Breakpoint reached at address %x\n", mem->reg[15]);
      mem->reg[15] += 2;
      if (!is_short)
	mem->reg[15] += 2;
      return 0;
    }
    
    if (mem->break_list[mem->reg[15]-mem->txt->vaddr] == 2) {
      mem->break_list[mem->reg[15]-mem->txt->vaddr] = 0;
      return 0;
    }
      
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
  }
  free(instruction);
  return 0;
}

int execute_instruction(word binary, dico* instruction, unsigned int* it_state, memory mem) {
  int setflags = 1;
  if ((*it_state & 0xf) != 0) { //If in IT Block
    int temp = (*it_state & 0xF) << 1;
    if (!conditionPassed(mem, (*it_state >> 4))) { //Conditions non remplies
      *it_state = (*it_state & 0xE0) + temp; //Mise à jour de it_state
      return 0;
    }
    setflags = 0;
    *it_state = (*it_state & 0xE0) + temp; //Mise à jour de it_state
  }
  
  if (!strcmp(instruction->id_debug, "ADD_Imm_T1")) {
      ADD_Imm_T1(binary, mem, setflags);
      return 0;
    }
  else if (!strcmp(instruction->id_debug, "ADD_Imm_T2")) {
    ADD_Imm_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "ADD_Imm_T3")) {
    ADD_Imm_T3(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "ADD_Imm_T4")) {
    ADD_Imm_T4(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "ADD_Reg_T1")) {
    ADD_Reg_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "ADD_Reg_T2")) {
    ADD_Reg_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "ADD_Reg_T3")) {
    ADD_Reg_T3(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "B_T1")) {
    B_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "B_T2")) {
    B_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "B_T3")) {
    B_T3(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "B_T4")) {
    B_T4(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "BL_T1")) {
    BL_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "BX_T1")) {
    BX_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "CMP_Imm_T1")) {
    CMP_Imm_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "CMP_Imm_T2")) {
    CMP_Imm_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "CMP_Reg_T1")) {
    CMP_Reg_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "CMP_Reg_T2")) {
    CMP_Reg_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "CMP_Reg_T3")) {
    CMP_Reg_T3(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "IT_T1")) {
    IT_T1(binary, mem, setflags, it_state);
    return 0;
  }  
  else if (!strcmp(instruction->id_debug, "LDR_Imm_T1")) {
    LDR_Imm_T1(binary, mem, setflags);
    return 0;
  }  
  else if (!strcmp(instruction->id_debug, "LDR_Imm_T2")) {
    LDR_Imm_T2(binary, mem, setflags);
    return 0;
  }  
  else if (!strcmp(instruction->id_debug, "LDR_Imm_T3")) {
    LDR_Imm_T3(binary, mem, setflags);
    return 0;
  }  
  else if (!strcmp(instruction->id_debug, "LDR_Imm_T4")) {
    LDR_Imm_T4(binary, mem, setflags);
    return 0;
  }  
  else if (!strcmp(instruction->id_debug, "LDR_Lit_T1")) {
    LDR_Lit_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "LDR_Lit_T2")) {
    LDR_Lit_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "MOV_Imm_T1")) {
    MOV_Imm_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "MOV_Imm_T2")) {
    MOV_Imm_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "MOV_Imm_T3")) {
    MOV_Imm_T3(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "MOV_Reg_T1")) {
    MOV_Reg_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "MOV_Reg_T2")) {
    MOV_Reg_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "MOV_Reg_T3")) {
    MOV_Reg_T3(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "MOVT_T1")) {
    MOVT_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "MUL_T1")) {
    MUL_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "MUL_T2")) {
    MUL_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "POP_T1")) {
    POP_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "POP_T2")) {
    POP_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "POP_T3")) {
    POP_T3(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "PUSH_T1")) {
    PUSH_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "PUSH_T2")) {
    PUSH_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "PUSH_T3")) {
    PUSH_T3(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "STR_Imm_T1")) {
    STR_Imm_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "STR_Imm_T2")) {
    STR_Imm_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "STR_Imm_T3")) {
    STR_Imm_T3(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "STR_Imm_T4")) {
    STR_Imm_T4(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "STR_Reg_T1")) {
    STR_Reg_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "STR_Reg_T2")) {
    STR_Reg_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "SUB_Imm_T1")) {
      SUB_Imm_T1(binary, mem, setflags);
      return 0;
    }
  else if (!strcmp(instruction->id_debug, "SUB_Imm_T2")) {
    SUB_Imm_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "SUB_Imm_T3")) {
    SUB_Imm_T3(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "SUB_Imm_T4")) {
    SUB_Imm_T4(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "SUB_Reg_T1")) {
    SUB_Reg_T1(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "SUB_Reg_T2")) {
    SUB_Reg_T2(binary, mem, setflags);
    return 0;
  }
  else if (!strcmp(instruction->id_debug, "SVC_T1")) {
    SVC_T1(binary, mem, setflags);
    return 0;
  }
  
  else {
    WARNING_MSG("Warning : function not found\n");
    return 1;
    }
  return 0;
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
