#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

char * ask_for_file(){
    char * file_name;
    printf("Enter the file name: ");
    scanf("%s", file_name);
}


int menu(){
    int choice;
    while(1){
        printf("1. Select file to process\n2. Exit Program\n");
        printf("Enter a choice 1 or 2: ");
        scanf("%d", &choice);
        if(choice == 1 || choice == 2){
            return choice;
        }
    }
}

int menu1(){
    int file = 0;
    int exit = 0;
    while(exit == 0){
        printf("\nWhilch file would you like to process?");
        printf("\n1.The largest file\n2.The smallest file\n3.Specify File\n");
        printf("Enter 1, 2 or 3: ");
        scanf("%d", &file);
        switch(file){
            case(1):
                return 1;
            case(2):
                //Must change to not exit based on parameters of while loop in main
                return 2;
            case(3):
                ask_for_file();
                return 3;
            case(4):
                return 2;
            default:
                printf("Error:: Invalid Selection (Valid Numbers are 1-3)\n");
        }
    }
}


int main(){
    int exit = menu();
    while(exit != 2){
        exit = menu1();
    }    
}