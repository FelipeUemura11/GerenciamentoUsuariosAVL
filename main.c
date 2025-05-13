#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Usuario{
    int id;
    char nome[45];
    char email[45];
};

struct No{
    struct Usuario usuario;
    struct No *esq;
    struct No *dir;
    struct No *pai;
    int altura;
};

// Funcao para obter a altura de um noh
int altura(struct No *no) {
    if(no == NULL){
        return 0;
    }
    return no->altura;
}

// Funcao para obter o fator de balanceamento
int fatorBalanceamento(struct No *no) {
    if(no == NULL){
        return 0;
    }
    return altura(no->esq) - altura(no->dir);
}

// Funcao para atualizar a altura de um noh
void atualizarAltura(struct No *no) {
    if (no == NULL) return;
    int alturaEsq = altura(no->esq);
    int alturaDir = altura(no->dir);
    no->altura = (alturaEsq > alturaDir ? alturaEsq : alturaDir) + 1;
}

void rotacaoEsq(struct No **raizAntiga){
    if(*raizAntiga == NULL || (*raizAntiga)->dir == NULL){
        return;
    }
    struct No *novaRaiz = (*raizAntiga)->dir;
    struct No *temp = novaRaiz->esq;

    novaRaiz->esq = *raizAntiga;
    (*raizAntiga)->dir = temp;
    
    novaRaiz->pai = (*raizAntiga)->pai;
    (*raizAntiga)->pai = novaRaiz;

    if(temp != NULL){
        temp->pai = *raizAntiga;
    }
    if(novaRaiz->pai != NULL){
        if(novaRaiz->pai->esq == *raizAntiga){
            novaRaiz->pai->esq = novaRaiz;
        }else{
            novaRaiz->pai->dir = novaRaiz;
        }
    }
    *raizAntiga = novaRaiz;

    // atualizar alturas apos rotacao
    atualizarAltura((*raizAntiga)->esq);
    atualizarAltura(*raizAntiga);
}

void rotacaoDir(struct No **raizAntiga){
    if(*raizAntiga == NULL || (*raizAntiga)->esq == NULL){
        return;
    }

    struct No *novaRaiz = (*raizAntiga)->esq;
    struct No *temp = novaRaiz->dir;

    novaRaiz->dir = *raizAntiga;
    (*raizAntiga)->esq = temp;

    novaRaiz->pai = (*raizAntiga)->pai;
    (*raizAntiga)->pai = novaRaiz;

    if(temp != NULL){
        temp->pai = *raizAntiga;
    }

    if(novaRaiz->pai != NULL){
        if(novaRaiz->pai->esq == *raizAntiga){
            novaRaiz->pai->esq = novaRaiz;
        } else {
            novaRaiz->pai->dir = novaRaiz;
        }
    }

    *raizAntiga = novaRaiz;

    // atualizar alturas apos rotacao
    atualizarAltura((*raizAntiga)->dir);
    atualizarAltura(*raizAntiga);
}

// Funcao para balancear a arvore AVL
void balancear(struct No **raiz) {
    if (*raiz == NULL) return;

    // atualiza a altura do noh atual
    atualizarAltura(*raiz);

    // calcula o fator de balanceamento
    int fb = fatorBalanceamento(*raiz);

    // Caso 1: rotacao a direita
    if (fb > 1 && (*raiz)->esq != NULL && fatorBalanceamento((*raiz)->esq) >= 0) {
        rotacaoDir(raiz);
    }
    // Caso 2: rotacao a esquerda
    else if (fb < -1 && (*raiz)->dir != NULL && fatorBalanceamento((*raiz)->dir) <= 0) {
        rotacaoEsq(raiz);
    }
    // Caso 3: rotacao dupla a direita
    else if (fb > 1 && (*raiz)->esq != NULL && fatorBalanceamento((*raiz)->esq) < 0) {
        rotacaoEsq(&(*raiz)->esq);
        rotacaoDir(raiz);
    }
    // Caso 4: rotacao dupla a esquerda
    else if (fb < -1 && (*raiz)->dir != NULL && fatorBalanceamento((*raiz)->dir) > 0) {
        rotacaoDir(&(*raiz)->dir);
        rotacaoEsq(raiz);
    }
}

