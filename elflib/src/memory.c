/**
 * @file memory.c
 * @author Mickael Albisser
 * @brief Memory + registers management functions
 */

#include "elf/memory.h"


memory stack_set(word* reg, memory mem)
{
  mem.stack = malloc(sizeof(segment));
  mem.stack->vaddr = 0xff7ff000;
  mem.stack->size = 0x00800000; //Stack size
  mem.stack->raddr = calloc(mem.stack->size, sizeof(byte));
  reg[13] = 0xfffff000; //sp points the first address of the stack
  return mem;
}


memory memory_allocation(memory mem, char* section_name, byte* segdata, size_t size, size_t adress)
{
  if (size == 0) return mem;

  if (strcmp(section_name,".rodata") == 0) {
    mem.rodata = malloc(sizeof(segment));
    mem.rodata->raddr = calloc(size, sizeof(byte));
    mem.rodata->vaddr = adress;
    mem.rodata->size = size;
    mem.rodata->raddr = memcpy(mem.rodata->raddr, segdata, size);
    return mem;
  }
  if (strcmp(section_name, ".text") == 0) {
    mem.txt = malloc(sizeof(segment));
    mem.txt->raddr = calloc(size, sizeof(byte));
    mem.txt->vaddr = adress; 
    mem.txt->size = size;
    mem.txt->raddr = memcpy(mem.txt->raddr, segdata, size);
    return mem;
  }
  if (strcmp(section_name,".data") == 0) {
    mem.data = malloc(sizeof(segment));
    mem.data->raddr = calloc(size, sizeof(byte));
    mem.data->vaddr = adress;
    mem.data->size = size;
    mem.data->raddr = memcpy(mem.data->raddr, segdata, size);
    return mem;
  }
  if (strcmp(section_name, ".bss") == 0) {
    mem.bss = malloc(sizeof(segment));
    mem.bss->raddr = calloc(size, sizeof(byte));
    mem.bss->vaddr = adress;
    mem.bss->size = size;
    mem.bss->raddr = memcpy(mem.bss->raddr, segdata, size);
    return mem;
  }
  return mem;
}


byte read_memory_value(size_t addr, memory mem)
{
  //For each segment, checks if the address is there, and returns the value stored in the cell
  byte value = NULL; 
  size_t offset;
  if (mem.rodata!=NULL && addr > mem.rodata->vaddr && addr < mem.rodata->vaddr + mem.rodata->size) {
    offset = addr - mem.rodata->vaddr;
    value = *(mem.rodata->raddr + offset);
  }
  else if (mem.txt!=NULL && addr > mem.txt->vaddr && addr < mem.txt->vaddr + mem.txt->size) {
    offset = addr - mem.txt->vaddr;
    value = *(mem.txt->raddr + offset);
  }
  else if (mem.data!=NULL && addr > mem.data->vaddr && addr < mem.data->vaddr + mem.data->size) {
    offset = addr - mem.data->vaddr;
    value = *(mem.data->raddr + offset);
 }
  else if (mem.bss!=NULL && addr > mem.bss->vaddr && addr < mem.bss->vaddr + mem.bss->size) {
    offset = addr - mem.bss->vaddr;
    value = *(mem.bss->raddr + offset);
  }
  else if (mem.stack!=NULL && addr > mem.stack->vaddr && addr < mem.stack->vaddr + mem.stack->size) {
    offset = addr - mem.stack->vaddr;
    value = *(mem.stack->raddr + offset);
  }
  return value;
}


int write_memory_value(size_t addr, byte value, memory mem)
{
  //For each segment, checks if the address is there, and changes the value stored in the cell
  int e = 1; //Error value
  size_t offset;
  if (mem.rodata!=NULL && addr > mem.rodata->vaddr && addr < mem.rodata->vaddr + mem.rodata->size) {
    offset = addr - mem.rodata->vaddr;
    *(mem.rodata->raddr + offset) = value;
    e = 0;
  }
  else if (mem.txt!=NULL && addr > mem.txt->vaddr && addr < mem.txt->vaddr + mem.txt->size) {
    offset = addr - mem.txt->vaddr;
    *(mem.txt->raddr + offset) = value;
    e = 0;
  }
  else if (mem.data!=NULL && addr > mem.data->vaddr && addr < mem.data->vaddr + mem.data->size) {
    offset = addr - mem.data->vaddr;
    *(mem.data->raddr + offset) = value;
    e = 0;
 }
  else if (mem.bss!=NULL && addr > mem.bss->vaddr && addr < mem.bss->vaddr + mem.bss->size) {
    offset = addr - mem.bss->vaddr;
    *(mem.bss->raddr + offset) = value;
    e = 0;
  }
  else if (mem.stack!=NULL && addr > mem.stack->vaddr && addr < mem.stack->vaddr + mem.stack->size) {
    offset = addr - mem.stack->vaddr;
    *(mem.stack->raddr + offset) = value;
    e = 0;
  }
  return e;
}


int memory_free(memory mem)
{
  int e = 1; //Error value
  if (mem.rodata->size != 0) {
    free(mem.rodata->raddr);
    free(mem.rodata);
    e = 0;
  }
  if (mem.txt->size != 0) {
    free(mem.txt->raddr);
    free(mem.txt);
    e = 0;
  }
  if (mem.data->size != 0) {
    free(mem.data->raddr);
    free(mem.data);
    e = 0;
  }
  if (mem.bss->size != 0) {
    free(mem.bss->raddr);
    free(mem.bss);
    e = 0;
  }
  if (mem.stack->size != 0) {
    free(mem.stack->raddr);
    free(mem.stack);
    e = 0;
  }
  return e;
}
