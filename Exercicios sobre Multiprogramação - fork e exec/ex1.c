#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main() {
    int filhos, pid, vivos = 0, matar, i;
    int *pids;

    printf("Digite a quantidade de filhos a serem criadas: ");
    scanf("%d", &filhos);
    
    pids = (int *)malloc(filhos * sizeof(int));
    
    for (i = 0; i < filhos; i++) {
        pid = fork();
        
        if (pid == 0)
            while (1);
        else {
            pids[i] = pid;
            vivos++;
        }
    }

    do {
        int existe = 0;
    
        printf("\nFilhos vivos: \n");
        for (i = 0; i < filhos; i++) {
            if (pids[i] != -1) {
                printf("- %d\n", pids[i]);
            }
        }

        if (vivos > 0) {
            printf("\nDigite o PID a ser finalizado: ");
            scanf("%d", &matar);

            for (i = 0; i < filhos; i++) {
                if (pids[i] == matar) {
                    kill(matar, 9);
                    pids[i] = -1;
                    vivos--;
                    printf("Processo filho %d finalizado.\n", matar);
                    existe++;
                }
            }
            if(existe == 0)
              printf("\nProcesso filho %d nao foi encontrado\n", matar);
        }

    } while (vivos > 0);

    printf("Encerrando o processo pai.\n");

    free(pids);
}

