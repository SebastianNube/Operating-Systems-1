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
