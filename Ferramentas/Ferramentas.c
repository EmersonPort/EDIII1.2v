#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "../auxiliar/functions.h"

//Emerson Pereira Portela Filho 11800625 

//Carla Nunes da Cruz 8479343

void fill_trash(FILE* fp,int n){// preenche um arquivo com trash n vezes
        for(int i=0;i<n;i++){
            fwrite(trash,sizeof(char),1,fp);
        }
    }


void getstr(FILE* file1,char* string){//pega uma string do  aquivo csv
    char c;
    char s[40];
    int i=0;
    fread(&c,sizeof(char),1,file1);
    while(c!=','){// a leitura vai ate a virgula
        s[i]=c;
        fread(&c,sizeof(char),1,file1);
        i++;
    }
    s[i]='\0';
    strncpy(string,s,i+1);
    //printf("%s\n",string);
}

int potencia(int n,int pot){ // retorna n^(pot) com pot um inteiro positivo
    int r=1;
    for(int i=0;i<pot;i++){
        r=r*n;
    }
    return r;
}

int getint(int* p,FILE* file ) { // pega um inteiro de um arquivo csv
    char s[10];
    int i=0;
    if(fread(&(s[i]),sizeof(char),1,file)!=1) return 0;// retorna 0 se acabou o arquivo
    while(s[i]!=',' && s[i]!='\r'){
        i++;
        fread(&(s[i]),sizeof(char),1,file);//leitura de um carac correspodende ao numero
    }
    int r=0;
    for(int j=0;j<i;j++){
        r= r+ ((int) s[j]- '0')*potencia(10,i-j-1);//já que os primeiros sao os de maior ordem 
    }// - '0' para retorna o valor do digito
    
    if (r==0) *p=-1;
    else{ *p=r;}
    return 1;// retorna 1 se foi tudo bem
}


void insereg(FILE* file,Dados* d,int flag){// insere um registro
    long int tamanho_nomeEstacao =strlen(d->nomeEstacao);
    long int tamanho_nomeLinha =strlen(d->nomeLinha);
    (d->nomeEstacao)[tamanho_nomeEstacao]='|';
    (d->nomeLinha)[tamanho_nomeLinha]='|';
    tamanho_nomeEstacao++;
    tamanho_nomeLinha++;
    d->tamanhoRegistro=32+ tamanho_nomeEstacao + tamanho_nomeLinha;// 32= 6*4 +8 : (todos os outros ints depois do campo tamanhoRegistr) + (campo proxLista)

    if(flag==0){// flag==0 -> ha limitacao de tamanho
        int espaco;
        fwrite(&(d->removido),sizeof(char),1,file);
        fread(&espaco,sizeof(int),1,file);
        fwrite(&(d->proxLista),sizeof(long int),1,file);

        fwrite(&(d->codEstacao),sizeof(int),1,file);
        fwrite(&(d->codLinha),sizeof(int),1,file);    
        fwrite(&(d->codProxEstacao),sizeof(int),1,file);    
        fwrite(&(d->distProxEstacao),sizeof(int),1,file);    
        fwrite(&(d->codLinhaIntegra),sizeof(int),1,file);    
        fwrite(&(d->codEstIntegra),sizeof(int),1,file);
        espaco= espaco- d->tamanhoRegistro;
        
        fwrite(d->nomeEstacao,sizeof(char),tamanho_nomeEstacao,file);
        fwrite(d->nomeLinha,sizeof(char),tamanho_nomeLinha,file);
        fill_trash(file,espaco); }
    else{// flag==1 -> n ha limitacao de tamanho
        fwrite(&(d->removido),sizeof(char),1,file);
        fwrite(&(d->tamanhoRegistro),sizeof(int),1,file);
        fwrite(&(d->proxLista),sizeof(long int),1,file);
        fwrite(&(d->codEstacao),sizeof(int),1,file);
        fwrite(&(d->codLinha),sizeof(int),1,file);    
        fwrite(&(d->codProxEstacao),sizeof(int),1,file);    
        fwrite(&(d->distProxEstacao),sizeof(int),1,file);    
        fwrite(&(d->codLinhaIntegra),sizeof(int),1,file);    
        fwrite(&(d->codEstIntegra),sizeof(int),1,file);       
        fwrite(d->nomeEstacao,sizeof(char),tamanho_nomeEstacao,file);
        fwrite(d->nomeLinha,sizeof(char),tamanho_nomeLinha,file);
        
    }
}

