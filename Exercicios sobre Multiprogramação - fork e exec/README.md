1 ) Desenvolva um programa que crie uma quantidade de processos filho informada pelo usuário. Esses processos filho deverão executar laços infinitos sem processamento algum. Após todos os processos filho criados, enquanto houver processos filho, o processo pai deve apresentar todos os PIDs dos processos criados e perguntar ao usuário qual ele deseja finalizar (matar). Quando todos os processos filho estiverem finalizados (mortos), o pai deve informar que todos os filhos morreram e também deve terminar. A função kill (pid, 9) mata (finaliza) o processo que tiver PID pid.


2) Faça um teste de mesa para descobrir quantos processos são criados no programa a seguir. Você deve elaborar uma representação gráfica, em formato de arvore, para representar a hierarquia dos processos criados. Apresente também o que será impresso na tela. Suponha que o primeiro processo (pai) tenha PID 100 e os processos criados (filhos) vão recebendo números em sequencia (ex: 101, 102, 103). Somente execute o programa para corrigir sua resposta.

int main(void) {
  int i;
  for(i=0;i<3;i++) {
    fork();
    printf("Imprimiu %d!\n",getpid());
  }
}

3) Desenvolva um programa que crie uma quantidade de processos filho informada pelo usuário. Esses processos filho deverão ficar bloqueados por uma quantidade de segundos, também informada pelo usuário. Após todos os processos filho criados, o processo pai sempre deve informar sempre que um processo filho morrer. Assim que todos os processos filhos terminarem (morrerem), o processo pai também deve terminar informando que todos os filhos morreram.

OBS: A função wait(0) bloqueia o processo que chamou até um filho morrer. Quando wait(0) retornar -1 significa que não existem mais processos filho.  A função sleep(x) bloqueia o processo que chamou por x segundos.


4) Faça um teste de mesa para descobrir quantos processos são criados pelo programa a seguir. Você deve elaborar uma representação gráfica em formato de arvore (como apresentado em aula) para representar a hierarquia dos processos criados. Apresente também o que será impresso na tela. Suponha que o primeiro processo (pai) tenha PID 100 e os processos criados (filhos) vão recebendo números em sequencia (ex: 101, 102, 103). Somente execute o programa para corrigir sua resposta.

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void) {
 int i=2, j=1;
 printf("\nPID: %d\n",getpid());
 fork();
 j=wait(0);
 if(j==-1)
   i++;
 else
   i--;
 printf("PID: %d - Valor de i %d\n",getpid(),i);
 fork();
 j=wait(0);
 if(j==-1)
   i++;
 else
   i--;
 printf("PID: %d - Valor de i %d\n",getpid(),i);
}
