/**
 * @file memory.c
 * @author Mickael Albisser
 * @brief Memory + registers management functions
 */

#include "memory.h"

memory stack_set(memory mem)
{
  mem->stack->vaddr = 0xff7ff000;
  mem->stack->size = 0x00800000; //Stack size
  mem->stack->raddr = calloc(mem->stack->size, sizeof(byte));
  mem->reg[13] = 0xfffff000; //sp points the first address of the stack
  return mem;
}


void memory_allocation(memory mem, char* section_name, byte* segdata, size_t size, size_t adress)
{
  if (size == 0) return;
  if (!strcmp(section_name,".rodata")) {
    WARNING_MSG("RODATA\n");
    mem->rodata->raddr = calloc(size, sizeof(byte));
    mem->rodata->vaddr = adress;
    mem->rodata->size = size;
    memcpy(mem->rodata->raddr, segdata, size);
    return;
  }
  if (!strcmp(section_name, ".text")) {
    mem->txt->raddr = calloc(size, sizeof(byte));
    mem->txt->vaddr = adress; 
    mem->txt->size = size;
    memcpy(mem->txt->raddr, segdata, size);
    return;
  }
  if (!strcmp(section_name,".data")) {
    mem->data->raddr = calloc(size, sizeof(byte));
    mem->data->vaddr = adress;
    mem->data->size = size;
    memcpy(mem->data->raddr, segdata, size);
    return;
  }
  if (!strcmp(section_name, ".bss")) {
    mem->bss->raddr = calloc(size, sizeof(byte));
    mem->bss->vaddr = adress;
    mem->bss->size = size;
    memcpy(mem->bss->raddr, segdata, size);
    return;
  }
  return;
}


byte read_memory_value(size_t addr, memory mem)
{
  //For each segment, checks if the address is there, and returns the value stored in the cell
  byte value = NULL; 
  size_t offset;
  if (mem->rodata!=NULL && addr >= mem->rodata->vaddr && addr < mem->rodata->vaddr + mem->rodata->size) {
    offset = addr - mem->rodata->vaddr;
    value = *(mem->rodata->raddr + offset);
  }
  else if (mem->txt!=NULL && addr >= mem->txt->vaddr && addr < mem->txt->vaddr + mem->txt->size) {
    offset = addr - mem->txt->vaddr;
    value = *(mem->txt->raddr + offset);
  }
  else if (mem->data!=NULL && addr >= mem->data->vaddr && addr < mem->data->vaddr + mem->data->size) {
    offset = addr - mem->data->vaddr;
    value = *(mem->data->raddr + offset);
 }
  else if (mem->bss!=NULL && addr >= mem->bss->vaddr && addr < mem->bss->vaddr + mem->bss->size) {
    offset = addr - mem->bss->vaddr;
    value = *(mem->bss->raddr + offset);
  }
  else if (mem->stack!=NULL && addr >= mem->stack->vaddr && addr < mem->stack->vaddr + mem->stack->size) {
    offset = addr - mem->stack->vaddr;
    value = *(mem->stack->raddr + offset);
  }
  return value;
}


int write_memory_value(size_t addr, byte value, memory mem)
{
  //For each segment, checks if the address is there, and changes the value stored in the cell
  int e = 1; //Error value
  size_t offset;
  if (mem->rodata!=NULL && addr >= mem->rodata->vaddr && addr < mem->rodata->vaddr + mem->rodata->size) {
    offset = addr - mem->rodata->vaddr;
    *(mem->rodata->raddr + offset) = value;
    e = 0;
  }
  else if (mem->txt!=NULL && addr >= mem->txt->vaddr && addr < mem->txt->vaddr + mem->txt->size) {
    offset = addr - mem->txt->vaddr;
    *(mem->txt->raddr + offset) = value;
    e = 0;
  }
  else if (mem->data!=NULL && addr >= mem->data->vaddr && addr < mem->data->vaddr + mem->data->size) {
    offset = addr - mem->data->vaddr;
    *(mem->data->raddr + offset) = value;
    e = 0;
 }
  else if (mem->bss!=NULL && addr >= mem->bss->vaddr && addr < mem->bss->vaddr + mem->bss->size) {
    offset = addr - mem->bss->vaddr;
    *(mem->bss->raddr + offset) = value;
    e = 0;
  }
  else if (mem->stack!=NULL && addr >= mem->stack->vaddr && addr < mem->stack->vaddr + mem->stack->size) {
    offset = addr - mem->stack->vaddr;
    *(mem->stack->raddr + offset) = value;
    e = 0;
  }
  return e;
}


int memory_free(memory mem)
{
  int e = 1; //Error value
  if (mem->rodata != NULL && mem->rodata->size != 0) {
    free(mem->rodata->raddr);
    free(mem->rodata);
    e = 0;
  }
  if (mem->txt != NULL && mem->txt->size != 0) {
    free(mem->txt->raddr);
    free(mem->txt);
    e = 0;
  }
  if (mem->data != NULL && mem->data->size != 0) {
    free(mem->data->raddr);
    free(mem->data);
    e = 0;
  }
  if (mem->bss != NULL && mem->bss->size != 0) {
    free(mem->bss->raddr);
    free(mem->bss);
    e = 0;
  }
  if (mem->stack != NULL && mem->stack->size != 0) {
    free(mem->stack->raddr);
    free(mem->stack);
    e = 0;
  }
  if (mem->break_list != NULL) {
    free(mem->break_list);
    e = 0;
  }
  return e;
}

