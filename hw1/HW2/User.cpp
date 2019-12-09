#include <iostream>
#include <string>
#include "User.hpp"


using namespace std;

User::User(string name,int fd){
    this->name = name;
    this->sfd = fd;
}

// User::~User()
// {
// }
// void User::set_Opponent(const User &opponent){
//     this->opponent = opponent;
// }
