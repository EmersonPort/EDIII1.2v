#ifndef _defines_
#define _defines_

#define trash "$"

typedef struct{
    char status;
    long int topoLista;
    int nroEstacoes;
    int nroParesEstacao;
} Cabecalho;

typedef struct{

    char removido;
    int tamanhoRegistro;
    long int proxLista;


    int codEstacao;
    //nomeEstacao :na ordem csv
    int codLinha;
    //nomeLinha :na ordem csv
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;

    char nomeEstacao[40];
    char nomeLinha[40];

} Dados;

#endif