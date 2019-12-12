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
    istringstream iss(request);
    string tmp;
    vector<string> v;

    // Get Path and Method
    while(getline(iss,tmp,' ')){
        v.push_back(tmp);
    }
    method = v[0];
    path = v[1];
    size_t pos = v[1].find("?");
    path = v[1].substr(0,pos);
    if(method=="GET"){
        cerr<<"-----------GET-----------"<<endl;
        string token ,tmp;
        token = v[1].substr(pos+1,v[1].size()-pos-1);
        while((pos=token.find(";"))!=string::npos){
                tmp = token.substr(0,pos);
                addParam(tmp);
                token.erase(0,pos+1); //skip "&"
        }
        addParam(token);
        cerr<<"--------End of GET--------"<<endl;
    }
    else if(method=="POST"){
        cerr<<"--------POST---------"<<endl;
        pos = 0;
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
    // handle cookie
    size_t pos2;
    pos = request.find("Cookie: ")+8;
    if(pos!=string::npos){
        string tmp;
        pos2 = request.find("\r\n",pos);
        string token = request.substr(pos,pos2-pos);
        // cerr<<"cookie = "<<token<<endl;
        while((pos=token.find(";"))!=string::npos){
                tmp = token.substr(0,pos);
                addCookie(tmp);
                token.erase(0,pos+2); //skip "; "
        }
        addCookie(token);

    }

}

Http::~Http()
{
}
string Http::getHeaders(){
    string tmp="HTTP/1.1 200 OK\r\n";
    map<string, string>::iterator iter;
    for(iter = this->headers.begin(); iter!=this->headers.end(); iter++){
        tmp += iter->first+": "+iter->second+"\r\n";
    }
    return tmp + "\r\n";
}
string Http::getResponse(){
    text = "";
    text += getHeaders();
    text += readFile();
    text += "\0";
    return text;
}
string Http::getResponse(string str){
    text = "";
    text += getHeaders();
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
void Http::setHeader(string key,string value){
    this->headers[key]=value;
}
void Http::addCookie(string str){
    size_t pos;
    pos = str.find("=");
    string a,b;
    if(pos!=string::npos){
        a = str.substr(0,pos);
        b = str.substr(pos+1,str.length()-pos-1);
        cookies[a] = b;
    }
}
string Http::getCookie(string str){
    return this->cookies[str];
}
void Http::addParam(string str){
    size_t pos;
    pos = str.find("=");
    string a,b;
    if(pos!=string::npos){
        a = str.substr(0,pos);
        b = str.substr(pos+1,str.length()-pos-1);
        paramdata[a] = b;
    }
}
string Http::getParamdata(string str){
    return this->paramdata[str];
}