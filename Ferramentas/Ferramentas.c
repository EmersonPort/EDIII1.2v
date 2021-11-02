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
    (d->nomeEstacao)[tamanho_nomeEstacao]='|';// poe o | para insercao
    (d->nomeLinha)[tamanho_nomeLinha]='|';
    tamanho_nomeEstacao++;
    tamanho_nomeLinha++;
    d->tamanhoRegistro=32+ tamanho_nomeEstacao + tamanho_nomeLinha;// 32= 6*4 +8 : (todos os outros ints depois do campo tamanhoRegistr) + (campo proxLista)

    if(flag==0){// flag==0 -> ha limitacao de tamanho
        int espaco;
        fwrite(&(d->removido),sizeof(char),1,file);
        fread(&espaco,sizeof(int),1,file);// Faz a leitura(fread) do espaco disponivel
        fwrite(&(d->proxLista),sizeof(long int),1,file);

        fwrite(&(d->codEstacao),sizeof(int),1,file);
        fwrite(&(d->codLinha),sizeof(int),1,file);    
        fwrite(&(d->codProxEstacao),sizeof(int),1,file);    
        fwrite(&(d->distProxEstacao),sizeof(int),1,file);    
        fwrite(&(d->codLinhaIntegra),sizeof(int),1,file);    
        fwrite(&(d->codEstIntegra),sizeof(int),1,file);
        espaco= espaco- d->tamanhoRegistro;//Agora , o espaco que resta sera usado pelo trash
        
        fwrite(d->nomeEstacao,sizeof(char),tamanho_nomeEstacao,file);
        fwrite(d->nomeLinha,sizeof(char),tamanho_nomeLinha,file);
        fill_trash(file,espaco); }
    else{// flag==1 -> n ha limitacao de tamanho
        fwrite(&(d->removido),sizeof(char),1,file);//Simplesmente escreve todos os dados no arquivo
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

void limpad(Dados* d){// Deixa os dados do registro d zerados(Apenas os que podem assumir esse valor)
    d->codLinha=-1;
    strcpy(d->nomeLinha,"");
    d->codProxEstacao=-1;
    d->distProxEstacao=-1;
    d->codLinhaIntegra=-1;
    d->codEstIntegra=-1;}


void printint(int x){// Imprime o valor , caso nao seja nulo
    if(x==-1)printf("NULO ");
    else{printf("%i ",x);}
}


int pegaRegistro(Dados* d,FILE* file){//Pega o registro seguinte e poe em d
    char string[50];
    char* sub;
    if(fread(&(d->removido),sizeof(char),1,file)!=1) return 0;//caso nao consiga puxar dados do aquivo retorna 0 (ERRO)
    fread(&(d->tamanhoRegistro),sizeof(int),1,file);
    
    if (d->removido=='1'){
        fseek(file,d->tamanhoRegistro,SEEK_CUR);//Se estiver removido pula pro proximo
        return -1;}// -1 indica que o registro foi pulado
    fread(&(d->proxLista),sizeof(long int),1,file);
    fread(&(d->codEstacao),sizeof(int),1,file);
    fread(&(d->codLinha),sizeof(int),1,file);    
    fread(&(d->codProxEstacao),sizeof(int),1,file);    
    fread(&(d->distProxEstacao),sizeof(int),1,file);    
    fread(&(d->codLinhaIntegra),sizeof(int),1,file);    
    fread(&(d->codEstIntegra),sizeof(int),1,file);
    fread(string,sizeof(char),d->tamanhoRegistro-32,file);
    sub=strtok(string,"|");// Leitura ate o |
    strcpy(d->nomeEstacao,sub);
    sub=strtok(NULL,"|");
    if(sub==NULL) strcpy(d->nomeLinha,"");// sub recebe NULL caso tenha ||
    else{strcpy(d->nomeLinha,sub);} 
    return 1;//Tudo ok

}

void imprimeRegistro(Dados* d){// imprime todos os dados do registro
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


void restrinja(Dados* d){//restrinje os dados do registro
    char campobuscado[15];
    scanf(" %s ",campobuscado);
    // dependendo do campobuscado o scan sera diferente
    if(strcmp(campobuscado,"codEstacao")==0) scanf("%i ",&(d->codEstacao));
    else if(strcmp(campobuscado,"nomeEstacao")==0) scan_quote_string(d->nomeEstacao);
    else if(strcmp(campobuscado,"codLinha")==0) scanf("%i ",&(d->codLinha));
    else if(strcmp(campobuscado,"nomeLinha")==0) scan_quote_string(d->nomeLinha);
    else if(strcmp(campobuscado,"codProxEstacao")==0) scanf("%i ",&(d->codProxEstacao)); 
    else if(strcmp(campobuscado,"distProxEstacao")==0) scanf("%i ",&(d->distProxEstacao));
    else if(strcmp(campobuscado,"codLinhaIntegra")==0) scanf("%i ",&(d->codLinhaIntegra));
    else if(strcmp(campobuscado,"codEstIntegra")==0) scanf("%i ",&(d->codEstIntegra));} 


void generalize(Dados* d){// Generaliza os dados : -2 eh tido como valor sem restricao
    d->codEstacao=-2;// -2 nao conta como elemento buscado
    strcpy(d->nomeEstacao,"Sem restrição");// Sem restricao tb n
    d->codLinha=-2;
    strcpy(d->nomeLinha,"Sem restrição");
    d->codProxEstacao=-2;
    d->distProxEstacao=-2;
    d->codLinhaIntegra=-2;
    d->codEstIntegra=-2;}

int compativel(Dados* r,Dados* d){// retorna 1 se r eh compativel com d e 0 se n for
    if(d->codEstacao!=-2 && d->codEstacao!=r->codEstacao) return 0;// Se d n tiver valor definido (tudo igual a -2) qualquer r eh compativel
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
    long int bts_anterior;//byte offsets
    long int bts_atual=1;
    long int bts_prox=*topolista;
    Dados* r = (Dados*) malloc(sizeof(Dados));

    while(1){//O while fica ate ter um return
        if(bts_prox==-1){// So poe no final e n muda a lista
            fseek(file,0,SEEK_END);
            insereg(file,d,1);//flag=1 pq n tem limite de tamnho
            free(r);
            return;}

        fseek(file,bts_prox,SEEK_SET);//leitura do proximo byte offset
        fread(&(r->removido),sizeof(char),1,file);
        fread(&(r->tamanhoRegistro),sizeof(int),1,file);
        fread(&(r->proxLista),sizeof(long int),1,file);
        bts_anterior = bts_atual;//atualiza o byte offset
        bts_atual = bts_prox;
        bts_prox = r->proxLista;
        if(r->tamanhoRegistro>=d->tamanhoRegistro) {
            if(bts_anterior==1){// 1 eh bts de cabecalho
                *topolista=bts_prox;}
            else {
                fseek(file,bts_anterior+5,SEEK_SET);//mudando o prox bts do registro anterior
                fwrite(&(bts_prox),sizeof(long int),1,file);}

            fseek(file,bts_atual,SEEK_SET);
            insereg(file,d,0);
            free(r);
            return;}}}


int intteclado(FILE* file){// leitura de inteiro do teclado, se n for "NULO".
    char c;
    int soma=0;
    scanf(" %c",&c);
    if(c=='N' || c=='n') {//pegando string NULO
        getchar();
        getchar();
        getchar();
        return -1;}
    soma= (int) (c - '0');
    while(c=getchar(),c!=' ' && c!='\r' && c!='\n'){// pegando numero
        soma= soma*10;
        soma += (int) (c - '0');}
    return soma;

}


void atualiza_ca(FILE* file,Cabecalho* pca){
    fseek(file,17,SEEK_SET);//bts q termina o cabecalho
    pca->nroEstacoes=0;
    pca->nroParesEstacao=0;
    char nomesEstacoes[500][40];//matriz de nomes de estacoes
    char pares[500][500]={0};// matriz de pares zerada
    Dados* d = (Dados*) malloc(sizeof(Dados));
    int status;
        while((status=pegaRegistro(d,file))!=0){
            if (status==-1) continue;
            int i=0;
            for(;i<pca->nroEstacoes;i++) if(strcmp(nomesEstacoes[i],d->nomeEstacao)==0) break;
            if(i==pca->nroEstacoes) {//:se tiver terminado o for sem o break
                strcpy(nomesEstacoes[i],d->nomeEstacao);// aumenta a lista de estacoes
                pca->nroEstacoes++;}
            if(d->codProxEstacao!=-1 && pares[d->codEstacao][d->codProxEstacao]!='1') {// Se o par n tiver marcado
                    pares[d->codEstacao][d->codProxEstacao]='1';//marca o par
                    pca->nroParesEstacao++;}  // add o par      
            }
    fseek(file,0,SEEK_SET);//volta e atualiza o cabecalho no arquivo
    fwrite(&(pca->status),sizeof(char),1,file);
    fwrite(&(pca->topoLista),sizeof(long int),1,file);
    fwrite(&(pca->nroEstacoes),sizeof(int),1,file);
    fwrite(&(pca->nroParesEstacao),sizeof(int),1,file);
    free(d);}