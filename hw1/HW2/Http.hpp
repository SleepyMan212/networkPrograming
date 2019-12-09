#include <iostream>
#include <map>
using namespace std;
class Http
{
private:
    /* data */
    string text;
    string method;
    string path;
    void addFormdata(string str);
    map<string, string> formdata;
    string headers;
public:
    Http(const string &request);
    ~Http();
    string getResponse();
    string getResponse(string str);
    string readFile();
    string getMethod();
    string getPath();
    string getFormdata(string str);
    
};
