#include <iostream>

using namespace std;
/*
    status
        0-> wait
        1-> apply
        2-> answer apply
        4-> playing
*/
class User
{
private:
    /* data */
    string name;
    int sfd;
    int status;
    // User opponent;
public:
    User(string name,int fd);
    User();
    // ~User();
    // void set_Opponent(const User &opponent);
    string getName();
    int getStatus();
    void setStatus(int t);
    bool operator() (const User& lhs, const User& rhs) const;
    bool operator <(const User& rhs) const;

};

