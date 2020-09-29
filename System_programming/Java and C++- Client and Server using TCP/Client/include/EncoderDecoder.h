//
// Created by reut on 11/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_ENCODERDECODER_H
#define BOOST_ECHO_CLIENT_ENCODERDECODER_H

#endif //BOOST_ECHO_CLIENT_ENCODERDECODER_H

#include <vector>
#include "string"
#include <boost/asio.hpp>
#include "StompFrame.h"
#include "User.h"

using std::string;

class EncoderDecoder{

private:
    std::vector<char> msg;
    StompFrame *decodedStomp;
    int receiptSubscribe;
    int recieiptUnSubscribe;
    int counterId;
    User curruser;

public:
    EncoderDecoder();
    StompFrame stringtoFrame (const string);

    string Frametostring(const StompFrame &p);

    std::vector<std::string> split(std::string s, std::string delimiter);

    const vector<char> &getMsg() const;

    StompFrame *getDecodedStomp() const;

    StompFrame getStomp(string encode);

    virtual ~EncoderDecoder()= default;

    virtual EncoderDecoder& operator=(const EncoderDecoder& packet)= default;

    void setUser(User user);
};


