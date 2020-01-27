#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TAD do aluno
typedef struct _aluno {
  char nome[50];
  int matricula;
  char curso[3];
  char disciplina[4];
  float faltas;
  float nota;
  char mencao[4];
} aluno;

// TAD de disciplinas
typedef struct {
  char nome[4];
  int alunos;
} disciplina;

typedef struct _item {
  void *atual;
  void *prox;
} item;

typedef struct _lista {
  item *inicio;
  item *final;
} lista;

void inserirFinal(lista *LISTA, void *tad);

lista *lerLista();
void salvarLista(lista *LISTA);
void mostrarLista(lista *LISTA);

void adicionarAluno(lista *LISTA);

int main() {
  // ler arquivo
  int option = 0;
  lista *alunos = lerLista();

  while (option != 10) {
    printf("Olá Professor, \n");
    printf("<X> alunos não estão matriculados. \n");
    printf("O que deseja fazer: \n");
    printf("1 Listar disciplinas \n");
    printf("2 Adicionar disciplina \n");
    printf("3 Remover desciplina \n");
    printf("4 Listar alunos sem disciplina \n");
    printf("5 Adicionar aluno \n");
    printf("6 Remover aluno \n");
    printf("7 Incluir aluno em disciplina \n");
    printf("8 Gerenciar disciplina \n");
    printf("9 Salvar alterações \n");
    printf("10 Sair \n");
    printf("Digite a opção: ");
    scanf("%d", &option);
    system("clear");

    switch (option) {
    case 0:
      mostrarLista(alunos);
      break;
    case 5:
      adicionarAluno(alunos);
      break;
    case 9:
      salvarLista(alunos);
      break;
    }
  }
  return 0;
}

void inserirFinal(lista *LISTA, void *tad) {
  item *i = (item *)malloc(sizeof(item));
  i->atual = tad;

  if (LISTA->inicio == NULL) { // Lista vazia
    LISTA->inicio = i;
  } else {
    LISTA->final->prox = i;
  }
  LISTA->final = i;
}

// Lê do arquivo como binário
// deve ser mudada posteriormente
// para ler do arquivo em texto
lista *lerLista() {
  lista *l = (lista *)malloc(sizeof(lista));
  FILE *arquivo = fopen("alunos.txt", "rb");
  if (arquivo) {
    aluno *tmp = (aluno *)malloc(sizeof(aluno));
    while (fread(tmp, sizeof(aluno), 1, arquivo)) {
      aluno *a = (aluno *)malloc(sizeof(aluno));
      *a = *tmp;
      inserirFinal(l, a);
    }
    free(tmp);
    fclose(arquivo);
  }
  return l;
}

// Salva no arquivo com binário
// dever ser mudada posteriormente
// para salvar como arquivo de texto
void salvarLista(lista *LISTA) {
  FILE *arquivo = fopen("alunos.txt", "wb");
  item *atual = LISTA->inicio;
  while (atual) {
    fwrite(atual->atual, sizeof(aluno), 1, arquivo);
    atual = atual->prox;
  }
  fclose(arquivo);
}

void mostrarLista(lista *LISTA) {
  item *atual = LISTA->inicio;
  while (atual) {
    aluno *a = atual->atual;
    printf("%s %d %s\n", a->nome, a->matricula, a->curso);
    atual = atual->prox;
  }
}

void adicionarAluno(lista *LISTA) {
  aluno *novo = (aluno *)malloc(sizeof(aluno));

  printf("Adicionar aluno:\n");
  printf("Nome: ");
  scanf("%s", novo->nome);
  printf("Matricula: ");
  scanf("%d", &novo->matricula);
  printf("Curso: ");
  scanf("%s", novo->curso);

  inserirFinal(LISTA, novo);
}
