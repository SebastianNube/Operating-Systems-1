#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct movie{
    char* name;
    int year;
    char * lang;
    float ratings;
    struct movie* next;
};

struct List{
    struct movie* head;
    int size;
    char * name;
};

int validation(int, char **);
void print_movie(struct movie*);
void print_list(struct List);
void highest_movie_each_year(struct List);
void sort_by_language(struct List, char *);
int prompt_user2(int , int);
void movies_from_specific_year(struct List);
struct movie* create_node(char*);
struct movie* read_file(char *);
int prompt_user();
int set_size(struct List);
int run(struct List);