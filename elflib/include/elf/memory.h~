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

typedef uint8_t byte;
typedef uint32_t word;
typedef struct segment segment;
typedef struct memory memory;

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
};

/**
 * Sets the stack
 * @reg the array representing the registers to initiate the stack pointer
 * @mem the structure representing the memory where the stack will be
 * @return the memory
 */
memory stack_set (word* reg, memory mem);

/**
 * Allocates the memory for the different segments,
 * knowing their sizes (in bytes)
 * @rodata_size size of segment rodata
 * @txt_size size of segment txt
 * @data_size size of segment data
 * @bss_size size of segment bss
 * @return the memory structure
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

#endif
