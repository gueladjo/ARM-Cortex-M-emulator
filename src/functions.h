#ifndef _FUNCTIONS
#define _FUNCTIONS


int loadcmd(interpreteur inter, memory mem);
int dispcmd(interpreteur inter, memory mem);
int setcmd(interpreteur inter, memory mem);
int debugcmd(interpreteur inter);
int assertcmd(interpreteur inter, memory mem);
int resumecmd(interpreteur inter);
int disasmcmd(interpreteur inter, memory mem);

#endif
