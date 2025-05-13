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
};
// Função para inserir um novo usuario na arvore
void inserirUsuario(struct No **raiz, struct Usuario novoUsuario){
    if(*raiz == NULL){
        *raiz = (struct No *)malloc(sizeof(struct No));
        if(*raiz != NULL){
            (*raiz)->usuario = novoUsuario;
            (*raiz)->esq = NULL;
            (*raiz)->dir = NULL;
        }else{
            printf("ERROR : insercao do [%s] invalida, memoria insuficiente.\n", novoUsuario.nome);
        }
    }else{
        if(strcmp(novoUsuario.nome, (*raiz)->usuario.nome) < 0){
            inserirUsuario(&((*raiz)->esq), novoUsuario);
        }else if(strcmp(novoUsuario.nome, (*raiz)->usuario.nome) > 0){
            inserirUsuario(&((*raiz)->dir), novoUsuario);
        }else{
            printf("Usuario com este nome ja existe na lista...\n");
        }
    }
}

// Função de busca que retorna o noh encontrado
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
// Função para listar todos os usuarios da arvore
void listarUsuarios(struct No *raiz){
    if(raiz != NULL){
        listarUsuarios(raiz->esq);
        printf(" >> ID: %i\n", raiz->usuario.id);
        printf(" >> Nome: %s\n", raiz->usuario.nome);
        printf(" >> Email: %s\n", raiz->usuario.email);
        listarUsuarios(raiz->dir);
    }
}
// Função de busca que retorna o noh encontrado
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
    char buscarUsuarioNome[45];

    while(opc != 0){
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
                //removerUsuario(&raiz);    
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
                    printf(" >> Usuario nao encontrado...\n");
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
