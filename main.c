#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_BUILTIN 3

void process_input(char*);
void echo(char** , int);
void type(char*);
int is_shell_builtin(char*);
char* find_path(const char*);
int is_executable(const char*);

const char *shell_builtin[NUM_BUILTIN] = {"echo", "exit", "type"};

int main(){
    while(1){
        setbuf(stdout, NULL);
        printf("$");
        char input[100];
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';

        process_input(input);
        
    }
    return 0;
}

void process_input(char* input){
    //clean input 
    char *argv[10];
    int argc = 0;
    char *token = strtok(input, " ");
    while (token != NULL){
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL;

    //check for commands
    //exit
    if (!strcmp(argv[0], "exit")){
        exit(0);
    }
    
    //echo 
    if (!strcmp(argv[0], "echo")){
        echo(argv, argc);
    }

    //type
    if (!strcmp(argv[0], "type")){
        type(argv[1]);
    }
}

void echo(char** argv, int argc){
    for (int i = 1; i < argc; i++){
        printf("%s", argv[i]);
    }
    printf("\n");
}

void type(char *argv){
    if (is_shell_builtin(argv)){
        printf("%s is a shell builtin\n", argv);
    }else{
        char *dir_path = find_path(argv);
        if (dir_path != NULL){
            printf("%s is %s\n", argv, dir_path);
        }
        else{
            printf("%s not found\n", argv);
        }
    }
}

int is_shell_builtin(char *argv){
    for (int i =0; i < NUM_BUILTIN; i++){
        if (!strcmp(argv, shell_builtin[i]))
            return 1;
    }
    return 0;
}

char* find_path(const char* arg){
    char* path_env = getenv("PATH");
    if (path_env == NULL)
        return NULL;
    
    char* path_copy = strdup(path_env);
    char* dir = strtok(path_copy, ":");
    static char full_path[1024];

    while (dir != NULL){
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, arg);
        if (is_executable(full_path)){
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

int is_executable(const char* path){
    return access(path, X_OK) == 0;
}