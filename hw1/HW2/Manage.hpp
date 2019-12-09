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
    // vector<string> users;
public:
    Manage();
    ~Manage();
    void addUser(const User &user);
    vector<string> getAllUser();
};
