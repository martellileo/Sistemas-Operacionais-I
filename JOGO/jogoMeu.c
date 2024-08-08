#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <process.h>

#define COLUNAS 119
#define LINHAS 29
#define OBJETIVOMAX 1
#define METEOROSMAX 40

typedef struct {
    int coluna;
    int linha;
} Meteoro;

Meteoro meteoros[METEOROSMAX];
HANDLE mutex;
int pontuacao = 0;
int vidas = 3;
int estrela = 0;
int estrelaColuna, estrelaLinha;
int linhaNave = 25, colunaNave = 60;
int jogoRodando = 1;
int numeroMeteoros = 0;

void configurarTela() { // config cmd
    CONSOLE_CURSOR_INFO info; // representa cursor
    HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE); // representa console

    SetConsoleTitle("nave"); // seta nome console
    SMALL_RECT windowSize = {0, 0, COLUNAS + 1, LINHAS + 1}; // tamanho janela
    SetConsoleWindowInfo(wHnd, 1, &windowSize);

    COORD bufferSize = {COLUNAS + 1, LINHAS + 1}; // seta buffer
    SetConsoleScreenBufferSize(wHnd, bufferSize);

    info.dwSize = 100; // seta cursor invisivel
    info.bVisible = FALSE;
    SetConsoleCursorInfo(wHnd, &info);
}

// impressoes
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (short)x;
    coord.Y = (short)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void imprimirNave(int linha, int coluna) {
    gotoxy(coluna, linha);
    printf("%c", 202);
    fflush(stdout);
}

void apagarNave(int linha, int coluna) {
    gotoxy(coluna, linha);
    printf(" ");
    fflush(stdout);
}

void imprimirContorno(int linha, int coluna) {
    gotoxy(coluna, linha);
    printf("%c", 219);
    fflush(stdout);
}

void imprimirTexto(int linha, int coluna) {
    gotoxy(coluna, linha);
    printf("L O A D I N G . . .");
    fflush(stdout);
}

void apagarTexto(int linha, int coluna) {
    gotoxy(coluna, linha);
    printf("                      ");
    fflush(stdout);
}

void imprimirPontuacao(int pontos) {
    gotoxy(3, 28);
    printf("Pontos: %d", pontos);
    fflush(stdout);
}

void imprimirVidas(int vidas) {
    gotoxy(3, 29);
    printf("Vidas: %d", vidas);
    fflush(stdout);
}

void imprimirEstrela(int coluna, int linha) {
    gotoxy(coluna, linha);
    printf("%c", 207);
    fflush(stdout);
}

void removerEstrela() {
    gotoxy(estrelaColuna, estrelaLinha);
    printf(" ");
    fflush(stdout);
    estrela--;
}

void imprimirMeteoro(int coluna, int linha) {
    gotoxy(coluna, linha);
    printf("%c", 254);
    fflush(stdout);
}

void apagarMeteoro(int coluna, int linha) {
    gotoxy(coluna, linha);
    printf(" ");
    fflush(stdout);
}

void telaBranca() {
    for (int y = 0; y < LINHAS; y++) {
        for (int x = 0; x < COLUNAS; x++) {
            imprimirContorno(y, x);
        }
    }
}

// funcionamento do jogo
void intro() {
    int linha = 1, coluna = 3;

    imprimirTexto(5, 53);

    while (linha < LINHAS - 1) {
        imprimirContorno(linha++, coluna);
        Sleep(15);
    }
    linha--;
    coluna++;
    while (coluna < COLUNAS) {
        imprimirContorno(linha, coluna++);
        Sleep(2);
    }
    coluna--;
    linha--;
    while (linha > 0) {
        imprimirContorno(linha--, coluna);
        Sleep(15);
    }
    while (coluna > 2) {
        imprimirContorno(linha, coluna--);
        Sleep(2);
    }

    apagarTexto(5, 53);
}

void gerarEstrela() {
    while (jogoRodando) {
        Sleep(1000); // timer pra nascer outra estrela

        WaitForSingleObject(mutex, INFINITE);

        if (estrela < OBJETIVOMAX) { // gera estrela se menor que a quantidade maxima(1)
            do {
                estrelaColuna = rand() % (COLUNAS - 4) + 4; // nao estrapolar
                estrelaLinha = rand() % (LINHAS - 3) + 1;   // os limites
            } while (estrelaColuna == colunaNave && estrelaLinha == linhaNave);
            imprimirEstrela(estrelaColuna, estrelaLinha);
            estrela++;
        }
        ReleaseMutex(mutex);
    }
}

