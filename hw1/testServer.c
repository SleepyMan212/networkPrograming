#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(){
	pid_t pid;
	char inputBuffer[2000000] = {};
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
			send(connfd,data,sizeof(data),0);
			int t = recv(connfd,inputBuffer,sizeof(inputBuffer),0);
			if(t>=0){
				char *ret = strstr(inputBuffer,"POST");
				if(ret){
					printf("Get: %s\n",inputBuffer);
					printf("-------------------------------------\n");
					// printf("t = %d\n",t);
					// printf("Get: %s\n",inputBuffer);
					printf("A POST received\n");
					// printf("%s\n",ret);		

					char imgWord[] = "Content-Type: image/";
					char splitWord[] = "-----------------------------";
					char imgType[10] = {};
					char filename[256] = {};
					int k = 0;
					FILE* tmp = NULL;
					// find filename
					char tmpWord[] = "filename=\"";
					ret = strstr(inputBuffer,tmpWord);
					for(k=0;ret[k+strlen(tmpWord)]!='"'; k++){
						filename[k]=ret[k+strlen(tmpWord)];
					}
					filename[k]='\0';
					printf("filename = %s\n",filename);

					// find begin of image
					ret = strstr(ret,imgWord);
					for(k=0;ret[k+strlen(imgWord)]!='\n'; k++);

					// find image size
					int offset = strlen(imgWord)+k+3;
					char *start =ret+offset;
					int size = 0;
					for(size=0; size<sizeof(inputBuffer);size++){
						if(!strncmp((start+size),splitWord,strlen(splitWord))) break;	
					}
					printf("size = %d\n",size);

					// write image to file
					tmp = fopen(filename,"w+");
					fwrite(start,1,size,tmp);
					// printf("-------------------------------------\n");

					fclose(tmp);
				}
			}
			close(connfd);
			exit(0);
		}else{
			close(connfd);
			int waitStatus;
			wait(&waitStatus);
		}
	}

	return 0;
}

