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
    void addCookie(string str);
    void addParam(string str);
    map<string, string> formdata;
    map<string, string> paramdata;
    map<string,string> headers;
    map<string,string> cookies;
public:
    Http(const string &request);
    ~Http();
    string getResponse();
    string getResponse(string str);
    string readFile();
    string getMethod();
    string getPath();
    string getFormdata(string str);
    string getParamdata(string str);
    string getHeaders();
    string getCookie(string str);
    void setHeader(string key,string value);
    
};
