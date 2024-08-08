#include <stdio.h>
#include <windows.h>
#include <process.h>

#define TF 2

char aux = 'A';

void imprimir(int nthread){
    while(1)
        printf("\nThread: %d - %c", nthread, aux);
}

int main(){
    for(int i = 0; i < TF; i++){
        _beginthread(imprimir, NULL, i+1);
    }
    while(aux != 27)
        if(_kbhit()){
            aux = _getch();
        }
}