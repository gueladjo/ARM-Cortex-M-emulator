/*
 * \author (m) François Portet 2015 <francois.portet@imag.fr>
 * \brief test main for loading ELF file in memory.
 *--------------------------------------------------------------------------
 * Ce programme est un exemple de comment lire les sections et symboles 
 * d'un fichier ELF en utilisant l'API elf.
 *
 * ATTENTION, il s'agit juste d'un exemple qui doit être completer avec votre
 * definition de memoire virtuelle
 */


#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include "common/bits.h"
#include "common/notify.h"
#include "elf/elf.h"
#include "elf/syms.h"
#include "elf/relocator.h"
#include "elf/memory.h"

// On fixe ici une adresse basse dans la mémoire virtuelle. Le premier segment
// ira se loger à cette adresse.
#define START_MEM 0x3000

// nombre max de sections que l'on extraira du fichier ELF
#define NB_SECTIONS 4

// nom de chaque section
#define TEXT_SECTION_STR ".text"
#define RODATA_SECTION_STR ".rodata"
#define DATA_SECTION_STR ".data"
#define BSS_SECTION_STR ".bss"

// fonction affichant les octets d'un segment sur la sortie standard
// parametres :
//   name           : nom de la section dont le contenue est affichée
//   start          : adresse virtuelle d'implantation du contenu de la section dans la memoire 
//   content        : le contenu de la section à afficher
//   taille         : taille en octet de la section à afficher

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


// le main charge un fichier elf en entrée en utilisant
// les arguments du prototype de la fonction main (cf. fiches infos)
//
int main (int argc, char *argv[]) {


    char* section_names[NB_SECTIONS]= {TEXT_SECTION_STR,RODATA_SECTION_STR,DATA_SECTION_STR,BSS_SECTION_STR};
    scntab section_table;
    unsigned int nsegments;
    int i=0;
    unsigned int type_machine;
    unsigned int endianness;   //little ou big endian
    unsigned int bus_width;    // 32 bits ou 64b&its
    unsigned int next_segment_start = START_MEM; // compteur pour designer le début de la prochaine section

    //TODO declarer une memoire virtuelle, c'est elle qui contiendra toute les données du programme
    memory memory;
    memory.stack=NULL;
    memory.bss=NULL;
    memory.data=NULL;
    memory.txt=NULL;
    memory.rodata=NULL;
    word reg[16];
    stab symtab= new_stab(0); // table des symboles
    FILE * pf_elf;

    if ((argc < 2) || (argc > 2)) {
        printf("Usage: %s <fichier elf> \n", argv[0]);
        exit(1);
    }

    if ((pf_elf = fopen(argv[1],"r")) == NULL) {
        ERROR_MSG("cannot open file %s", argv[1]);
    }

    if (!assert_elf_file(pf_elf))
        ERROR_MSG("file %s is not an ELF file", argv[1]);


    // recuperation des info de l'architecture
    elf_get_arch_info(pf_elf, &type_machine, &endianness, &bus_width);
    elf_load_scntab(pf_elf, bus_width, &section_table);
    // et des symboles
    elf_load_symtab(pf_elf, bus_width, endianness, &symtab);

		 

		nsegments=0;
    next_segment_start = START_MEM;
    byte *ehdr    = __elf_get_ehdr(pf_elf );    
    for (i=0; i<NB_SECTIONS; i++) {
        printf("\n***** Processing section named %s\n", section_names[i]); 

        uint32_t taille;
        byte* content = elf_extract_scn_by_name(ehdr, pf_elf, section_names[i], &taille, NULL );

               
        
        if (content!=NULL){
            print_section_raw_content(section_names[i],next_segment_start,content,taille);
            next_segment_start+= ((taille+0x1000)>>12 )<<12; // on arrondit au 1k suppérieur
            nsegments++;
	    memory = memory_allocation(memory, section_names[i], content, taille, next_segment_start); 
            //TODO: copier le contenu dans la memoire avant de liberer
            free(content);
        }
        else{
            DEBUG_MSG("section %s not present in the elf file",section_names[i]);
        }
    }
		free(ehdr); 

    //TODO allouer la pile (et donc modifier le nb de segments)
        
    memory = stack_set(reg, memory);

    printf("\n------ Fichier ELF \"%s\" : sections lues lors du chargement ------\n", argv[1]) ;
    stab32_print( symtab );
    printf("Displaying memory...\n");
    size_t j=0;
    byte x;
    for (j=0;j<= 0xfffff000;j++) {
      x = read_memory_value(j, memory);
      if (x!=0) {
	printf("%zx = %"PRIu8"\n", j, x);
  }

    }
    // on fait le ménage avant de partir
    del_stab( symtab );
    del_scntab( section_table );
    fclose(pf_elf);
    puts("");
    return 0;
} 
