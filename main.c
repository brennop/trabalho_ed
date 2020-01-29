#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define enter()                                                                \
  while (getchar() != '\n')                                                    \
    ;                                                                          \
  getchar();

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
  int tam;
} lista;

/* funções helpers para manipular listas */

/* insere tad no final de LISTA */
void inserirFinal(lista *LISTA, void *tad);
/* aplica (*f) em cada item da lista */
void iterarLista(lista *LISTA, void (*f)(item *));
/* busca chave em LISTA baseado em (*f) */
item *buscarItem(lista *LISTA, void *chave, int (*f)(item *, void *));
/* remove ITEM de LISTA */
void removerItem(lista *LISTA, item *ITEM);

/* funções para manipular o arquivo.txt */
lista *lerLista();
void salvarLista(lista *LISTA);
void mostrarLista(lista *LISTA);
lista *processarDisciplinas(lista *LISTA);

void listarDisciplinas(lista *LISTA);                        /* 1 */
void adicionarDisciplina(lista *LISTA);                      /* 2 */
void removerDisciplina(lista *LISTA);                        /* 3 */
void semDisciplina(lista *LISTA);                            /* 4 */
void adicionarAluno(lista *LISTA);                           /* 5 */
void removerAluno(lista *alunos, lista *disciplinas);        /* 6 */
void incluirAluno(lista *alunos, lista *disciplinas);        /* 7 */
void gerenciarDisciplina(lista *disciplinas, lista *alunos); /* 8 */

void menuDisciplina(disciplina *d, lista *alunos);

void listarAlunos(disciplina *d, lista *alunos);           /* 1 */
void removerAlunoDisciplina(disciplina *d, lista *alunos); /* 2 */
void atribuirNota(disciplina *d, lista *alunos);           /* 3 */
void atribuirFaltas(disciplina *d, lista *alunos);         /* 4 */
void processarTurma(disciplina *d, lista *alunos);         /* 5 */

int main() {
  // ler arquivo
  int option = 0;
  lista *alunos = lerLista();
  lista *disciplinas = processarDisciplinas(alunos);

  for (;;) {
    system("clear");
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

    switch (option) {
    case 0: mostrarLista(alunos); break;
    case 1: listarDisciplinas(disciplinas); break;
    case 2: adicionarDisciplina(disciplinas); break;
    case 3: removerDisciplina(disciplinas); break;
    case 4: semDisciplina(alunos); break;
    case 5: adicionarAluno(alunos); break;
    case 6: removerAluno(alunos, disciplinas); break;
    case 7: incluirAluno(alunos, disciplinas); break;
    case 8: gerenciarDisciplina(disciplinas, alunos); break;
    case 9: salvarLista(alunos); break;
    case 10: return 0;
    }
    enter(); // TODO: Melhor usabilidade
  }
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
  LISTA->tam++;
}

void iterarLista(lista *LISTA, void (*f)(item *)) {
  item *atual = LISTA->inicio;
  while (atual) {
    (*f)(atual);
    atual = atual->prox;
  }
}

lista *lerLista() {
  lista *l = (lista *)malloc(sizeof(lista));
  l->tam = 0;
  FILE *arquivo = fopen("alunos.txt", "r");
  if (arquivo) {
    char tmp[100];
    while (fscanf(arquivo, "%s", tmp) != EOF) {
      aluno *a = (aluno *)malloc(sizeof(aluno));
      /* faz um parse do formato da linha */
      sscanf(tmp, "%[^;];%d;%[^;];%[^;];%f;%f;%[^\n]", a->nome,
                  &a->matricula, a->curso, a->disciplina, &a->faltas,
                  &a->nota, a->mencao);
      inserirFinal(l, a);
    }
    fclose(arquivo);
  }

  return l;
}

void salvarLista(lista *LISTA) {
  FILE *arquivo = fopen("alunos.txt", "w");
  item *atual = LISTA->inicio;
  while (atual) {
    /* fwrite(atual->atual, sizeof(aluno), 1, arquivo); */
    aluno *a = atual->atual;
    fprintf(arquivo, "%s;%d;%s;%s;%f;%f;%s\n", a->nome, a->matricula, a->curso,
            a->disciplina, a->faltas, a->nota, a->mencao);
    atual = atual->prox;
  }
  fclose(arquivo);
}

