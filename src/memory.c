/**
 * @file memory.c
 * @author Mickael Albisser
 * @brief Memory + registers management functions
 */

#include "memory.h"


memory stack_set(word* reg, memory mem)
{
  mem.stack = malloc(sizeof(segment));
  mem.stack->vaddr = 0xff7ff000;
  mem.stack->size = 0x00800000; //Stack size
  mem.stack->raddr = calloc(mem.stack->size, sizeof(byte));
  reg[13] = 0xfffff000; //sp points the first address of the stack
  return mem;
}


memory memory_allocation(memory mem, size_t rodata_size, size_t txt_size, size_t data_size, size_t bss_size)
{
  mem.rodata = malloc(sizeof(segment));
  mem.txt = malloc(sizeof(segment));
  mem.data = malloc(sizeof(segment));
  mem.bss = malloc(sizeof(segment));
  if (rodata_size != 0) {
    mem.rodata->raddr = calloc(rodata_size, sizeof(byte));
    mem.rodata->vaddr = 0x1000;
    mem.rodata->size = rodata_size;
  }
  if (txt_size !=0) {
    mem.txt->raddr = calloc(txt_size, sizeof(byte));
    mem.txt->vaddr = (mem.rodata->vaddr + mem.rodata->size + (0x1000 - mem.rodata->size % 0x1000)); //Calculating the best emplacement for segment based on the precedent one
    mem.txt->size = txt_size;
  }
  if (data_size !=0) {
    mem.data->raddr = calloc(data_size, sizeof(byte));
    mem.data->vaddr = (mem.txt->vaddr + mem.txt->size + (0x1000 - mem.txt->size % 0x1000));
    mem.data->size = data_size;
  }
  if (bss_size !=0) {
    mem.bss->raddr = calloc(bss_size, sizeof(byte));
    mem.bss->vaddr = (mem.data->vaddr + mem.data->size + (0x1000 - mem.data->size % 0x1000));
    mem.bss->size = bss_size;
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
