#ifndef _IO_PESSOA_
#define _IO_PESSOA_

#include <stdio.h>
#include <stdlib.h>
#include "../defines.h"

int getint(int* p,FILE* file );
//int escovabitsli(long int* p,FILE* file )
void imprime(FILE* file);
void aplicacao1(FILE* file1,FILE* file2);
void aplicacao2(FILE* file);
void aplicacao3(FILE* file);
void aplicacao4(FILE* file);
void aplicacao5(FILE* file);
void erronatela(FILE* f1,char *str2);
void imprimet(FILE* file);
#endif