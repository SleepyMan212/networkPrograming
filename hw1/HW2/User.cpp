#include <iostream>
#include <string>
#include "User.hpp"


using namespace std;

User::User(string name,int fd){
    this->name = name;
    this->sfd = fd;
    this->status = 0;
    this->player = 0;
}
User::User(){
    this->name = "";
    this->player = 0;
    this->status = 0;
}
const string User::getName() const {
    return name;
}
int User::getStatus(){
    return status;
}
void User::setStatus(int t){
    this->status = t;
}
// bool operator() (const User& lhs, const User& rhs) const{
//        return lhs.name < rhs.name;
// }
bool User::operator <(const User& rhs) const{
        return name < rhs.name;
}
int User::getPlayer(){
    return this->player;
}
void User::setPlayer(int t){
    this->player = t;
}
// User::~User()
// {
// }
// void User::set_Opponent(const User &opponent){
//     this->opponent = opponent;
// }
