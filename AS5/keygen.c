#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
    const char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    if(argc < 2){
        printf("Not enough arguements\n");
        return 0;
    }
    else{
        int length = atoi(argv[1]);
        char key[length + 1];
        int r, i;
        time_t t;
        srand((unsigned) time(&t));
        for(i = 0; i < length; i++){
            r = rand() % 27;
            key[i] = alpha[r];
        }
        for(i = 0; i < length; i++){
            printf("%c", key[i]);
        }
        printf("\n");
    }
}