#include <iostream>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netdb.h>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

using namespace std;
int main(){
    // Declare the variable
    int sockfd,connfd;
    socklen_t addrlen;
    pid_t pid;
    // struct sockaddr_in serverInfo, clientInfo;
    struct sockaddr_storage client;
    struct addrinfo hints, *serverInfo;
    // Init server socket
    addrlen = sizeof(client);
	memset(&hints, 0, sizeof hints);
    hints.ai_family = PF_INET;          // use IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;        // fill in my IP for me
    getaddrinfo(NULL, "8700", &hints, &serverInfo);

    if((sockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol))==0){
		// perror("socket error!");
        cerr<<"socket error!";
		exit(0);
	}
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    if(( bind(sockfd, serverInfo->ai_addr, serverInfo->ai_addrlen))<0){
		cerr<<"bind error";
		exit(0);
	}
    listen(sockfd,5);

    freeaddrinfo(serverInfo);

    // Begin to accept the connection
    for(;;){
        connfd = accept(sockfd,(struct sockaddr*)&client,&addrlen);
        if((pid=fork())==0){
            close(sockfd);
        }else{
            close(connfd);
            int waitStatus;
            wait(&waitStatus);
        }
    }
    
}