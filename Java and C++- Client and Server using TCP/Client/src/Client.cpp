//
// Created by duekdani@wincs.cs.bgu.ac.il on 14/01/2020.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <boost/lexical_cast.hpp>
#include "../include/StompconnectionHandler.h"

int main (int argc, char *argv[]) {

    EncoderDecoder encoderDecoder;
    //   std::vector<std::string> words = encoderDecoder.split(line," ");
    Protocol aProtocol(encoderDecoder);


    StompconnectionHandler stompconnectionHandler(encoderDecoder, aProtocol);

    std::thread th1(&StompconnectionHandler::KeyBoard, &stompconnectionHandler);
    std::thread th2(&StompconnectionHandler::SendtoServer, &stompconnectionHandler);
    th1.join();
    th2.join();
}