void threadNave() {
    char tecla;

    while (jogoRodando) {
        tecla = _getch();
        WaitForSingleObject(mutex, INFINITE);

        switch (tecla) {
        case -32:
            tecla = _getch();
            switch (tecla) {
            case 72: // cima
                if (linhaNave > 1) {
                    apagarNave(linhaNave, colunaNave);
                    linhaNave--;
                }
                break;

            case 80: // baixo
                if (linhaNave < LINHAS - 3) {
                    apagarNave(linhaNave, colunaNave);
                    linhaNave++;
                }
                break;

            case 77: // direita
                if (colunaNave < COLUNAS - 2) {
                    apagarNave(linhaNave, colunaNave);
                    colunaNave++;
                }
                break;

            case 75: // esquerda
                if (colunaNave > 4) {
                    apagarNave(linhaNave, colunaNave);
                    colunaNave--;
                }
                break;
            }
            imprimirNave(linhaNave, colunaNave);
        }

        if (colunaNave == estrelaColuna && linhaNave == estrelaLinha) { // comparar colisao com a estrela e somar pontuacao
            pontuacao += 100;
            imprimirPontuacao(pontuacao);
            removerEstrela();
            imprimirNave(linhaNave, colunaNave);
        }

        ReleaseMutex(mutex);
    }
}

void threadMeteoros() {
    while (jogoRodando) {
        Sleep(250); // velocidade que eles caem

        WaitForSingleObject(mutex, INFINITE);

        if (numeroMeteoros < METEOROSMAX) { // rand dos meteoros
            int colunaNova, linhaNova;
            do {
                colunaNova = rand() % (COLUNAS - 5) + 4;
                linhaNova = 1;
            } while (colunaNova == estrelaColuna && linhaNova == estrelaLinha);

            meteoros[numeroMeteoros].coluna = colunaNova;
            meteoros[numeroMeteoros].linha = linhaNova;
            imprimirMeteoro(meteoros[numeroMeteoros].coluna, meteoros[numeroMeteoros].linha);

            numeroMeteoros++;
        }

        for (int i = 0; i < numeroMeteoros; i++) { // parte de descer o meteoro
            apagarMeteoro(meteoros[i].coluna, meteoros[i].linha); 

            meteoros[i].linha++; // desce a linha meteoro

            if (meteoros[i].coluna == estrelaColuna && meteoros[i].linha == estrelaLinha) { // pro meteoro nao sumir com a estrela
                meteoros[i].linha++;
            }

            if (meteoros[i].linha < LINHAS - 3) { // imprime o meteoro se tiver dentro do limite
                imprimirMeteoro(meteoros[i].coluna, meteoros[i].linha);
            } else {
                for (int j = i; j < numeroMeteoros - 1; j++) { // meteoro saiu do limite
                    meteoros[j] = meteoros[j + 1];
                }
                numeroMeteoros--;
                i--; // diminui o array
            }
            if(meteoros[i].coluna == colunaNave && meteoros[i].linha == linhaNave){ // colisao da nave com meteoro
                vidas--;
                imprimirVidas(vidas);
                for (int j = i; j < numeroMeteoros - 1; j++)
                    meteoros[j] = meteoros[j + 1];
                numeroMeteoros--;
                i--; // diminui o array
                imprimirNave(linhaNave, colunaNave);
            }

            if (vidas <= 0) // termina execucao do do-while e fecha o jogo
                jogoRodando = 0; 
        }
        ReleaseMutex(mutex);
    }
}

void gameover(){
    system("cls");
    telaBranca();
    Sleep(10);
    system("cls");
    telaBranca();
    Sleep(10);
    system("cls");
    telaBranca();
    
    Sleep(3000);
    system("cls");

    gotoxy(COLUNAS / 2 - 5, LINHAS / 2 - 1);
    printf("GAME OVER");
    gotoxy(COLUNAS / 2 - 5, LINHAS / 2);
    printf("PONTOS: %d", pontuacao);

    Sleep(5000);
    system("cls");
}

int main() {

    do {
        mutex = CreateMutex(NULL, FALSE, NULL);

        configurarTela();
        intro();
        Sleep(1000);
        imprimirNave(linhaNave, colunaNave);
        imprimirPontuacao(pontuacao);
        imprimirVidas(vidas);

        _beginthread(threadNave, 0, NULL);
        _beginthread(threadMeteoros, 0, NULL);

        gerarEstrela();

        CloseHandle(mutex);
    } while (vidas > 0 && jogoRodando);

    Sleep(500);
    gameover();
}