// Funcao para inserir um novo usuario na arvore
void inserirUsuario(struct No **raiz, struct Usuario novoUsuario){
    if(*raiz == NULL){
        *raiz = (struct No *)malloc(sizeof(struct No));
        if(*raiz != NULL){
            (*raiz)->usuario = novoUsuario;
            (*raiz)->esq = NULL;
            (*raiz)->dir = NULL;
            (*raiz)->pai = NULL;
            (*raiz)->altura = 1;
            printf("Usuario [%s] cadastrado com sucesso!\n", novoUsuario.nome);
        }else{
            printf("ERROR : insercao do [%s] invalida, memoria insuficiente.\n", novoUsuario.nome);
            return;
        }
    }else{
        if(strcmp(novoUsuario.nome, (*raiz)->usuario.nome) < 0){
            inserirUsuario(&((*raiz)->esq), novoUsuario);
            if((*raiz)->esq != NULL){  // verifica se a insercao foi bem sucedida
                (*raiz)->esq->pai = *raiz; // pai do filho a esq recebe o noh atual
            }
        }else if(strcmp(novoUsuario.nome, (*raiz)->usuario.nome) > 0){
            inserirUsuario(&((*raiz)->dir), novoUsuario);
            if((*raiz)->dir != NULL){  // verifica se a insercao foi bem sucedida
                (*raiz)->dir->pai = *raiz; // pai do filho a dir recebe o noh atual
            }
        }else{
            printf("Usuario com este nome ja existe na lista...\n");
            return;
        }
        balancear(raiz);
    }
}

int removerUsuario(struct No **raiz, char removerUsuarioNome[45]){
    if(*raiz == NULL){
        printf("Usuario nao encontrado!\n");
        return 0;
    }

    int comparacao = strcmp(removerUsuarioNome, (*raiz)->usuario.nome);
    // procura o usuario a ser removido
    if(comparacao < 0){
        return removerUsuario(&((*raiz)->esq), removerUsuarioNome);
    }
    else if(comparacao > 0){
        return removerUsuario(&((*raiz)->dir), removerUsuarioNome);
    }
    else{
        // >> noh encontrado <<
        struct No *temp = *raiz;
        
        // Caso 1: noh folha
        if((*raiz)->esq == NULL && (*raiz)->dir == NULL){
            *raiz = NULL;
        }
        // Caso 2: noh com um filho
        else if((*raiz)->esq == NULL){
            *raiz = (*raiz)->dir;
            if(*raiz != NULL){  // verifica se o novo noh nao eh NULL
                (*raiz)->pai = temp->pai;
            }
        }
        else if((*raiz)->dir == NULL){
            *raiz = (*raiz)->esq;
            if(*raiz != NULL){  // verifica se o novo noh nao eh NULL
                (*raiz)->pai = temp->pai;
            }
        }
        // Caso 3: noh com dois filhos
        else{
            struct No *sucessor = (*raiz)->dir;
            while(sucessor->esq != NULL){
                sucessor = sucessor->esq;
            }
            
            // copia os dados do sucessor para o noh atual
            (*raiz)->usuario = sucessor->usuario;
            
            // remove o sucessor
            if(sucessor->pai == *raiz){
                sucessor->pai->dir = sucessor->dir;
                if(sucessor->dir != NULL){
                    sucessor->dir->pai = sucessor->pai;
                }
            }else{
                sucessor->pai->esq = sucessor->dir;
                if(sucessor->dir != NULL){
                    sucessor->dir->pai = sucessor->pai;
                }
            }
            temp = sucessor;
        }
        
        free(temp);
        if(*raiz != NULL){  // soh balanceia se a raiz nao for NULL
            balancear(raiz);
        }
        return 1;
    }
}

