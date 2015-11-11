#include "disassembly.h"

int is_16bits(byte* header)
{
  byte mask = 0xF0;
  byte sig1 = 0xF0;
  byte sig2 = 0xE0;

  if ((*header & mask) == sig1 || (*header & mask) == sig2) return 0;
  return 1;  
}

int it_condition(unsigned int it_state, char* condition) {
  int cond;
  cond = it_state >> 4;
  if (cond == 0) strcpy(condition, "EQ");
  if (cond == 1) strcpy(condition, "NE");
  if (cond == 2) strcpy(condition, "HS");
  if (cond == 3) strcpy(condition, "LO");
  if (cond == 4) strcpy(condition, "MI");
  if (cond == 5) strcpy(condition, "PL");
  if (cond == 6) strcpy(condition, "VS");
  if (cond == 7) strcpy(condition, "VC");
  if (cond == 8) strcpy(condition, "HI");
  if (cond == 9) strcpy(condition, "LS");
  if (cond == 10) strcpy(condition, "GE");
  if (cond == 11) strcpy(condition, "LT");
  if (cond == 12) strcpy(condition, "GT");
  if (cond == 13) strcpy(condition, "LE");
  if (cond == 14) strcpy(condition, "AL");

  return 0;
}

int it_XYZ(unsigned int it_state, char* XYZ) {
  unsigned int mask = it_state & 0xF;
  unsigned int firstcond = (it_state & 0x10) >> 4;
  
  if (mask == 8) strcpy(XYZ, "");
  if (mask == (4 + (firstcond << 3))) strcpy(XYZ, "T");
  if (mask == (4 + (~firstcond << 3))) strcpy(XYZ, "E");
  if (mask == (2 + (firstcond << 3) + (firstcond << 2))) strcpy(XYZ, "TT");
  if (mask == (2 + (~firstcond << 3) + (firstcond << 2))) strcpy(XYZ, "ET");
  if (mask == (2 + (firstcond << 3) + (~firstcond << 2))) strcpy(XYZ, "TE");
  if (mask == (2 + (~firstcond << 3) + (~firstcond << 2))) strcpy(XYZ, "EE");
  if (mask == (1 + (firstcond << 3) + (firstcond << 2) + (firstcond << 1))) strcpy(XYZ, "TTT");
  if (mask == (1 + (~firstcond << 3) + (firstcond << 2) + (firstcond << 1))) strcpy(XYZ, "ETT");
  if (mask == (1 + (firstcond << 3) + (~firstcond << 2) + (firstcond << 1))) strcpy(XYZ, "TET");
  if (mask == (1 + (~firstcond << 3) + (~firstcond << 2) + (firstcond << 1))) strcpy(XYZ, "EET");
  if (mask == (1 + (firstcond << 3) + (firstcond << 2) + (~firstcond << 1))) strcpy(XYZ, "TTE");
  if (mask == (1 + (~firstcond << 3) + (firstcond << 2) + (~firstcond << 1))) strcpy(XYZ, "ETE");
  if (mask == (1 + (firstcond << 3) + (~firstcond << 2) + (~firstcond << 1))) strcpy(XYZ, "TEE");
  if (mask == (1 + (~firstcond << 3) + (~firstcond << 2) + (~firstcond << 1))) strcpy(XYZ, "EEE");

  return 0;
}

int search_instruction(word binary, dico* dictionary, dico* instruction, int is_short)
{
  int i =0, dico_size = 53;
  for (i = 0; i < dico_size; i++) {
    if ((binary & dictionary[i].mask) == dictionary[i].sig) {
      *instruction = dictionary[i];
      return 0; 
    }
  }

  return 1;
}

int decode_instruction(int binary, dico* instruction, int* in_it, unsigned int* it_state, int print) {
  if (instruction->it == 3) { //If IT instruction
    word mask;
    int temp;
    unsigned int start, end;
    sscanf(instruction->immediate_index, "%u-%u", &end, &start);
    mask = create_mask(start, end);
    *it_state = binary & mask;
    temp = *it_state;
    *in_it = 5; //N+1 instructions under IT
    while ((temp & 1) != 1) {
      *(in_it)--;
      temp = temp >> 1;
    }
    if(print) {
      char XYZ[4];
      char condition[3];
      it_XYZ(*it_state, XYZ);
      it_condition(*it_state, condition);
      printf("IT%s %s\n", XYZ, condition);
    }
    return 0;
  }
  
  if (print) { //If we display instruction
    if (*it_state <= 0) { //If not in IT block
      
      if (instruction->it == 0 || instruction->it == 1) {
	char* token;
	unsigned int start, end, var;
	word mask;
	printf("%s ", instruction->mnemo);
	
	if (instruction->registers_index != 'N') {
	  switch (instruction->treatReg) { //Treatment on reg
	    
	  case 0:
	    token = strtok(instruction->registers_index, ":");
	    do {
	      sscanf(token, "%u-%u", &end, &start);
	      mask = create_mask(start, end);
	      var = (mask & binary) >> start;
	      printf("r%u", var);
	      if ((token = strtok(NULL, ":")) != NULL) printf(", ");
	    } while (token != NULL);
	    break;
	  case 1:
	    token = strtok(instruction->registers_index, ":");
	    sscanf(token, "%u-%u", &end, &start);
	    mask = create_mask(start, end);
	    var = (mask & binary) >> start;
	    var =+ (binary & 0x80) >> 4;
	    printf("r%u, ", var);
	    token = strtok(NULL, ":");
	    sscanf(token, "%u-%u", &end, &start);
	    mask = create_mask(start, end);
	    var = (mask & binary) >> start;
	    printf("r%u", var);
	  }
	}

	if (instruction->immediate_index != 'N') {
	  printf("imm ");
	}
      }
    }
  }
  printf("\n");
  return 0;
}

