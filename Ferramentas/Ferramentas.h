#ifndef _FE_METRO_
#define _FE_METRO_

#include <stdio.h>
#include <stdlib.h>
#include "../defines.h"


void getstr(FILE* file1,char* string);
int getint(int* p,FILE* file );
void insereg(FILE* file,Dados* d,int flag);
void limpad(Dados* d);
int pegaRegistro(Dados* d,FILE* file);
void imprimeRegistro(Dados* d);
void restrinja(Dados* d);
void generalize(Dados* d);
int compativel(Dados* r,Dados* d);
void firstfit(FILE* file,Dados* d,long int* topolista);
int intteclado(FILE* file);
void atualiza_ca(FILE* file,Cabecalho* pca);
#endif