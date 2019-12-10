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

User Manage::getUser(string name){
    for (size_t i = 0; i < users.size(); i++){
        if(users[i].getName()==name)
            return users[i];
    }
    
}
void Manage::setOpponent(string name1,string name2){
        User u1 = getUser(name1);
        User u2 = getUser(name2);
        // User a;
        // opponents.insert(pair<User, User>(a,a));
        opponents[u1]=u2;
        opponents[u2]=u1;
}
void Manage::setOpponent(User u1,User u2){
        opponents[u1]=u2;
        opponents[u2]=u1;
}
void Manage::applyOpponent(string name1,string name2){
        User u1 = getUser(name1);
        User u2 = getUser(name2);
        u1.setStatus(1);
        u2.setStatus(2);
        setOpponent(u1,u2);
}
void Manage::playWithOpponent(string name1,string name2){
        User u1 = getUser(name1);
        User u2 = getUser(name2);
        u1.setStatus(4);
        u2.setStatus(4);
}
void Manage::resetOpponent(string name1,string name2){
        User u1 = getUser(name1);
        User u2 = getUser(name2);
        opponents.erase(u1);
        opponents.erase(u2);
        u1.setStatus(0);
        u2.setStatus(0);
}
string Manage::getOpponent(string name){
    map<User, User>::iterator iter;
    User u = getUser(name);
    iter = opponents.find(u);
    if(iter!=opponents.end()){
        return iter->second.getName();
    }else{
        return "";
    }
    
}