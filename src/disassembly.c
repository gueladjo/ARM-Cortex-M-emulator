/**
 * @file disassembly.c
 * @author Mickael Albisser 
 * @brief Disassembly functions
 */

#include "disassembly.h"
#include "notify.h"

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
