#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "../auxiliar/functions.h"

//Emerson Pereira Portela Filho 11800625 

//Carla Nunes da Cruz 8479343

void fill_trash(FILE* fp,int n){
        for(int i=0;i<n;i++){
            fwrite(trash,sizeof(char),1,fp);
        }
    }


int getstr(FILE* file1,char* string){
    char c;
    char s[40];
    int i=0;
    fread(&c,sizeof(char),1,file1);
    while(c!=','){
        s[i]=c;
        fread(&c,sizeof(char),1,file1);
        i++;
    }
    s[i]='|';
    i++;
    s[i]='\0';
    strncpy(string,s,i+1);
    //printf("%s\n",string);
    return i;
}

int potencia(int n,int pot){
    int r=1;
    for(int i=0;i<pot;i++){
        r=r*n;
    }
    return r;
}

int getint(int* p,FILE* file ) {
    char s[10];
    int i=0;
    if(fread(&(s[i]),sizeof(char),1,file)!=1) return 0;
    while(s[i]!=',' && s[i]!='\r'){
        i++;
        fread(&(s[i]),sizeof(char),1,file);
    }
    int r=0;
    for(int j=0;j<i;j++){
        r= r+ ((int) s[j]- '0')*potencia(10,i-j-1);
    }
    
    if (r==0) *p=-1;
    else{ *p=r;}
    return 1;
}


void imprime(FILE* file){
    char c;
    while(fread(&c,sizeof(char),1,file)==1) printf("%c",c);

}

void aplicacao1(FILE* file1,FILE* file2){
    fseek(file1,101,SEEK_SET);
    //fseek(file2,17,SEEK_SET);
    Dados* d = (Dados*) malloc(sizeof(Dados));
    d->removido='0';
    d->proxLista=-1;
    Cabecalho ca;
    ca.status='1';
    ca.topoLista=-1;
    ca.nroEstacoes=0;
    ca.nroParesEstacao=0;
    fwrite(&(ca.status),sizeof(char),1,file2);
    fwrite(&(ca.topoLista),sizeof(long int),1,file2);
    fwrite(&(ca.nroEstacoes),sizeof(int),1,file2);
    fwrite(&(ca.nroParesEstacao),sizeof(int),1,file2);
    short int vestacoes[1000]={0};
    char c;
    while(getint(&(d->codEstacao),file1)==1){
        printf("%d\n",d->codEstacao);
        if(vestacoes[d->codEstacao]==0){
            vestacoes[d->codEstacao]=1;
            ca.nroEstacoes++;}
        int tamanho_nomeEstacao= getstr(file1,d->nomeEstacao);
        getint(&(d->codLinha),file1);
        //printf("%d\n",d->codLinha);
        int tamanho_nomeLinha= getstr(file1,d->nomeLinha);
        getint(&(d->codProxEstacao),file1);
        //printf("%d\n",d->codProxEstacao);
        getint(&(d->distProxEstacao),file1);
        //printf("%d\n",d->distProxEstacao);
        getint(&(d->codLinhaIntegra),file1);
        //printf("%d\n",d->codLinhaIntegra);
        getint(&(d->codEstIntegra),file1);
        //printf("%d\n",d->codEstIntegra);
        fread(&c,sizeof(char),1,file1);
      
        d->tamanhoRegistro=32+ tamanho_nomeLinha + tamanho_nomeEstacao;
        if(d->codProxEstacao!=-1) ca.nroParesEstacao++;
        
        fwrite(&(d->removido),sizeof(char),1,file2);
        fwrite(&(d->tamanhoRegistro),sizeof(int),1,file2);
        fwrite(&(d->proxLista),sizeof(long int),1,file2);
        fwrite(&(d->codEstacao),sizeof(int),1,file2);
        fwrite(&(d->codLinha),sizeof(int),1,file2);
        fwrite(&(d->codProxEstacao),sizeof(int),1,file2);
        fwrite(&(d->distProxEstacao),sizeof(int),1,file2);
        fwrite(&(d->codLinhaIntegra),sizeof(int),1,file2);
        fwrite(&(d->codEstIntegra),sizeof(int),1,file2);
        fwrite(d->nomeEstacao,sizeof(char),tamanho_nomeEstacao,file2);
        fwrite(d->nomeLinha,sizeof(char),tamanho_nomeLinha,file2);}

    fseek(file2,0,SEEK_SET);
    ca.status='0';
    fwrite(&(ca.status),sizeof(char),1,file2);
    fwrite(&(ca.topoLista),sizeof(long int),1,file2);
    fwrite(&(ca.nroEstacoes),sizeof(int),1,file2);
    fwrite(&(ca.nroParesEstacao),sizeof(int),1,file2);
    free(d);}

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
    strcpy(d->nomeLinha,sub);
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



