#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <math.h>
#define TF 10000

double matriz[TF][TF];
int linhas, threads;

void preencher(int n){
    int inicio = n * linhas;
    int fim = (n *linhas) + linhas;

    for(int i = inicio; i < fim; i++){
        for(int j = 0; j < TF; j++){
            matriz[i][j] = (i * pow(j,3) * pow(i,4))/pow(i,2);
            printf("[%lld]\n", matriz[i][j]);
        }
    }
}

int main(){
    int qntd;

    printf("\n Digite a qnt de threads: ");
    scanf("%d", &qntd);

    linhas = TF/qntd;

    for(int i = 0; i < qntd; i++){
        _beginthread(preencher, NULL, i);
    }
    getch();
}
