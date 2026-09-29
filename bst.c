#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

typedef struct node { 
    int dado; 
    struct node *esquerda;
    struct node *direita;
} node;

node *criarnode(int dado){
    node* resultado = malloc(sizeof(node));
    if (resultado != NULL){
        resultado->esquerda = NULL;
        resultado->direita = NULL;
        resultado->dado = dado;
    }
    return resultado;
}

void printtabs(int tabs){ 
    for (int i = 0; i < tabs; i++){
        printf("\t"); 
    }
}

void printnode(node *raiz, int nivel){ 
    if (raiz == NULL){
        printtabs(nivel); 
        printf("--<vazio>--\n");
        return;
    }

    printtabs(nivel);
    printf("dado = %d\n", raiz->dado);
    
    printtabs(nivel);
    printf("esquerda:\n");
    printnode(raiz->esquerda, nivel+1);
    
    printtabs(nivel);
    printf("direita:\n");
    
    printnode(raiz->direita, nivel +1);
    printtabs(nivel);
    printf("~exito~\n");
  
}

void preordem(node *raiz){ 
    if (raiz == NULL){
        return;
    }

    printf("%d ", raiz->dado); 
    preordem(raiz->esquerda);
    preordem(raiz->direita);
}

void emordem(node *raiz){ 
     if (raiz == NULL){
        return;
    }

    emordem(raiz->esquerda); 
    printf("%d ", raiz->dado); 
    emordem(raiz->direita);
}

void posordem(node *raiz){
    if (raiz == NULL){
        return;
    }

    posordem(raiz->esquerda); 
    posordem(raiz->direita); 
    printf("%d ", raiz->dado); 
}

bool inserir(node **raizptr, int dado){ 
    node *raiz = *raizptr;
    if (raiz == NULL){ 
        (*raizptr) = criarnode(dado); 
        return true; 
    }

    if (dado == raiz->dado){ 
        return false;
    }

    if (dado < raiz->dado){ 
        return inserir(&(raiz->esquerda), dado);

    } else {
        return inserir(&(raiz->direita), dado);
    }

}

bool retirar(node **raizptr, int dado){
    node *raiz = *raizptr;

    if (raiz == NULL){
        return false; 
    }

    if (dado < raiz->dado){
        return retirar(&(raiz->esquerda), dado);

    } else if (dado > raiz->dado){
        return retirar(&(raiz->direita), dado);

    } else {

        if (raiz->esquerda == NULL){
            *raizptr = raiz->direita;
            free(raiz);

        } else if (raiz->direita == NULL){
            *raizptr = raiz->esquerda;
            free(raiz);

        } else {
            node *paidosucessor = raiz;
            node *sucessor = raiz->direita;

            while (sucessor->esquerda != NULL){
                paidosucessor = sucessor;
                sucessor = sucessor->esquerda;
            }

            raiz->dado = sucessor->dado;

            if (paidosucessor == raiz){
                paidosucessor->direita = sucessor->direita;
            } else {
                paidosucessor->esquerda = sucessor->direita;
            }

            free(sucessor);
        }

        return true;
    }
}



bool encontrar(node *raiz, int dado){
        if (raiz == NULL) return false;
        if (raiz->dado == dado){
            return true; 
        }
        if (dado < raiz->dado){ 
            return encontrar(raiz->esquerda, dado); 
            return encontrar(raiz->direita, dado); 
        }
}



void liberar(node *raiz){ 
    if (raiz == NULL){
        return;
    }

    liberar(raiz->esquerda); 
    liberar(raiz->direita); 
    free(raiz);
}

int main(){ 
    node *raiz = NULL; 
    int opcao, valor;

    bool rodando = true; 

    while (rodando){
        printf("\n--- MENU ---\n");
        printf("1 - inserir valor\n");
        printf("2 - buscar valor\n");
        printf("3 - exibir árvore (formato original)\n");
        printf("4 - exibir em pré-ordem\n");
        printf("5 - exibir em ordem\n");
        printf("6 - exibir em pós-ordem\n");
        printf("7 - remover valor\n");     
        printf("8 - sair\n");
        printf("escolha: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
               printf("digite o valor para inserir: ");
                scanf("%d", &valor);
                if (inserir(&raiz, valor)){
                    printf("valor inserido com sucesso\n");
                } else {
                    printf("valor já existe na árvore\n");
                }
            break;

            case 2:
                printf("digite o valor para buscar: ");
                scanf("%d", &valor);
                printf("%d (%d)\n", valor, encontrar(raiz, valor));
            break;

            case 3:
                printnode(raiz, 0);
            break;

            case 4:
                preordem(raiz);
                printf("\n");
            break;

            case 5:
                emordem(raiz);
                printf("\n");
            break;

            case 6:
                posordem(raiz);
                printf("\n");
            break;

            case 7:
               printf("digite o valor para retirar: ");
               scanf("%d", &valor);
               if (retirar(&raiz, valor)){
               printf("valor removido com sucesso\n");
               } else {
               printf("valor não encontrado na árvore\n");
               }
            break;

            case 8:
                rodando = false;
                printf("saindo da operação...\n");
            break;

            default:
                printf("opção invalida!\n");
            break;
        }

    }

    liberar(raiz);
    raiz = NULL;

    return 0;
} 
