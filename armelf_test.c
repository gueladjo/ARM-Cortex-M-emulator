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

//nom du prefix à appliquer pour la section
#define RELOC_PREFIX_STR ".rel"


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



/*--------------------------------------------------------------------------  */
/**
 * @param mem l'ensemble des segments
 * @param symtab la table des symbole du programme
 * @param la table des sections du fichier objet
 *
 * @brief Cette fonction verifie s'il existe dans la table des symboles un 
 * symbole $d appartenant à la section .text. Si cela est le cas, il y a un 
 * literal pool en fin de section .text et la fin du segment .text est modifiée
 * pour s'arreter à l'adresse de $d. Sinon, la fin de la section .text est non 
 * modifiée.
 *
 * TODO: vous devez adapter cette fonction à votre structure de données mémoire
 * attention, votre structure de segment dois avoir un champ "end"
 *
 */
void set_end_of_text_section(mem memory, stab* symtab, scntab section_tab) {

    segment* seg =NULL;
    int i;

		// on cherche le segment .text
    for (i=0; i<memory->nseg; i++) {
        if (memory->seg[i].name!=NULL && strcmp(memory->seg[i].name,".text")==0) {
            seg = memory->seg+i;
            break;
        }
    }

    if ( seg == NULL ){
    	ERROR_MSG("Section text non trouvée \n");
    }

		// init de la fin de la section à la valeur par défaut 
    seg->end._32 = seg->start._32+seg->size._32;

		// on parcourt la table des symboles pour chercher $d
    for (i=0; i<symtab->size; i++){
        if (symtab->sym[i].name!=NULL && strcmp(symtab->sym[i].name,"$d")==0) {
        		// si le symbole appartient à la section nommée .text
            if (0 == strcmp(".text", section_tab.scn[symtab->sym[i].scnidx].name)) {
            		// la fin est l'adresse de $d soit le début du segment text + le décalage 
            		// du symbole par rapport au début 
                seg->end._32 = seg->start._32+symtab->sym[i].addr._32;
            }
        }
    }
}



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
 * VOUS DEVEZ COMPLETER CETTE FONCTION POUR METTRE EN OEUVRE LA RELOCATION !!
 */
void reloc_segment(FILE* fp, segment seg, mem memory,unsigned int endianness,stab* symtab) {
    byte *ehdr    = __elf_get_ehdr( fp );
    uint32_t  scnsz  = 0;
    Elf32_Rel *rel = NULL;
    char* reloc_name = malloc(strlen(seg.name)+strlen(RELOC_PREFIX_STR)+1);
    scntab section_tab;

    // on recompose le nom de la section
    memcpy(reloc_name,RELOC_PREFIX_STR,strlen(RELOC_PREFIX_STR)+1);
    strcat(reloc_name,seg.name);

    // on récupere le tableau de relocation et la table des sections
    rel = (Elf32_Rel *)elf_extract_scn_by_name( ehdr, fp, reloc_name, &scnsz, NULL );
    elf_load_scntab(fp ,32, &section_tab);

    if (rel != NULL &&seg.content!=NULL && seg.size._32!=0) {

        INFO_MSG("--------------Relocation de %s-------------------\n",seg.name) ;
        INFO_MSG("Nombre de symboles a reloger: %ld\n",scnsz/sizeof(*rel)) ;

        int i,j;
        unsigned int V = 0;
        unsigned int S = 0;
        unsigned int A = 0;
        unsigned int T = 0;
        unsigned int word =0;

        for (i=0; i<scnsz/sizeof(*rel); i++) {
            T =0;
            __Elf_Rel_flip_endianness((byte *)(rel+i), 32, endianness );
            unsigned int offset = rel[i].r_offset; // decalage par rapport au debut de section
            int sym = (int) ELF32_R_SYM(rel[i].r_info);
            int type = (unsigned char) ELF32_R_TYPE(rel[i].r_info);

            DEBUG_MSG("Entrée de relocation : offset %08x info =%08x sym = %08x type =%08x\n",rel[i].r_offset,rel[i].r_info,sym,type);
            DEBUG_MSG("Symbole en fonction duquel on reloge : symbole %s de type %s\n",symtab->sym[sym].name,ARM32_REL[type]);


            if (symtab->sym[sym].type==STT_SECTION) {
                for (j=0; j<memory->nseg; j++)
                    if (0 == strncmp(memory->seg[j].name, section_tab.scn[symtab->sym[sym].scnidx].name, strlen(memory->seg[j].name))) {
                        S = memory->seg[j].start._32;
                        break;
                    }
            }
            else {
                WARNING_MSG("%d non traité pour l'instant",symtab->sym[sym].type) ;
                continue;
            }

						//TODO: testez le type et modifiez les données à reloger des sections 

        }
    }
    del_scntab(section_tab);
    free( rel );
    free( reloc_name );
    free( ehdr );
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
    unsigned int bus_width;    // 32 bits ou 64bits
    unsigned int next_segment_start = START_MEM; // compteur pour designer le début de la prochaine section

    //TODO declarer une memoire virtuelle, c'est elle qui contiendra toute les données du programme
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

		//TODO allouer la memoire virtuelle


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
            //TODO: copier le contenu dans la memoire avant de liberer
            free(content);
        }
        else{
            DEBUG_MSG("section %s not present in the elf file",section_names[i]);
        }
    }
		free(ehdr); 

    //TODO allouer la pile (et donc modifier le nb de segments)

    printf("\n------ Fichier ELF \"%s\" : sections lues lors du chargement ------\n", argv[1]) ;
    stab32_print( symtab );

    // on fait le ménage avant de partir
    del_stab( symtab );
    del_scntab( section_table );
    fclose(pf_elf);
    puts("");
    return 0;
}
