/*
 *
 *  Trabalho de ED - 2020.0
 *  Brenno Cordeiro - 190127465
 *  Marco Túlio -
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TAD de Aluno */
typedef struct _aluno {
  char nome[50];
  int matricula;
  char curso[3];
  char disciplina[4];
  float faltas;
  float nota;
  char mencao[4];
} aluno;

/* TAD de Disciplina */
typedef struct {
  char nome[4];
  int alunos;
} disciplina;

/* TAD de Cursos em disciplina */
typedef struct {
  char nome[3];
  int matriculados;
  int aprovados;
  int SS;
  int MS;
  int MM;
  int MI;
  int II;
  int SR;
} curso;

/* item/nó de uma lista */
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

/*
 * funções helpers para manipular listas
 */

int vazia(lista *l);
/* insere tad no final de LISTA */
void inserirFinal(lista *LISTA, void *tad);
/* aplica (*f) em cada item da lista */
void iterarLista(lista *LISTA, void (*f)(item *));
/* busca chave em LISTA baseado em (*f) */
item *buscarItem(lista *LISTA, void *chave, int (*f)(item *, void *));
/* remove ITEM de LISTA */
void removerItem(lista *LISTA, item *ITEM);

/*
 * funções para manipular o arquivo alunos.txt
 */

/* lê as informações do arquivo alunos.txt */
lista *lerLista();
/* salva as informações no arquivo alunos.txt */
void salvarLista(lista *LISTA);
/* gera lista de disciplinas baseado na lista de alunos */
lista *processarDisciplinas(lista *LISTA);
/* conta quantos alunos não tem disciplina */
int alunosSemDisciplina(lista *alunos);

/*
 * função para exibir uma mensagem e esperar
 * um 'enter' do usuário
 */
void enter(char *mensagem);

/*
 * funções da tela inicial
 */
void listarDisciplinas(lista *LISTA);                        /* 1 */
void adicionarDisciplina(lista *LISTA);                      /* 2 */
void removerDisciplina(lista *disciplinas, lista *alunos);   /* 3 */
void semDisciplina(lista *LISTA);                            /* 4 */
void adicionarAluno(lista *LISTA);                           /* 5 */
void removerAluno(lista *alunos, lista *disciplinas);        /* 6 */
void incluirAluno(lista *alunos, lista *disciplinas);        /* 7 */
void gerenciarDisciplina(lista *disciplinas, lista *alunos); /* 8 */

/*
 * Funções da tela de gerenciamento de disciplina
 */
void menuDisciplina(disciplina *d, lista *alunos);
void listarAlunos(disciplina *d, lista *alunos);           /* 1 */
void removerAlunoDisciplina(disciplina *d, lista *alunos); /* 2 */
void atribuirNota(disciplina *d, lista *alunos);           /* 3 */
void atribuirFaltas(disciplina *d, lista *alunos);         /* 4 */
void processarTurma(disciplina *d, lista *alunos);         /* 5 */

int main() {
  int option = 0;
  lista *alunos = lerLista();
  lista *disciplinas = processarDisciplinas(alunos);

  for (;;) {
    system("clear");
    printf("Olá Professor, \n");
    printf("%d alunos não estão matriculados. \n", alunosSemDisciplina(alunos));
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
    case 1: listarDisciplinas(disciplinas); break;
    case 2: adicionarDisciplina(disciplinas); break;
    case 3: removerDisciplina(disciplinas, alunos); break;
    case 4: semDisciplina(alunos); break;
    case 5: adicionarAluno(alunos); break;
    case 6: removerAluno(alunos, disciplinas); break;
    case 7: incluirAluno(alunos, disciplinas); break;
    case 8: gerenciarDisciplina(disciplinas, alunos); break;
    case 9: salvarLista(alunos); break;
    case 10: return 0;
    }
  }
}

void enter(char *mensagem) {
  printf("%s", mensagem);
  printf("Pressione enter para voltar.\n");
  while (getchar() != '\n') {}
  getchar();
}

int vazia(lista *l) { return l->inicio == NULL; };

