//
// Created by reut on 11/01/2020.
//

#include "../include/StompFrame.h"
#include <map>
#include <iostream>

using std::string;

StompFrame::StompFrame(string getype, std::unordered_map<string, string> getheaders): type(getype),headers(getheaders),body(" "){}
StompFrame::StompFrame(string getype, std::unordered_map<string, string> getheaders, string body):type(getype),headers(getheaders),body(body) {}

const std::unordered_map<string, string> &StompFrame::getHeaders() const {
    return headers;
}

const string &StompFrame::getBody() const {
    return body;
}

const string &StompFrame::getType() const {
    return type;
}


void StompFrame::setType(const string &type) {
    StompFrame::type = type;
}

void StompFrame::setBody(const string &body) {
    StompFrame::body = body;
}

void StompFrame::setHeaders(const unordered_map<string, string> &headers) {
    this->headers=headers;
}

StompFrame::StompFrame() {

}

string StompFrame::toString() {

    string out;
    string answer;
    map<string, string>::iterator it;
    for (std::pair<std::string, string> element : headers){
        out=out+element.first+":"+element.second+'\n';
    }


    if(body.empty()) {
        answer = type+'\n'+out+'\n';
    }
    else{
        answer = type+'\n'+out+'\n'+body+'\n';
    }

    return answer;
}





