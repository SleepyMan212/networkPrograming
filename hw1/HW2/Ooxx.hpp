#include <iostream>
#include <vector>

using namespace std;

class Ooxx
{
private:
    /* data */
    // vector<User> board;
    string user;
    vector<int> board;
public:
    Ooxx(string user);
    ~Ooxx();
    void setBoard(const vector<int> v);
    vector<int> getBoard();
    string getName();
};

