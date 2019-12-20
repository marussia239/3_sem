#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int flag = 0;

int rfile(char** str)
{
    const int symb = 10000000;
    int fd = open("file.txt", O_RDONLY);
    char* string = malloc(symb*sizeof(char));
    int byte = read(fd, string, symb);
    string = realloc(string, byte);
    *str = string;
    close(fd);
    return byte;
}

void wfile(char* string, int sizestr)
{
    int fd = open("copy.txt", O_WRONLY | O_CREAT, 0644);
    write(fd, string, sizestr);
    close(fd);
}

void sigp(int signo)
{
    flag = 1;
    printf("Received: SIGUSR1\n");
}

void sigch(int signo)
{
    static char* str = NULL;
    if(!str) str = calloc(10000000, sizeof(char));
    static int bit = 0;
    switch (signo)
    {
        case SIGUSR1:
            if (str[bit/8] & (1 << (bit % 8)))
            {
                printf("%s\n", str);
                wfile(str, bit/8);
                exit(0);
            }
            str[bit/8] |= 1 << (bit % 8);
            printf("Received: SIGUSR1\n");
            break;
        case SIGUSR2:
            bit++;
            printf("Received: SIGUSR2\n");
            break;
    }
    kill(getppid(), SIGUSR1);
}

void waitch()
{
    while (flag == 0)
        usleep(1);
    flag = 0;
}

int main(void)
{
    char* pstr;
    signal(SIGUSR1, sigch);
    signal(SIGUSR2, sigch);
    int pid = fork();
    if (pid)
    {
        signal(SIGUSR1, sigp);
        int bytes = rfile(&pstr);
        for (int i = 0; i < bytes*8; i++)
        {
            if (pstr[i/8] & (1 << (i % 8)))
            {
                kill(pid, SIGUSR1);
                waitch();
                kill(pid, SIGUSR2);
                waitch();
            }
            else
            {
                kill(pid, SIGUSR2);  
                waitch();
            }         
        }
        kill(pid, SIGUSR1);
        waitch();
        kill(pid, SIGUSR1);
    }
    else
    {
        while(1) 
            sleep(1);
    }
    return 0;
}
