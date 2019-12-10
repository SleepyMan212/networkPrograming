#include <iostream>
#include "User.hpp"
#include <map>
#include <vector>
using namespace std;
class Manage
{
private:
    /* data */
    vector<User> users;
    map<User,User> opponents;
    // vector<string> users;
public:
    Manage();
    ~Manage();
    void addUser(const User &user);
    vector<string> getAllWaitingUser();
    void setOpponent(string name1,string name2);
    void setOpponent(User u1,User u2);
    // void setOpponent(const User &user);
    // const User getOpponent(const User &user1,const User &user2);
    string  getOpponent(string name);
    // const User getOpponentAns(string name1,string name2);
    User getUser(string name);
    void applyOpponent(string name1,string name2);
    // User getUser(const User &user);
    void resetOpponent(string name1,string name2);
    void playWithOpponent(string name1,string name2);

};
