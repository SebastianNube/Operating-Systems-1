#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int validation(int argc, char * argv[]){
    char *valid[] = {"movies.csv", "Movies.csv"};
    if(argc == 2){
        for(int i=0; i < 2; i++){
            if(strcmp(argv[1], valid[i]) == 0){ 
                return 0;
            }
        }
        printf("Error::Incorrect File Name\n");
        return 1;
    }
    else{
        printf("Error::Too Many Entry Parameters\n");
        return 1;
    }
}

void print_movie(struct movie* node){
    printf("Name: %s    ",node->name);
    printf("Year: %d    ",node->year);
    printf("Languages: %s    ",node->lang);
    printf("Rating: %f.1\n",node->ratings);
    return;
}

void print_list(struct List list){
    struct movie* curr = list.head;
    while(curr != NULL){
        print_movie(curr);
        curr= curr->next;
    }
    return;
}

void highest_movie_each_year(struct List list){
    //printf("Made it in\n");
    struct movie* curr = list.head;
    int year_lowest = curr->year;
    int year_highest = curr->year;
    //printf("Highest Year: %d, Lowest Year: %d\n", year_highest,year_lowest);
    while(curr){
        if(curr->year < year_lowest){
            year_lowest = curr->year;
        }
        if(curr->year > year_highest){
            year_highest = curr->year;
        }
        curr = curr->next; 
        //printf("Iterated\n");
    }
    curr = list.head;
    struct movie* highest;
    //printf("Highest Year: %d, Lowest Year: %d\n", year_highest,year_lowest);
    printf("\n");
    while(year_lowest <= year_highest){
        //Run through all movies
        while(curr != NULL){
            //check for in right year
            if(curr->year == year_lowest){
                if(highest->year != year_lowest){
                    highest = curr;
                }
                //compare current highest with current
                if(highest->ratings < curr->ratings){                        
                    highest = curr;
                }
            }
            curr= curr->next;
        }
        
        //at end of movie list, print highest
        //iterate to next year & reset current movie
        if(highest->year == year_lowest){
            printf("%d  %.1f  %s\n", highest->year, highest->ratings, highest->name);
        }
        curr = list.head;
        year_lowest++;
    }
    printf("\n");
    return;
}

void sort_by_language(struct List list){
    struct movie* curr = list.head;
    char spacer[2] = ";";
    char* selection = malloc(256*sizeof(char *));
    printf("Enter a language to search for: ");
    scanf("%s", selection);
    char lang_temp[100];
    char* token;
    int check = 0;
    printf("\n");
    while(curr != NULL){
        token = NULL;
        strcpy( lang_temp ,curr->lang);
        token = strtok(lang_temp, spacer);
        while(token != NULL){
            if(strcmp(token, selection) == 0){
                printf("%d  %s\n", curr->year, curr->name);
                check = 1;
            }
            token = strtok(NULL, spacer);
            //printf("\t%s %s\t", curr->name, token);
        }

        curr = curr->next;
    }
    if(check == 0){
        printf("Error::No Movies with language: %s\n", selection);
    }
    printf("\n");
    return;
}

int prompt_user2(int year_highest, int year_lowest){
    int selection;
    printf("Enter a year between %d - %d: ", year_lowest, year_highest);
    scanf("%d", &selection);
    if(selection <= year_highest && selection >= year_lowest){
        return selection;
    }
    else{
        printf("Error::Invalid Year selected\n");
        selection = prompt_user2(year_highest, year_lowest);
    }
    return selection;
}

void movies_from_specific_year(struct List list){
    struct movie* curr = list.head;
    int year_lowest = curr->year;
    int year_highest = curr->year;
    while(curr->next){
        if(curr->year < year_lowest){
            year_lowest = curr->year;
        }
        if(curr->year > year_highest){
            year_highest = curr->year;
        }
        curr = curr->next; 
    }
    int selection = prompt_user2(year_highest, year_lowest);
    curr = list.head;
    int check = 0;
    printf("\n");
    while(curr){
        if(curr->year== selection){
            printf("%s\n", curr->name);
            check++;
        }
        curr = curr->next;
    }
    if(check < 1){
        printf("Error::No Movies Within that Year\n");
    }
    printf("\n");
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
    //print_movie(new);
    return new;
}

struct movie* read_file(char *name){
    char *line;
    int line_size = 50;
    line = malloc(line_size * sizeof(char* ));
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

int prompt_user(){
    int selection;
    printf("1. Show Movies Released in the specified year.\n");
    printf("2. Show the Highest Rate Movie for Each Year.\n");
    printf("3. Show the title and year of release for specific language\n");
    printf("4. Exit from the program\n");
    printf("Your Choice:  ");
    scanf("%d", &selection);
    switch(selection){
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
        case 5:
            return 5;
        default:
            printf("Error:: Choose a number between 1-4\n\n");
            selection = prompt_user();
    }
    return selection;
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

int run(struct List list){
    int choice = prompt_user();
    int made;
    switch(choice){
        case 1:
            movies_from_specific_year(list);
            return choice;
        case 2:
            highest_movie_each_year(list);
            return choice;
        case 3:
            sort_by_language(list);
            return choice;
        case 4:
            printf("Quitting Program\n\n");
            return choice;
        case 5:
            printf("\n");
            print_list(list);
            printf("\n");
            return choice;
        default:
            break;
    }
    return 4;
}

int main(int argc, char * argv[]){
    if(validation(argc, argv) != 0){
        printf("Error::Invalid File\n");
        return 0;
    }
    struct List list;
    list.head = read_file(argv[1]);
    list.size = set_size(list);
    list.name = argv[1];
    printf("List Created\n");
    printf("Able to Process file %s and parse %d movies\n", list.name ,list.size);
    int exit = 0;
    while(exit != 4){
        exit = run(list);
    }
    printf("Exit Successful\n");
}