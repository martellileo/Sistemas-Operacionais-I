#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int qnt_filhos, temporizador, pid, status;

    printf("\nFilhos a criar: ");
    scanf("%d", &qnt_filhos);

    printf("\nSleep time: ");
    scanf("%d", &temporizador);

    for (int i = 0; i < qnt_filhos; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Erro ao criar processo filho");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            printf("Filho %d criado.\n", getpid());
            sleep(temporizador);
            printf("Filho %d acordou.\n", getpid());
            exit(EXIT_SUCCESS);
        }
    }

    while (1) {
        pid = wait(&status);
        
        if (pid == -1) {
            printf("Todos os filhos acordaram.\n");
            break;
        } else {
            printf("Filho %d acordou.\n", pid);
        }
    }

    return 0;
}
