/**
 * @file memory.h
 * @author Mickael Albisser + Moctar Ba
 * @brief Interpreter main
 */

#include "interpreter.h"
#include "parsing.h"
#include "functions.h"

/**
 * Programme principal
 */
int main ( int argc, char *argv[] ) {
    /* exemples d'utilisation des macros du fichier notify.h */
    INFO_MSG("Un message INFO_MSG : Debut du programme %s", argv[0]); /* macro INFO_MSG */
    WARNING_MSG("Un message WARNING_MSG !"); /* macro INFO_MSG */
    DEBUG_MSG("Un message DEBUG_MSG !"); /* macro DEBUG_MSG : uniquement si compil en mode DEBUG_MSG */
    interpreteur inter=init_inter(); /* structure gardant les infos et états de l'interpreteur*/
    memory memory;
    memory = malloc(sizeof(*memory));
    memory->rodata = malloc(sizeof(segment));
    memory->txt = malloc(sizeof(segment));
    memory->data = malloc(sizeof(segment));
    memory->bss = malloc(sizeof(segment));
    memory->stack = malloc(sizeof(segment));


    FILE *fp = NULL; /* le flux dans lequel les commande seront lues : stdin (mode shell) ou un fichier */

    if ( argc > 2 ) {
        usage_ERROR_MSG( argv[0] );
        exit( EXIT_FAILURE );
    }
    if(argc == 2 && strcmp(argv[1], "-h") == 0) {
        usage_ERROR_MSG( argv[0] );
        exit( EXIT_SUCCESS );
    }

    /*par defaut : mode shell interactif */
    fp = stdin;
    inter->mode = INTERACTIF;
    if(argc == 2) {
        /* mode fichier de commandes */
        fp = fopen( argv[1], "r" );
        if ( fp == NULL ) {
            perror( "fopen" );
            exit( EXIT_FAILURE );
        }
        inter->mode = SCRIPT;
    }

    /* boucle infinie : lit puis execute une cmd en boucle */
    while ( 1 ) {

        if (acquire_line( fp,  inter)  == 0 ) {
            /* Une nouvelle ligne a ete acquise dans le flux fp*/

            int res = execute_cmd(inter, memory); /* execution de la commande */

            // traitement des erreurs
            switch(res) {
            case CMD_OK_RETURN_VALUE:
                break;
            case CMD_EXIT_RETURN_VALUE:
                /* sortie propre du programme */
                if ( inter->mode == SCRIPT ) {
                    fclose( fp );
                }
                del_inter(inter);
	        memory_free(memory);
                exit(EXIT_SUCCESS);
                break;
            default:
                /* erreur durant l'execution de la commande */
                /* En mode "fichier" toute erreur implique la fin du programme ! */
                if (inter->mode == SCRIPT) {
                    fclose( fp );
                    del_inter(inter);
		    memory_free(memory);
                    /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                    ERROR_MSG("ERREUR DETECTEE. Aborts");
                }
                break;
            }
        }
        if( inter->mode == SCRIPT && feof(fp) ) {
            /* mode fichier, fin de fichier => sortie propre du programme */
            DEBUG_MSG("FIN DE FICHIER");
            fclose( fp );
            del_inter(inter);
	    memory_free(memory);
            exit(EXIT_SUCCESS);
        }
    }
    /* tous les cas de sortie du programme sont gérés plus haut*/
    ERROR_MSG("SHOULD NEVER BE HERE\n");
}