void mostrarAluno(item *i) {
  aluno *a = i->atual;
  printf("%s %d %s %s %f %f %s\n", a->nome, a->matricula, a->curso,
         a->disciplina, a->faltas, a->nota, a->mencao);
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
    if (*a->disciplina != '\0') {
      item *i = buscarItem(disciplinas, a->disciplina, buscarDisciplina);
      if (i) {
        disciplina *d = i->atual;
        d->alunos++;
      } else {
        disciplina *d = (disciplina *)malloc(sizeof(disciplina));
        strcpy(d->nome, a->disciplina);
        d->alunos = 1;
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
  if (*a->disciplina == '\0')
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

  /* inicializa os valores restantes para não serem
   * prenchidos com lixo */
  *novo->disciplina = *novo->mencao = '\0';
  novo->faltas = novo->nota = 0;

  inserirFinal(LISTA, novo);
}

int buscarMatricula(item *i, void *chave) {
  return ((aluno *)i->atual)->matricula == *(int *)chave;
}

void removerAluno(lista *alunos, lista *disciplinas) {
  int matricula;
  printf("Remover aluno:\n");
  printf("Matrícula: ");
  scanf("%d", &matricula);
  item *itemAluno = buscarItem(alunos, &matricula, buscarMatricula);

  if (itemAluno == NULL) {
    printf("Matrícula não encontrada\n");
    return;
  }

  aluno *a = itemAluno->atual;

  if (*a->disciplina) {
    printf("Aluno possui disciplina!\n");
    return;
  }


  /* remove aluno da lista
   * e libera espaço de memória */
  removerItem(alunos, itemAluno);
  free(itemAluno);
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

  item *itemDisciplina = buscarItem(disciplinas, sigla, buscarDisciplina);
  if (itemDisciplina) {
    aluno *a = itemAluno->atual;
    disciplina *d = itemDisciplina->atual;
    strcpy(a->disciplina, sigla);
    d->alunos++;
  } else {
    printf("Disciplina não encontrada\n");
  }
}

void gerenciarDisciplina(lista *disciplinas, lista *alunos) {
  char sigla[4];
  printf("Gerenciar disciplina: ");
  scanf("%s", sigla);
  item *i = buscarItem(disciplinas, sigla, buscarDisciplina);
  if (i) {
    disciplina *d = i->atual;
    menuDisciplina(d, alunos);
  }
}

void menuDisciplina(disciplina *d, lista *alunos) {
  int opcao = -1;
  for (;;) {
    system("clear");
    printf("\
Gerenciando disciplina %s\n\
Quantidade de alunos: %d\n\
Opções:\n\
1 Listar alunos\n\
2 Remover aluno da disciplina\n\
3 Atribuir nota a aluno\n\
4 Atribuir faltas a aluno\n\
5 Processar turmar\n\
6 Voltar\n\
Escolha uma opção: ",
           d->nome, d->alunos);
    scanf("%d", &opcao);
    switch (opcao) {
    case 1: listarAlunos(d, alunos); break;
    case 2: removerAlunoDisciplina(d, alunos); break;
    case 3: atribuirNota(d, alunos); break;
    case 4: atribuirFaltas(d, alunos); break;
    case 5: processarTurma(d, alunos); break;
    case 6: return;
    }
  }
}

void listarAlunos(disciplina *d, lista *alunos) {
  system("clear");
  printf("Alunos de %s\n", d->nome);
  printf("Matrícula | Nome | Faltas | Nota | Menção\n");
  item *atual = alunos->inicio;
  while (atual) {
    aluno *a = atual->atual;
    if (strcmp(d->nome, a->disciplina) == 0)
      printf("%d\t%s\t%d\%\t%.2f\t%s\n", a->matricula, a->nome, (int)a->faltas,
             a->nota, a->mencao);
    atual = atual->prox;
  }
  printf("Pressione enter para voltar.\n");
  enter();
}

void removerAlunoDisciplina(disciplina *d, lista *alunos) {
  int matricula;
  printf("Digite a matrícula do aluno a ser removido de %s\n", d->nome);
  printf("Matricula: ");
  scanf("%d", &matricula);

  item *i = buscarItem(alunos, &matricula, buscarMatricula);
  if (i) {
    aluno *a = i->atual;
    if (strcmp(a->disciplina, d->nome) == 0)
      *a->disciplina = '\0';
  } else
    printf("Matrícula não encontrada\n");
}

void atribuirNota(disciplina *d, lista *alunos) {
  int matricula;

  printf("Atribuir nota a aluno de %s\n", d->nome);
  printf("Matricula: ");
  scanf("%d", &matricula);
  item *i = buscarItem(alunos, &matricula, buscarMatricula);
  if (i == NULL) {
    printf("Matrícula não encontrada\n");
    return;
  }
  aluno *a = i->atual;

  printf("Nota: ");
  scanf("%f", &a->nota);
}

void atribuirFaltas(disciplina *d, lista *alunos) {
  int matricula;

  printf("Atribuir faltas a aluno de %s\n", d->nome);
  printf("Matricula: ");
  scanf("%d", &matricula);
  item *i = buscarItem(alunos, &matricula, buscarMatricula);
  if (i == NULL) {
    printf("Matrícula não encontrada\n");
    return;
  }
  aluno *a = i->atual;

  printf("Faltas (%): ");
  scanf("%f", &a->faltas);
}

void processarTurma(disciplina *d, lista *alunos) {
  printf("Informações de %s\n", d->nome);
  printf("Curso | Matriculados | Aprovados | Reprovados\n");
  printf("Pressione enter para voltar.\n");
  enter();
}
