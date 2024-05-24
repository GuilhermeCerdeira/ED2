#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct processo{
    int codigo;
    char nome[20];
    char estado[20];
    int prioridade;
} Processo;

typedef struct heap{
    Processo **vetor;
    int capacidade;
} MaxHeap;

typedef struct no{
    Processo *processo;
    struct no *esquerda;
    struct no *direita;
    struct no *pai;
} No;

void alterarHeap(MaxHeap *heap, No *raiz, int codigo, int prioridade);
No *terminarAVL(No *no, MaxHeap *heap, int chave);
MaxHeap *criarHeap(int capacidade);
void inserirHeap(MaxHeap *heap, Processo *processo);
Processo *removerHeap(MaxHeap *heap, No *raiz);
void imprimirHeap(MaxHeap *heap);
void destruirHeap(MaxHeap *heap);
Processo *criarProcesso(int codigo, char nome[], char estado[], int prioridade);
No *inserirAVL(No *no, Processo *processo);  
No *buscarAVL(No *no, int codigo);
No *removerAVL(No *no, int chave);
void imprimirAVL(No *no);

int main(){
    No *raiz = NULL;
    Processo *processo = NULL;
    MaxHeap *heap = criarHeap(10);

    FILE *infile = fopen("in.txt", "r");
    FILE *outfile = fopen("out.txt", "w");

    if(infile == NULL || outfile == NULL){
        perror("Erro ao abrir arquivos.");
        return 1;
    }

    char operacao[20];
    int codigo;
    char nome[20];
    char estado[20];
    int prioridade;

    while(fscanf(infile, "%s", operacao) != EOF){
        if(strcmp(operacao, "InserirAVL") == 0){
            fscanf(infile, "%d %s %d %s", &codigo, nome, &prioridade, estado);
            processo = criarProcesso(codigo, nome, estado, prioridade);
            raiz = inserirAVL(raiz, processo);
            inserirHeap(heap, processo);
        } else if(strcmp(operacao, "ListarAVL") == 0){
            imprimirAVL(raiz);
        } else if(strcmp(operacao, "TerminarAVL") == 0){
            raiz = terminarAVL(raiz, heap, codigo);
        } else if(strcmp(operacao, "AlterarHeap") == 0){
            fscanf(infile, "%d %d", &codigo, &prioridade);
            alterarHeap(heap, raiz, codigo, prioridade);
        } else if(strcmp(operacao, "ListarHeap") == 0){
            imprimirHeap(heap);
        } else if(strcmp(operacao, "RemoverHeap") == 0){
            removerHeap(heap, raiz);
        }else{
            printf("Essa operacao nao existe\n");
        }
    }

    fclose(infile);
    fclose(outfile);

    return 0;
}

void troca(Processo **v1, Processo **v2){
    Processo *aux = *v1;
    *v1 = *v2;
    *v2 = aux;
}

void heapify(MaxHeap *heap, int i){
    int maior = i;
    int filhoEsquerda = 2 * i;
    int filhoDireita = 2 * i + 1;

    if(filhoEsquerda <= heap->vetor[0]->prioridade && heap->vetor[filhoEsquerda]->prioridade > heap->vetor[maior]->prioridade){
        maior = filhoEsquerda;
    }

    if(filhoDireita <= heap->vetor[0]->prioridade && heap->vetor[filhoDireita]->prioridade > heap->vetor[maior]->prioridade){
        maior = filhoDireita;
    }

    if(maior != i){
        troca(&heap->vetor[i], &heap->vetor[maior]);
        heapify(heap, maior);
    }
}

int buscarPosicaoHeap(MaxHeap *heap, int chave){
    if(heap != NULL && heap->vetor != NULL){
        if(heap->vetor[0]->prioridade != 0){
            for(int i = 1; i <= heap->capacidade; i++){
                if(heap->vetor[i]->prioridade == chave){
                    return i;
                }
            }
        }
        return -1;
    }
    return -1;
}

void alterarHeap(MaxHeap *heap, No *raiz, int codigo, int prioridade){
    No *no = buscarAVL(raiz, codigo);
    if(no != NULL){
        int i = buscarPosicaoHeap(heap, no->processo->prioridade);
        if(i != -1){
            heap->vetor[i]->prioridade = prioridade;
            heapify(heap, i);
        }
    }
}

Processo *removerHeapChave(MaxHeap *heap, int chave){
    if(heap != NULL && heap->vetor != NULL){
        if(heap->vetor[0]->prioridade == 0){
            printf("Heap vazio!\n");
            return NULL;
        }
        
        Processo *raiz = heap->vetor[chave];
        heap->vetor[chave] = heap->vetor[heap->vetor[0]->prioridade];
        heap->vetor[0]->prioridade--;
        heapify(heap, chave);
        return raiz;
    }
    return NULL;
}

No *terminarAVL(No *raiz, MaxHeap *heap, int chave){
    if(raiz){
        No *no = buscarAVL(raiz, chave);
        if(no){
            int i = buscarPosicaoHeap(heap, no->processo->prioridade);
            if(i != -1){
                removerHeapChave(heap, i);
                heapify(heap, i);
            }
            return removerAVL(raiz, chave);
        }
        return raiz;
    }
    return raiz;
}

MaxHeap *criarHeap(int capacidade){
    MaxHeap *heap =(MaxHeap*) malloc(sizeof(MaxHeap));
    heap->vetor =(Processo**) malloc((capacidade + 1)  *sizeof(Processo*));
    heap->capacidade = capacidade;
    heap->vetor[0] = NULL;
    heap->vetor[0] =(Processo*) malloc(sizeof(Processo));
    heap->vetor[0]->prioridade = 0;
    return heap;
}

