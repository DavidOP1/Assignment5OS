/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
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
#define PORT "3490"  // the port users will be connecting to
#define SIZE 1024
#define BACKLOG 10   // how many pending connections queue will hold
#define PAGESIZE 1024*1024



struct flock lock;
/////////////////////////////////////////////////////////////////////////////////////////STACK////////////////////////////////////////////////////////////////////////////
char (*stack)[1024]= (char(*)[1024])mmap(0, SIZE,PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);
int * top = (int*)mmap(NULL,8,PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);

////////////////////////////////////////////////////////////////////////////////////////////////////////TOP COMMAND///////////////////////////////////

char * TOP() {
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    *top=-1;
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) { // this is the child process
        close(sockfd); // child doesn't need the listener
            // if (send(new_fd,"asd", 13, 0) == -1)
            //     perror("send");
            // close(new_fd);
            // exit(0);
        char command[SIZE];
        int numbytes;
        char buf2[SIZE];
        int numbytes2;
        int sock = new_fd;
        if (send(sock, "connected", strlen("connected"), 0)== -1)
        {
            perror("send"); 
        } 
        int live=1;
        while (live)
        {
        printf("CONNECTED TO CLIENT!\n");
        memset(command,0,strlen(command));
        lock.l_type = F_WRLCK;
        fcntl(sock, F_SETLK, &lock);
        if ((numbytes = recv(sock, command, SIZE-1, 0)) == -1) {
            perror("recv");
            //exit(1);
        }
        command[numbytes] = '\0';
        memset(buf2,0,strlen(buf2));

        ////////////////////////////////////////////////////////////////////////////////////////////////////////POP COMMAND///////////////////////////////////

        if (strlen(command)==0)
        {
            live=0;
            close(sock);
        }
        //printf("pid inside = %d", getpid());
        if(!strcmp("POP",command)){
        POP();
        if (send(sock,"POPPED",strlen("POPPED"), 0)== -1)
            perror("send"); 
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////TOP COMMAND///////////////////////////////////

    else if(!strcmp("TOP",command)){
    //send the string at the top.
    if (send(sock,TOP(),strlen(TOP()), 0)== -1)
        perror("send"); 
        //printf("SENT TOP TO : %d\n",getpid());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////PUSH COMMAND///////////////////////////////////

    else if(!strcmp("PUSH",command)){
     if (send(sock, "GOT PUSH",strlen("GOT PUSH"), 0)== -1)
        perror("send"); 
     if ((numbytes2 = recv(sock, buf2, SIZE-1, 0)) == -1) {
        perror("recv");
        close(sock);
        live=0;
     }
    buf2[numbytes2] = '\0';
    PUSH(buf2);
    }
    lock.l_type = F_UNLCK;
    fcntl(sock, F_SETLKW, &lock);
    }
    while(1){sleep(10000000000000);}//THis is a function since I didnt understand how to kill the thread/

        }
        close(new_fd);  // parent doesn't need this
    }

    return 0;
}