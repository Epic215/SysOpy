#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

int global=0;

int main(int argc, char *argv[]) {
    DIR* dir = opendir(argv[1]);
    if (dir) {
        closedir(dir);
    } else {
        printf("Katalog nie istnieje\n");
        return -1;
    }
    if(argc != 2) {
        printf("Zla ilosc argumentow\n");
        return -1;
    }
    printf("Nazwa programu: %s\n",argv[0]);
    int pid=fork();
    int local=0;
    if (pid==0){
        printf("child process\n");
        local++;
        global++;
        printf("child pid = %d, parent pid = %d\n",getpid(),getppid());
        printf("child's local = %d, child's global = %d\n",local,global);
        int status=execl("/bin/ls", "ls", "-l",argv[1],NULL);
        exit(status);
    }
    int wstatus;
    wait(&wstatus);
    int child_exit_code= WIFEXITED(wstatus);
    printf("parent process\n");
    printf("parent pid = %d, child pid = %d\n",getpid(),pid);
    printf("Child exit code: %d\n",child_exit_code);
    printf("parent's local = %d, parent's global = %d\n",local,global);

    return child_exit_code;
}
