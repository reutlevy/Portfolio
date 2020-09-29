#ifndef SESSION_H_
#define SESSION_H_
#include "json.hpp"
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include "../include/Action.h"


class User;
class Watchable;

using namespace std;

class Session{
public:
    Session(const std::string &configFilePath);

    Session(const Session &other); //copy constructor
    Session(Session &&other); //move constructor
    Session& operator=(const Session& other); //copy Assignment operator
    Session& operator=(Session&& other); //move Assignment operator
    ~Session(); //destructor


    void start();
    std::string getCommand();
    std::vector<Watchable*> getcontent();
    std::vector<BaseAction*> getactionsLog();
    void addaction(BaseAction* input);
    std::unordered_map<std::string,User*> getmap();
    User* getactive();
    void setactive(string);
    void setCommand(const std::string& command);
    void setexit(bool exit);
    void adduser(string name,User* user);
    bool finduser(string);
    void deleteuser(string user);
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string command;
    bool exit;

    void copy(const Session &other);
    void clear();
};
#endif