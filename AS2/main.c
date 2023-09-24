#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "main.h"

char* ask_for_file(){
    char * file_name = calloc(sizeof(char), 256);
    printf("Enter the file name: ");
    scanf("%s", file_name);
    FILE * fp = fopen(file_name, "r");
    if(fp == NULL){
        printf("Error File Not Found\n");
        file_name = ask_for_file();
    }
    fclose(fp);
    //printf("%s\n", file_name);
    return file_name;
}

long int get_file_length(struct dirent* files){
    FILE * fp = fopen(files->d_name, "r");
    if(fp == NULL){
        return 0;
    }
    fseek(fp, 0L, SEEK_END);
    long int length = ftell(fp);
    fclose(fp);
    return length;
}

int check_name(const char *a , const char* b){
    if(strncmp(a,b, strlen(b))== 0){
        return 1;
    }
    return 0;
}

char * find_file(int choice){
    struct dirent * files;
    char* file_to_save_largest = calloc(sizeof(char), 256);
    char* file_to_save_smallest = calloc(sizeof(char), 256); 
    DIR *dir  = opendir(".");
    if(dir == NULL){
        printf("Directory Cannot Be Opened\n");
        return -1;
    }

    long int largest = 0;
    long int smallest = 100000000;
    while((files = readdir(dir)) != NULL){
        if(check_name(files->d_name, "movies")){
            long int length = get_file_length(files);
            if(length > largest){
                largest = length;
                strcpy(file_to_save_largest, files->d_name);
                //printf("Changing Largest: %lli,   %s\n", largest, file_to_save_largest);
            }
            if(length < smallest){
                smallest = length;
                strcpy(file_to_save_smallest, files->d_name);
                //printf("Chaning Smallest: %lli,    %s\n", smallest, file_to_save_smallest);
            }   

        }
    }
    printf("Choice: %d, Largest: %s, Smallest: %s", choice, file_to_save_largest, file_to_save_smallest);
    if(choice == 1){
        closedir(dir);
        return file_to_save_largest;
    }
    else if(choice == 2){
        closedir(dir);
        return file_to_save_smallest;
    }
    else{
        printf("Error Picking File\n");
    }
}

int set_size(struct List list){
    struct movie* curr = list.head;
    list.size = 0;
    while(curr){
        list.size++;
        curr = curr->next;
    }
    return list.size;
}

void print_movie(struct movie* node){
    printf("Name: %s    ",node->name);
    printf("Year: %d    ",node->year);
    printf("Languages: %s    ",node->lang);
    printf("Rating: %f.1\n",node->ratings);
    return;
}

struct movie* create_node(char* line){
    //printf("Making new movie node\n");
    struct movie* new = (struct movie *)malloc(sizeof(struct movie));
    char* token;
    const char spacer[2] = ",";
    token = strtok(line, spacer);
    new->name = calloc(strlen(token)+ 1, sizeof(char));
    strcpy(new->name, token);
    token = strtok(NULL, spacer);
    new->year = atoi(token);
    token = strtok(NULL,spacer);
    new->lang = calloc(strlen(token)+ 1, sizeof(char));
    strcpy(new->lang, token);
    token = strtok(NULL, spacer);
    new->ratings = atof(token);
    new->next = NULL;
    print_movie(new);
    return new;
}


struct movie* read_file(char *name){
    char *line;
    int line_size = 32;
    line = (char *)malloc(line_size * sizeof(char));
    FILE *f = fopen(name,"r");
    if(f == NULL){
        printf("Error:: Cannot Open File\n");
        return 0;
    }
    struct movie* head = NULL;
    struct movie* curr = NULL;
    while(fgets(line,100, f) != NULL){
        //printf("%s", line);
        struct movie* newnode = create_node(line);
        if(head == NULL){
            head = newnode;
            curr = newnode;
        }
        else{
            curr->next = newnode;
            curr = newnode;
        }
    }
    return head;
}

void process_file(char * file){
    struct List list;
    list.head = read_file(file);
    list.size = set_size(list);
    list.name = file;
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
    char* file_to_scrub;
    while(exit == 0){
        printf("\nWhilch file would you like to process?");
        printf("\n1.The largest file\n2.The smallest file\n3.Specify File\n");
        printf("Enter 1, 2 or 3: ");
        scanf("%d", &file);
        switch(file){
            case(1):
                file_to_scrub = find_file(1);
                process_file(file_to_scrub);
                return 1;
            case(2):
                file_to_scrub = find_file(2);
                process_file(file_to_scrub);
                return 5;
            case(3):
                file_to_scrub = ask_for_file();
                process_file(file_to_scrub);
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