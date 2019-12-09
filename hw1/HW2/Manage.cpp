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

vector<string> Manage::getAllUser(){
    size_t len = this->users.size();
    vector<string> list;

    for (size_t i = 0; i < len; i++){
        list.push_back(users[i].getName());
    }
    return list;
    
}
