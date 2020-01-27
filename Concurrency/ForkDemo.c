//note fork is not part of posix and therefore this code will only work in Linux

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int iStatus;
    //fork process
    int iPid = fork();
    //executes all of code following fork
    if(iPid < 0){
        printf("its shagged mate\n");
    }
    //process id assinged to child is 0 so...
    else if(iPid ==0){
        //print from child process..
        printf("child\n");
    }
    //else iPID greater than 0 suggesting it is the parent process..
    else if(iPid > 0){
        //so wait until child thread has executed before executing..
        waitpid(iPid,&iStatus,0);
        //before printing from parent
        printf("parent\n");
    }
}