void limpad(Dados* d){
    d->codLinha=-1;
    strcpy(d->nomeLinha,"");
    d->codProxEstacao=-1;
    d->distProxEstacao=-1;
    d->codLinhaIntegra=-1;
    d->codEstIntegra=-1;

}


void printint(int x){
    if(x==-1)printf("NULO ");
    else{printf("%i ",x);}
}


int pegaRegistro(Dados* d,FILE* file){
    char string[50];
    char* sub;
    if(fread(&(d->removido),sizeof(char),1,file)!=1) return 0;
    fread(&(d->tamanhoRegistro),sizeof(int),1,file);
    
    if (d->removido=='1'){
        fseek(file,d->tamanhoRegistro,SEEK_CUR);
        return -1;}
    fread(&(d->proxLista),sizeof(long int),1,file);
    fread(&(d->codEstacao),sizeof(int),1,file);
    fread(&(d->codLinha),sizeof(int),1,file);    
    fread(&(d->codProxEstacao),sizeof(int),1,file);    
    fread(&(d->distProxEstacao),sizeof(int),1,file);    
    fread(&(d->codLinhaIntegra),sizeof(int),1,file);    
    fread(&(d->codEstIntegra),sizeof(int),1,file);
    fread(string,sizeof(char),d->tamanhoRegistro-32,file);
    sub=strtok(string,"|");
    strcpy(d->nomeEstacao,sub);
    sub=strtok(NULL,"|");
    if(sub==NULL) strcpy(d->nomeLinha,"");
    else{strcpy(d->nomeLinha,sub);} 
    return 1;

}

void imprimeRegistro(Dados* d){
        printf("%i ",d->codEstacao);     

        if(strlen(d->nomeEstacao)==0) printf("NULO ");
        else{printf("%s ",d->nomeEstacao);}

        printint(d->codLinha);

        if(strlen(d->nomeLinha)==0) printf("NULO ");
        else{printf("%s ",d->nomeLinha);}
    
        printint(d->codProxEstacao);
        printint(d->distProxEstacao);
        printint(d->codLinhaIntegra);
        printint(d->codEstIntegra);
        printf("\n");

}


void restrinja(Dados* d){
    char campobuscado[15];
    scanf(" %s ",campobuscado);
    //fgets(campobuscado,15,stdin);
    //printf("%s\n",campobuscado);

    if(strcmp(campobuscado,"codEstacao")==0) scanf("%i ",&(d->codEstacao));
    else if(strcmp(campobuscado,"nomeEstacao")==0) scan_quote_string(d->nomeEstacao);
    else if(strcmp(campobuscado,"codLinha")==0) scanf("%i ",&(d->codLinha));
    else if(strcmp(campobuscado,"nomeLinha")==0) scan_quote_string(d->nomeLinha);
    else if(strcmp(campobuscado,"codProxEstacao")==0) scanf("%i ",&(d->codProxEstacao)); 
    else if(strcmp(campobuscado,"distProxEstacao")==0) scanf("%i ",&(d->distProxEstacao));
    else if(strcmp(campobuscado,"codLinhaIntegra")==0) scanf("%i ",&(d->codLinhaIntegra));
    else if(strcmp(campobuscado,"codEstIntegra")==0) scanf("%i ",&(d->codEstIntegra));
    //printf("%s  %i  %s\n",d->nomeEstacao,d->codEstacao,d->nomeLinha);
} 


void generalize(Dados* d){
    d->codEstacao=-2;
    strcpy(d->nomeEstacao,"Sem restrição");
    d->codLinha=-2;
    strcpy(d->nomeLinha,"Sem restrição");
    d->codProxEstacao=-2;
    d->distProxEstacao=-2;
    d->codLinhaIntegra=-2;
    d->codEstIntegra=-2;}