void aplicacao2(FILE* file){
    Cabecalho ca;
    fread(&(ca.status),sizeof(char),1,file);
    fread(&(ca.topoLista),sizeof(long int),1,file);
    fread(&(ca.nroEstacoes),sizeof(int),1,file);
    fread(&(ca.nroParesEstacao),sizeof(int),1,file);
    //fseek(file,17,SEEK_SET);
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



void aplicacao3(FILE* file){
    int n;
    scanf("%i\n",&n);
    Dados* d = (Dados*) malloc(sizeof(Dados));
    generalize(d);
    
    for(int i=0;i<n;i++) restrinja(d);
    //int codEstacao=0;
    //char nomeEstacao[40];
    //char nomeLinha[40];

    //char cod[]="codEstacao";
    //char nEstacao[]="nomeEstacao";
    //char nLinha[]="nomeLinha";
    Dados* r = (Dados*) malloc(sizeof(Dados));
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
    short int Matestacoes[500][500]={0};
    
    for(int i=0;i<n;i++){
        if(i>0) fseek(file,17,SEEK_SET);
        generalize(d);
        scanf("%i ",&x);
        for(int j=0;j<x;j++) restrinja(d);
        int status;
        while((status=pegaRegistro(r,file))!=0){
            if (status==-1) continue;
            if (compativel(r,d)!=1){
                if(r->codProxEstacao==-1)Matestacoes[r->codEstacao][0]=1;
                else {Matestacoes[r->codEstacao][r->codProxEstacao]=1;}
                continue;
            } 
            ca.nroEstacoes--;
            r->removido='1';
            r->proxLista=ca.topoLista;
            //if(r->codProxEstacao!=-1) ca.nroParesEstacao--;
            //printf("%ld\n",ftell(file));
            ca.topoLista = ftell(file)-r->tamanhoRegistro -5;
            fseek(file,ca.topoLista,SEEK_SET);
            //printf("%d %ld\n",r->tamanhoRegistro,ftell(file));
            //printf("%c %d %ld",r->removido,r->tamanhoRegistro,r->proxLista);
            //printf("%ld\n",ca.topoLista);
            fread(&(r->removido),sizeof(char),1,file);
            fread(&(r->tamanhoRegistro),sizeof(int),1,file);
            fread(&(r->proxLista),sizeof(long int),1,file);
            //printf("\t\t%c %d %ld\n",r->removido,r->tamanhoRegistro,r->proxLista);
            fseek(file,r->tamanhoRegistro-8,SEEK_CUR);}}


    free(d);
    free(r);
    //ca.nroEstacoes=0;
    ca.nroParesEstacao=0;
    for(int i=0;i<500;i++){
        int somadepares=0;
        for(int j=1;j<500;j++){
            somadepares+=Matestacoes[i][j];}
        ca.nroParesEstacao+=somadepares;
        somadepares+=Matestacoes[i][0];}
        //if(somadepares>0) ca.nroEstacoes++;}
    fseek(file,0,SEEK_SET);
    fwrite(&(ca.status),sizeof(char),1,file);
    fwrite(&(ca.topoLista),sizeof(long int),1,file);
    fwrite(&(ca.nroEstacoes),sizeof(int),1,file);
    fwrite(&(ca.nroParesEstacao),sizeof(int),1,file);
    

}


void insereg(FILE* file,Dados* d){
    int num_lixo;
    fwrite(&(d->removido),sizeof(char),1,file);
    fread(&num_lixo,sizeof(int),1,file);
    fwrite(&(d->proxLista),sizeof(long int),1,file);
    
    fwrite(&(d->codEstacao),sizeof(int),1,file);
    fwrite(&(d->codLinha),sizeof(int),1,file);    
    fwrite(&(d->codProxEstacao),sizeof(int),1,file);    
    fwrite(&(d->distProxEstacao),sizeof(int),1,file);    
    fwrite(&(d->codLinhaIntegra),sizeof(int),1,file);    
    fwrite(&(d->codEstIntegra),sizeof(int),1,file);
    num_lixo= num_lixo- d->tamanhoRegistro;
    long int tamanho_nomeEstacao =strlen(d->nomeEstacao);
    long int tamanho_nomeLinha =strlen(d->nomeLinha);
    (d->nomeEstacao)[tamanho_nomeEstacao]='|';
    tamanho_nomeEstacao++;
    (d->nomeLinha)[tamanho_nomeLinha]='|';
    tamanho_nomeLinha++;
    fwrite(d->nomeEstacao,sizeof(char),tamanho_nomeEstacao,file);
    fwrite(d->nomeLinha,sizeof(char),tamanho_nomeLinha,file);
    fill_trash(file,num_lixo);  

}



void firstfit(FILE* file,Dados* d,long int* topolista){
    long int bts_anterior;
    long int bts_atual=1;
    long int bts_prox=*topolista;
    Dados* r = (Dados*) malloc(sizeof(Dados));

    while(1){
        if(bts_prox==-1){
            fseek(file,0,SEEK_END);
            insereg(file,d);
            break;}

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
            insereg(file,d);
            break;}}}


