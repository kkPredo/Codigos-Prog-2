#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>  //faz o valor booleano funcionar

typedef struct node { //estrutura de um nó de arvore binaria
    int dado; 
    int altura; //utilizada na avl para saber a altura da arvore
    struct node *esquerda;
    struct node *direita;
    
} node;

node *criarnode(int dado){
    node* resultado = malloc(sizeof(node));
    if (resultado != NULL){
        resultado->esquerda = NULL;
        resultado->direita = NULL;
        resultado->dado = dado;
        resultado->altura = 1; //todo novo no entra com 1 de altura
    }
    return resultado;
}


bool encontrar(node *raiz, int dado){
    if (raiz == NULL) return false; //se não existir, não faz nada
    if (raiz->dado == dado){
        return true; //valor achado
    }
    if (dado < raiz->dado){ 
        return encontrar(raiz->esquerda, dado);  //procura pela esquerda 
    } else{
        return encontrar(raiz->direita, dado);  //procura pela direita
    }
}

void printtabs(int tabs){ //mesma coisa q apertar o TAB, mas usado para organizar melhor, funcinalidade estetica do printnode
    for (int i = 0; i < tabs; i++){
        printf("\t"); 
    }
}

void printnode(node *raiz, int nivel){  //feito de forma recursiva
    if (raiz == NULL){
        printtabs(nivel); //determina em qual nivel da arvore está o nó
        printf("<<vazio>>\n");
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
    printf("~~exito~~\n");
  
}
//----------------Funções da AVL---------------//
int altura(struct node *n) //serve para pegar a altura da arvore
{
    if (n == NULL)
        return 0;
    return n->altura; //devolve 0 se  nó é nulo
}

int maior(int a, int b){ //função q define qual nó é maior, esquerda ou direita 
    if(a > b){
        return a;
    }
    return b;
}

void atualizaraltura(node *raiz){ //recalcula a altura de um nó olhando os filhos
    raiz->altura = 1 + maior(altura(raiz->esquerda), altura(raiz->direita));
} 

int fatorbalaceamento(node *raiz){ //se for positivo estão ta pesado para a esquerda
    if(raiz == NULL){              //se for negativo, então ta pesado para a direita
        return 0;// se for nulo, então não faz nada
    }
    return altura(raiz->esquerda) - altura(raiz->direita);
}   //subtrai o lado esquerdo pelo direito

void rotacaodireita(node **raizptr){//utiliza **raizptr para trocar a raiz da subarvore diretamente, sem precisar devolver ponteiro sla
    node *raiz = *raizptr;
    node *novaraiz = raiz->esquerda;
    raiz->esquerda = novaraiz->direita;
    novaraiz->direita = raiz;

    atualizaraltura(raiz);      //quem subiu primeiro
    atualizaraltura(novaraiz);  //quem desceu depois

    *raizptr = novaraiz; //a subarvore cmeça em nova raiz
}

void rotacaoesquerda(node **raizptr){//espelha a rotação da direita
    node *raiz = *raizptr;
    node *novaraiz = raiz->direita;
    raiz->direita = novaraiz->esquerda;
    novaraiz->esquerda = raiz;

    atualizaraltura(raiz);      //quem desceu primeiro
    atualizaraltura(novaraiz);  //quem subiu depois

    *raizptr = novaraiz;
}

void balancear(node **raizptr){
    node *raiz = *raizptr;

    if(raiz == NULL){
        return;
    }

    atualizaraltura(raiz);
    int fb = fatorbalaceamento(raiz);
    
    if(fb > 1){//pesado para a esquerda
        if(fatorbalaceamento(raiz->esquerda) < 0){// caso esquerda-direita: rotação dupla
            rotacaoesquerda(&(raiz->esquerda));
        }
        rotacaodireita(raizptr);

    }else if(fb < -1){//pesado para a direita
        if(fatorbalaceamento(raiz->direita) > 0){ // caso direita-esquerda: rotação dupla
            rotacaodireita(&(raiz->direita));
        }
        rotacaoesquerda(raizptr);

    }
}

void mostraralturaefator(node *raiz){
    if (raiz == NULL){
        return;
    }

    printf("Nó %d -> Altura: %d | Fator de balanceamento: %d\n", raiz->dado, altura(raiz), fatorbalaceamento(raiz));

    mostraralturaefator(raiz->esquerda);

    mostraralturaefator(raiz->direita);
}

//---------------------------------------------//

//valor booleano para decidir true(verdadeiro) ou false(falso)
bool inserir(node **raizptr, int dado){ 
    node *raiz = *raizptr;
    if (raiz == NULL){ //arvore vazia
        (*raizptr) = criarnode(dado);  //cria um nó em um lugar com valor nulo
        return true;   //true e false fazem parte do valor booleano
    }

    if (dado == raiz->dado){ //não permite repetir o valor do dado na avore caso ja exista
        return false;
    }

    bool inseriu;

    //dados inseridos menores q o dados da raiz vão pra esquerda, e os dados maiores pra direita;

    if (dado < raiz->dado){ //se o valor inserido for menor q o valor da arvore, ele oolha pra esquerda
        inseriu = inserir(&(raiz->esquerda), dado);//guarda o dado inves de usar um simples return

    } else {    //se o valor for maior, ele olha pra direita
        inseriu = inserir(&(raiz->direita), dado);
    }

    if(inseriu){
        balancear(raizptr);
    }

    return inseriu;
}


void preordem(node *raiz){ //imprime a raiz antes de ir pros filhos
    if (raiz == NULL){
        return;
    }

    printf("%d ", raiz->dado);  //primeiro o dado do nó atual
    preordem(raiz->esquerda);   //depois olha pra esquerda
    preordem(raiz->direita);    //depois olha pra direita
}

void emordem(node *raiz){    //imprime a raiz entre os filhos por isso sai ordenado
     if (raiz == NULL){
        return;
    }

    emordem(raiz->esquerda);    //esquerda primeiro 
    printf("%d ", raiz->dado);  //então raiz
    emordem(raiz->direita);     //depois o resto sendo a direita
}

void posordem(node *raiz){
    if (raiz == NULL){
        return;
    }

    posordem(raiz->esquerda);    //primeiro esquerda
    posordem(raiz->direita);     //depois direita
    printf("%d ", raiz->dado);     //termina com raiz
}


//valor booleano para decidir true(verdadeiro) ou false(falso)
bool retirar(node **raizptr, int dado){ //nó apontando para um ponteiro de ponteiro de nó, valor para inserir
    node *raiz = *raizptr;
    
    if (raiz == NULL){
        return false; 
    }

    bool removeu;

    if (dado < raiz->dado){
        removeu = retirar(&(raiz->esquerda), dado);//segue a mesma logica do inserir

    } else if (dado > raiz->dado){
       removeu = retirar(&(raiz->direita), dado);

    } else {

        if (raiz->esquerda == NULL){
            *raizptr = raiz->direita;
            free(raiz);
            return true; //quem chamou o pai é quem balanceia sla

        } else if (raiz->direita == NULL){
            *raizptr = raiz->esquerda;
            free(raiz);
            return true;

        } else { // dois filhos: acha o sucessor (menor da direita)
            node *sucessor = raiz->direita;

            while (sucessor->esquerda != NULL){
                sucessor = sucessor->esquerda;
            }

            raiz->dado = sucessor->dado;

            // em vez de mexer nos ponteiros na mão, remove o
            // sucessor recursivamente na subárvore direita. assim todos
            // os nós do caminho até ele também são rebalanceados
            removeu = retirar(&(raiz->direita), sucessor->dado);

        }
    }
        
    if (removeu){//na volta da recursão rebalanceia
        balancear(raizptr);
    }
 
    return removeu;

}

void liberar(node *raiz){ //libera memoria pra evitar memory leak, sempre pelos filhos primeiro para não perder a referencia
    if (raiz == NULL){
        return; //nada pra liberar
    }

    liberar(raiz->esquerda); //libera tudo da esquerda primeiro
    liberar(raiz->direita); //depois tudo da direita
    free(raiz);            //só no final libera o nó atual
}   

int main(){ 
    node *raiz = NULL; //cria a arvore sem valor
    int opcao, valor;

    bool rodando = true; //controla o loop do menu

    while (rodando){
        printf("\n--- MENU ---\n");
        printf("1 - inserir valor\n");
        printf("2 - buscar valor\n");
        printf("3 - exibir árvore (formato original)\n");
        printf("4 - exibir em pré-ordem\n");
        printf("5 - exibir em ordem\n");
        printf("6 - exibir em pós-ordem\n");
        printf("7 - remover valor\n");  
        printf("8 - exibir altura e fator de balanceamento\n");   
        printf("9 - sair\n");
        printf("escolha: ");
        scanf("%d", &opcao);
        printf("\n");

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
               mostraralturaefator(raiz);
               break;

            case 9:
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
