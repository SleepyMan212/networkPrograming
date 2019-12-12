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
#include <string>
#include <arpa/inet.h>
#include "Http.hpp"
#include "Server.hpp"
#include "Json.hpp"
#ifndef UserManage
#define UserManage
#include "Manage.hpp"
#endif


Server::Server(){
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

void * Server::get_in_addr(struct sockaddr *sa){
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
        char tt[1000];
        
        Http http(buffer);

        // dispathch to correct function
        string method = http.getMethod();
        string path = http.getPath();
        string name = http.getCookie("userName");
        if(name.empty()){
            name = http.getFormdata("userName");
            userManage->addUser(User(name,cfd));
            http.setHeader("Set-Cookie","userName="+name);
        }else if(!userManage->isUserExist(name)){
            userManage->addUser(User(name,cfd));
        }
        cerr<<"PATH = "<<path<<endl;
        cerr<<"METHOD = "<<method<<endl;

        if(method=="GET"&&path=="/"){
            http.setHeader("Content-Type","text/html; charset=utf-8");
            tmp = http.getResponse();
        }else if(method=="POST"&&path=="/playerlist.html"){
            http.setHeader("Content-Type","text/html; charset=utf-8");
            
            tmp = http.getResponse();
        }else if(method=="GET"&&path=="/getAllUser"){
            cerr<<"get all user"<<endl;
            string name = http.getCookie("userName");
            cerr<<"name = "<<name<<endl;
            vector<string> v = userManage->getAllWaitingUser();
            
            for(vector<string>::iterator iter=v.begin(); iter!=v.end();){
                if(*iter==name){
                    iter = v.erase(iter);
                    break;
                }else{
                    iter++;
                }
            }
            
            Json json;
            json.addJson("users",v);
            http.setHeader("Content-Type","application/json; charset=utf-8");
            tmp = http.getResponse(json.getJson());
        }else if(method=="GET"&&path=="/playwith"){
            string t1 = http.getCookie("userName");
            string t2 = http.getParamdata("opponent");
            userManage->applyOpponent(t1,t2);
            int player = userManage->getUserPlayer(t1);
            sprintf(tt,"player=%d",player);
            http.setHeader("Set-Cookie",string(tt));  // maybe have bug
            tmp=http.getResponse("");
        }else if(method=="GET"&&path=="/isHaveChanllege"){
            Json json;
            string t1 = http.getCookie("userName");
            // cerr<<"t1 = "<<t1<<endl;
            // cerr<<"wqeeqe = "<<userManage->getUserStatus(t1)<<endl;
            if(userManage->getUserStatus(t1)==2){
                string opponent = userManage->getOpponent(t1);
                cerr<<"opponent = "<<opponent<<endl;
                json.addJson("opponent",opponent);
            }else{
                json.addJson("opponent","");
            }
            
            http.setHeader("Content-Type","application/json; charset=utf-8'");
            tmp = http.getResponse(json.getJson());
        }else if(method=="POST"&&path=="/reply"){
            string t1 = http.getCookie("userName");
            string t2 = userManage->getOpponent(t1);
            string a = http.getFormdata("answer");
            int ans = a.empty()?0:atoi(a.c_str());
            int status = userManage->getUserStatus(t1);
            cerr<<"ans = "<<ans<<endl;
            // int ans;
            if(ans==1&&status==2){         // opponent accept
                userManage->playWithOpponent(t1,t2);
                int player = userManage->getUserPlayer(t1);
                sprintf(tt,"player=%d",player);
                http.setHeader("Set-Cookie",string(tt));  // maybe have bug
                this->boards.push_back(Ooxx(t1));
                this->boards.push_back(Ooxx(t2));
            }else{  // opponent reject
                userManage->resetOpponent(t1,t2);
            }

            tmp=http.getResponse("");
            // cerr<<"res = "<<tmp<<endl;
        }else if(method=="GET"&&path=="/isPlaying"){
            string t1 = http.getCookie("userName");
            int t = userManage->getUserStatus(t1);
            int status = t==3?1:0;
            // int player = userManage->getUserPlayer(t1);
            Json json;
            json.addJson("status",status);
            // json.addJson("player",player);
            http.setHeader("Content-Type","application/json; charset=utf-8");
            tmp = http.getResponse(json.getJson());
        }else if(method=="GET"&&path=="/play.html"){
            http.setHeader("Content-Type","text/html; charset=utf-8");
            tmp = http.getResponse();
        }else if(method=="POST"&&path=="/chess"){
            cerr<<"--------Begin of request--------"<<endl;
            cerr<<buffer<<endl;
            cerr<<"---------End of request---------"<<endl;
            string t1 = http.getCookie("userName");
            string t2 = userManage->getOpponent(t1);
            int place = atoi(http.getFormdata("place").c_str());
            cerr<<"place = "<<place<<endl;
            int player = userManage->getUserPlayer(t1);
            cerr<<"player = "<<player<<endl;
            if(player != 0){
                vector<int> board;
                int idx1,idx2;
                idx1=idx2=0;
                // get board and user's board
                for (size_t i = 0; i < boards.size(); i++){
                    if(boards[i].getName()==t1){
                        board = this->boards[i].getBoard();
                        idx1 = i;
                    }else if(boards[i].getName()==t2){
                        idx2 = i;
                    }else if(idx1!=0&&idx2!=0) break;
                }
                cerr<<"AAAAA"<<endl;
                // update board
                board[place] = player;
                boards[idx1].setBoard(board);          
                boards[idx2].setBoard(board);
            }

            tmp=http.getResponse("");
        }else if(method=="GET"&&path=="/getBoard"){
            string t1 = http.getCookie("userName");
            vector<int> board;
            cerr<<"t1 = "<<t1<<endl;
            for (size_t i = 0; i < boards.size(); i++){
                cerr<<"board name = "<<boards[i].getName()<<endl;
                if(boards[i].getName()==t1){
                    board = this->boards[i].getBoard();
                    break;
                }
            }
            cerr<<board.size()<<endl;
            Json json;
            json.addJson("board",board);

            http.setHeader("Content-Type","application/json; charset=utf-8'");
            tmp = http.getResponse(json.getJson());
        }else if(method=="GET"&&path=="/logout"){
            string t1 = http.getCookie("userName");
            string t2 = userManage->getOpponent(t1);
            userManage->resetOpponent(t1,t2);
            userManage->delUser(t1);
            http.setHeader("Set-Cookie","userNmae="+t1+"; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT");
            // http.setHeader("Set-Cookie","player="+t1+"; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT");
            // Set-Cookie: token=deleted; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT
            http.setHeader("Content-Type","text/html; charset=utf-8");
            tmp = http.getResponse("ok");
        }else{
            cerr<<"Network error"<<endl;
            cerr<<"path = "<<path<<endl;
            tmp = http.getResponse("path have error");
        }
        
        
        // Send to client
        const char *aa=tmp.c_str();
        // cerr<<"aa = "<<aa<<endl;
        int ttt = send(cfd,aa,strlen(aa),0);

        // // close client
        close(cfd); // bye!
        FD_CLR(cfd, &master); // remove from master set

    }
}
Server::~Server(){
    delete this->userManage;
}