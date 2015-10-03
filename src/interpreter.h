#ifndef _INTERPRETER
#define _INTERPRETER

/* mode d'interaction avec l'interpreteur (exemple)*/
typedef enum {INTERACTIF, SCRIPT, DEBUG_MODE} inter_mode;

/** structure passée en parametre qui contient la connaissance de l'état de
 * l'interpréteur
 */
typedef struct {
    inter_mode mode;
    char input[MAX_STR];
    char* from;
    char first_token;
} *interpreteur;

interpreteur init_inter(void); 

void del_inter(interpreteur inter);
int execute_cmd(interpreteur inter);
int acquire_line(FILE *fp, interpreteur inter);

#endif
