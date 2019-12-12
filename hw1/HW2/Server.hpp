#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
// #include "Manage.hpp"
#ifndef UserManage
#define UserManage
#include "Manage.hpp"
#include "Ooxx.hpp"
#endif
#define BUFFERSIZE 102400
using namespace std;
class Server
{
private:
    int sockfd;                                 // listening socket descriptor
    int connfd;                                 // newly accept()ed socket descriptor
    int fdMax;                                  // maximum file descriptor number
    socklen_t addrlen;
    // pid_t pid;
    fd_set master;                              // master file descriptor list
    fd_set read_fds;                            // temp file descriptor list for select()
    struct sockaddr_storage client;
    struct addrinfo hints, *serverInfo;
    int yes;                                    // 供底下的 setsockopt() 設定 SO_REUSEADDR
    char remoteIP[INET6_ADDRSTRLEN];
    char buffer[BUFFERSIZE];
    int ret;
    struct timeval tv;
    string str;
    vector<Ooxx> boards;

public:
    Server(/* args */);
    ~Server();
    void *get_in_addr(struct sockaddr *sa);     // get sockaddr, IPv4 or IPv6:
    void buildServer();
    void handleClient(int fd);
    Manage *userManage;
};
