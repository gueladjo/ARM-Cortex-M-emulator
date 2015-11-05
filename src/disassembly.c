#include "disassembly.h"

int is_16bits(byte* header)
{
  int mask = 0xF8000000;
  int sig1 = 0xF0000000;
  int sig2 = 0xF8000000;
  int sig3 = 0xE8000000;

  if (*header & mask == sig1 || *header & mask == sig2 || *header & mask == sig3) return 1;
  return 0;  
}

int create_mask(unsigned int a, unsigned int b)
{
  unsigned r = 0;
  for (unsigned i = a; i <= b; i++)
    r = r | 1 << i;

  return r;
}

int search_instruction(int binary, dico* dictionary, dico* instruction, int is_short)
{
  int i =0, dico_size = 53;
  if (is_short) int16_t short_binary = binary;

  for (i = 0; i < dico_size; i++) {
    if (binary & dico[i].mask == dico[i].signature) {
      instruction = &dico[i];
      return 0; 
    }
  }

  return 1;
}

int print_instruction(int binary, dico instruction, byte* header, dico* dictionary, memory mem)
{
  printf("%x       ", binary);
  byte* iterator;
  int i = 0; 
  dico temp;
  if (instruction.it == 1) {
    while((iterator = header - 2) != mem->txt->raddr && i!=4) {
      int is_short = is_16bits(iterator);
      if (is_short) {
        int16_t bin;
        memcpy(&bin, header, sizeof(bin));
      }

      else {
        int32_t bin;
        memcpy(&bin, header, sizeof(bin));
      }

      if (search_instruction(bin, dictionary, temp, is_16bits(iterator))) {
        i++;
        if (temp.sig == 0xBF00) {
	        
        }
      }
    }
    
    printf("%s ", instruction.mnemo);
    char* regs = instruction.registers_index;
    if (*regs != 'N') {
      char* token = strtok(regs, ":");
      unsigned int start, end, reg;
      do {
        sscanf(token, "%u-%u", &start, &end);
        unsigned int rmask = create_mask(start, end);
        reg = rmask & binary;
	reg = reg >> start;
        printf("r%u", reg);
	if ((token = strtok(NULL, ":")) != NULL) printf(", ");
      } while ((token != NULL);
    }
  
    char* imms = instruction.immediate_index;
    if (*imms != 'N') {
      printf(", ");
      char* token = strtok(imms, ":");
      unsigned int start, end, imm;
      do {
        sscanf(token, "%u-%u", &start, &end);
        unsigned int imask = create_mask(start, end);
        imm = imask & binary;
	imm = imm >> start;
        printf("#%u", imm);
	if ((token = strtok(NULL, ":")) != NULL) printf(", ");
      } while ((token != NULL);
    }
  }

  else if (instruction.it == 0) {
    while((iterator = header - 2) != mem->txt->raddr && i!=4) {
      int is_short = is_16bits(iterator);
      if (is_short) {
        int16_t bin;
        memcpy(&bin, header, sizeof(bin));
      }

      else {
        int32_t bin;
        memcpy(&bin, header, sizeof(bin));
      }

      if (search_instruction(bin, dictionary, temp, is_16bits(iterator))) {
        i++;
        if (temp.sig == 0xBF00) {
	        
        }
      }
    }
    
    printf("%sS ", instruction.mnemo);
    char* regs = instruction.registers_index;
    if (*regs != 'N') {
      char* token = strtok(regs, ":");
      unsigned int start, end, reg;
      do {
        sscanf(token, "%u-%u", &start, &end);
        unsigned int rmask = create_mask(start, end);
        reg = rmask & binary;
	reg = reg >> start;
        printf("r%u", reg);
	if ((token = strtok(NULL, ":")) != NULL) printf(", ");
      } while ((token != NULL);
    }
  
    char* imms = instruction.immediate_index;
    if (*imms != 'N') {
      printf(", ");
      char* token = strtok(imms, ":");
      unsigned int start, end, imm;
      do {
        sscanf(token, "%u-%u", &start, &end);
        unsigned int imask = create_mask(start, end);
        imm = imask & binary;
	imm = imm >> start;
        printf("#%u", imm);
	if ((token = strtok(NULL, ":")) != NULL) printf(", ");
      } while ((token != NULL);
    }
 
  }

  else {
    printf("%sS ", instruction.mnemo);
    char* regs = instruction.registers_index;
    if (*regs != 'N') {
      char* token = strtok(regs, ":");
      unsigned int start, end, reg;
      do {
        sscanf(token, "%u-%u", &start, &end);
        unsigned int rmask = create_mask(start, end);
        reg = rmask & binary;
	reg = reg >> start;
        printf("r%u", reg);
	if ((token = strtok(NULL, ":")) != NULL) printf(", ");
      } while ((token != NULL);
    }
  
    char* imms = instruction.immediate_index;
    if (*imms != 'N') {
      printf(", ");
      char* token = strtok(imms, ":");
      unsigned int start, end, imm;
      do {
        sscanf(token, "%u-%u", &start, &end);
        unsigned int imask = create_mask(start, end);
        imm = imask & binary;
	imm = imm >> start;
        printf("#%u", imm);
	if ((token = strtok(NULL, ":")) != NULL) printf(", ");
      } while ((token != NULL);
    }
 
  }

  return 0; 
}

int read_instruction(byte* header)
{
  int is_short = is_16bits(header);
  if (is_short) {
    int16_t binary;
    memcpy(&binary, header, sizeof(binary));
    dico instruction;
    search_instruction(binary, dico, &instruction, is_short);
    print_instruction(binary, instruction, header, dico);
  
    return 0; 
  }

  else {
    int32_t binary;
    memcpy(&binary, header, sizeof(binary));
    dico instruction;
    search_instruction(binary, dico, &instruction, is_short);
    print_instruction(binary, instruction, header, dico);
  
    return 0; 
  }
}
