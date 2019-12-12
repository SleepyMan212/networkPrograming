#include "Ooxx.hpp"
#include <iostream>
#include <vector>
// #include "User.hpp"

using namespace std;

Ooxx::Ooxx(string user): user(user){
    board = vector<int>(9);
}

Ooxx::~Ooxx()
{
}
vector<int> Ooxx::getBoard(){
    return board;
}
string Ooxx::getName(){
    return user;
}
void Ooxx::setBoard(const vector<int> v){
    this->board = v;
}