// Funcao de busca que retorna o noh encontrado
struct No *buscarUsuario(struct No *raiz, char buscarUsuarioNome[45]){
    if(raiz == NULL){
        return NULL;
    }else{
        if(strcmp(buscarUsuarioNome, raiz->usuario.nome) == 0){
            return raiz;
        }else if(strcmp(buscarUsuarioNome, raiz->usuario.nome) < 0){
            return buscarUsuario(raiz->esq, buscarUsuarioNome);
        }else{
            return buscarUsuario(raiz->dir, buscarUsuarioNome);
        }
    }
}
// Funcao para listar todos os usuarios da arvore
void listarUsuarios(struct No *raiz){
    if(raiz != NULL){
        listarUsuarios(raiz->esq);
        printf(" >> ID: %i\n", raiz->usuario.id);
        printf(" >> Nome: %s\n", raiz->usuario.nome);
        printf(" >> Email: %s\n", raiz->usuario.email);
        listarUsuarios(raiz->dir);
    }
}
// Funcao de busca que retorna o noh encontrado
void liberarArvore(struct No *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

int main(){

    struct No *raiz = NULL;
    struct Usuario usuario;
    int opc = -1;
    char buscarUsuarioNome[45], removerUsuarioNome[45];

    while(opc != 0){ // menu de opcoes
        printf("\n========= USUARIOS AVL =========\n");
        printf("<1> -  Inserir usuario -  <1>\n");
        printf("<2> -  Remover usuario -  <2>\n");
        printf("<3> -  Buscar usuario  -  <3>\n");
        printf("<4> -  Listar usuarios -  <4>\n");
        printf("<0> -       Sair       -  <0>\n");
        printf("\n========= USUARIOS AVL =========\n");

        printf("\n >> Escolha uma opcao: ");
        scanf("%i", &opc);
        getchar();

        switch(opc){
            case 1:
                printf(" >> ID do usuario: ");
                scanf("%i", &usuario.id);
                getchar();

                printf(" >> Nome do usuario: ");
                fgets(usuario.nome, 45, stdin);
                usuario.nome[strcspn(usuario.nome, "\n")] = 0;

                printf(" >> Email do usuario: ");
                fgets(usuario.email, 45, stdin);
                usuario.email[strcspn(usuario.email, "\n")] = 0;

                inserirUsuario(&raiz, usuario);
                break;
            case 2:
                printf(" >> Nome do usuario: ");
                fgets(removerUsuarioNome, 45, stdin);
                removerUsuarioNome[strcspn(removerUsuarioNome, "\n")] = 0;

                removerUsuario(&raiz, removerUsuarioNome);
                printf(" >> Usuario removido com sucesso!\n");
                break;
            case 3:
                printf(" >> Nome do usuario: ");
                fgets(buscarUsuarioNome, 45, stdin);
                buscarUsuarioNome[strcspn(buscarUsuarioNome, "\n")] = 0;

                struct No *encontrado = buscarUsuario(raiz, buscarUsuarioNome);

                if(encontrado != NULL){
                    printf(" >> Usuario encontrado: \n");
                    printf(" >> ID: %i\n", encontrado->usuario.id);
                    printf(" >> Nome: %s\n", encontrado->usuario.nome);
                    printf(" >> Email: %s\n", encontrado->usuario.email);
                }else{
                    printf("Usuario nao encontrado...\n");
                }
                break;
            case 4:
                printf("\n>>> Listagem de usuarios <<<\n");
                listarUsuarios(raiz);
                break;
            case 0:
                printf(" >> Saindo do sistema <<\n");
                break;
            default:
                printf(" >> Opcao invalida << \n");
                break;
        }
        
    }
    liberarArvore(raiz);
    return 0;
}
