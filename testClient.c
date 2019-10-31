#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(){
	int sd,connfd;
	struct sockaddr_in info;
	// init
	memset(&info,0,sizeof(info));
	info.sin_family = PF_INET;
	info.sin_addr.s_addr = inet_addr("127.0.0.1");
	info.sin_port = htons(8700);

	if((sd = socket(AF_INET,SOCK_STREAM,0))==0){
		perror("socket error!");
		exit(1);
	}

	if((connfd = connect(sd,(struct sockaddr*)&info,sizeof(info)) < 0)){
		perror("connect error");
		exit(1);
	}	

	char *msg = "Hi there";
	char recMsg[100] = {};
	send(sd,msg,sizeof(msg),0);
	recv(sd,recMsg,sizeof(recMsg),0);

	printf("client Socket\n");
	printf("Get: %s\n",recMsg);
	close(sd);
	return 0;
}

