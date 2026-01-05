#include <stdio.h>
#include <stdlib.h>


typedef struct {
    int id;
    int tempoChegada;
    int duracao;
    int prioridade;
    int inicio;
    int termino;
    int turnaround;
    int waiting;
    int response;
} Processo;

typedef struct Elemento {
    Processo dado;
    struct Elemento *proximo;
} Elemento;

typedef struct {
    Elemento *primeiro;
    Elemento *ultimo;
} GerenciadorFila;

void prepararFila(GerenciadorFila *f) {
    f->primeiro = NULL;
    f->ultimo = NULL;
}

int estaVazia(GerenciadorFila *f) {
    return f->primeiro == NULL;
}

void adicionarProcesso(GerenciadorFila *f, Processo p) {
    Elemento *novoItem = (Elemento *)malloc(sizeof(Elemento));
    novoItem->dado = p;
    novoItem->proximo = NULL;

    if (f->ultimo == NULL) {
        f->primeiro = novoItem;
        f->ultimo = novoItem;
    } else {
        f->ultimo->proximo = novoItem;
        f->ultimo = novoItem;
    }
}

Processo removerProcesso(GerenciadorFila *f) {
    Elemento *alvo = f->primeiro;
    Processo p = alvo->dado;

    f->primeiro = alvo->proximo;
    if (f->primeiro == NULL)
        f->ultimo = NULL;

    free(alvo);
    return p;
}

void executarFIFO(GerenciadorFila *f) {
    int cronometro = 0;
    int contadorProcessos = 0;
    float acumulaEspera = 0, acumulaTurnaround = 0, acumulaResposta = 0;

    printf("\n--- RESULTADOS DO ESCALONAMENTO FIFO ---\n");
    printf("ID | Chegada | Duracao | Inicio | Termino | Espera | TAT\n");

    while (!estaVazia(f)) {
        Processo p = removerProcesso(f);

        if (cronometro < p.tempoChegada)
            cronometro = p.tempoChegada;

        p.inicio = cronometro;
        p.termino = cronometro + p.duracao;
        p.turnaround = p.termino - p.tempoChegada;
        p.waiting = p.turnaround - p.duracao;
        p.response = p.inicio - p.tempoChegada;

        cronometro = p.termino;
        acumulaEspera += p.waiting;
        acumulaTurnaround += p.turnaround;
        acumulaResposta += p.response;
        contadorProcessos++;

        printf("%2d | %7d | %7d | %6d | %7d | %6d | %3d\n",
               p.id, p.tempoChegada, p.duracao, p.inicio,
               p.termino, p.waiting, p.turnaround);
    }

    if (contadorProcessos > 0) {
        printf("\n--- METRICAS FINAIS ---\n");
        printf("Tempo medio de Espera (Wait):  %.2f\n", acumulaEspera / contadorProcessos);
        printf("Tempo medio de Retorno (TAT):  %.2f\n", acumulaTurnaround / contadorProcessos);
        printf("vazao (Throughput):            %.2f proc/un\n", (float)contadorProcessos / cronometro);
    }
}

int main() {
    int total;
    GerenciadorFila fila;

    prepararFila(&fila);

    printf("Quantos processos deseja simular? ");
    if (scanf("%d", &total) != 1 || total <= 0) return 0;

    for (int i = 0; i < total; i++) {
        Processo p;
        p.id = i + 1;

        printf("\nDados do Processo %d:\n", p.id);
        printf("  -> Tempo de Chegada: ");
        scanf("%d", &p.tempoChegada);
        printf("  -> Duracao (Burst): ");
        scanf("%d", &p.duracao);

        adicionarProcesso(&fila, p);
    }

    executarFIFO(&fila);

    return 0;
}