int compativel(Dados* r,Dados* d){
    if(d->codEstacao!=-2 && d->codEstacao!=r->codEstacao) return 0;
    if(strcmp(d->nomeEstacao,"Sem restrição")!=0 && strcmp(d->nomeEstacao, r->nomeEstacao)!=0) return 0;
    if(d->codLinha!=-2 && d->codLinha!=r->codLinha) return 0;
    if(strcmp(d->nomeLinha,"Sem restrição")!=0 && strcmp(d->nomeLinha, r->nomeLinha)!=0) return 0;
    if(d->codProxEstacao!=-2 && d->codProxEstacao!=r->codProxEstacao) return 0;
    if(d->distProxEstacao!=-2 && d->distProxEstacao!=r->distProxEstacao) return 0;
    if(d->codLinhaIntegra!=-2 && d->codLinhaIntegra!=r->codLinhaIntegra) return 0;
    if(d->codEstIntegra!=-2 && d->codEstIntegra!=r->codEstIntegra) return 0;
    return 1;
    
}


void firstfit(FILE* file,Dados* d,long int* topolista){
    long int bts_anterior;
    long int bts_atual=1;
    long int bts_prox=*topolista;
    Dados* r = (Dados*) malloc(sizeof(Dados));

    while(1){
        if(bts_prox==-1){
            fseek(file,0,SEEK_END);
            insereg(file,d,1);
            free(r);
            return;}

        fseek(file,bts_prox,SEEK_SET);
        fread(&(r->removido),sizeof(char),1,file);
        fread(&(r->tamanhoRegistro),sizeof(int),1,file);
        fread(&(r->proxLista),sizeof(long int),1,file);
        bts_anterior = bts_atual;
        bts_atual = bts_prox;
        bts_prox = r->proxLista;
        if(r->tamanhoRegistro>=d->tamanhoRegistro) {
            if(bts_anterior==1){
                *topolista=bts_prox;}
            else {
                fseek(file,bts_anterior+5,SEEK_SET);
                fwrite(&(bts_prox),sizeof(long int),1,file);}

            fseek(file,bts_atual,SEEK_SET);
            insereg(file,d,0);
            free(r);
            return;}}}


int intteclado(FILE* file){
    char c;
    int soma=0;
    scanf(" %c",&c);
    if(c=='N' || c=='n') {
        getchar();
        getchar();
        getchar();
        return -1;}
    soma= (int) (c - '0');
    while(c=getchar(),c!=' ' && c!='\r' && c!='\n'){
        soma= soma*10;
        soma += (int) (c - '0');}
    return soma;

}


void atualiza_ca(FILE* file,Cabecalho* pca){
    fseek(file,17,SEEK_SET);
    pca->nroEstacoes=0;
    pca->nroParesEstacao=0;
    char nomesEstacoes[500][40];
    char pares[500][500]={0};
    Dados* d = (Dados*) malloc(sizeof(Dados));
    int status;
        while((status=pegaRegistro(d,file))!=0){
            if (status==-1) continue;
            int i=0;
            for(;i<pca->nroEstacoes;i++) if(strcmp(nomesEstacoes[i],d->nomeEstacao)==0) break;
            if(i==pca->nroEstacoes) {
                strcpy(nomesEstacoes[i],d->nomeEstacao);
                pca->nroEstacoes++;}
            if(d->codProxEstacao!=-1 && pares[d->codEstacao][d->codProxEstacao]!='1') {
                    pares[d->codEstacao][d->codProxEstacao]='1';
                    pca->nroParesEstacao++;}        
            }
    fseek(file,0,SEEK_SET);
    fwrite(&(pca->status),sizeof(char),1,file);
    fwrite(&(pca->topoLista),sizeof(long int),1,file);
    fwrite(&(pca->nroEstacoes),sizeof(int),1,file);
    fwrite(&(pca->nroParesEstacao),sizeof(int),1,file);
    free(d);}