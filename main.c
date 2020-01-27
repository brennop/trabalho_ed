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

/* item de uma lista */
typedef struct _item {
  void *atual;
  void *prox;
} item;

/* struct de lista simplesmente encadeada */
typedef struct _lista {
  item *inicio;
  item *final;
} lista;

/* funções helpers para manipular listas */
void inserirFinal(lista *LISTA, void *tad); /* insere tad no final de LISTA */
void iterarLista(lista *LISTA,
                 void (*f)(item *)); /* aplica (*f) em cada item da lista */
item *buscarItem(lista *LISTA, void *chave,
                 int (*f)(item *,
                          void *)); /* busca chave em LISTA baseado em (*f) */
void removerItem(lista *LISTA, item *ITEM); /* remove ITEM de LISTA */

/* funções para manipular o arquivo.txt */
lista *lerLista();
void salvarLista(lista *LISTA);
void mostrarLista(lista *LISTA);
lista *processarDisciplinas(lista *LISTA);

void listarDisciplinas(lista *LISTA);                 /* 1 */
void adicionarDisciplina(lista *LISTA);               /* 2 */
void removerDisciplina(lista *LISTA);                 /* 3 */
void semDisciplina(lista *LISTA);                     /* 4 */
void adicionarAluno(lista *LISTA);                    /* 5 */
void removerAluno(lista *LISTA);                      /* 6 */
void incluirAluno(lista *alunos, lista *disciplinas); /* 7 */

int main() {
  // ler arquivo
  int option = 0;
  lista *alunos = lerLista();
  lista *disciplinas = processarDisciplinas(alunos);

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
    case 1:
      listarDisciplinas(disciplinas);
      break;
    case 2:
      adicionarDisciplina(disciplinas);
      break;
    case 3:
      removerDisciplina(disciplinas);
      break;
    case 5:
      adicionarAluno(alunos);
      break;
    case 6:
      removerAluno(alunos);
      break;
    case 7:
      incluirAluno(alunos, disciplinas);
      break;
    case 8:
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
  i->prox = NULL;

  if (LISTA->inicio == NULL) { // Lista vazia
    LISTA->inicio = i;
  } else {
    LISTA->final->prox = i;
  }
  LISTA->final = i;
}

void iterarLista(lista *LISTA, void (*f)(item *)) {
  item *atual = LISTA->inicio;
  while (atual) {
    (*f)(atual);
    atual = atual->prox;
  }
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

void mostrarAluno(item *i) {
  aluno *a = i->atual;
  printf("%s %d %s %s\n", a->nome, a->matricula, a->curso, a->disciplina);
}

void mostrarLista(lista *LISTA) { iterarLista(LISTA, mostrarAluno); }

item *buscarItem(lista *LISTA, void *chave, int (*f)(item *, void *)) {
  item *atual = LISTA->inicio;
  while (atual) {
    if ((*f)(atual, chave))
      return atual;
    atual = atual->prox;
  }
  return NULL;
}

void removerItem(lista *LISTA, item *ITEM) {
  item *anterior = LISTA->inicio;
  while (anterior->prox) {
    if (anterior->prox == ITEM) {
      anterior->prox = ITEM->prox;
      return;
    }
    anterior = anterior->prox;
  }
}

int buscarDisciplina(item *i, void *chave) {
  disciplina *d = i->atual;
  return strcmp(d->nome, (char *)chave) == 0;
}

// Lê as disciplinas do arquivo
lista *processarDisciplinas(lista *LISTA) {
  lista *disciplinas =
      (lista *)malloc(sizeof(lista)); /* Aloca a lista de disciplinas */
  disciplinas->inicio = NULL;         /* Garante que o início é NULL */

  item *atual = LISTA->inicio;
  while (atual) {
    aluno *a = atual->atual;
    if (a->disciplina != NULL) {
      item *i = buscarItem(LISTA, a->disciplina, buscarDisciplina);
      if (i) {
        disciplina *d = i->atual;
        d->alunos++;
      } else {
        disciplina *d = (disciplina *)malloc(sizeof(disciplina));
        strcpy(d->nome, a->disciplina);
        inserirFinal(disciplinas, d);
      }
    }
    atual = atual->prox;
  }
  return disciplinas;
}

void listarDisciplinas(lista *disciplinas) {
  item *atual = disciplinas->inicio;
  while (atual) {
    disciplina *d = atual->atual;
    printf("%s %d\n", d->nome, d->alunos);
    atual = atual->prox;
  }
}

void adicionarDisciplina(lista *LISTA) {
  char sigla[4];
  printf("Adicionar disciplina\n");
  printf("Digite a sigla: ");
  scanf("%s", sigla);
  if (buscarItem(LISTA, sigla, buscarDisciplina)) {
    printf("Disciplina já existente\n");
  } else {
    disciplina *d = (disciplina *)malloc(sizeof(disciplina));
    strcpy(d->nome, sigla);
    d->alunos = 0;
    inserirFinal(LISTA, d);
  }
}

void removerDisciplina(lista *LISTA) {
  char sigla[4];
  printf("Adicionar disciplina\n");
  printf("Digite a sigla: ");
  scanf("%s", sigla);
  item *i = buscarItem(LISTA, sigla, buscarDisciplina);
  if (i) {
    removerItem(LISTA, i);
    free(i);
  } else {
    printf("Disciplina não existente\n");
  }
}

void mostrarAlunoSemDisciplina(item *i) {
  aluno *a = i->atual;
  if (a->disciplina[0] == '\0')
    printf("%s %d %s\n", a->nome, a->matricula, a->curso);
}

void semDisciplina(lista *LISTA) {
  iterarLista(LISTA, mostrarAlunoSemDisciplina);
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

int buscarMatricula(item *i, void *chave) {
  aluno *a = i->atual;
  return a->matricula == *(int *)chave;
}

void removerAluno(lista *LISTA) {
  int matricula;
  printf("Remover aluno:\n");
  printf("Matrícula: ");
  scanf("%d", &matricula);
  item *i = buscarItem(LISTA, &matricula, buscarMatricula);
  if (i) {
    removerItem(LISTA, i);
    free(i);
  } else {
    printf("Matrícula não encontrada\n");
  }
}

void incluirAluno(lista *alunos, lista *disciplinas) {
  int matricula;
  char sigla[4];

  printf("Incluir aluno:\n");
  printf("Matrícula: ");
  scanf("%d", &matricula);
  item *itemAluno = buscarItem(alunos, &matricula, buscarMatricula);
  if (itemAluno == NULL) {
    printf("Matrícula não encontrada\n");
    return;
  }
  printf("Disciplina: ");
  scanf("%s", sigla);

  if (buscarItem(disciplinas, sigla, buscarDisciplina)) {
    aluno *a = itemAluno->atual;
    strcpy(a->disciplina, sigla);
  } else {
    printf("Disciplina não encontrada\n");
  }
}
