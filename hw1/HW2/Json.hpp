#include <iostream>
#include <vector>

using namespace std;

class Json
{
private:
    /* data */
    string text;
    void addComma();
public:
    Json(/* args */);
    ~Json();
    void addJson(string key,vector<string> v);
    void addJson(string key,vector<int> v);
    void addJson(string key,string value);
    void addJson(string key,int value);
    string getJson();
    void clear();
};


