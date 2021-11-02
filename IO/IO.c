#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "../auxiliar/functions.h"
#include "../Ferramentas/Ferramentas.h"

//Emerson Pereira Portela Filho 11800625 

//Carla Nunes da Cruz 8479343


void aplicacao1(FILE* file1,FILE* file2){
    fseek(file1,101,SEEK_SET);//posicao em que comeca as iformacoes que serao lidas
    //fseek(file2,17,SEEK_SET);
    Dados* d = (Dados*) malloc(sizeof(Dados));// d armazena o registro q sera lido e armazenado
    d->removido='0';
    d->proxLista=-1;
    limpad(d);
    Cabecalho ca; 
    ca.status='0';
    ca.topoLista=-1;
    ca.nroEstacoes=0;
    ca.nroParesEstacao=0;
    fwrite(&(ca.status),sizeof(char),1,file2); //criando o cabecalho do arquivo
    fwrite(&(ca.topoLista),sizeof(long int),1,file2);
    fwrite(&(ca.nroEstacoes),sizeof(int),1,file2);
    fwrite(&(ca.nroParesEstacao),sizeof(int),1,file2);
    //short int vestacoes[1000]={0};// armazena os numEstacoes setados
    char c;// carac. que eh usado para pegar o \n
    while(getint(&(d->codEstacao),file1)==1){// enquanto tiver leitura
        //printf("%d\n",d->codEstacao);
        /*if(vestacoes[d->codEstacao]==0){// se essa estacao n estiver setada , ela fica
            vestacoes[d->codEstacao]=1;
            ca.nroEstacoes++;}// +estacoes , pois ha uma estacao nova*/
        getstr(file1,d->nomeEstacao);
        getint(&(d->codLinha),file1);
        //printf("%d\n",d->codLinha);
        getstr(file1,d->nomeLinha);
        getint(&(d->codProxEstacao),file1);
        //printf("%d\n",d->codProxEstacao);
        getint(&(d->distProxEstacao),file1);
        //printf("%d\n",d->distProxEstacao);
        getint(&(d->codLinhaIntegra),file1);
        //printf("%d\n",d->codLinhaIntegra);
        getint(&(d->codEstIntegra),file1);
        //printf("%d\n",d->codEstIntegra);
        fread(&c,sizeof(char),1,file1);// pega o \n
        int j=0;
        insereg(file2,d,1);//insere registro sem limitacao de espaco
        limpad(d);}

    fseek(file2,0,SEEK_SET);//voltamos para atualizar o cabecalho 
    ca.status='1';
    atualiza_ca(file2,&ca);
    free(d);}


void aplicacao2(FILE* file){
    fseek(file,17,SEEK_SET);
    //printf("%c %li %i %i \n",ca.status,ca.topoLista,ca.nroEstacoes,ca.nroParesEstacao);
    Dados* d = (Dados*) malloc(sizeof(Dados));
    int flag=1;
    int status;
    while((status=pegaRegistro(d,file))!=0){
        //printf("%c %i %li ",d->removido,d->tamanhoRegistro,d->proxLista);
        if(status==-1) continue;
        flag=0;
        imprimeRegistro(d);
        }
    free(d);
    if(flag==1)printf("Registro inexistente.\n");}


void aplicacao3(FILE* file){
    int n;
    scanf("%i\n",&n);
    Dados* d = (Dados*) malloc(sizeof(Dados));
    Dados* r = (Dados*) malloc(sizeof(Dados));
    generalize(d);
    
    for(int i=0;i<n;i++) restrinja(d);
    //int codEstacao=0;
    //char nomeEstacao[40];
    //char nomeLinha[40];

    //char cod[]="codEstacao";
    //char nEstacao[]="nomeEstacao";
    //char nLinha[]="nomeLinha";
    
    int status=0;
    int flag=1;
    fseek(file,17,SEEK_SET);
    while((status=pegaRegistro(r,file))!=0){
        if (status==-1 || compativel(r,d)!=1) continue;      
        flag=0;
        imprimeRegistro(r);}

free(d);
free(r);
if(flag==1)printf("Registro inexistente.\n"); 
}


void aplicacao4(FILE* file){
    int n;
    int x;
    scanf("%i\n",&n);
    Dados* d = (Dados*) malloc(sizeof(Dados));
    Dados* r = (Dados*) malloc(sizeof(Dados));
    Cabecalho ca;
    fread(&(ca.status),sizeof(char),1,file);
    fread(&(ca.topoLista),sizeof(long int),1,file);
    fread(&(ca.nroEstacoes),sizeof(int),1,file);
    fread(&(ca.nroParesEstacao),sizeof(int),1,file);
    
    for(int i=0;i<n;i++){
        if(i>0) fseek(file,17,SEEK_SET);
        generalize(d);
        scanf("%i ",&x);
        for(int j=0;j<x;j++) restrinja(d);
        int status;
        while((status=pegaRegistro(r,file))!=0){
            if (status==-1 || compativel(r,d)!=1) continue;
            r->removido='1';
            r->proxLista=ca.topoLista;
            //if(r->codProxEstacao!=-1) ca.nroParesEstacao--;
            //printf("%ld\n",ftell(file));
            ca.topoLista = ftell(file)-r->tamanhoRegistro -5;
            fseek(file,ca.topoLista,SEEK_SET);
            //printf("%d %ld\n",r->tamanhoRegistro,ftell(file));
            //printf("%c %d %ld",r->removido,r->tamanhoRegistro,r->proxLista);
            //printf("%ld\n",ca.topoLista);
            fwrite(&(r->removido),sizeof(char),1,file);
            fwrite(&(r->tamanhoRegistro),sizeof(int),1,file);
            fwrite(&(r->proxLista),sizeof(long int),1,file);
            //printf("\t\t%c %d %ld\n",r->removido,r->tamanhoRegistro,r->proxLista);
            fseek(file,r->tamanhoRegistro-8,SEEK_CUR);}}


    free(d);
    free(r);
    //ca.nroEstacoes=0;
    /*ca.nroParesEstacao=0;
    for(int i=0;i<500;i++){
        int somadepares=0;
        for(int j=1;j<500;j++){
            somadepares+=Matestacoes[i][j];}
        ca.nroParesEstacao+=somadepares;
        somadepares+=Matestacoes[i][0];}*/
        //if(somadepares>0) ca.nroEstacoes++;}
    atualiza_ca(file,&ca);}

