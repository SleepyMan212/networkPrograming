#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <map>
#include "Http.hpp"

using namespace std;

Http::Http(const string &request){
    headers="HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n";
    
    istringstream iss(request);
    string tmp;
    vector<string> v;

    // Get Path and Method
    while(getline(iss,tmp,' ')){
        v.push_back(tmp);
    }
    method = v[0];
    path = v[1];

    if(method=="POST"){
        cerr<<"--------POST---------"<<endl;
        size_t pos = 0;
        string token,tmp;
        pos = request.find("Content-Type: application/x-www-form-urlencoded");
        if(pos!=string::npos){
            string delimiter="\r\n\r\n";
            pos = request.find(delimiter)+4; // skip CRLF twice
            if(pos!=string::npos){
                token =  request.substr(pos,request.size()-pos);
            }
            cerr<<"token = "<<token<<endl;
            while((pos=token.find("\n"))!=string::npos){
                    tmp = token.substr(0, pos);
                    addFormdata(tmp);
                    token.erase(0, pos + 2);    //skip CRLF
            }
            addFormdata(token);
        }
        cerr<<"--------END OF POST---------"<<endl;

    }
}

Http::~Http()
{
}
string Http::getResponse(){
    text = "";
    text += this->headers;
    text += readFile();
    text += "\0";
    return text;
}
string Http::getResponse(string str){
    text = "";
    text += this->headers;
    text += str;
    text += "\0";
    return text;
}
string Http::readFile(){
    fstream file;
    string tmp,str;
    string filename;
    if(path=="/"){
        filename="index.html";
    }else{
        path = path.substr(1,path.size()-1);;
        filename=path;

    }
    file.open(filename.c_str(), ios::in);\
    if(!file.is_open()){
        return "404 not file";
    }
    while(!file.eof()){   //若到達檔案結尾則”file>>str”將 傳回0， 跳出while回圈
        getline(file,tmp);
        str+=tmp;
        str+="\n";
    }
    file.close();
    return str;
}
string Http::getMethod(){
    return method;
}
string Http::getPath(){
    return path;
}
// string Http::getPath(){
//     return path;
// }
void Http::addFormdata(string str){
    size_t pos;
    pos = str.find("=");
    string a,b;
    if(pos!=string::npos){
        a = str.substr(0,pos);
        b = str.substr(pos+1,str.length()-pos-1);
        formdata[a] = b;
    }
}
string Http::getFormdata(string str){
    map<string, string>::iterator iter;
    iter = formdata.find(str);

    if(iter != formdata.end())
        return formdata[str];
    else
        return "";
}

