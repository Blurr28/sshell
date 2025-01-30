#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_BUILTIN 5

void process_input(char*);
void echo(char** , int);
void type(char*);
int is_shell_builtin(char*);
char* find_path(const char*);
int is_executable(const char*);
void execute_prog(char**);
void working_dir();
void change_dir(const char*);
void startup_art();
const char *shell_builtin[NUM_BUILTIN] = {"echo", "exit", "type", "pwd", "cd"};

int main(){
    startup_art();
    while(1){
        setbuf(stdout, NULL);
        printf("$ ");
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
    else if (!strcmp(argv[0], "echo")){
        echo(argv, argc);
    }

    //type
    else if (!strcmp(argv[0], "type")){
        type(argv[1]);
    }

    else if (!(find_path(argv[0]) == NULL)){
        execute_prog(argv);
    }

    else if (!strcmp(argv[0], "cd")){
        change_dir(argv[1]);
    }

    else if (!(strcmp(argv[0], "pwd"))){

    }

    else {
        printf("%s: Command not found\n", argv[0]);
    }
}

void echo(char** argv, int argc){
    for (int i = 1; i < argc; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");
}

void type(char *argv){
    if (argv == NULL){
        return;
    }
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

void execute_prog(char** argv){
    pid_t pid = fork();

    if (pid == 0){
        if (execvp(argv[0], argv) == -1){
            perror("execvp failed\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid > 0){
        int status;
        waitpid(pid, &status, 0);
    }
    else{
        perror("fork failed\n");
        exit(EXIT_FAILURE);
    }
}

void working_dir(){
    char* dir = getenv("PWD");
    printf("%s", dir);
}

void change_dir(const char* path){
    char* home = getenv("HOME");
    static char prev_dir[1024];

    if (path == NULL || strcmp(path, "~") == 0){
        path = home;
    } else if (strcmp(path, "-") == 0) {
        if (prev_dir[0] == '\0'){
            fprintf(stderr, "cd: No previous directory\n");
            return;
        }
        path = prev_dir;
    }

    char current_dir[1024];
    getcwd(current_dir, sizeof(current_dir));

    if (chdir(path) != 0){
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "cd: %s", path);
        perror(error_msg);
    } else {
        strcpy(prev_dir, current_dir);
    }

}

void startup_art(){
    printf("\n");
    printf("       _____ _                 _         _____ _          _ _ \n");
    printf("      / ____(_)               | |       / ____| |        | | |\n");
    printf("     | (___  _ _ __ ___  _ __ | | ___  | (___ | |__   ___| | |\n");
    printf("      \\___ \\| | '_ ` _ \\| '_ \\| |/ _ \\  \\___ \\| '_ \\ / _ \\ | |\n");
    printf("      ____) | | | | | | | |_) | |  __/  ____) | | | |  __/ | |\n");
    printf("     |_____/|_|_| |_| |_| .__/|_|\\___| |_____/|_| |_|\\___|_|_|\n");
    printf("                        | |                                   \n");
    printf("                        |_|                                   \n");
    printf("\n");
}