void print_section_raw_content(char* name, unsigned int start, byte* content, unsigned int taille) {
  int k;
  unsigned char octet =0;
  printf("\n section %s loaded at %x :\n",name,start); 
  if (content!=NULL && taille>0) {
    for(k=0; k<taille; k+=1) {
      // on affiche le contenu de la section qui devrait se retrouver 
      // en "memoire virtuelle" à l'adresse virtuelle start+k 
      // (*(content + k) dans la mémoire physique)
      if(k%16==0) printf("\n  0x%08x ", start + k);
      octet = *((unsigned char *) (content+k));
      printf("%02x ",	octet);
    }
  }
  printf("\n");
}

int read_word(size_t adress, memory mem)
{
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

  return value;
}

int write_word(size_t adress, int value, memory mem)
{
  byte bValue[4];
  bValue[0] = (byte)((value & 0xff000000) >> 24);
  bValue[1] = (byte)((value & 0x00ff0000) >> 16);
  bValue[2] = (byte)((value & 0x0000ff00) >> 8);
  bValue[3] = (byte)((value & 0x000000ff));

  int i =0;
  if (mem->endianness == LSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[i], mem)) {
        WARNING_MSG("address is not valid %s\n", "write_word");
        return 1;
      }
    }
  }
  else if (mem->endianness == MSB) {
    for (i=0;i<=3;i++) {
      if (write_memory_value(adress+i, bValue[3-i], mem)) {
        WARNING_MSG("address is not valid %s\n", "write_word");
        return 1;
      }
    }
  }
  return 0;
}

int load (int no_args, char *elf_file, size_t start_mem, memory memory) {
  WARNING_MSG("\n LOAD");
  char* section_names[NB_SECTIONS]= {TEXT_SECTION_STR,RODATA_SECTION_STR,DATA_SECTION_STR,BSS_SECTION_STR};
  scntab section_table;
  unsigned int nsegments;
  int i=0;
  unsigned int type_machine;
  unsigned int endianness;   //little ou big endian
  unsigned int bus_width;    // 32 bits ou 64b&its
  unsigned int next_segment_start = DEFAULT_START; // compteur pour designer le début de la prochaine section
  stab symtab= new_stab(0); // table des symboles
  FILE * pf_elf;
  
  if ((no_args < 1) || (no_args > 2)) {
    printf("Usage: load <fichier elf> {<addresse>}\n");
    return 1;
  }
  
  if (no_args == 2) { //Si qu'un seul argument, début de memoire par défaut
    start_mem += 0xfff;
    start_mem -= start_mem%0x1000;
    next_segment_start = start_mem;
  }
  
  if ((pf_elf = fopen(elf_file,"r")) == NULL) {
    ERROR_MSG("cannot open file %s", elf_file);
  }
  if (!assert_elf_file(pf_elf))
    ERROR_MSG("file %s is not an ELF file", elf_file);
  
  // recuperation des info de l'architecture
  elf_get_arch_info(pf_elf, &type_machine, &endianness, &bus_width);
  elf_load_scntab(pf_elf, bus_width, &section_table);
  // et des symboles
  elf_load_symtab(pf_elf, bus_width, endianness, &symtab);

  nsegments=0;
  
  byte *ehdr    = __elf_get_ehdr(pf_elf );    
  for (i=0; i<NB_SECTIONS; i++) {
    printf("\n***** Processing section named %s\n", section_names[i]); 
    uint32_t taille;
    byte* content = elf_extract_scn_by_name(ehdr, pf_elf, section_names[i], &taille, NULL );
    if (content!=NULL){
      print_section_raw_content(section_names[i],next_segment_start,content,taille);
      nsegments++;
      //Copie le contenu dans la mémoire
      
      memory_allocation(memory, section_names[i], content, taille, next_segment_start);
      next_segment_start += ((taille+0x1000)>>12 )<<12; // on arrondit au 1k suppérieur
      free(content);
    }
    else{
      DEBUG_MSG("section %s not present in the elf file",section_names[i]);
    }
  }
  rewind(pf_elf);

  // Relocation
  reloc_segment(pf_elf, *(memory->data), ".data", memory, endianness, &symtab);  
  reloc_segment(pf_elf, *(memory->txt), ".text", memory, endianness, &symtab);   
  reloc_segment(pf_elf, *(memory->rodata), ".rodata", memory, endianness, &symtab);   
  reloc_segment(pf_elf, *(memory->bss), ".bss", memory, endianness, &symtab);   

  free(ehdr);
  //Alloue le stack
  stack_set(memory);
  //Alloue la liste des breakpoints
  memory->break_list = calloc(memory->txt->size, sizeof(int));
  printf("\n------ Fichier ELF \"%s\" : sections lues lors du chargement ------\n", elf_file) ;
  stab32_print( symtab );
  memory->endianness = endianness;
  // on fait le ménage avant de partir
  del_stab( symtab );
  del_scntab( section_table );
  fclose(pf_elf);
  puts("");
  return 0;
}
