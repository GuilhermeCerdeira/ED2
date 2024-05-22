#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct processo{
    int codigo;
    char nome[20];
    char estado[20];
    int prioridade;
} processo;

typedef struct no{
    struct processo *processo;
    struct no *esquerda;
    struct no *direita;
    struct no *pai;
} No;

No *criarprocesso(int codigo, char nome[], char estado[], int prioridade);
No* insercao(No* no, int codigo, char nome[], char estado[], int prioridade);  
No *busca(No* no, int codigo);  
No* sucessor(No* no); 
No* remocao(No* no, int chave);
void imprimir(No* no);

int main(){
    No *raiz = NULL;
    FILE *infile = fopen("in.txt", "r");
    FILE *outfile = fopen("out.txt", "w");

    if (infile == NULL || outfile == NULL) {
        perror("Erro ao abrir arquivos.");
        return 1;
    }

    char operacao[20];
    int codigo;
    char nome[20];
    char estado[20];
    int prioridade;

    while (fscanf(infile, "%s", operacao) != EOF) {
        if (strcmp(operacao, "InserirAVL") == 0) {
            fscanf(infile, "%d %s %d %s", &codigo, nome, &prioridade, estado);
            raiz = insercao(raiz, codigo, nome, estado, prioridade);
        } else if (strcmp(operacao, "ListarAVL") == 0) {
            imprimir(raiz);
        } else if (strcmp(operacao, "TerminarAVL") == 0) {
            fscanf(infile, "%d", &codigo);
            raiz = remocao(raiz, codigo);
        } else {
            printf("Essa operacao nao existe\n");
        }
    }

    fclose(infile);
    fclose(outfile);

    return 0;
}

No *criarprocesso(int codigo, char nome[], char estado[], int prioridade){
    No *novo = (No *)malloc(sizeof(No));
    novo->processo = (processo *)malloc(sizeof(processo));
    novo->processo->codigo = codigo;
    strcpy(novo->processo->nome, nome);
    strcpy(novo->processo->estado, estado);
    novo->processo->prioridade = prioridade;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->pai = NULL;

    return novo;
}

No* insercao(No* no, int codigo, char nome[], char estado[], int prioridade){  
    if(no == NULL){
        return criarprocesso(codigo, nome, estado, prioridade); //Cria um novo nó se a árvore estiver vazia
    }

    if(codigo <= no->processo->codigo){
        no->esquerda = insercao(no->esquerda, codigo, nome, estado, prioridade);  // Insere à esquerda se a chave for menor ou igual
        no->esquerda->pai = no;
    }
    else if(codigo > no->processo->codigo){
        no->direita = insercao(no->direita, codigo, nome, estado, prioridade); // Insere à direita se a chave for maior
        no->direita->pai = no;
    }
    
    return no;
}

No *busca(No* no, int codigo){  
    if(no == NULL){
        return NULL; // Retorna NULL se a árvore estiver vazia ou o valor não for encontrado
    }
    if(no->processo->codigo == codigo){
        return no; // Retorna o nó se o valor for encontrado
    }else if(codigo < no->processo->codigo){
        return busca(no->esquerda, codigo); // Busca na subárvore esquerda se a chave for menor
    }else{
        return busca(no->direita, codigo); // Busca na subárvore direita se a chave for maior
    }
}

No* sucessor(No* no){ 
    while(no->esquerda != NULL){
        no = no->esquerda; // Percorre a árvore para a esquerda até encontrar o nó mais à esquerda
    }
    return no; // Retorna o nó mais à esquerda, que é o sucessor
}

No* remocao(No* no, int chave){ 
    if(no == NULL){
        return NULL; // Retorna NULL se a árvore estiver vazia
    }

    if(chave < no->processo->codigo){
        no->esquerda = remocao(no->esquerda, chave); // Remove da subárvore esquerda se a chave for menor
    }else if(chave > no->processo->codigo){
        no->direita = remocao(no->direita, chave); // Remove da subárvore direita se a chave for maior
    }
    else{
        // Caso em que encontramos o nó a ser removido
        if(no->esquerda == NULL){
            No* temp = no->direita;
            free(no->processo);
            free(no);
            return temp; // Se o nó não tiver filho esquerdo, é substituído pelo direito
        }
        else if(no->direita == NULL){
            No* temp = no->esquerda;
            free(no->processo);
            free(no);
            return temp; // Se o nó não tiver filho direito, é substituído pelo esquerdo
        }
        No* temp = sucessor(no->direita); // Encontra o sucessor na árvore direita
        no->processo->codigo = temp->processo->codigo; // Copia o valor do sucessor para o nó a ser removido
        strcpy(no->processo->nome, temp->processo->nome);
        strcpy(no->processo->estado, temp->processo->estado);
        no->processo->prioridade = temp->processo->prioridade;
        no->direita = remocao(no->direita, temp->processo->codigo); // Remove o sucessor
    }
    return no;
}

void imprimir(No* no) {
    if (no == NULL) return;
    imprimir(no->esquerda);
    printf("%d %s %s %d\n", no->processo->codigo, no->processo->nome, no->processo->estado, no->processo->prioridade);
    imprimir(no->direita);
}
