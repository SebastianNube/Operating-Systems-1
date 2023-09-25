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
