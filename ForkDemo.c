#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int iStatus;
    int iPid = fork();
    if(iPid <0){
        printf("fork error\n");
    }
    else if(iPid == 0){
        printf("child process\n");
    }
    else if(iPid > 0)
    {
        waitpid(iPid,&iStatus,0);
        printf("parent\n");
    }


    return 0;
}