void aplicacao5(FILE* file){
    int n;
    int x;
    scanf("%i\n",&n);
    Cabecalho ca;
    fread(&(ca.status),sizeof(char),1,file);
    //printf("%ld\n",ftell(file));
    fread(&(ca.topoLista),sizeof(long int),1,file);
    fread(&(ca.nroEstacoes),sizeof(int),1,file);
    fread(&(ca.nroParesEstacao),sizeof(int),1,file);
    Dados* d = (Dados*) malloc(sizeof(Dados));
    d->removido='0';
    d->proxLista=-1;
    for(int i=0;i<n;i++) {
    scanf("%i ",&(d->codEstacao));
    scan_quote_string(d->nomeEstacao);
    scanf("%i ",&(d->codLinha));
    scan_quote_string(d->nomeLinha);
    scanf("%i ",&(d->codProxEstacao)); 
    scanf("%i ",&(d->distProxEstacao));
    scanf("%i ",&(d->codLinhaIntegra));
    scanf("%i ",&(d->codEstIntegra));
    long int tamanho_nomeEstacao =strlen(d->nomeEstacao)+1;
    long int tamanho_nomeLinha =strlen(d->nomeLinha)+1;
    d->tamanhoRegistro=32+ tamanho_nomeEstacao + tamanho_nomeLinha;

    firstfit(file,d,&(ca.topoLista));
    ca.nroEstacoes++;}
    fseek(file,0,SEEK_SET);
    fwrite(&(ca.status),sizeof(char),1,file);
    fwrite(&(ca.topoLista),sizeof(long int),1,file);
    fwrite(&(ca.nroEstacoes),sizeof(int),1,file);
    fwrite(&(ca.nroParesEstacao),sizeof(int),1,file);}




















int pegaRegistro2(Dados* d,FILE* file){
    char string[50];
    char* sub;
    if(fread(&(d->removido),sizeof(char),1,file)!=1) return 0;
    fread(&(d->tamanhoRegistro),sizeof(int),1,file);
    
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
    strcpy(d->nomeLinha,sub);
    return 1;

}

void imprimeRegistro2(Dados* d){
        printf("%c %i %ld ",d->removido,d->tamanhoRegistro,d->proxLista);
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

void imprimet(FILE* file){
    Cabecalho ca;
    fread(&(ca.status),sizeof(char),1,file);
    fread(&(ca.topoLista),sizeof(long int),1,file);
    fread(&(ca.nroEstacoes),sizeof(int),1,file);
    fread(&(ca.nroParesEstacao),sizeof(int),1,file);
    printf("%c\t\t%ld\t\t%d\t\t%d\n\n",ca.status,ca.topoLista,ca.nroEstacoes,ca.nroParesEstacao);
    Dados* r = (Dados*) malloc(sizeof(Dados));
    while(pegaRegistro2(r,file)!=0){
            //if (status==-1) continue;
            imprimeRegistro2(r);           
            
            }
    free(r);}
