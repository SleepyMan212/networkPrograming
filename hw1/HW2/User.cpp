#include <iostream>
#include <string>
#include "User.hpp"


using namespace std;

User::User(string name,int fd){
    this->name = name;
    this->sfd = fd;
    this->status = 0;
}
User::User(){

}
string User::getName(){
    return name;
}
int User::getStatus(){
    return status;
}
void User::setStatus(int t){
    status = t;
}
// bool operator() (const User& lhs, const User& rhs) const{
//        return lhs.name < rhs.name;
// }
bool User::operator <(const User& rhs) const{
        return name < rhs.name;
}
// User::~User()
// {
// }
// void User::set_Opponent(const User &opponent){
//     this->opponent = opponent;
// }
