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


void imprime(FILE* file){
    char c;
    while(fread(&c,sizeof(char),1,file)==1) printf("%c",c);

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
    char nomesEstacoes[500][40];
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
        if(d->codProxEstacao!=-1) ca.nroParesEstacao++;
        int j=0;
        for(;j<ca.nroEstacoes;j++) if(strcmp(d->nomeEstacao,nomesEstacoes[j])==0) break;
        if(j==ca.nroEstacoes) {
            strcpy(nomesEstacoes[j],d->nomeEstacao);
            ca.nroEstacoes++;}
        insereg(file2,d,1);//insere registro sem limitacao de espaco
        limpad(d);}

    fseek(file2,0,SEEK_SET);//voltamos para atualizar o cabecalho 
    ca.status='1';
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
    char nomesEstacoes[500][40];
    ca.nroEstacoes=0;
    
    for(int i=0;i<n;i++){
        if(i>0) fseek(file,17,SEEK_SET);
        generalize(d);
        scanf("%i ",&x);
        for(int j=0;j<x;j++) restrinja(d);
        int status;
        while((status=pegaRegistro(r,file))!=0){
            if (status==-1) continue;
            if (compativel(r,d)!=1){
                if(i==n-1){
                    int k=0;
                    for(;k<ca.nroEstacoes;k++) if(strcmp(nomesEstacoes[k],r->nomeEstacao)==0)break;
                    if(k==ca.nroEstacoes){ 
                        strcpy(nomesEstacoes[k],r->nomeEstacao);
                        ca.nroEstacoes++;}
                }             
                continue;
            } 
            if(r->codProxEstacao!=-1) ca.nroParesEstacao--;
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
    fseek(file,0,SEEK_SET);
    fwrite(&(ca.status),sizeof(char),1,file);
    fwrite(&(ca.topoLista),sizeof(long int),1,file);
    fwrite(&(ca.nroEstacoes),sizeof(int),1,file);
    fwrite(&(ca.nroParesEstacao),sizeof(int),1,file);
    

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
    if(sub==NULL) strcpy(d->nomeLinha,"");
    else{strcpy(d->nomeLinha,sub);}    
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
        

}

void imprimet(FILE* file){
    Cabecalho ca;
    fread(&(ca.status),sizeof(char),1,file);
    fread(&(ca.topoLista),sizeof(long int),1,file);
    fread(&(ca.nroEstacoes),sizeof(int),1,file);
    fread(&(ca.nroParesEstacao),sizeof(int),1,file);
    printf("%c\t\t%ld\t\t%d\t\t%d\n\n",ca.status,ca.topoLista,ca.nroEstacoes,ca.nroParesEstacao);
    Dados* r = (Dados*) malloc(sizeof(Dados));
    int i=1;
    while(pegaRegistro2(r,file)!=0){
            //if (status==-1) continue;
            imprimeRegistro2(r);
            printf(" Posição: %d\n",i);
            i++;}
    free(r);}




    void erronatela(FILE* f1,char *str2){
        long int t1,t2,erro;
        char c1;
        char c2;
        int pipes=0;
        int v1,v2;
        FILE* f2 = fopen(str2,"rb");
             
        fseek(f1, 0, SEEK_END);
	    t1 = ftell(f1);
        fseek(f2, 0, SEEK_END);
	    t2 = ftell(f2);
        fseek(f1, 0, SEEK_SET);
        fseek(f2, 0, SEEK_SET);
        for (int i=0;i<t1 && i<t2;i++){
            fread(&c1,1,1,f1);
            fread(&c2,1,1,f2);
            if(c1=='|') pipes++;
            if(c1!=c2){
                erro=ftell(f1)-1;
                printf("%c\t%c\t\t%li\n",c1,c2,erro);
                fseek(f1,-1,SEEK_CUR);
                fseek(f2,-1,SEEK_CUR);
                fread(&v1,4,1,f1);
                fread(&v2,4,1,f2);
                printf("Pipes: %d \n%d\t%d\n",pipes,v1,v2);

                break;}

        }
        fclose(f2);
    }
