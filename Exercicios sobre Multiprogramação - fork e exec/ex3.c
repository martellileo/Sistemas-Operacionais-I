#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
  int pid, filhos, tempo, status;

  printf("Filhos a criar: ");
  scanf("%d", &filhos);

  printf("Sleep time: ");
  scanf("%d", &tempo);

  for(int i = 0; i<filhos; i++) {
    pid = fork();
    if(pid == 0) {
        printf("Filho %d criado\n", getpid());
        sleep(tempo);
        printf("Filho %d acordou.\n", getpid());
        exit(0); 
    }
  }

  do {
      pid = wait(0);
      if(pid != -1) {
          printf("Todos os filhos acordaram\n", pid);
      } else {
          printf("Processo Pai acordou\n");
      }

  }while (pid != -1);
}
