#ifndef CONNECTION_HANDLER__
#define CONNECTION_HANDLER__

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <mutex>
#include "Protocol.h"
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

class StompconnectionHandler {

private:
    short port_;
    boost::asio::io_service io_service_;   // Provides core I/O functionality
    tcp::socket socket_;
    std::string host_;
    EncoderDecoder encoderDecoder;
    Protocol aProtocol;
    bool Userconnected= false;
    bool Stop= false;
    boost::mutex Keyboard;
    boost::mutex Server;
    User myUser;
    int connectionid=0;

public:
    StompconnectionHandler(EncoderDecoder encoderDecoder1, Protocol aProtocol1);
    virtual ~StompconnectionHandler();

    // Connect to the remote machine
    bool connect();

    // Read a fixed number of bytes from the server - blocking.
    // Returns false in case the connection is closed before bytesToRead bytes can be read.
    bool getBytes(char bytes[], unsigned int bytesToRead);

    // Send a fixed number of bytes from the client - blocking.
    // Returns false in case the connection is closed before all the data is sent.
    bool sendBytes(const char bytes[], int bytesToWrite);

    // Read an ascii line from the server
    // Returns false in case connection closed before a newline can be read.
    bool getLine(std::string& line);

    // Send an ascii line from the server
    // Returns false in case connection closed before all the data is sent.
    bool sendLine(std::string& line);

    // Get Ascii data from the server until the delimiter character
    // Returns false in case connection closed before null can be read.
    bool getFrameAscii(std::string& frame, char delimiter);

    // Send a message to the remote host.
    // Returns false in case connection is closed before all the data is sent.
    bool sendFrameAscii(const std::string& frame, char delimiter);

    // Close down the connection properly.
    void close();
    const std::string &getHost() const;

    const short getPort() const;

    void KeyBoard();

    void SendtoServer();

    void setHost(const string &host);

    void setUserconnected(bool userconnected);

    void setPort(short port);


}; //class ConnectionHandler

#endif