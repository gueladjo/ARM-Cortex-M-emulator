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


// Fonction permettant de verifier si une chaine de caracteres
// est bien dans la Section Header String Table
// parametres :
// 		       name : le nom de la chaine recherchée
//  section_table : la table des sections
//
// retourne 1 si present, 0 sinon
//int is_in_shstrtab(char* name, scntab section_table) {
//    int i;
//    for (i=0; i<section_table.size; i++) {
//        if (!strcmp(section_table.scn[i].name,name)) return 1;
//    }
//    return 0;
//}

// Cette fonction calcule le nombre de segments à prevoir
// Elle cherche dans la table de sections si les sections predefinies
// s'y trouve
// parametres :
//  section_table : la table des sections
//  section_names : le noms des sections que l'on cherche
//    nb_sections : le nombre de noms de section que l'on cherche
//
// retourne le nombre de sections trouvées
//unsigned int get_nsegments(scntab section_table,char* section_names[],int nb_sections) {
//    unsigned int n=0;
//    int i;
//    for (i=0; i<nb_sections; i++) {
//        if (is_in_shstrtab(section_names[i],section_table)) n++;
//    }
//    return n;
//}


// fonction permettant d'extraire une section du fichier ELF et de la chargée dans le segment du même nom
// parametres :
//   fp         : le pointeur du fichier ELF
//   memory     : la structure de mémoire virtuelle
//   scn        : le nom de la section à charger
//   permission : l'entier représentant les droits de lecture/ecriture/execution
//   add_start  : l'addresse virtuelle à laquelle la section doit être chargée
//
// retourne 0 en cas de succes, une valeur non nulle sinon
/*
int elf_load_section_in_memory(FILE* fp, mem memory, char* scn,unsigned int permissions,unsigned long long add_start) {
    byte *ehdr    = __elf_get_ehdr( fp );
    byte *content = NULL;
    uint  textsz  = 0;
    vsize sz;
    vaddr addr;

    byte *useless = elf_extract_section_header_table( ehdr, fp );
    free( useless );

    if ( NULL == ehdr ) {
        WARNING_MSG( "Can't read ELF file" );
        return 1;
    }

    if ( 1 == attach_scn_to_mem(memory, scn, SCN_ATTR( WIDTH_FROM_EHDR( ehdr ), permissions ) ) ) {
        WARNING_MSG( "Unable to create %s section", scn );
        free( ehdr );
        return 1;
    }

    content = elf_extract_scn_by_name( ehdr, fp, scn, &textsz, NULL );
    if ( NULL == content ) {
        WARNING_MSG( "Corrupted ELF file" );
        free( ehdr );
        return 1;
    }

    switch( WIDTH_FROM_EHDR(ehdr) ) {
    case 32 :
        sz._32   = textsz; // +8: In case adding a final sys_exit is needed 
        addr._32 = add_start;
        break;
    case 64 :
        sz._64   = textsz; // +8: In case adding a final sys_exit is needed 
        addr._64 = add_start;
        break;
    default :
        WARNING_MSG( "Wrong machine width" );
        return 1;
    }

    if ( 1 == fill_mem_scn(memory, scn, sz, addr, content ) ) {
        free( ehdr );
        free( content );
        WARNING_MSG( "Unable to fill in %s segment", scn );
        return 1;
    }

    free( content );
    free( ehdr );

    return 0;
}



/*--------------------------------------------------------------------------  */
/**
 * @param fp le fichier elf original
 * @param seg le segment à reloger
 * @param mem l'ensemble des segments
 * @param endianness le boutisme du programme
 * @param symtab la table des symbole du programme
 * @param symtab_libc la table des symbole de la libc (NULL si inutile)
 * @param fp_libc le fichier elf de la libc (NULL si inutile)
 *
 * @brief Cette fonction effectue la relocation du segment passé en parametres
 * @brief l'ensemble des segments doit déjà avoir été chargé en memoire.
 *
 * VOUS DEVEZ COMPLETER CETTE FONCTION POUR METTRE EN OEUVRE LA RELOCATION !!
 */
 /*
void reloc_segment(FILE* fp, segment seg, void* memory,unsigned int endianness,stab* symtab) {
    byte *ehdr    = __elf_get_ehdr( fp );
    uint32_t  scnsz  = 0;
    Elf32_Rel *rel = NULL;
    char* reloc_name = malloc(strlen(seg.name)+strlen(RELOC_PREFIX_STR)+1);
    scntab section_tab;
    scntab section_tab_libc;

    // on recompose le nom de la section
    memcpy(reloc_name,RELOC_PREFIX_STR,strlen(RELOC_PREFIX_STR)+1);
    strcat(reloc_name,seg.name);

    // on récupere le tableau de relocation et la table des sections
    rel = (Elf32_Rel *)elf_extract_scn_by_name( ehdr, fp, reloc_name, &scnsz, NULL );
    elf_load_scntab(fp ,32, &section_tab);

    if (symtab_libc!=NULL && fp_libc!=NULL)
        elf_load_scntab(fp_libc ,32, &section_tab_libc);

    //TODO : faire la relocation ci-dessous !
    if (rel != NULL &&seg.content!=NULL && seg.size._32!=0) {

        INFO_MSG("--------------Relocation de %s-------------------\n",seg.name) ;
        INFO_MSG("Nombre de symboles a reloger: %ld\n",scnsz/sizeof(*rel)) ;




        int i,j;
        uint32_t * donnees = ((uint32_t *)seg.content);
        unsigned int V=0;
        unsigned int AHL=0;
        unsigned int S =0;
        unsigned int P =0;
        unsigned int A =0;
        unsigned int word =0;
        unsigned int word2=0;


        for (i=0; i<scnsz/sizeof(*rel); i++) {
            __Elf_Rel_flip_endianness((byte *)(rel+i), 32, endianness );
            unsigned int offset = rel[i].r_offset; // decalage par rapport au debut de section
            unsigned int index =0;                    // index de la données à modifier (depend du type de reloc)
            int sym = (int) ELF32_R_SYM(rel[i].r_info);
            int type = (unsigned char) ELF32_R_TYPE(rel[i].r_info);

            DEBUG_MSG("offset %08x info =%08x sym = %08x type =%08x\n",rel[i].r_offset,rel[i].r_info,sym,type);
            DEBUG_MSG(" symbole %s de type %s\n",symtab->sym[sym].name,MIPS32_REL[type]);


            P = seg.start._32 + offset;

            if (symtab->sym[sym].type==STT_SECTION) {
                for (j=0; j<memory->nseg; j++)
                    if (0 == strncmp(memory->seg[j].name, section_tab.scn[symtab->sym[sym].scnidx].name, strlen(memory->seg[j].name))) {
                        S = memory->seg[j].start._32;
                        DEBUG_MSG("section %s à l'@ :%08x avec S = %08x\n", memory->seg[j].name, memory->seg[j].start._32,S);
                        break;
                    }
            }
            else if(symtab->sym[sym].type==STT_FUNC) {
                DEBUG_MSG("STT_FUNC") ;
                S = symtab->sym[sym].addr._32;
                for (j=0; j<memory->nseg; j++)
                    if (0 == strncmp(memory->seg[j].name, section_tab.scn[symtab->sym[sym].scnidx].name, strlen(memory->seg[j].name))) {
                        S = memory->seg[j].start._32 + S;
                        break;
                    }
                DEBUG_MSG("section %s à l'@ :%08x avec S = %08x\n", memory->seg[j].name, memory->seg[j].start._32,S);
            }

            else if(symtab->sym[sym].type==STT_NOTYPE) {

                int idx=-1;
                // on cherche s'il ne serait pas dans la table des symboles de libc
                if (symtab_libc!=NULL) {
                    for (j=0; j<symtab_libc->size; j++) {
                        if (0 == strcmp(symtab_libc->sym[j].name,symtab->sym[sym].name)) {
                            printf("\nSybomle %s %s val = %08x\n",symtab->sym[sym].name,symtab_libc->sym[j].name,symtab_libc->sym[j].addr._32);
                            idx = symtab_libc->sym[j].scnidx;
                            S = symtab_libc->sym[j].addr._32;
                            break;
                        }
                    }
                    if (idx==-1) {
                        WARNING_MSG("Symbole %s non défini",symtab->sym[sym].name) ;
                        continue;
                    }
                    for (j=0; j<memory->nseg; j++) {
                        int offset = strlen("libc");
                        DEBUG_MSG("test %d seg = %s avec off %s sec_tab %s\n",j,memory->seg[j].name,memory->seg[j].name+offset, section_tab_libc.scn[idx].name);
                        if (0 == strncmp(memory->seg[j].name+offset, section_tab_libc.scn[idx].name, strlen(memory->seg[j].name+offset))) {
                            S = memory->seg[j].start._32 + S;
                            break;
                        }
                    }
                    if (j==memory->nseg) {
                        WARNING_MSG("Symbole %s non défini",symtab->sym[sym].name) ;
                        continue;
                    }
                    DEBUG_MSG("section %s à l'@ :%08x avec S = %08x\n", memory->seg[j].name, memory->seg[j].start._32,S);
                }
                else {
                    WARNING_MSG("%d non traité pour l'instant",symtab->sym[sym].type) ;
                    continue;
                }
            }
            index = offset/4; // indice de l'instruction pour ce type de relocation
            word = donnees[index];
            __flip_endianness((byte*)&word, sizeof(word));

            DEBUG_MSG("S:%08x P:%08x %u avant : %08x\n",S,P,index,word);
            switch(type) {
            case R_MIPS_32:
                // indice de la zone sur 32 bits
                A = word;
                V = S+A;
                break;
            case R_MIPS_26:

                A = word& 0x03FFFFFF;
                V = ((A<<2)|((P&0xF0000000)+S))>>2;
                V = ((word & 0xFC000000) | (V&0x03FFFFFF));


                break;
            case R_MIPS_HI16:

                word2 =donnees[index+1];
                __flip_endianness((byte*)&word2, sizeof(word2));
                AHL = ((word&0x0000FFFF)<<16) + ((short) (word2&0x0000FFFF));
                V = ((AHL+S) - (short) (AHL+S)) >> 16;
                V = (word & 0xFFFF0000) | (V & 0x0000FFFF);

                break;
            case R_MIPS_LO16:

                V = (AHL+S)&0x0000FFFF;			// AHL on reprend la valeur précédente...
                V = (word & 0xFFFF0000) | (V & 0x0000FFFF);

                break;
            default:
                ERROR_MSG("Reloc type %d = %s non IMPLEMENTE",type,MIPS32_REL[type]) ;
            }

            DEBUG_MSG("après : %08x\n",V);
            __flip_endianness((byte*)&V, sizeof(V));
            donnees[index] = V;

        }


    }
    del_scntab(section_tab);
    free( rel );
    free( reloc_name );
    free( ehdr );

}

*/

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
    unsigned int bus_width;    // 32 bits ou 64bits
    unsigned int next_segment_start = START_MEM; // compteur pour designer le début de la prochaine section

    //TODO declarer une memoire virtuelle, c'est elle qui contiendra toute les données du programme
    memory memory;
    word stack[16];
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

        memory = memory_allocation(memory, section_names[i], content, taille, next_segment_start);        
        
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
        
    memory = stack_set(stack, memory);

    printf("\n------ Fichier ELF \"%s\" : sections lues lors du chargement ------\n", argv[1]) ;
    stab32_print( symtab );

    // on fait le ménage avant de partir
    del_stab( symtab );
    del_scntab( section_table );
    fclose(pf_elf);
    puts("");
    return 0;
}
