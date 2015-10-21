#include "interpreter.h"
#include "memory.h"
#include <string.h>
#include "parsing.h"

int loadcmd(interpreteur inter, memory mem) {
  int no_args;
  char* token;
  char* elf_file;
  size_t addr;
  
  if ((token = get_next_token(inter)) == NULL) {
    WARNING_MSG("Usage : load <fichier elf> {<adresse>}\n");
    return 1;
  }
  else {
    elf_file = token;
    no_args = 1;
  }

  token = get_next_token(inter);
  if(token!=NULL && !is_hexa(token)) {
    WARNING_MSG("Attention : adresse de depart non hexadecimale. Utilisation de la valeur par défaut.\n");
  }
  else if(token!=NULL && is_hexa(token)) {
    sscanf(token, "%zx", &addr);
    no_args = 2;
  }
  load(no_args, elf_file, addr, mem);
  return 0;
}
 
int debugcmd(interpreteur inter)
{
  DEBUG_MSG("Chaine : %s", inter->input);
  char* token;
  if ((token = get_next_token(inter)) != NULL) {
    WARNING_MSG("debug command doesn't take any argument %s\n", "debugcmd");
    return 1;
  }
    
  if (inter->mode == DEBUG_MODE) {
    WARNING_MSG("Interpreter is already in DEBUG MODE %s\n", "debugcmd");
    return 1;
  }

  if (inter->mode == INTERACTIF) {
    WARNING_MSG("Interpreter is already in INTERACTIVE MODE%s\n", "debugcmd");
    return 1;
  }

  if (inter->mode == SCRIPT) {
    inter->mode = DEBUG_MODE;
    return 0;
  }

  return 1;
}

int resumecmd(interpreteur inter) 
{
  DEBUG_MSG("Chaine : %s", inter->input);
  char* token;
  if ((token = get_next_token(inter)) != NULL) {
    WARNING_MSG("resume command doesn't take any argument %s\n", "resumecmd");
    return 1;
  }
  if (inter->mode == DEBUG_MODE) {
    inter->mode == SCRIPT;
    return 0;
  }

  if (inter->mode == INTERACTIF) {
    WARNING_MSG("Interpreter is in INTERACTIVE MODE %s\n", "resumecmd");
    return 1;
  }

  if (inter->mode == SCRIPT) {
    WARNING_MSG("Interpreter is in SCRIPT MODe %s\n", "resumecmd");
    return 1;
  }

  return 1;     
}

int dispcmd(interpreteur inter, memory mem)
{
  DEBUG_MSG("Chaine : %s", inter->input);
    
  char* token = NULL;

  if ((token = get_next_token(inter)) == NULL) {
    WARNING_MSG("no argument given to command %s\n", "dispcmd");
    return 1;
  }

  if (strcmp(token, "mem") == 0) {
    token = get_next_token(inter);
    if (token  == NULL) {
      WARNING_MSG("not enough arguments given to command %s\n", "dispcmd");
      return 1;
    }
    while (token != NULL) {
      if (strcmp(token, "map") == 0) {
	if (get_next_token(inter) != NULL) {
	  WARNING_MSG("too many arguments %s\n", "dispcmd");
	  return 1;
	}
	printf("Virtual memory map (8 segments)\n");
	printf(".text  \tr-x\tVaddr: %zx\tSize: %zd bytes\n", mem->txt->vaddr, mem->txt->size);    
	printf(".data  \trw-\tVaddr: %zx\tSize: %zd bytes\n", mem->data->vaddr, mem->data->size);    
	printf(".bss   \trw-\tVaddr: %zx\tSize: %zd bytes\n", mem->bss->vaddr, mem->bss->size);    
	printf("[stack]\trw-\tVaddr: %zx\tSize: %zd bytes\n", mem->stack->vaddr, mem->stack->size);    
	return 0;
      }
      char* tok_mem1 = token;
      char* tok_mem2 = get_next_token(inter);
      tok_mem2 = get_next_token(inter);
      if (is_adress(tok_mem1)) {
	if (tok_mem2 == NULL) {
	  WARNING_MSG("second argument is not an adress range %s\n", "dispcmd");
	  return 1;
	} 
	if (is_adress(tok_mem2)) {
	  size_t adress1 = strtol(tok_mem1,NULL,16);
	  size_t adress2 = strtol(tok_mem2,NULL,16);
	  if ((adress2 - adress1) > 17) {
	    WARNING_MSG("adress range must not contain more than 16 adresses %s\n","dispcmd");
	    return 1;
	  } 
	  int j = 0;
	  byte value;
	  value = read_memory_value(adress1, mem);
	  printf("%zx\t%x  ", adress1, value);
	  for(j = 1; adress1 + j <= adress2; j++) {
	    value = read_memory_value(adress1 + j, mem);
	    printf("%x  ", value);
	  }
	}
	else {
	  WARNING_MSG("second argument is not an adress range %s\n","dispcmd");
	  return 1;
	}
      }
      printf("\n");
      if((token = get_next_token(inter) == NULL)) return 0;
    }


	
    WARNING_MSG("first argument not a valid memory range or command%s\n", "dispcmd");
    return 1;
  }

  if (strcmp(token, "reg") == 0) {
    if ((token = get_next_token(inter)) == NULL) {
      WARNING_MSG("not enough arguments given to command %s\n", "dispcmd");
      return 1;
    }

    int i = 0;
    while (token != NULL && is_register(token)) {
      if (i == 4) printf("\n");
      int ind = reg_index(token);
      printf("%s: %x\t", token, mem->reg[reg_index(token)]);
      i++; 
      char* token = get_next_token(inter);
    }
	
    if (token == NULL) return 0;
	        
    else {
      WARNING_MSG("argument not a valid register%s\n", "dispcmd");
      return 1;
    }
  }

  else {
    WARNING_MSG("value %s is not a valid argument of command %s\n", token, "dispcmd");
    return 1;
  }

  return 1;
}

