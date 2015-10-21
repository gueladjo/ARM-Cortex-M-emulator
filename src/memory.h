/**
 * @file memory.h
 * @author Mickael Albisser
 * @brief Memory + registers management functions - header
 */
#ifndef _MEMORY
#define _MEMORY

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include "bits.h"
#include "notify.h"
#include "elf.h"
#include "syms.h"
#include "relocator.h"
#include "memory.h"

#define NB_SECTIONS 4 //Nombre max de sections

#define DEFAULT_START 0x3000 //Valeur par défaut du début de mem
#define TEXT_SECTION_STR ".text" //Nom des sections
#define RODATA_SECTION_STR ".rodata"
#define DATA_SECTION_STR ".data"
#define BSS_SECTION_STR ".bss"


typedef uint8_t byte;
typedef uint32_t word;
typedef struct segment segment;
typedef struct memory* memory;

struct segment
{
  byte* raddr; //Real address, beginning of segment
  size_t size; //Segment size
  size_t vaddr; //Virtual address, beginning of segment
};

struct memory
{
  segment* stack;
  segment* bss;
  segment* data;
  segment* txt;
  segment* rodata;
  word reg[16];
};



/**
 * Fonction qui affiche les segments d'un octet sur la sortie standard
 * @name Nom de la section affichée
 * @start Adresse virtuelle du segment
 * @content Le contenu de la section
 * @taille Taille de la section
 * @return Rien
 */
void print_section_raw_content(char* name, unsigned int start, byte* content, unsigned int taille);

/**
 * Charge un fichier elf en mémoire, avec éventuellement
 * une adresse de début spécifiée
 * @no_args Nombre d'arguments
 * @elf_file Nom du fichier à charger
 * @start_mem L'adresse de début (si applicable)
 * @memory La mémoire
 * @return Une valeur permettant de tester l'erreur
 */

int load (int no_args, char *elf_file, size_t start_mem, memory memory);

/**
 * Sets the stack
 * @reg the array representing the registers to initiate the stack pointer
 * @mem the structure representing the memory where the stack will be
 * @return nothing
 */
memory stack_set (memory mem);

/**
 * Allocates the memory for the different segments,
 * knowing their sizes (in bytes)
 * @rodata_size size of segment rodata
 * @txt_size size of segment txt
 * @data_size size of segment data
 * @bss_size size of segment bss
 * @return nothing
 */
memory memory_allocation(memory mem, char* section_name, byte* segdata, size_t size, size_t adress);

/**
 * Returns value stored at the address addr
 * Looks in each segment if the address is there,
 * and if case, returns the value. Returns NULL if
 * address does not exist
 * @addr the searched address
 * @mem the structure containing the memory
 * @return the value stored
 */
byte read_memory_value(size_t addr, memory mem);

/**
 * Change the value stored at the address addr
 * It's the exact reverse operation as before
 * it works almost the same way. Returns 0 if
 * the address exist and could be changed, 
 * 1 in the other case
 * @addr the searched address
 * @value the value to replace the precedent one
 * @mem the memory
 * @return an int allowing to check errors
 */
int write_memory_value(size_t addr, byte value, memory mem);

/**
 * Frees the memory. Returns 0 in case of success,
 * 1 if nothing has been freed
 * @mem the memory
 * @return an int to check errors
 */
int memory_free(memory mem);
int read_word(size_t adress, memory mem);
#endif
