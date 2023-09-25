#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct movie{
    char* name;
    int year;
    struct movie* next;
};

struct List{
    struct movie* head;
};

long int get_file_length(struct dirent*);
char* ask_for_file();
int check_name(const char *, const char*);
char * find_file(int);
void print_movie(struct movie*);
struct movie* create_node(char*);
struct movie* read_file(char *);
void make_new_files(struct List, char *, int);
void process_file(char *);
int menu();
int menu1();