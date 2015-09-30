#ifndef _PARSING
#define _PARSING

enum {HEXA,UNKNOWN};
char* get_next_token(interpreteur inter);
int is_hexa(char* chaine);
int get_type(char* chaine);
void string_standardise( char* in, char* out );

#endif
