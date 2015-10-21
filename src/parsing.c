#include "interpreter.h"
#include "parsing.h"


/**
 * return le prochain token de la chaine actuellement
 * analysée par l'interpreteur
 * La fonction utilise une variable interne de l'interpreteur
 * pour gérer les premiers appels a strtok
 * @inter le pointeur vers l'interpreteur
 * @return un pointeur vers le token ou NULL
 */
char* get_next_token(interpreteur inter) {

    char       *token = NULL;
    char       *delim = " \t\n";

    if ( inter->first_token == 0 ) {
        token = strtok_r( inter->input, delim, &(inter->from) );
        inter->first_token = 1;
    }
    else {
        token = strtok_r( NULL, delim, &(inter->from) );
    }

    if ( NULL == token ) {
        inter->first_token = 0;
    }

    return token;
}

/**
 * teste si un token est une valeur hexa
 * ATTENTION cette méthode n'est pas complete et ne fonctionnera pas dans tous les cas
 * essayer avec 0x567ZRT...
 *@param chaine le token à analyser
 *@return 0 si non-hexa, non null autrement
 */

/* int is_hexa(char* chaine) {
    int i;
    return (chaine!=NULL && strlen(chaine)>2&& chaine[0]=='0' && chaine[1]=='x' && sscanf(chaine,"%x",&i)==1);
} */

// check LONG_MAX LONG_MIN errno

int is_hexa(char* chaine) {
    char* p;
    if (strcmp(chaine, "0x0") == 0) return 1;
    return (chaine!=NULL && strlen(chaine)>=3 && chaine[0]=='0' && chaine[1]=='x' && strtol(chaine, &p, 0) != 0 && *p == '\0');
}


/**
 * retourne le type du token (fonction très incomplete)
 * @param chaine le token à analyser
 * @return un entier correspondant au type du token
 *
 */
int get_type(char* chaine) {
    if (is_hexa(chaine))
        return HEXA;
    if (strcmp(chaine, "0") == 0)
	return INT; 
    char* end;
    int i = strtol(chaine, &end, 0);
    if (i == 0 || *end != '\0')
        return UNKNOWN; 
    return INT;
}

/**ster(char* reg) {
    return (strcmp(reg, "r0") && strcmp(reg, "r1") && strcmp(reg, "r2") && strcmp(reg, "r3") && strcmp(reg, "r4") && strcmp(reg, "r5") && strcmp(reg, "r6") && strcmp(reg, "r7") && strcmp(reg, "r8") &&  strcmp(reg, "r9") && strcmp(reg, "r10") && strcmp(reg, "r11") && strcmp(reg, "r12") && strcmp(reg, "sp") && strcmp(reg, "lr") && strcmp(reg, "pc") && strcmp(reg, "aspr") && strcmp(reg, "all"));
}

 * @param in Input line (possibly very badly written).
 * @param out Line in a suitable form for further analysis.
 * @return nothing
 * @brief This function will prepare a line for further analysis.
 *
 * This function will prepare a line for further analysis and check for low-level syntax errors.
 * colon, brackets, commas are surrounded with blank; tabs are replaced with blanks.
 * negs '-' are attached to the following token (e.g., "toto -   56" -> "toto -56")  .
 */
void string_standardise( char* in, char* out ) {
    unsigned int i=0, j;

    for ( j= 0; i< strlen(in); i++ ) {

        /* insert blanks around special characters*/
        if (in[i]==':' || in[i]=='+' || in[i]=='~') {
            out[j++]=' ';
            out[j++]=in[i];
            out[j++]=' ';

        }

        /* remove blanks after negation*/
        else if (in[i]=='-') {
            out[j++]=' ';
            out[j++]=in[i];
            while (isblank((int) in[i+1])) i++;
        }

        /* insert one blank before comments */
        else if (in[i]=='#') {
            out[j++]=' ';
            out[j++]=in[i];
        }
        /* translate tabs into white spaces*/
        else if (isblank((int) in[i])) out[j++]=' ';
        else out[j++]=in[i];
    }
}

int is_adress(char* adr)
{
    int hexValue;
    if (is_hexa(adr)) {
	sscanf(adr, "%x", &hexValue);
    }
    else
        return 0;
    if(hexValue <= 0) {
       WARNING_MSG("invalid call. The adress should be positive.\n");
       return 0;
    }
    return 1;
}

int reg_index(char* reg)
{
    if (strcmp(reg, "sp") == 0) return 13;
    if (strcmp(reg, "lr") == 0) return 14;
    if (strcmp(reg, "pc") == 0) return 15;
    if (strcmp(reg, "aspr") == 0) return 16;
    return atoi(reg);
}

int is_register(char* reg) {
    return (strcmp(reg, "r0") && strcmp(reg, "r1") && strcmp(reg, "r2") && strcmp(reg, "r3") && strcmp(reg, "r4") && strcmp(reg, "r5") && strcmp(reg, "r6") && strcmp(reg, "r7") && strcmp(reg, "r8") &&  strcmp(reg, "r9") && strcmp(reg, "r10") && strcmp(reg, "r11") && strcmp(reg, "r12") && strcmp(reg, "r13") && strcmp(reg, "r14") && strcmp(reg, "r15") && strcmp(reg, "sp") && strcmp(reg, "lr") && strcmp(reg, "pc") && strcmp(reg, "aspr"));
}
