#include "disassembly.h"

int is_16bits(byte* header)
{
  byte mask = 0xF0;
  byte sig1 = 0xF0;
  byte sig2 = 0xE0;

  if ((*header & mask) == sig1 || (*header & mask) == sig2) return 0;
  return 1;  
}

int it_condition(unsigned int firstcond, char* condition)
{
  if (firstcond == 0) strcpy(condition, "EQ");
  if (firstcond == 1) strcpy(condition, "NE");
  if (firstcond == 2) strcpy(condition, "HS");
  if (firstcond == 3) strcpy(condition, "LO");
  if (firstcond == 4) strcpy(condition, "MI");
  if (firstcond == 5) strcpy(condition, "PL");
  if (firstcond == 6) strcpy(condition, "VS");
  if (firstcond == 7) strcpy(condition, "VC");
  if (firstcond == 8) strcpy(condition, "HI");
  if (firstcond == 9) strcpy(condition, "LS");
  if (firstcond == 10) strcpy(condition, "GE");
  if (firstcond == 11) strcpy(condition, "LT");
  if (firstcond == 12) strcpy(condition, "GT");
  if (firstcond == 13) strcpy(condition, "LE");
  if (firstcond == 14) strcpy(condition, "AL");

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
  if (is_short) {
    int16_t short_binary = binary;
  }
  for (i = 0; i < dico_size; i++) {
    if ((binary & dictionary[i].mask) == dictionary[i].sig) {
      *instruction = dictionary[i];
      return 0; 
    }
  }

  return 1;
}

int print_instruction(int binary, dico* instruction, byte* header, dico* dictionary, memory mem)
{ 
  byte* iterator;
  int i = 0; 
  dico* temp;
  temp = malloc(sizeof(*temp));
  printf("%x       ", binary);
  if (instruction->it == 1) {
    char condition[3];
    condition[0] = '\0';
    while((iterator = header - 2) != mem->txt->raddr && i!=4) {
      int is_short = is_16bits(iterator);
      int32_t bin;
      if (is_short) {
        memcpy(&bin, header, 2);
      }
      
      else {
        memcpy(&bin, header, 4);
      }

      if (search_instruction(bin, dictionary, temp, is_16bits(iterator))) {
        i++;
        if (temp->sig == 0xBF00) {
	  int firstcond = (0x0010 & bin) >> 4;
	  int maskit = bin & 0x000F;
	  int j = 0;
          if ((maskit & 1) == 1) j = 4;	        
          else if ((maskit & (1 << 1)) == 1) j = 3;      
          else if ((maskit & (1 << 2)) == 1) j = 2;
          else if ((maskit & (1 << 3)) == 1) j = 1;

	  if (j >= i) {	    
            if (((maskit & (1 << (5 - i))) == (firstcond & 1)) || j == i)
	      it_condition(firstcond, condition);
	    else {
	      if (firstcond == 14) it_condition(firstcond, condition);
	      else if (firstcond % 2 == 0) it_condition(firstcond + 1, condition);
	      else if (firstcond % 2 == 1) it_condition(firstcond - 1, condition);
	    }	        
	  }
	}
      }
    }
    printf("%s%s ", instruction->mnemo, condition);
    char* regs = instruction->registers_index;
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
      } while ((token != NULL));
    }
  
    char* imms = instruction->immediate_index;
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
      } while ((token != NULL));
    }
  }

  else if (instruction->it == 0) {
    char condition[3];
    condition[0] = '\0';
    while((iterator = header - 2) != mem->txt->raddr && i!=4) {
      int is_short = is_16bits(iterator);
      int32_t bin;
      if (is_short) {
        memcpy(&bin, header, 2);
      }

      else {
        memcpy(&bin, header, 4);
      }

      if (search_instruction(bin, dictionary, temp, is_16bits(iterator))) {
        i++;
        if (temp->sig == 0xBF00) {
	  int firstcond = (0x0010 & bin) >> 4;
	  int maskit = bin & 0x000F;
	  int j = 0;
          if ((maskit & 1) == 1) j = 4;	        
          else if ((maskit & (1 << 1)) == 1) j = 3;      
          else if ((maskit & (1 << 2)) == 1) j = 2;
          else if ((maskit & (1 << 3)) == 1) j = 1;
	  
	  if (j >= i) {
            if (((maskit & (1 << (5 - i))) == (firstcond & 1)) || j == i)
	      it_condition(firstcond, condition);
	    else {
	      if (firstcond == 14) it_condition(firstcond, condition);
	      else if (firstcond % 2 == 0) it_condition(firstcond + 1, condition);
	      else if (firstcond % 2 == 1) it_condition(firstcond - 1, condition);
            }
          }     
        }
      }
    }
    
    
    if (condition[0] == '\0') printf("%sS ", instruction->mnemo);
    else printf("%s%s", instruction->mnemo, condition);
    char* regs = instruction->registers_index;
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
      } while ((token != NULL));
    }
  
    char* imms = instruction->immediate_index;
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
      } while ((token != NULL));
    }
 
  }

  else {
    printf("%sS ", instruction->mnemo);
    char* regs = instruction->registers_index;
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
      } while ((token != NULL));
    }
  
    char* imms = instruction->immediate_index;
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
      } while ((token != NULL));
    }
 
  }

  return 0; 
}

int read_instruction(byte* header, dico* dictionary, memory mem)
{
  int is_short = is_16bits(header+1);
  int i;
  if (is_short) {
    uint16_t binary;
    memcpy(&binary, header, 2);
    dico *instruction;
    search_instruction(binary, dictionary, instruction, is_short);
    print_instruction(binary, instruction, header, dictionary, mem);
  
    return 0; 
  }

  else {
    word binary = 0;
    memcpy(&binary, header, 2);
    binary = binary << 16;
    word temp = 0;
    memcpy(&temp, header+2, 2);
    binary = binary + temp;
    dico *instruction;
    instruction = malloc(sizeof(*instruction));
    search_instruction(binary, dictionary, instruction, is_short);
    print_instruction(binary, instruction, header, dictionary, mem);
  
    return 0; 
  }
}


int disasm(size_t startadress, size_t endadress, dico* dico, memory mem)
{
  size_t nb_bytes = endadress - startadress;
  byte* header;
  header =  mem->txt->raddr + startadress - mem->txt->vaddr;
  int i = 0;
  while(i <= nb_bytes) {
    printf("%lu :: ", startadress + i);
    if (is_16bits(header)) {
      read_instruction(header, dico, mem);
      i = i + 2;
      header = header + 2;
    }
    else {
      read_instruction(header, dico, mem);
      i = i + 4;
      header = header + 4;     
    }
  }
  return 0;
}

void extract_dico(char* dico_file, dico* dico) {
  int i;
  FILE* pf_dico;
  char token[16];
  if ((pf_dico = fopen(dico_file, "r")) == NULL)
    ERROR_MSG("Cannot open file %s", dico_file);
  for (i=0;i<9;i++) {
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
  }
  return;
}
