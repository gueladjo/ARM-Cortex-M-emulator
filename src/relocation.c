/**
 * @file memory.h
 * @author Moctar Ba
 * @brief Relocation
 */

#include "memory.h"

/*--------------------------------------------------------------------------  */
/**
 * @param fp le fichier elf original
 * @param seg le segment à reloger
 * @param mem l'ensemble des segments
 * @param endianness le boutisme du programme
 * @param symtab la table des symbole du programme
 *
 * @brief Cette fonction effectue la relocation du segment passé en parametres
 * @brief l'ensemble des segments doit déjà avoir été chargé en memoire.
 *
 */
void reloc_segment(FILE* fp, segment seg, char* segname, memory mem, unsigned int endianness,stab* symtab) {
  byte *ehdr    = __elf_get_ehdr(fp);
  uint32_t  scnsz  = 0;
  Elf32_Rel *rel = NULL;
  char* reloc_name = malloc(strlen(segname)+strlen(RELOC_PREFIX_STR)+1);
  scntab section_tab;

  // on recompose le nom de la section
  memcpy(reloc_name,RELOC_PREFIX_STR,strlen(RELOC_PREFIX_STR)+1);
  strcat(reloc_name,segname);

  // on récupere le tableau de relocation et la table des sections
  rel = (Elf32_Rel *)elf_extract_scn_by_name( ehdr, fp, reloc_name, &scnsz, NULL );
  elf_load_scntab(fp ,32, &section_tab);

  if (rel != NULL && seg.raddr!=NULL && seg.size !=0) {

      INFO_MSG("--------------Relocation de %s-------------------\n",segname) ;
      INFO_MSG("Nombre de symboles a reloger: %ld\n",scnsz/sizeof(*rel)) ;

      int i = 0, j = 0;
      unsigned int V = 0;
      unsigned int S = 0;
      unsigned int A = 0;
      unsigned int T = 0;
      
      for (i=0; i<scnsz/sizeof(*rel); i++) {
          T =0;
          __Elf_Rel_flip_endianness((byte *)(rel+i), 32, endianness);
          unsigned int offset = rel[i].r_offset; // decalage par rapport au debut de section
          int sym = (int) ELF32_R_SYM(rel[i].r_info);
          int type = (unsigned char) ELF32_R_TYPE(rel[i].r_info);
	  int P = seg.vaddr + offset;

          DEBUG_MSG("Entrée de relocation : offset %08x info =%08x sym = %08x type =%08x\n",rel[i].r_offset,rel[i].r_info,sym,type);
          DEBUG_MSG("Symbole en fonction duquel on reloge : symbole %s de type %d\n",symtab->sym[sym].name, type);

          if (symtab->sym[sym].type==STT_SECTION) {
            S = symtab->sym[sym].addr._32;
 
	    if (type == R_ARM_ABS8) {
              uint8_t oct = read_memory_value(P, mem);
              if (oct < 0)
                A = oct | 0xFFFFFF00;
              else
                A = oct;
              V = A + S;
              write_memory_value(P, V, mem);  
              printf("A: %d, P: %d, S: %d, T: %d, V:%x\n", A, P, S, T, V);
            }
            
            if (type == R_ARM_ABS32) {
              A = read_word(P, mem) ;
              V = (S + A) | T;
              printf("A: %d, P: %d, S: %d, T: %d, V:%x\n", A, P, S, T, V);
              write_word(P, V, mem);
            }

            if (type == R_ARM_THM_CALL) {
            }
                         
          } 
          
          else if (symtab->sym[sym].type==STT_FUNC) {
          
            T = 1;
            S = symtab->sym[sym].addr._32;   

	    if (type == R_ARM_ABS8) {
              A = read_memory_value(P, mem) ;
              V = A + S;
              printf("A: %d, P: %d, S: %d, T: %d, V:%x\n", A, P, S, T, V);
              write_memory_value(P, V, mem);  
            }
            
            if (type == R_ARM_ABS32) {
              A = read_word(P, mem) << 1;
              V = (S + A) | T;
              printf("A: %d, P: %d, S: %d, T: %d, V:%x\n", A, P, S, T, V);
              write_word(P, V, mem);
            }

            if (type == R_ARM_THM_CALL) {
            }               
          } 

          else if (symtab->sym[sym].type==STT_NOTYPE) {
          
            S = symtab->sym[sym].addr._32;   

	    if (type == R_ARM_ABS8) {
              A = read_memory_value(P, mem);
              V = A + S;
              printf("A: %d, P: %d, S: %d, T: %d, V:%x\n", A, P, S, T, V);
              write_memory_value(P, V, mem);  
            }
            
            if (type == R_ARM_ABS32) {
              A = read_word(P, mem);
              V = (S + A) | T;
              printf("A: %d, P: %d, S: %d, T: %d, V:%x\n", A, P, S, T, V);
              write_word(P, V, mem);
            }

            if (type == R_ARM_THM_CALL) {
            }               
          }  
       
          else {
              WARNING_MSG("%d non traité pour l'instant",symtab->sym[sym].type) ;
              continue;
          }
      }
  }
  del_scntab(section_tab);
  free( rel );
  free( reloc_name );
  free( ehdr );
}


