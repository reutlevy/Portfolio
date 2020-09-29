//
// Created by reut on 11/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_STOMPFRAME_H
#define BOOST_ECHO_CLIENT_STOMPFRAME_H

#endif //BOOST_ECHO_CLIENT_STOMPFRAME_H

#include "string"
#include <vector>
#include <map>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

using std::string;
using namespace std;
class StompFrame {

private:
    string type;
    std::unordered_map<string, string> headers;
    string body;

public:
    StompFrame();

    StompFrame(string getype, std::unordered_map<string,string> getheaders);

    StompFrame(string getype, std::unordered_map<string,string> getheaders, string body);


    const string &getType() const;

    const std::unordered_map<string, string> &getHeaders() const;

    const string &getBody() const;

    virtual ~StompFrame()= default;

    virtual StompFrame& operator=(const StompFrame& packet)= default;

    void setType(const string &type);

    void setHeaders(const unordered_map<string, string> &headers);

    void setBody(const string &body);

    string toString();

};
