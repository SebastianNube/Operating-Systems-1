#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char * HOME_DIR = ".";

char * input_line(){
    char * input = malloc(256 * sizeof(char *));
    printf(":");
    scanf("%s", input);
    //printf("%s\n", input);
    return input;
}

void run(){
    printf("$ smallsh\n");
    char * input = malloc(2566 * sizeof(char *));
    while(strcmp(input, "exit") != 0){
        input = input_line();
        if(strcmp(input, "cd") == 0){
            change_directory()
        }
        else if(strcmp(input, "ls") == 0){

        }
        else{
            printf("What the fuck is that shit\n");
        }
    }
}


int main(){
    run();
}