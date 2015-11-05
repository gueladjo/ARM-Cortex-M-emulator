#ifndef _PARSING
#define _PARSING


char* get_next_token(interpreteur inter);
int is_hexa(char* chaine);
int get_type(char* chaine);
void string_standardise( char* in, char* out );
int is_adress(char* adr);
int is_register(char* reg);
int reg_index(char* reg);

#endif
