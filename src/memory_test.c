/**
 * @file memory_test.c 
 * @author Mickael Albisser 
 * @brief "memory" module test program
 */

#include "memory.h"

int main()
{
  //Test of stack
  size_t i;
  memory mem;
  word reg[17];
  byte x;
  int e;
  printf("Setting up stack...\n");
  mem = stack_set(reg, mem);
  printf("SP = %"PRIu32"\n", reg[13]);
  printf("Setting up memory...\n");
  mem = memory_allocation(mem, 0x12fc, 0x3500, 0xfc2, 0xff);
  printf("Displaying memory...\n");
  for (i=0;i<=0xfffff000;i++) {
    x = read_memory_value(i, mem);
    e = write_memory_value(i, 1, mem);
  }
  printf("Displaying and changing OK\n");
  printf("Freeing memory...\n");
  memory_free(mem);
  printf("Memory freed !\n");
  return 0;
}
