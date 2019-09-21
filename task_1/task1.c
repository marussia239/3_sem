#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h>
#include <string.h>

int main()
{
	while(1) {
        const pid_t pid = fork();
        if (pid) {
            int status;
            waitpid(pid, &status, 0);
            if (WEXITSTATUS(status) == 42)
                printf("Parsing error");
            else
                printf("Ret code: %d\n", WEXITSTATUS(status));
            
        } 
        else {
            char* string = malloc(2097152);
            char** parsedstring = malloc(1000000 * sizeof(char*));
            fgets(string, 2097152, stdin);
            int i = 0;
            for (char *p = strtok(string, " \n"); p != NULL; p = strtok(NULL, " \n"))
                parsedstring[i++] = p;
            parsedstring[i] = NULL;
            execvp(parsedstring[0], parsedstring);
            exit(42);
        }
    }
}