#include <iostream>

using namespace std;
/*
    status
        0-> wait
        1-> apply
        2-> answer apply
        3-> playing
    player
        -1  ->  X
        1   ->  O
*/
class User
{
private:
    /* data */
    string name;
    int sfd;
    int status;
    int player;
    // User opponent;
public:
    User(string name,int fd);
    User();
    // ~User();
    // void set_Opponent(const User &opponent);
    const string getName() const;
    int getStatus();
    void setStatus(int t);
    // bool operator() (const User& lhs, const User& rhs) const;
    bool operator <(const User& rhs) const;
    int getPlayer();
    void setPlayer(int t);


};

