#include "run.h"

int run(dico* dictionary, memory mem) {
  dico* instruction = malloc(sizeof(*instruction));
  int* in_it = malloc(sizeof(*in_it)); //Between 1 and 4 : in IT block
  unsigned int* it_state = malloc(sizeof(*it_state)); //condition + mask
  int is_short;
  word binary;
  byte* header;

  while (mem->reg[15] <= mem->txt->vaddr + mem->txt->size) {
    header = mem->txt->raddr + mem->reg[15] - mem->txt->vaddr;
    is_short = is_16bits(header+1);
    binary = 0;

    if (is_short) {
      memcpy(&binary, header, 2);
      search_instruction(binary, dictionary, instruction, is_short);
      execute_instruction(binary, instruction, in_it, it_state);
      mem->reg[15] = mem->reg[15] + 2;
    }
      else {
      word temp = 0;
      memcpy(&binary, header, 2);
      memcpy(&temp, header+2, 2);
      binary = (binary << 16) + temp;
      search_instruction(binary, dictionary, instruction, is_short);
      execute_instruction(binary, instruction, in_it, it_state);
      mem->reg[15] = mem->reg[15] + 4;
    }
    (*in_it)--;
  }
  free(instruction);
  return 0;
}

int execute_instruction(word binary, dico* instruction, int* in_it, unsigned int* it_state) {
  return 0;
}
