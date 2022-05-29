#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#define SIZE 4096*1024
using namespace std;

char (*stack)[1024]= (char(*)[1024])mmap(0, SIZE,PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);
int * top = (int*)mmap(NULL,8,PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);

////////////////////////////////////////////////////////////////////////////////////////////////////////TOP COMMAND///////////////////////////////////

string TOP() {
    if (*top == -1) {
        return "STACK IS EMPTY";
    } else {
        return stack[*top];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////PUSH COMMAND///////////////////////////////////


void PUSH(char * value) {
   *top=*top+1;
   strcpy(stack[*top],value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////POP COMMAND///////////////////////////////////

void POP() {
    if (*top == -1) {
        //do nothing
    } else {
        memset(stack[*top],0,1024);
        *top=*top-1;
    }
}