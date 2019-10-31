#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(){
	pid_t pid;
	char inputBuffer[200000] = {};
	//char message[] = {"Hi,this server.\n"};
	//char message[] = {"<h1>Hello, this is the network programming home work.</h1><img src='./test.jpg'/>"};
	char message[] = {"<h1>Hello, this is the network programming home work.</h1><img src='https://timesofindia.indiatimes.com//thumb/msid-67586673,width-800,height-600,resizemode-4/67586673.jpg'/><form enctype='multipart/form-data' action='./' method='POST'><input type='file' name='img'><input type='submit'></form>"};
	int sd,connfd;
	struct sockaddr_in serverInfo,clientInfo;
	int addrlen = sizeof(clientInfo);
	char header[] = "HTTP/1.1 200 OK\r\nServer: CPi\r\nContent-Type\r\nContent-length:200000\r\n\r\n";
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
		puts("new connection be builded!!");
		if((pid=fork())==0){
			close(sd);
			char data[2048] = {};
			snprintf(data,sizeof(data),"%s %s",header,message);
//			printf("%s\n",data);
			send(connfd,data,sizeof(data),0);
			// begin to send img
/*			char p_array[2048];
			FILE *image = fopen("test.jpg","r");
			fseek(image,0,SEEK_END);
			int size = ftell(image);
			fseek(image,0,SEEK_SET);
			printf("Total Picture size = %d\n",size);

			printf("Sending Picture size\n");
*/
//			send(connfd,&size,sizeof(int),0);
			//send(connfd,(void*)&size,sizeof(int),0);
/*
			int nb = fread(data,1,sizeof(data),image);
			while(!feof(image)){
				send(connfd,data,sizeof(data),0);
			 	nb = fread(data,1,sizeof(data),image);
			}
*/
//			send(connfd,header,sizeof(header),0);
//			send(connfd,message,sizeof(message),0);
			int t = recv(connfd,inputBuffer,sizeof(inputBuffer),0);
//			printf("t = %d\n",t);
			if(t>=0){
				printf("Get: %s\n",inputBuffer);
				char *ret = strstr(inputBuffer,"POST");
				if(ret){
					printf("A POST received\n");
					ret = strstr(inputBuffer,"Content-Type: image/");
					printf("ret");
					printf("\n---------------\n");
					printf("%s\n",ret);
					FILE* tmp = fopen("aa.txt","w+");
					printf("sizeof(ret)=%d\nstrlen(ret=%d\n",sizeof(ret),strlen(ret));
					//int t = fwrite(ret,1,sizeof(ret),tmp);
					//fprintf(stderr,"t = %d\n",t);
					for(int i=0; ret[i]!=0; i++){
						printf("%c",ret[i]);
						fwrite(&ret[i],1,sizeof(char),tmp);
					}

					fclose(tmp);
					printf("\nend of write----------------\n");
				//	printf("%s\n",ret);
				}
			}
//			for(;;);
			close(connfd);
	//		sleep(5);
			exit(0);
		}else{
			close(connfd);
			int waitStatus;
			wait(&waitStatus);
		}
	}

	return 0;
}