word create_mask(unsigned int a, unsigned int b)
{
  word r = 0;
  int i;
  for ( i = a; i <= b; i++)
    r = r | 1 << i;
  return r;
}
  
int disasm(size_t startadress, size_t endadress, dico* dictionary, memory mem)
{
  size_t nb_bytes = (endadress - mem->txt->vaddr);
  byte* header;
  int i = 0;
  word binary;
  dico* instruction;
  int is_short;
  int* in_it = malloc(sizeof(*in_it)); //Between 1 and 4 : in IT block
  unsigned int* it_state = malloc(sizeof(*it_state)); //condition + mask
  int print;

  *it_state = 0;
  instruction = malloc(sizeof(*instruction));
  header =  mem->txt->raddr;
  while (i <= nb_bytes) {
    if(i >= (startadress - mem->txt->vaddr) && i <= (endadress - mem->txt->vaddr)) {
      print = 1;
      printf("%x :: ", startadress + i);
    }
    else {
      print = 0;
    }
    is_short = is_16bits(header+1);
    binary = 0;
    
    if (is_short) {
      memcpy(&binary, header, 2);
      search_instruction(binary, dictionary, instruction, is_short);
      decode_instruction(binary, instruction, in_it, it_state, print);
      i = i + 2;
      header = header + 2;
    }
    else {
      word temp = 0;
      memcpy(&binary, header, 2);
      memcpy(&temp, header+2, 2);
      binary = (binary << 16) + temp;
      search_instruction(binary, dictionary, instruction, is_short);
      decode_instruction(binary, instruction, in_it, it_state, print);
      i = i + 4;
      header = header + 4;     
    }
  }
  free(instruction);
  return 0;
}

void extract_dico(char* dico_file, dico* dico) {
  int i;
  FILE* pf_dico;
  char token[32];
  if ((pf_dico = fopen(dico_file, "r")) == NULL)
    ERROR_MSG("Cannot open file %s", dico_file);
  for (i=0;i<11;i++) {
    fscanf(pf_dico, "%s", token);
  }
  for (i=0;i<=52;i++) {
    fscanf(pf_dico, "%s", token);
    strcpy(dico[i].id_debug, token);
    fscanf(pf_dico, "%s", token);
    strcpy(dico[i].mnemo, token);
    fscanf(pf_dico, "%s", token);
    dico[i].size = atoi(token);
    fscanf(pf_dico, "%s", token);
    dico[i].sig = strtol(token, NULL, 16);
    fscanf(pf_dico, "%s", token);
    dico[i].mask = strtol(token, NULL, 16);
    fscanf(pf_dico, "%s", token);
    dico[i].nb_op = atoi(token);
    fscanf(pf_dico, "%s", token);
    strcpy(dico[i].registers_index, token);
    fscanf(pf_dico, "%s", token);
    strcpy(dico[i].immediate_index, token);
    fscanf(pf_dico, "%s", token);
    dico[i].it = atoi(token);
    fscanf(pf_dico, "%s", token);
    dico[i].treatImm = atoi(token);
    fscanf(pf_dico, "%s", token);
    dico[i].treatReg = atoi(token);
  }
  fclose(pf_dico);
  return;
}

word ThumbExpandImm(unsigned int i, unsigned int imm3, unsigned int imm8) {
  word imm12, imm32;
  imm12 = (i << 11) + (imm3 << 8) + imm8;
  if (!(imm12 & 0xC00)) {
    switch(imm12 & 0x300) {
    case 0:
      imm32 = imm12;
      break;
    case 0x100:
      imm32 = (imm8 << 16) + imm8;
      break;
    case 0x200:
      imm32 = (imm8 << 24) + (imm8 << 8);
      break;
    case 0x300:
      imm32 = (imm8 << 24) + (imm8 << 16) + (imm8 << 8) + imm8;
      break;
    }
  }
  else {
    word temp;
    word offset;
    temp = ((imm8 & 0x7F) + 0x80) << 24; //1bcdefgh (see pg A5-116)
    offset = (i << 4) + (imm3 << 1) + ((imm8 & 0x80) >> 7) - 8; //offset to apply, see i:imm3:a
    imm32 = temp >> offset;
  }
  return imm32;
}

 char* DecodeImmShift(unsigned int imm3, unsigned int imm2, unsigned int type, char* ret) {
  char temp[16];
  unsigned int imm5;
  imm5 = (imm3 << 2) + imm2;
  switch(type) {
  case 0:
    strcpy(ret, "LSL");
    sprintf(temp, "%u", imm5);
    strcat(ret, " #");
    strcat(ret, temp);
    break;
  case 1:
    strcpy(ret, "LSR");
    if (!imm5)
      imm5 = 32;
    sprintf(temp, "%u", imm5);
    strcat(ret, " #");
    strcat(ret, temp);
    break;
  case 2:
    strcpy(ret, "ASR");
    if (!imm5)
      imm5 = 32;
    sprintf(temp, "%u", imm5);
    strcat(ret, " #");
    strcat(ret, temp);
    break;
  case 3:
    if (!imm5)
      strcpy(ret, "RRX");
    else {
      strcpy(ret, "ROR");
      sprintf(temp, "%u", imm5);
      strcat(ret, " #");
      strcat(ret, temp);
    }
    break;
  default:
    ERROR_MSG("Wrong shift type %s", "disasmcmd");
  }
  return ret;
}
