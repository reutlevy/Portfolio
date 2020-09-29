//
// Created by danie on 1/13/2020.
//

#ifndef BOOST_ECHO_CLIENT_READFROMSOCKET_H
#define BOOST_ECHO_CLIENT_READFROMSOCKET_H


#include "connectionHandler.h"
#include "EncoderDecoder.h"


class Protocol {
private:
    EncoderDecoder encoderDecoder;
    User user;

public:

    Protocol(EncoderDecoder encoderDecoder1);
    const User getUser();
    StompFrame process(StompFrame frame);
    void setUser(User user);
};


#endif //BOOST_ECHO_CLIENT_READFROMSOCKET_H