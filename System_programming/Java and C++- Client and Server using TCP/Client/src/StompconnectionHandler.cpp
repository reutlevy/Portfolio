
#include "boost/lexical_cast.hpp"
#include "../include/StompconnectionHandler.h"

class error_code;

using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

StompconnectionHandler::StompconnectionHandler(EncoderDecoder encoderDecoder1,
                                               Protocol aProtocol1):encoderDecoder(encoderDecoder1),aProtocol(aProtocol1),io_service_(), socket_(io_service_){

}

StompconnectionHandler::~StompconnectionHandler() {
    close();
}

bool StompconnectionHandler::connect() {
    std::cout << "Starting connect to "
              << host_ << ":" << port_ << std::endl;
    try {
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
        boost::system::error_code error;
        socket_.connect(endpoint, error);
        if (error)
            throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool StompconnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
    boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp ) {
            tmp += socket_.read_some(boost::asio::buffer(bytes+tmp, bytesToRead-tmp), error);
        }
        if(error)
            throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool StompconnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
    boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
            tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
        if(error)
            throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool StompconnectionHandler::getLine(std::string& line) {
    return getFrameAscii(line, '\0');
}

bool StompconnectionHandler::sendLine(std::string& line) {
    return sendFrameAscii(line, '\0');
}

bool StompconnectionHandler::getFrameAscii(std::string& frame, char delimiter) {
    char ch;
    // Stop when we encounter the null character.
    // Notice that the null character is not appended to the frame string.
    try {
        do{
            if(!getBytes(&ch, 1))
            {
                return false;
            }
            if(ch!='\0')
                frame.append(1, ch);
        }while (delimiter != ch);
    } catch (std::exception& e) {
        std::cerr << "recv failed2 (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}


bool StompconnectionHandler::sendFrameAscii(const std::string& frame, char delimiter) {
    bool result=sendBytes(frame.c_str(),frame.length());
    if(!result) return false;
    return sendBytes(&delimiter,1);
}

// Close down the connection properly.
void StompconnectionHandler::close() {
    try{
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}

const string &StompconnectionHandler::getHost() const {
    return host_;
}

const short StompconnectionHandler::getPort() const {
    return port_;
}

void StompconnectionHandler::KeyBoard() {

    while ((!Stop)) {

      //  cout << "please enter command" << endl;
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        std::vector<std::string> words = encoderDecoder.split(line, " ");

        if ((words[0] == "login") & (!Userconnected)) {
            User user(words[2], words[3], connectionid);
            connectionid++;
            this->aProtocol.setUser(user);
            this->encoderDecoder.setUser(user);
            std::vector<std::string> hostport = encoderDecoder.split(words[1], ":");
            host_ = (hostport[0]);
            this->port_ = boost::lexical_cast<short>(hostport[1]);
            aProtocol.setUser(user);
            Userconnected = true;
            StompFrame stompFrame = encoderDecoder.stringtoFrame(line);
            string send = stompFrame.toString();


            connect();

            if(!sendLine(send))
                cout << "an error accured while sending the message" << endl;

            cout << "------message out------" << endl;
            cout << send << endl;

        } else if (Userconnected) {

            StompFrame stompFrame = encoderDecoder.stringtoFrame(line);
            string send = stompFrame.toString();

            if(stompFrame.getType()=="ERRORTYPE"){
                cout<< "the input is invalid please try again...." <<endl;
             //   Userconnected= false;
              //  Stop=true;
            }

            if(stompFrame.getType()=="DISCONNECT"){
                cout<< "Disconnecting...." <<endl;
                Userconnected= false;
                Stop=true;
            }

            if(stompFrame.getType()=="ERROR"){
                cout<< "There was an error...." <<endl;
                Userconnected= false;
                Stop=true;
            }

            if (!sendLine(send)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                Stop = true;
            }

            cout << "------message out------" << endl;
            cout << send << endl;
        }

    }
}

void StompconnectionHandler::SendtoServer() {

    while ((!Stop)) {
        if (Userconnected) {
            string answer;
            if (getLine(answer)) {

                int len = answer.length();

                StompFrame answer1 = encoderDecoder.getStomp(answer); //change the answer to stomp frame
                cout << answer1.toString() <<endl;
                StompFrame answer2 = aProtocol.process(answer1);

                if (answer2.getType() != "NOTHING") {
                    cout << "------The client's response is-----" << endl;
                    cout << answer2.toString() << endl;
                  //  cout << " The answer is not null " << endl;
                    string send = answer2.toString();
                    if(sendLine(send)) {
                        cout << "------message out------" << endl;
                        cout << send << endl;
                        if (Stop) {
                            cout << "stop the connection";
                        }
                    }
                }

            }
        }
    }
}

void StompconnectionHandler::setUserconnected(bool userconnected) {
    Userconnected = userconnected;
}

void StompconnectionHandler::setHost(const string &host) {
    host_ = host;
}

void StompconnectionHandler::setPort(short port) {
    port_ = port;
}










