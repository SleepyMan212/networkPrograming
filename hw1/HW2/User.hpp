#include <iostream>

using namespace std;
class User
{
private:
    /* data */
    string name;
    int sfd;
    // User opponent;
public:
    User(string name,int fd);
    // ~User();
    // void set_Opponent(const User &opponent);
    string getName(){
        return name;
    }
};

