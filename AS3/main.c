#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

struct line{
    char * line;
    char * commands;
    char ** args;
    int num_args;
    char * inputfiles;
    char * outputfiles;
    int    back;        //1 is background process   0 is foreground process
};

struct line * create_line(){
    struct line * newNode = calloc(sizeof(struct line), 1);
    char * whole = calloc(sizeof(char),2048);
    fgets(whole, 2048, stdin);
    char * internalpointer = malloc(strlen(whole));
    strcpy(internalpointer, whole);
    const char spacer[2] = " ";
    char * token = strtok_r(internalpointer, spacer, &internalpointer);
    if(strstr(token, "#") != NULL){
        return NULL;
    }
    newNode->commands = calloc(strlen(token) + 1, 1);
    strcpy(newNode->commands,token);
    int i = 0;
    newNode->args = (char **)calloc(sizeof(char *), 512);
    newNode->back = 0;
    token = strtok_r(internalpointer,spacer, &internalpointer);
    char * pop;
    while(token != NULL){
        //Must do variable expansion on $$       
        if(strcmp(token, ">") == 0 | strcmp(token, "<") == 0 | strcmp(token,"&") == 0){
            break;
        }
        if(strcmp(token, "\n") != 0){
            newNode->args[i] = (char *)calloc(sizeof(char), strlen(token)+1 );
            strcpy(newNode->args[i], token);
            i = i + 1;
            newNode->num_args = i;
            token = strtok_r(internalpointer,spacer, &internalpointer);
        }
    }

    while(token != NULL){
        if(strcmp(token,"&") == 0){
            newNode->back = 1;
        }
        if(strcmp(token,"<") == 0){
            token = strtok_r(internalpointer,spacer, &internalpointer);
            newNode->inputfiles = calloc(strlen(token)+1, 1);
            strcpy(newNode->inputfiles, token);
        }
        if(strcmp(token,">") == 0){
            token = strtok_r(internalpointer,spacer, &internalpointer);
            newNode->outputfiles = calloc(strlen(token)+1, 1);
            strcpy(newNode->outputfiles, token);
        }
        token = strtok_r(internalpointer, spacer, &internalpointer);
    }
    return newNode;
}

void print_line(struct line * curr){
    printf("Command:    %s\t", curr->commands);
    int i;
    if(curr->num_args != 0){
        printf("%d  Arguements: ", curr->num_args);
        for(i = 0; i < curr->num_args; i++){
            printf("%s\t", curr->args[i]);
        }
    }
    printf("\n");
}

void change_input_redirection(){
    //dup2

}

void change_output_redirection(){
    //dup2

}

void status(){
    //Print out exit status or terminating signal of the last foreground process
    //If run before a foreground process    -   Return 0
    //cd    status      and exit do not count as forground processes
}

void exit_program(){
    //Kill all processes and end program
}

void change_directory_program(struct line * curr, char * directory){    
    //Change the working directory of smallsh
    //if no path is specified, go HOME
    //Must support absolute and relative paths
    char * curr_dir = calloc(sizeof(char *), 200);
    char s[500]; 
    curr_dir = getcwd(s, 100);
    if(curr->num_args != 0){
        chdir(curr->args[0]);
        printf("Changing Directory to   %s\n", curr->args[0]);
        return;
    }
    chdir(directory);
    printf("Changing Directory to   %s\n", curr_dir);
    return;
}

int run_line(struct line * curr, char * directory){
    if(strcasecmp("exit\n", curr->commands) == 0){exit_program();return 0;}
    if(strcasecmp("cd",curr->commands) == 0| strcmp("cd\n", curr->commands) == 0){change_directory_program(curr, directory);return 1;}
    if(strcasecmp("status",curr->commands) == 0){status(); return 1;}
    if(curr->back == 1){
        int error;
        pid_t child_pid;
        child_pid = fork();
        //If there is no input output redirection for the child process, set file descriptor of the child to NULL
        if(curr->inputfiles == NULL | curr->outputfiles == NULL){
            //FIX   if(dup2("/dev/null",1) == -1){ perror("dup2"); return -1;}
            //FIX   if(dup2("/dev/null",1) == -1){ perror("dup2"); return -1;}
        }
        //child file descriptors must be set to specified value 
        else{
            if(curr->inputfiles != NULL){
                //FIX   if(dup2(curr->inputfiles,1) == -1){ perror("dup2"); return -1;}
            }
            if(curr->outputfiles != NULL){
                //FIX   if(dup2(curr->outputfiles,0) == -1){ perror("dup2"); return -1;}
            }
        }
        error = execvp(curr->commands, curr->args);
        if(error == -1){
            perror("execcvp");
            return 0;
        }

    }
    return 5;
}

int run(char * directory){
    // printf("PWD: %s\n", directory);
    int e = 0;
    printf(" :");
    struct line* current_line = create_line();
    if(current_line == NULL){
        return 1;
    }
    print_line(current_line);
    e = run_line(current_line, directory);
    return e;
}   

int main(){
    int l = getpid();
    char * HomeDirectory = calloc(sizeof(char *), 200);
    char s[500]; 
    HomeDirectory = getcwd(s, 100);
    //printf("PWD:%s\n",HomeDirectory);
    int exit = 1;
    while(exit != 0){
        exit = run(HomeDirectory);
    }
    printf("Exitted Successfully");
}