#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(){
	char inputBuffer[256] = {};
	char message[] = {"Hi,this server.\n"};
	int sd,connfd;
	struct sockaddr_in serverInfo,clientInfo;
	int addrlen = sizeof(clientInfo);
	// init
	memset(&serverInfo,0,sizeof(serverInfo));
	
	serverInfo.sin_family = PF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(8700);	

	if((sd = socket(AF_INET,SOCK_STREAM,0))==0){
		perror("socket error!");
		exit(0);
	}

	if((bind(sd,(struct scokaddr*)&serverInfo,sizeof(serverInfo)))<0){
		perror("bind error");
		exit(0);
	}

	listen(sd,5);
	for(;;){
		connfd = accept(sd,(struct sockaddr*)&clientInfo,&addrlen);
		send(connfd,message,sizeof(message),0);
		recv(connfd,inputBuffer,sizeof(inputBuffer),0);
		printf("Get: %s\n",inputBuffer);
	}

	return 0;
}