void inserirHeap(MaxHeap *heap, Processo *processo){
    if(heap != NULL && heap->vetor != NULL){
        if(heap->vetor[0]->prioridade == 0){
            heap->vetor[1] = processo;
            heap->vetor[0]->prioridade++;
        }
        else{
            if(heap->vetor[0]->prioridade == heap->capacidade){
                Processo **vet = realloc(heap->vetor,(heap->capacidade + 1) * 2 * sizeof(Processo*));
                if(vet != NULL){
                    heap->vetor = vet;
                    heap->capacidade *= 2;
                } else{
                    printf("Heap cheio! Não é possível inserir mais elementos.\n");
                    return;
                }
            }
            heap->vetor[0]->prioridade++;
            int i = heap->vetor[0]->prioridade;
            int pai = i / 2;
            heap->vetor[i] = processo;

            while(i > 1 && heap->vetor[i]->prioridade > heap->vetor[pai]->prioridade){
                troca(&heap->vetor[i], &heap->vetor[pai]);
                i = pai;
                pai = i / 2;
            }
        }
    }
}

Processo *removerHeap(MaxHeap *heap, No *raiz){
    if(heap != NULL && heap->vetor != NULL){
        if(heap->vetor[0]->prioridade == 0){
            printf("Heap vazio!\n");
            return NULL;
        }
        Processo *jorge = heap->vetor[1];
        raiz = removerAVL(raiz, heap->vetor[1]->codigo);
        heap->vetor[1] = heap->vetor[heap->vetor[0]->prioridade];
        heap->vetor[0]->prioridade--;
        heapify(heap, 1);
        return jorge;
    }
    return NULL;
}

void imprimirHeap(MaxHeap *heap){
    if(heap != NULL && heap->vetor != NULL){
        if(heap->vetor[0]->prioridade == 0){
            printf("Heap vazio!\n");
        } else{
            printf("Heap: ");
            for(int i = 1; i <= heap->vetor[0]->prioridade; i++){
                printf("%d ", heap->vetor[i]->prioridade);
            }
            printf("\n");
        }
    }
}

void destruirHeap(MaxHeap *heap){
    free(heap->vetor);
    free(heap);
}

Processo *criarProcesso(int codigo, char nome[], char estado[], int prioridade){
    Processo *novo =(Processo *)malloc(sizeof(Processo));
    novo->codigo = codigo;
    strcpy(novo->nome, nome);
    strcpy(novo->estado, estado);
    novo->prioridade = prioridade;

    return novo;
}

No *criarNo(Processo *processo){
    No *novo =(No *)malloc(sizeof(No));
    novo->processo = processo;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->pai = NULL;

    return novo;
}

No *inserirAVL(No *no, Processo *processo){  
    if(no == NULL){
        return criarNo(processo); //Cria um novo nó se a árvore estiver vazia
    }

    if(processo->codigo <= no->processo->codigo){
        no->esquerda = inserirAVL(no->esquerda, processo);  // Insere à esquerda se a chave for menor ou igual
        no->esquerda->pai = no;
    }
    else if(processo->codigo > no->processo->codigo){
        no->direita = inserirAVL(no->direita, processo); // Insere à direita se a chave for maior
        no->direita->pai = no;
    }
    
    return no;
}

No *buscarAVL(No *no, int codigo){  
    if(no == NULL){
        return NULL; // Retorna NULL se a árvore estiver vazia ou o valor não for encontrado
    }
    if(no->processo->codigo == codigo){
        return no; // Retorna o nó se o valor for encontrado
    }else if(codigo < no->processo->codigo){
        return buscarAVL(no->esquerda, codigo); // buscarAVL na subárvore esquerda se a chave for menor
    }else{
        return buscarAVL(no->direita, codigo); // buscarAVL na subárvore direita se a chave for maior
    }
}

No *sucessor(No *no){ 
    while(no->esquerda != NULL){
        no = no->esquerda; // Percorre a árvore para a esquerda até encontrar o nó mais à esquerda
    }
    return no; // Retorna o nó mais à esquerda, que é o sucessor
}

No *removerAVL(No *no, int chave){ 
    if(no == NULL){
        return NULL; // Retorna NULL se a árvore estiver vazia
    }

    if(chave < no->processo->codigo){
        no->esquerda = removerAVL(no->esquerda, chave); // Remove da subárvore esquerda se a chave for menor
    }else if(chave > no->processo->codigo){
        no->direita = removerAVL(no->direita, chave); // Remove da subárvore direita se a chave for maior
    }
    else{
        // Caso em que encontramos o nó a ser removido
        if(no->esquerda == NULL){
            No *temp = no->direita;
            free(no->processo);
            free(no);
            return temp; // Se o nó não tiver filho esquerdo, é substituído pelo direito
        }
        else if(no->direita == NULL){
            No *temp = no->esquerda;
            free(no->processo);
            free(no);
            return temp; // Se o nó não tiver filho direito, é substituído pelo esquerdo
        }
        No *temp = sucessor(no->direita); // Encontra o sucessor na árvore direita
        no->processo->codigo = temp->processo->codigo; // Copia o valor do sucessor para o nó a ser removido
        strcpy(no->processo->nome, temp->processo->nome);
        strcpy(no->processo->estado, temp->processo->estado);
        no->processo->prioridade = temp->processo->prioridade;
        no->direita = removerAVL(no->direita, temp->processo->codigo); // Remove o sucessor
    }
    return no;
}

void imprimirAVL(No *no){
    if(no == NULL) return;
    imprimirAVL(no->esquerda);
    printf("%d %s %s %d\n", no->processo->codigo, no->processo->nome, no->processo->estado, no->processo->prioridade);
    imprimirAVL(no->direita);
}