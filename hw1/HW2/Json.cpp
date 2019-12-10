/*
https://medium.com/@bojanmajed/standard-json-api-response-format-c6c1aabcaa6d
http://www.faqs.org/rfcs/rfc4627.html
*/
#include <iostream>
#include <vector>
#include "Json.hpp"
using namespace std;

Json::Json(/* args */){
    this->clear();
}
string Json::getJson(){
    int last = this->text.length()-1;
    this->text[last] = '}';
    return this->text;
}
void Json::addJson(string key,vector<string> v){
    string tmp = "\""+key+"\":[";
    for (size_t i = 0; i < v.size(); i++){
        tmp += "\"";
        tmp += v.at(i);
        tmp += "\"";
        if(v.size()-1!=i){
            tmp += ",";
        }
    }
    tmp += "]";
    this->text += tmp;
    this->addComma();
}
void Json::addJson(string key,string value){
    string tmp = "\""+key+"\":\""+value+"\"";
    this->text += tmp;
    this->addComma();
}
void Json::clear(){
    this->text = "{";
}
void Json::addComma(){
    this->text += ",";
}
Json::~Json(){
}