int setcmd(interpreteur inter, memory mem)
{
  DEBUG_MSG("Chaine : %s", inter->input);
    
  char* token = NULL;

  if ((token = get_next_token(inter)) == NULL) {
    WARNING_MSG("no argument given to command %s\n", "setcmd");
    return 1;
  }

  if (strcmp(token, "mem") == 0) {
    if ((token = get_next_token(inter)) == NULL) {
      WARNING_MSG("not enough arguments given to command %s\n", "setcmd");
      return 1;
    }
    //finish this
  }

  if (strcmp(token, "reg") == 0) {
    if ((token = get_next_token(inter)) == NULL) {
      WARNING_MSG("not enough arguments given to command %s\n", "setcmd");
      return 1;
    }

    if (is_register(token)) {
      char* value;
      if ((value = get_next_token(inter)) != NULL) {
	if (get_type(value) == INT || get_type(value) == HEXA)
	  mem->reg[reg_index(token)] = atoi(value);
	else {
	  WARNING_MSG("value is not an integer %s\n", "setcmd");
	  return 1;
	}
	return 0;
      }
      else {
	WARNING_MSG("not enough arguments given to command %s\n", "setcmd");
	return 1;	 
      }
    }
		
    else {
      WARNING_MSG("argument not a valid register%s\n", "setcmd");
      return 1;
    }
  }

  else {
    WARNING_MSG("value %s is not a valid argument of command %s\n", token, "setcmd");
    return 1;
  }
  return 1;
}

int assertcmd(interpreteur inter, memory mem) 
{
  DEBUG_MSG("Chaine : %s", inter->input);
    
  char* token = NULL;

  if ((token = get_next_token(inter)) == NULL) {
    WARNING_MSG("no argument given to command %s\n", "assertcmd");
    return 1;
  }

  if (strcmp(token, "reg") == 0) {
    if ((token = get_next_token(inter)) == NULL) {
      WARNING_MSG("not enough arguments given to command %s\n", "assertcmd");
      return 1;
    }
    if (is_register(token)) {
      char*  token1 = get_next_token(inter);
      if (get_type(token1) == INT || get_type(token1) == HEXA) {
	if (atoi(token1) == mem->reg[reg_index(token)])
	  return 0;
	return 1; 
      }
      else {
	WARNING_MSG("second argument is not an integer %s\n", "assertcmd");
	return 1;
      } 
    }
    else {
      WARNING_MSG("first argument not a valid register%s\n", "assertcmd");
      return 1;
    }
        

  }

  if (strcmp(token, "word") == 0) {
    if ((token = get_next_token(inter)) == NULL) {
      WARNING_MSG("not enough arguments given to command %s\n", "assertcmd");
      return 1;
    }
    if (is_adress(token)) {
      char * token1 = get_next_token(inter);
      if (get_type(token1) == INT || get_type(token1) == HEXA) {
	if (read_word(atoi(token), mem) == atoi(token1))
	  return 0; 
	return 1;
      }
      else{
	WARNING_MSG("second argument is not an integer %s\n", "assertcmd");
	return 1;
      } 
    }
    else {
      WARNING_MSG("first argument not a valid adress %s\n","assertcmd");
      return 1;
    }
  }

  if (strcmp(token, "byte") == 0) {
    if ((token = get_next_token(inter)) == NULL) {
      WARNING_MSG("not enough arguments given to command %s\n", "assertcmd");
      return 1;
    }
    if (is_adress(token)) {
      char * token1 = get_next_token(inter);
      if (get_type(token1) == INT || get_type(token1) == HEXA) {
	if (read_memory_value(atoi(token), mem) == atoi(token1)) return 0; 
	return 1;
      }
      else{
	WARNING_MSG("second argument is not an integer %s\n", "assertcmd");
	return 1;
      }
    }
    else {
      WARNING_MSG("first argument not a valid adress %s\n","assertcmd");
      return 1;
    }           
  }

  else {
    WARNING_MSG("value %s is not a valid argument of command %s\n", token, "assertcmd");
    return 1;
  }

  return 1;
}
