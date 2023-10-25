#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

struct node{
    int val;
    struct node * next;
};

struct List{
    int size;
    struct node* head;
};

struct node * create_node(struct node * head, int val_to_store){
    struct node * curr = head;
    while(curr->next != NULL){
        curr = curr->next;
    }
    struct node * new = malloc(sizeof(struct node *));
    new->next = NULL;
    new->val = val_to_store;
    curr->next = new;
    return head;
}


int main(int argc, char * argv[]){
    printf("File Read: %s\n", argv[1]);
    FILE * fp = fopen(argv[1], "r");
    char buffer[1024];
    while(fgets(buffer, 1024,fp)){

    }
    struct List list;
    list.size = 0;

}

