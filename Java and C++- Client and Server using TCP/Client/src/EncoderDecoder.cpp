//
// Created by reut on 11/01/2020.
//

#include "string"
#include "vector"
#include "../include/EncoderDecoder.h"
#include <unordered_map>

using namespace std;
using std::string;


EncoderDecoder::EncoderDecoder(){
    receiptSubscribe=70;
    recieiptUnSubscribe=100;
    counterId=1;
}

std::vector<std::string> EncoderDecoder::split(std::string s, std::string delimiter) {
    std::vector<std::string> list;
    std::vector<std::string> output;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}

StompFrame EncoderDecoder::stringtoFrame(const string string1) {
    int subscribe=70;
    unordered_map<string, string> header;
    StompFrame answer;
    std::vector<std::string> words = split(string1, " ");
    string type=words[0];
    if (type=="login"){
        string newtype="CONNECT";
        header["accept-version"]="1.2";
        header["host"]=words[1];
        header["login"]=words[2];
        header["passcode"]=words[3];
        answer.setType(newtype);
        answer.setHeaders(header);
        return answer;
    }
    else if (type=="join"){
        string newtype="SUBSCRIBE";
        header["destination"]=words[1];
        stringstream ss;
        ss << recieiptUnSubscribe;
        string str = ss.str();
        header["receipt"]=str;
        recieiptUnSubscribe++;


        ss << counterId;
        str= ss.str();
        header["id"]=str;
        counterId++;

        answer.setType(newtype);
        answer.setHeaders(header);
        return answer;
    }
    else if (type=="add"){
        string newtype="SEND";
        header["destination"]=words[1];
        string body=curruser.getName() +" had added the book "+words[2];
        answer.setType(newtype);
        answer.setHeaders(header);
        answer.setBody(body);
        return answer;
    }
    else if (type=="borrow"){
        string newtype="SEND";
        header["destination"]=words[1];
        string body= curruser.getName()+" wish to borrow "+words[2];
        answer.setType(newtype);
        answer.setHeaders(header);
        answer.setBody(body);
       // cout << "THE answer of borrow isss "+answer.toString() <<endl;
        return answer;
    }
    else if (type=="return"){
        string newtype="SEND";
        header["destination"]=words[1];
        string body="Returning "+words[2]+" to "+curruser.findBook(words[2]).gettakenFrom();
        answer.setType(newtype);
        answer.setHeaders(header);
        answer.setBody(body);
        return answer;
    }
    else if (type=="status"){
        string newtype="SEND";
        header["destination"]=words[1];
        string body="book status";
        answer.setType(newtype);
        answer.setHeaders(header);
        answer.setBody(body);
        return answer;
    }
    else if (type=="logout"){
        string newtype="DISCONNECT";
        stringstream ss;
        ss << recieiptUnSubscribe;
        string str = ss.str();
        header["receipt"]=str;
        recieiptUnSubscribe++;
        answer.setType(newtype);
        answer.setHeaders(header);
        return answer;
    } else {
        answer.setType("ERRORTYPE");
        return  answer;
    }
}

string EncoderDecoder::Frametostring(const StompFrame &p) {
    const std::string& type = p.getType();
    std::unordered_map<string,string> message=p.getHeaders();
    string answer;
    std::vector<std::string> splitmsg = split(message["body"], " ");//splits the client command
    if (type=="CONNECT"){
        answer="login "+message["host"]+" "+message["login"]+ " "+message["passcode"];
    }
    if(type=="SUBSCRIBE"){
        answer="join "+message["destination"];
    }
    if (type=="SEND"){
        if (message["body"].find("added")){
            string bookname;
            for(int i=5; (unsigned) i<splitmsg.size();i++){
                bookname=bookname+" "+splitmsg[i];
            }
            answer="add "+message["destination"]+" "+bookname;
        }
        if (message["body"].find("borrow")){
            string bookname;
            for(int i=4; (unsigned)i<splitmsg.size();i++){
                bookname=bookname+" "+splitmsg[i];
            }
            answer="borrow "+message["destination"]+" "+splitmsg[4];
        }
        if (message["body"].find("Returning")){
            string bookname;
            for(int i=1; (unsigned)i<splitmsg.size();i++){
                bookname=bookname+" "+splitmsg[i];
            }
            answer="return "+message["destination"]+" "+splitmsg[1];
        }
        if (message["body"].find("status")){
            answer="status "+message["destination"];
        }
    }
    if(type=="DISCONNECT"){
        answer="logout";
    }
    if(type=="UNSBSCRIBE"){
        answer="exit "+message["destination"];
    }
    return answer;
}

const vector<char> &EncoderDecoder::getMsg() const {
    return msg;
}

StompFrame *EncoderDecoder::getDecodedStomp() const {
    return decodedStomp;
}

StompFrame EncoderDecoder::getStomp(string encode) {
    unordered_map<string, string> headers;
    StompFrame answer;
    std::vector<std::string> words = split(encode, "\n");
    string type=words[0];
    if(type=="CONNECTED"){
        cout << "Someone connected"<<endl;
        answer.setType(type);
        std::vector<std::string> version = split(words[1], ":");
        headers["version"]=version[1];
        answer.setHeaders(headers);
    }
    if(type=="RECEIPT"){
        answer.setType(type);
        std::vector<std::string> version = split(words[1], ":");
        headers["receipt-id"]=version[1];
        answer.setHeaders(headers);
    }
    if(type=="MESSAGE"){
        answer.setType(type);
        std::vector<std::string> subscription = split(words[2], ":");
        headers["subscription"]=subscription[1];
        std::vector<std::string> Messageid = split(words[3], ":");
        headers["Message-id"]=Messageid[1];
        std::vector<std::string> destination = split(words[1], ":");
        headers["destination"]=destination[1];
        answer.setHeaders(headers);
   //     cout<<"----i insert body:"+words[5]<<endl;
        answer.setBody(words[5]);
    }
    if(type=="ERROR"){
        answer.setType(type);
        std::vector<std::string> receiptid = split(words[1], ":");
        headers["receipt-id"]=receiptid[1];
        std::vector<std::string> message = split(words[2], ":");
        headers["message"]=message[1];
        answer.setHeaders(headers);
        answer.setBody(words[3]);
    }
    return answer;
}

void EncoderDecoder::setUser(User user) {
    curruser=user;
}



