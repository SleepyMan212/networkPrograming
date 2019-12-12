#include <iostream>
#include <map>
#include <vector>
// #ifndef User
// #define User
// #include "User.hpp"
// #endif // MACRO
// #ifndef UserManage
// #define UserManage
#include "Manage.hpp"
// #endif

using namespace std;

Manage::Manage()
{
}

Manage::~Manage()
{
}
void Manage::addUser(const User &user){
// void Manage::addUser(User user){
    // cerr<<users.size()<<endl;
    users.push_back(user);
}

vector<string> Manage::getAllWaitingUser(){
    size_t len = this->users.size();
    vector<string> list;

    for (size_t i = 0; i < len; i++){
        if(users[i].getStatus()==0)
        list.push_back(users[i].getName());
    }
    return list;
    
}

const int Manage::getUser(string name)const {  
    for (size_t i = 0; i < users.size(); i++){
        // users[i].setPlayer(-1);
        if(users[i].getName()==name)
            // cerr<<"user name is "<<users[i].getName()<<endl;
            // cerr<<"1: user address is "<<&users[i]<<endl;
            return i;
    }
    return -1;
    
}
void Manage::setOpponent(string name1,string name2){
        int u1 = getUser(name1);
        int u2 = getUser(name2);
        // User a;
        // opponents.insert(pair<User, User>(a,a));
        opponents[users[u1]]=users[u2];
        opponents[users[u2]]=users[u1];
}
void Manage::setOpponent(User u1,User u2){
        opponents[u1]=u2;
        opponents[u2]=u1;
}
void Manage::applyOpponent(string name1,string name2){
        int u1 = getUser(name1);
        int u2 = getUser(name2);
        users[u1].setStatus(1);
        users[u2].setStatus(2);
        users[u1].setPlayer(-1);
        users[u2].setPlayer(1);
        setOpponent(users[u1],users[u2]);
}
void Manage::playWithOpponent(string name1,string name2){
        int u1 = getUser(name1);
        int u2 = getUser(name2);
        users[u1].setStatus(3);
        users[u2].setStatus(3);
}
void Manage::resetOpponent(string name1,string name2){
        int u1 = getUser(name1);
        int u2 = getUser(name2);
        if(u1==-1) return;
        opponents.erase(users[u1]);
        users[u1].setStatus(0);
        if(u2==-1) return;
        opponents.erase(users[u2]);
        users[u2].setStatus(0);
}
string Manage::getOpponent(string name){
    map<User, User>::iterator iter;
    int u = getUser(name);
    iter = opponents.find(users[u]);
    if(iter!=opponents.end()){
        return iter->second.getName();
    }else{
        return "";
    }
    
}
int Manage::getUserStatus(string name){
    int u = getUser(name);
    return users[u].getStatus();
}
int Manage::getUserPlayer(string name){
    int u = getUser(name);
    cerr<<"2: user address is "<<&u<<endl;
    cerr<<"click name = "<<users[u].getName()<<endl;
    cerr<<"click player = "<<users[u].getPlayer()<<endl;
    return users[u].getPlayer();
}
bool Manage::delUser(const string name){
    int u1 = getUser(name);
    if(u1==-1) return true;
    vector<User>::iterator iter;
    for(vector<User>::iterator iter=users.begin(); iter!=users.end();){
        if(iter->getName()==name){
            iter = users.erase(iter);
            return true;
        }else{
            iter++;
        }
    }
    return false;
}
bool Manage::isUserExist(string name){
    int u1 = getUser(name);
    if(u1==-1) return false;
    return true;
}