void inserirFinal(lista *LISTA, void *tad) {
  item *i = (item *)malloc(sizeof(item));
  i->atual = tad;
  i->prox = NULL;

  if (vazia(LISTA)) // Lista vazia
    LISTA->inicio = i;
  else
    LISTA->final->prox = i;

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
    char linhaAtual[100];
    while (fscanf(arquivo, "%s", linhaAtual) != EOF) {
      aluno *a = (aluno *)malloc(sizeof(aluno));
      /* faz um parse do formato da linha */
      sscanf(linhaAtual, "%[^;];%d;%[^;];%[^;];%f;%f;%[^\n]", a->nome,
             &a->matricula, a->curso, a->disciplina, &a->faltas, &a->nota,
             a->mencao);
      inserirFinal(l, a);
    }
    fclose(arquivo);
  }
  return l;
}

/* 9 */
void salvarLista(lista *LISTA) {
  FILE *arquivo = fopen("alunos.txt", "w");
  item *atual = LISTA->inicio;
  while (atual) {
    aluno *a = atual->atual;
    fprintf(arquivo, "%s;%d;%s;%s;%f;%f;%s\n", a->nome, a->matricula, a->curso,
            a->disciplina, a->faltas, a->nota, a->mencao);
    atual = atual->prox;
  }

  fclose(arquivo);
}

/*
 * Busca o primeiro 'chave' em 'LISTA'
 * testa o callback (*f) em cada elemento da lista
 * se (*f) retornar 'true', retorna o elemento
 * (*f) recebe o item a ser testado e um ponteiro para qual campo testar
 * retorna NULL caso não encontre
 */
item *buscarItem(lista *LISTA, void *chave, int (*f)(item *, void *)) {
  if (LISTA->inicio == NULL)
    return NULL;
  item *atual = LISTA->inicio;
  while (atual) {
    if ((*f)(atual, chave))
      return atual;
    atual = atual->prox;
  }
  return NULL;
}

void removerItem(lista *LISTA, item *ITEM) {
  if (ITEM == LISTA->inicio) {
    LISTA->inicio = ITEM->prox;
    free(ITEM);
    return;
  }
  item *anterior = LISTA->inicio;
  while (anterior->prox) {
    if (anterior->prox == ITEM) {
      anterior->prox = ITEM->prox;
      free(ITEM);
      return;
    }
    anterior = anterior->prox;
  }
}

/* callback para buscar disciplina pelo nome */
int buscarDisciplina(item *i, void *chave) {
  disciplina *d = i->atual;
  return strcmp(d->nome, (char *)chave) == 0;
}