void aplicacao5(FILE* file){
    int n;
    scanf("%i\n",&n);
    Cabecalho ca;
    fread(&(ca.status),sizeof(char),1,file);
    fread(&(ca.topoLista),sizeof(long int),1,file);
    fread(&(ca.nroEstacoes),sizeof(int),1,file);
    fread(&(ca.nroParesEstacao),sizeof(int),1,file);
    Dados* d = (Dados*) malloc(sizeof(Dados));
    d->removido='0';
    d->proxLista=-1;
    for(int i=0;i<n;i++) {
    d->codEstacao=intteclado(file);
    scan_quote_string(d->nomeEstacao);
    d->codLinha=intteclado(file);
    scan_quote_string(d->nomeLinha);
    d->tamanhoRegistro=34+ strlen(d->nomeEstacao) + strlen(d->nomeLinha);
    d->codProxEstacao=intteclado(file);
    d->distProxEstacao=intteclado(file);
    d->codLinhaIntegra=intteclado(file);
    d->codEstIntegra=intteclado(file);
    if(i!=n-1) while(getchar()!='\n');
    firstfit(file,d,&(ca.topoLista));}
    atualiza_ca(file,&ca);    
    free(d);}


void aplicacao6(FILE* file){
    int n;
    int x;
    int y;
    scanf("%i\n",&n);
    Cabecalho ca;
    fread(&(ca.status),sizeof(char),1,file);
    fread(&(ca.topoLista),sizeof(long int),1,file);
    fread(&(ca.nroEstacoes),sizeof(int),1,file);
    fread(&(ca.nroParesEstacao),sizeof(int),1,file);
    Dados* buscado = (Dados*) malloc(sizeof(Dados));
    Dados* atualizado = (Dados*) malloc(sizeof(Dados));
    Dados* r = (Dados*) malloc(sizeof(Dados));
    for(int i=0;i<n;i++) {
        if(i>0) fseek(file,17,SEEK_SET);
        generalize(buscado);
        scanf("%i ",&x);
        for(int j=0;j<x;j++) restrinja(buscado);
        generalize(atualizado);
        scanf("%i ",&y);
        for(int j=0;j<y;j++) restrinja(atualizado);
        int status;
        while((status=pegaRegistro(r,file))!=0){
            if (status==-1) continue;
            if (compativel(r,buscado)!=1) continue;
            if(atualizado->codEstacao!=-2) r->codEstacao=atualizado->codEstacao;
            if(atualizado->codLinha!=-2) r->codLinha=atualizado->codLinha;
            if(atualizado->codProxEstacao!=-2) r->codProxEstacao=atualizado->codProxEstacao;
            if(atualizado->distProxEstacao!=-2) r->distProxEstacao=atualizado->distProxEstacao;
            if(atualizado->codLinhaIntegra!=-2) r->codLinhaIntegra= atualizado->codLinhaIntegra;
            if(atualizado->codEstIntegra!=-2) r->codEstIntegra=atualizado->codEstIntegra;
            if(strcmp(atualizado->nomeEstacao,"Sem restrição")!=0) strcpy(r->nomeEstacao,atualizado->nomeEstacao);
            if(strcmp(atualizado->nomeLinha,"Sem restrição")!=0) strcpy(r->nomeLinha,atualizado->nomeLinha);
            int tamanho_atualizado= 34+ strlen(r->nomeEstacao)+ strlen(r->nomeLinha);
            //imprimeRegistro(r);
            if(tamanho_atualizado<=r->tamanhoRegistro){
                fseek(file,-r->tamanhoRegistro-5,SEEK_CUR);
                insereg(file,r,0);}             
            else{
                r->removido='1';
                r->proxLista=ca.topoLista;
                long int fim_regis=ftell(file);
                ca.topoLista = fim_regis-r->tamanhoRegistro -5;
                fseek(file,ca.topoLista,SEEK_SET);
                fwrite(&(r->removido),sizeof(char),1,file);
                fwrite(&(r->tamanhoRegistro),sizeof(int),1,file);
                fwrite(&(r->proxLista),sizeof(long int),1,file);
                r->removido='0';
                r->proxLista=-1;
                r->tamanhoRegistro=tamanho_atualizado;
                firstfit(file,r,&(ca.topoLista));
                fseek(file,fim_regis,SEEK_SET);}}}
    free(buscado);
    free(atualizado);
    free(r);
    atualiza_ca(file,&ca);}
