//
// Created by reut on 24/11/2019.
//
#include "../include/Action.h"
#include "../include/Session.h"
#include <iostream>
#include <algorithm>
#include <string>
#include "../include/json.hpp"
#include "../include/Watchable.h"
#include "../include/User.h"
#include <unordered_map>


using namespace std;

Session::Session(const std::string &configFilePath) : content(),actionsLog(),userMap(),activeUser(),command(),exit() {

    activeUser = new LengthRecommenderUser("default");
    adduser("default", activeUser);
    using json=nlohmann::json ;
    ifstream myJsonFile(configFilePath);
    json j;
    myJsonFile >>j;
    size_t movieid=0;
    while(j["movies"][movieid]!= nullptr){
        string name=j["movies"][movieid]["name"];
        int length=j["movies"][movieid]["length"];
        vector<string> tags=j["movies"][movieid]["tags"];
        size_t size = content.size()+1;
        Watchable* movie=new Movie(size,name,length,tags);
        content.push_back(movie);
        movieid++;
    }

    size_t seriesid=0;
    while (j["tv_series"][seriesid]!= nullptr){
        string name=j["tv_series"][seriesid]["name"];
        int length=j["tv_series"][seriesid]["episode_length"];
        int season=j["tv_series"][seriesid]["seasons"].size();
        vector<string> tags= j["tv_series"][seriesid]["tags"];
        for(int numseason=0;numseason<season;numseason++){
            for(int numepisode=0;numepisode<j["tv_series"][seriesid]["seasons"][numseason];numepisode++){
                Watchable *episode=new Episode(content.size()+1,name,length,numseason+1,numepisode+1,tags);
                content.push_back(episode);
            }
        }
        seriesid++;
    }
}

///////////////////////
/// copy constuctor ///
///////////////////////
Session::Session(const Session &other):content(),actionsLog(),userMap(),activeUser(),command(other.command),exit(other.exit) {
    copy(other);
}

///////////////////////
/// move constuctor ///
///////////////////////
Session::Session(Session &&other):content(move(other.content)),actionsLog(move(other.actionsLog)),userMap(move(other.userMap)),activeUser(other.activeUser),command(move(other.command)),exit(other.exit)  {
    other.activeUser = nullptr;
}

///////////////////////
/// copy assignment ///
///////////////////////
Session &Session::operator=(const Session &other) {
    if(this!=&other) {
        clear();
        copy(other);
    }
    return *this;
}




///////////////////////
/// move assignment ///
///////////////////////
Session &Session::operator=(Session &&other) {
    if (this!=&other){
        clear();
        content=move(other.content);
        actionsLog=move(other.actionsLog);
        userMap=move(other.userMap);
        command =other.command;
        activeUser =other.activeUser;
        exit=other.exit;
    }
    return *this;
}

///////////////////////
/// the  distructor ///
///////////////////////
Session::~Session() {
    clear();
}

///////////////////////
///rule of 5 HELPERS///
///////////////////////
void Session::clear() {
    for (size_t i = 0; i < content.size(); ++i) {
        delete (content.at(i));
    }
    content.clear();
    for (size_t i = 0; i < actionsLog.size(); ++i) {
        delete (actionsLog.at(i));
    }
    actionsLog.clear();
    for (auto p : userMap) {
        delete (p.second);
    }
    userMap.clear();
    activeUser = nullptr;
}

void Session::copy(const Session &other) {
    for (size_t i = 0; i < other.content.size(); ++i) {
        content.push_back(other.content[i]->clone());
    }
    for (size_t i = 0; i < other.actionsLog.size(); ++i) {
        actionsLog.push_back(other.actionsLog[i]->clone());
    }
    for(auto p : other.userMap){
        userMap.insert({p.first, p.second->clone()});
    }
    activeUser = userMap[other.activeUser->getName()];
    command =other.command;
    exit=other.exit;
    for(auto p:other.userMap){
        vector<Watchable*>* tmp=p.second->getRefhistory();
        for(std::size_t i=0;i<tmp->size();i++){
            tmp->at(i)=content.at(tmp->at(i)->getid()-1);
        }
    }
}


void Session::setexit(bool exit) {
    this->exit=exit;
}

std::string Session::getCommand() {
    return command;
}

void Session::setCommand(const std::string &command) {
    this->command=command;
}

std::vector<Watchable*> Session::getcontent() {
    return content;
}

void Session::addaction(BaseAction* input) {
    if(input->getStatus()!=PENDING)
        this->actionsLog.push_back(input);
}

bool Session::finduser(string name) {
    return this->userMap.find(name) != this->userMap.end();
}

std::vector<BaseAction*> Session::getactionsLog()  {
    return actionsLog;
}

std::unordered_map<std::string,User*> Session::getmap() {
    return userMap;
}

User* Session::getactive() {
    return activeUser;
}

void Session::setactive(string newuser) {
    std::unordered_map<std::string,User*>::const_iterator got = userMap.find (newuser);
    this->activeUser=got->second;
}

void Session::adduser(string name,User *user) {
    this->userMap.insert({name,user});
}
void Session::deleteuser(string user) {
    delete(userMap.at(user));
    userMap.erase(user);
}

void Session::start() {
    exit=false;
    cout << "SPLFLIX is now on!" << endl;
    const std::string& name= "default";

    BaseAction *action;

    while(!exit) {
        std::string input;
        cin>>this->command;
        // error=false;
        getline (std::cin,input);
        string firstWord = command.substr(0, command.find(' '));
        if(input.size()>0) {
            this->command = input;
        }
        if (firstWord == "createuser") {
            action = new CreateUser();
            action->act(*this);
        }

        if (firstWord == "changeuser") {
            action = new ChangeActiveUser();
            action->act(*this);
        }
        if (firstWord == "deleteuser") {
            action = new DeleteUser();
            action->act(*this);
        }
        if (firstWord == "dupuser"){
            action = new DuplicateUser();
            action->act(*this);
        }
        if (firstWord == "content") {
            action = new PrintContentList();
            action->act(*this);
        }
        if (firstWord == "watchhist") {
            action = new PrintWatchHistory();
            action->act(*this);
        }
        if (firstWord == "watch"){
            action = new Watch();
            action->act(*this);
        }
        if (firstWord == "log") {
            action = new PrintActionsLog();
            action->act(*this);
        }
        if (firstWord == "exit") {
            action = new Exit();
            action->act(*this);
        }

    }
}