lista *processarDisciplinas(lista *LISTA) {
  lista *disciplinas =
      (lista *)malloc(sizeof(lista)); /* Aloca a lista de disciplinas */
  disciplinas->inicio = NULL;         /* Garante que o início é NULL */

  item *atual = LISTA->inicio;
  while (atual) {
    aluno *a = atual->atual;
    /* verifica se disciplina não está em branco */
    if (*a->disciplina != '\0') {
      item *itemDisciplina =
          buscarItem(disciplinas, a->disciplina, buscarDisciplina);
      if (itemDisciplina) {
        disciplina *d = itemDisciplina->atual;
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

int alunosSemDisciplina(lista *alunos) {
  int t = 0;
  item *atual = alunos->inicio;
  while (atual) {
    aluno *a = atual->atual;
    t += *a->disciplina == '\0';
    atual = atual->prox;
  }
  return t;
}

void exibirDisciplina(item *i) {
  disciplina *d = i->atual;
  printf("%s\t%d\n", d->nome, d->alunos);
}

/* 1 */
void listarDisciplinas(lista *disciplinas) {
  printf("Disciplina Alunos\n");
  iterarLista(disciplinas, exibirDisciplina);
  enter("");
}

/* 2 */
void adicionarDisciplina(lista *LISTA) {
  char sigla[4];
  printf("Adicionar disciplina\n");
  printf("Digite a sigla: ");
  scanf("%s", sigla);
  if (buscarItem(LISTA, sigla, buscarDisciplina)) {
    printf("Disciplina já existente\n");
    enter("");
  } else {
    disciplina *d = (disciplina *)malloc(sizeof(disciplina));
    strcpy(d->nome, sigla);
    d->alunos = 0;
    inserirFinal(LISTA, d);
  }
}

/* 3 */
void removerDisciplina(lista *disciplinas, lista *alunos) {
  char sigla[4];
  printf("Remover disciplina\n");
  printf("Digite a sigla: ");
  scanf("%s", sigla);
  item *itemDisciplina = buscarItem(disciplinas, sigla, buscarDisciplina);
  if (itemDisciplina) {
    item *itemAluno = alunos->inicio;
    while (itemAluno) {
      aluno *a = itemAluno->atual;
      disciplina *d = itemDisciplina->atual;
      /* Se aluno pertencer a disciplina */
      if (strcmp(a->disciplina, d->nome) == 0)
        *a->disciplina = '\0'; /* Coloca a disciplina do aluno como vazia */
      itemAluno = itemAluno->prox;
    }
    removerItem(disciplinas, itemDisciplina);
  } else {
    enter("Disciplina não existente\n");
  }
}

void mostrarAlunoSemDisciplina(item *i) {
  aluno *a = i->atual;
  if (*a->disciplina == '\0')
    printf("%s\t%d\t%s\n", a->nome, a->matricula, a->curso);
}

/* 4 */
void semDisciplina(lista *LISTA) {
  printf("Nome\tMatricula Curso\n");
  iterarLista(LISTA, mostrarAlunoSemDisciplina);
  enter("");
}

/* 5 */
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

/* Busca aluno usando matricula como chave */
int buscarMatricula(item *i, void *chave) {
  return ((aluno *)i->atual)->matricula == *(int *)chave;
}

/* 6 */
void removerAluno(lista *alunos, lista *disciplinas) {
  int matricula;
  printf("Remover aluno:\n");
  printf("Matrícula: ");
  scanf("%d", &matricula);
  item *itemAluno = buscarItem(alunos, &matricula, buscarMatricula);

  if (itemAluno == NULL)
    return enter("Matrícula não encontrada\n");

  aluno *a = itemAluno->atual;

  if (*a->disciplina)
    return enter("Aluno possui disciplina!\n");

  /* remove aluno da lista
   * e libera espaço de memória */
  removerItem(alunos, itemAluno);
}

/* 7 */
void incluirAluno(lista *alunos, lista *disciplinas) {
  int matricula;
  char sigla[4];

  printf("Incluir aluno:\n");
  printf("Matrícula: ");
  scanf("%d", &matricula);
  item *itemAluno = buscarItem(alunos, &matricula, buscarMatricula);

  if (itemAluno == NULL)
    return enter("Matrícula não encontrada\n");

  printf("Disciplina: ");
  scanf("%s", sigla);

  item *itemDisciplina = buscarItem(disciplinas, sigla, buscarDisciplina);
  if (itemDisciplina) {
    aluno *a = itemAluno->atual;
    disciplina *d = itemDisciplina->atual;
    strcpy(a->disciplina, sigla);
    d->alunos++;
  } else
    return enter("Disciplina não encontrada\n");
}

/* 8 */
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

/*
 * menu de gerenciamento de disciplina
 */
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
5 Processar turma\n\
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

/* 2.1 */
void listarAlunos(disciplina *d, lista *alunos) {
  system("clear");
  printf("Alunos de %s\n", d->nome);
  printf("Matrícula | Nome | Faltas | Nota | Menção\n");
  item *atual = alunos->inicio;
  while (atual) {
    aluno *a = atual->atual;
    if (strcmp(d->nome, a->disciplina) == 0)
      printf("%d\t%s\t%.0f%%\t%.2f\t%s\n", a->matricula, a->nome,
             a->faltas * 100, a->nota, a->mencao);
    atual = atual->prox;
  }
  enter("");
}

/* 2.2 */
void removerAlunoDisciplina(disciplina *d, lista *alunos) {
  int matricula;
  printf("Digite a matrícula do aluno a ser removido de %s\n", d->nome);
  printf("Matricula: ");
  scanf("%d", &matricula);

  item *itemAluno = buscarItem(alunos, &matricula, buscarMatricula);
  if (itemAluno) {
    aluno *a = itemAluno->atual;
    if (strcmp(a->disciplina, d->nome) == 0)
      *a->disciplina = '\0';
  } else
    return enter("Matrícula não encontrada\n");
}

/* 2.3 */
void atribuirNota(disciplina *d, lista *alunos) {
  int matricula;

  printf("Atribuir nota a aluno de %s\n", d->nome);
  printf("Matricula: ");
  scanf("%d", &matricula);
  item *itemAluno = buscarItem(alunos, &matricula, buscarMatricula);
  if (itemAluno == NULL)
    return enter("Matrícula não encontrada\n");

  aluno *a = itemAluno->atual;

  printf("Nota: ");
  scanf("%f", &a->nota);
  if (a->faltas >= 0.25)
    return;
  else if (a->nota >= 9)
    strcpy(a->mencao, "SS");
  else if (a->nota >= 7)
    strcpy(a->mencao, "MS");
  else if (a->nota >= 5)
    strcpy(a->mencao, "MM");
  else if (a->nota >= 3)
    strcpy(a->mencao, "MI");
  else if (a->nota >= 1)
    strcpy(a->mencao, "II");
  else
    strcpy(a->mencao, "SR");
}

/* 2.4 */
void atribuirFaltas(disciplina *d, lista *alunos) {
  int matricula;

  printf("Atribuir faltas a aluno de %s\n", d->nome);
  printf("Matricula: ");
  scanf("%d", &matricula);
  item *itemAluno = buscarItem(alunos, &matricula, buscarMatricula);
  if (itemAluno == NULL)
    return enter("Matrícula não encontrada\n");

  aluno *a = itemAluno->atual;

  printf("Faltas (%%): ");
  scanf("%f", &a->faltas);
  if (a->faltas >= 0.25)
    strcpy(a->mencao, "SR");
}

/* callback para buscar curso baseado no nome do curso */
int buscarCurso(item *i, void *chave) {
  curso *c = i->atual;
  return strcmp(c->nome, (char *)chave) == 0;
}

void adicionarAlunoAoCurso(aluno *a, void *cursos) {
  item *itemCurso = buscarItem((lista *)cursos, a->curso, buscarCurso);
  curso *c;
  if (itemCurso) {
    c = itemCurso->atual;
    c->matriculados++;
  } else {
    c = (curso *)malloc(sizeof(curso));
    strcpy(c->nome, a->curso);
    c->matriculados = 1;
    c->aprovados = c->SS = c->SR = c->MS = c->MM = c->MI = c->II = 0;
    inserirFinal(cursos, c);
  }

  if (a->faltas >= .25)
    c->SR++;
  else if (a->nota >= 9)
    c->SS++;
  else if (a->nota >= 7)
    c->MS++;
  else if (a->nota >= 5)
    c->MM++;
  else if (a->nota >= 3)
    c->MI++;
  else if (a->nota >= 1)
    c->II++;
  else
    c->SR++;

  c->aprovados += (a->nota >= 5 && a->faltas < 0.25);
}

void mostrarCurso(item *i) {
  curso *c = i->atual;
  int reprovados = c->matriculados - c->aprovados;
  printf("%s\t%d\t\t%.2f%% (%d)\t%.2f%% (%d)\n", c->nome, c->matriculados,
         (float)c->aprovados / c->matriculados * 100, c->aprovados,
         (float)reprovados / c->matriculados * 100, reprovados);
}

void mostrarMencao(item *i) {
  curso *c = i->atual;
  printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\n", c->nome, c->SS, c->MS, c->MM, c->MI,
         c->II, c->SR);
}

/* 5 */
void processarTurma(disciplina *d, lista *alunos) {
  lista *cursos = (lista *)malloc(sizeof(lista));
  cursos->inicio = cursos->final = NULL;
  item *atual = alunos->inicio;
  while (atual) {
    aluno *a = atual->atual;
    if (strcmp(a->disciplina, d->nome) == 0)
      adicionarAlunoAoCurso(a, cursos);
    atual = atual->prox;
  }

  printf("Informações de %s\n", d->nome);
  printf("Curso | Matriculados | Aprovados | Reprovados\n");
  iterarLista(cursos, mostrarCurso);
  printf("\n\nMenção\n");
  printf("Curso SS MS MM MI II SR \n");
  iterarLista(cursos, mostrarMencao);
  free(cursos);
  enter("");
}
