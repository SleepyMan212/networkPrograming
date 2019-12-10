#include <iostream>
#include <cstdlib>
#include <cstdio>
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
#include <arpa/inet.h>
#include "Http.hpp"
#include "Server.hpp"
#include "Json.hpp"
#ifndef UserManage
#define UserManage
#include "Manage.hpp"
#endif


Server::Server()
{
    this->yes = 1;
    this->addrlen = sizeof(this->client);
	memset(&hints, 0, sizeof(this->hints));
    this->hints.ai_family = PF_INET;            // use IPv4
    this->hints.ai_socktype = SOCK_STREAM;
    this->hints.ai_flags = AI_PASSIVE;          // fill in my IP for me
    getaddrinfo(NULL, "8700", &(this->hints), &(this->serverInfo));
    FD_ZERO(&master);                           //clear the master and temp sets
    FD_ZERO(&read_fds);

    // set timeout
    this->tv.tv_sec = 1;
    this->tv.tv_usec = 0;

    this->userManage = new Manage();

    this->buildServer();

}

void * Server::get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
void Server::buildServer(){
    if((sockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol))==0){
        cerr<<"socket error!";
		exit(0);
	}

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if(( bind(sockfd, serverInfo->ai_addr, serverInfo->ai_addrlen))<0){
		cerr<<"bind error";
		exit(0);
	}

    freeaddrinfo(serverInfo);
    
    if((listen(sockfd,5))<0){
        cerr<<"listen error"<<endl;
    }
    cout<<"Listen on 127.0.0.1:8700"<<endl;
    // add the sockfd to the master set
    FD_SET(sockfd, &master);

    // keep track of the biggest file descriptor
    fdMax = sockfd; // so far, it's this one
    // main loop
    for(;;){
        read_fds = master;  // copy it
        int ret = select(fdMax+1,&read_fds,NULL,NULL,&tv);
        if(ret == -1){
            cerr<<"select error";
            exit(4);
        }
        for(int i=0; i<=fdMax; i++){
            // cerr<<"fd = "<<i<<endl;
            if(FD_ISSET(i,&read_fds)){
                if(i==sockfd){
                    connfd = accept(sockfd,(struct sockaddr*)&client,&addrlen);   
                    if(connfd==-1){
                        cerr<<"accept error";
                    }else{
                        FD_SET(connfd,&master);         // add to master fd_set
                        fdMax = max(fdMax,connfd);      // update fdMax
                        printf("selectserver: new connection from %s on socket %d\n",
                            inet_ntop(client.ss_family,
                            get_in_addr((struct sockaddr*)&client),
                            remoteIP, INET6_ADDRSTRLEN),
                            connfd);
                    }
                }else{
                    str = "";
                    this->handleClient(i);
                }
                
            }
        }
    }
}
/* 
handleServer(int fd);
fd is the client file descriptor
*/
void Server::handleClient(int cfd){
    int nbytes;
    // str = "";
    if ((nbytes = recv(cfd, buffer, sizeof(buffer), 0)) <= 0) {
        if (nbytes == 0) {
            // connection closed
            printf("selectserver: socket %d hung up\n", cfd);
        } else {
            perror("recv");
        }
        cerr<<"terminate the fd "<<cfd<<endl;
        close(cfd); // bye!
        FD_CLR(cfd, &master); // remove from master set
    }else{
        // we got some data from client
        string tmp;
        cerr<<"--------Begin of request--------"<<endl;
        cerr<<buffer<<endl;
        cerr<<"---------End of request---------"<<endl;
        Http http(buffer);

        // dispathch to correct function
        string method = http.getMethod();
        string path = http.getPath();
        cerr<<"PATH = "<<path<<endl;
        cerr<<"METHOD = "<<method<<endl;

        if(method=="GET"&&path=="/"){
            http.setHeader("Content-Type","text/html; charset=utf-8");
            tmp = http.getResponse();
        }else if(method=="POST"&&path=="/playerlist.html"){
            string name = http.getFormdata("userName");
            userManage->addUser(User(name,cfd));
            http.setHeader("Content-Type","text/html; charset=utf-8");
            http.setHeader("Set-Cookie","userName="+name);
            cerr<<http.getCookie("userName")<<endl;
            tmp = http.getResponse();
        }else if(method=="GET"&&path=="/getAllUser"){
            cerr<<"get all user"<<endl;
            vector<string> v = userManage->getAllUser();
            Json json;
            json.addJson("users",v);

    // "Content-Type: text/html; charset=utf-8\r\n";
            http.setHeader("Content-Type","application/json; charset=utf-8'");
            tmp = http.getResponse(json.getJson());
        }
        // Send to client
        const char *aa=tmp.c_str();
        int ttt = send(cfd,aa,strlen(aa),0);

        // // close client
        close(cfd); // bye!
        FD_CLR(cfd, &master); // remove from master set

    }
}
Server::~Server(){
    delete this->userManage;
}