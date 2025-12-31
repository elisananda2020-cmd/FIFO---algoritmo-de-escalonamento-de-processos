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

int compararChegada(const void *a, const void *b) {
    Processo *p1 = (Processo *)a;
    Processo *p2 = (Processo *)b;
    return p1->tempoChegada - p2->tempoChegada;
}

void fifo(Processo p[], int n) {
    qsort(p, n, sizeof(Processo), compararChegada);

    int tempoAtual = 0;
    float somaWT = 0, somaTAT = 0, somaRT = 0;

    for (int i = 0; i < n; i++) {
        if (tempoAtual < p[i].tempoChegada)
            tempoAtual = p[i].tempoChegada;

        p[i].inicio = tempoAtual;
        p[i].termino = tempoAtual + p[i].duracao;
        p[i].turnaround = p[i].termino - p[i].tempoChegada;
        p[i].waiting = p[i].turnaround - p[i].duracao;
        p[i].response = p[i].inicio - p[i].tempoChegada;

        tempoAtual = p[i].termino;

        somaWT += p[i].waiting;
        somaTAT += p[i].turnaround;
        somaRT += p[i].response;
    }

    printf("\nTabela de Processos (FIFO)\n");
    printf("ID | Chegada | Duracao | Inicio | Termino | WT | TAT | RT\n");
    for (int i = 0; i < n; i++) {
        printf("%2d | %7d | %7d | %6d | %7d | %2d | %3d | %2d\n",
               p[i].id, p[i].tempoChegada, p[i].duracao, p[i].inicio,
               p[i].termino, p[i].waiting, p[i].turnaround, p[i].response);
    }

    printf("\nMetricas do FIFO:\n");
    printf("Media Waiting Time: %.2f\n", somaWT / n);
    printf("Media Turnaround Time: %.2f\n", somaTAT / n);
    printf("Media Response Time: %.2f\n", somaRT / n);
    printf("Throughput: %.2f\n", (float)n / tempoAtual);
}

int main() {
    int n;
    printf("Quantidade de processos: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 1;

    Processo p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("\nProcesso %d\n", p[i].id);
        printf("Tempo de chegada: ");
        scanf("%d", &p[i].tempoChegada);
        printf("Duracao: ");
        scanf("%d", &p[i].duracao);
    }

    fifo(p, n);
    return 0;
}
