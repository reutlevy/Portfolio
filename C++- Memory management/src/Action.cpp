//
// Created by reut on 24/11/2019.
//

#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include <iostream>
#include <sstream>

using namespace std;

BaseAction::BaseAction():errorMsg(), status(PENDING) {
}


void BaseAction::complete() {
    status=COMPLETED;
}

void BaseAction::pending() {
    status=PENDING;
}

void BaseAction::error(const string& errorMsg){
    status =ERROR;
    this->errorMsg=errorMsg;
    cout<<errorMsg<< endl;
}

string BaseAction::getErrorMsg() const {
    return errorMsg;
}


string BaseAction::statustoString() const {
    if(status==PENDING)
        return "PENDING";
    if(status==COMPLETED)
        return "COMPLETED";
    else
        return "ERROR: "+errorMsg;
}

ActionStatus BaseAction::getStatus() const {
    if(status==PENDING)
        return PENDING;
    if(status==COMPLETED)
        return COMPLETED;
    else
        return ERROR;
}

void CreateUser::act(Session &sess) {
    this->pending();

    string input = sess.getCommand().substr(1);
    int index=input.find(' ');
    if(index==-1) {
        const char* error1=" the input is invalid";
        error(error1);
    }

    else {
        string secondword = input.substr(0, input.find(' '));
        string thirdword = input.substr(secondword.size() + 1);
        //string thirdword = input.substr(secondword.size(), input.find(' '));

        if (sess.finduser(secondword)) { //there is already a user
            const char *error1 = " the new user name is already taken";
            error(error1);
        } else if ((thirdword != "len") & (thirdword != "rer") & (thirdword != "gen")) {
            const char *error1 = " the recommendation algorithm is not one of the options";
            error(error1);


        } else {
            User *insert;
            if (thirdword == "len") {
                insert = new LengthRecommenderUser(secondword);
            }

            if (thirdword == "rer") {
                insert = new RerunRecommenderUser(secondword);
            }

            if (thirdword == "gen") {
                insert = new GenreRecommenderUser(secondword);
            }

            sess.adduser(secondword, insert);
            this->complete();
        }
    }
    sess.addaction(this);
}

BaseAction *CreateUser::clone() {
    return new CreateUser(*this);
}


string CreateUser::toString() const {
    // string *result=new string;
    string value= "CreateUser "+ this->statustoString();
    return value;
}

void ChangeActiveUser::act(Session &sess){
    this->pending();
    string input = sess.getCommand().substr(1);
    std::size_t index=input.find(' ');
    string secondword = input.substr(0,index);
    if (!sess.finduser(secondword)) { //there is already a user
        const char* error1="try to change to active user that doesnt exist";
        error(error1);
    }
    else {
        sess.setactive(secondword);
        this->complete();
    }
    sess.addaction(this);
}

string ChangeActiveUser::toString() const {
    string value= "ChangeActiveUser "+ this->statustoString();
    return value;
}

BaseAction *ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}

void DeleteUser::act(Session &sess) {
    this->pending();
    string input = sess.getCommand().substr(1);
    std::size_t index=input.find(' ');
    string secondword = input.substr(0, index);
    if  (!sess.finduser(secondword)) {
        const char* error1=" the user name is not exist";
        error(error1);
    }
    else {
        if(sess.getactive()->getName()==secondword){
            const char* error1=" try to delete user that is currently active";
            error(error1);
        }
        else {
            sess.deleteuser(secondword);
            this->complete();
        }
    }
    sess.addaction(this);
}


string DeleteUser::toString() const {
    // string *result=new string;
    string value= "DeleteUser "+ this->statustoString();
    return value;
}

BaseAction *DeleteUser::clone() {
    return new DeleteUser(*this);
}

void DuplicateUser::act(Session &sess) {
    this->pending();

    string input = sess.getCommand().substr(1);
    int index=input.find(' ');
    if(index==-1) {
        const char* error1=" the input is invalid";
        error(error1);
    }
    else {
        string secondword = input.substr(0, input.find(' '));
        string thirdword = input.substr(secondword.size() + 1);

        if (sess.getmap().find(secondword) == sess.getmap().end()) {
            const char *error1 = " the original user name is not exist";
            error(error1);
        } else if (sess.getmap().find(thirdword) != sess.getmap().end()) { // new name is already exist
            const char *error1 = " the new user name is already taken";
            error(error1);
        } else {
            User *src = sess.getmap()[secondword];
            User *newUser = src->clone();
            newUser->setName(thirdword);
            sess.adduser(thirdword,newUser);
            this->complete();
        }
    }
    sess.addaction(this);
}


string DuplicateUser::toString() const {
    // string *result=new string;
    string value= "Duplicate "+ this->statustoString();
    return value;
}

BaseAction *DuplicateUser::clone() {
    return new DuplicateUser(*this);
}

void PrintContentList::act(Session &sess) {
    this->pending();
    if(sess.getcontent().size()==0){
        const char* error1="there is not watchable content available";
        error(error1);
    }
    else {
        for(size_t i=0;i<sess.getcontent().size();i++){
            cout << sess.getcontent().at(i)->getid() <<"." << sess.getcontent().at(i)->toString() <<endl;
        }
        this->complete();
        sess.addaction(this);
    }
}

string PrintContentList::toString() const {
    // string *result=new string;
    string value= "PrintContentList "+ this->statustoString();
    return value;
}
BaseAction *PrintContentList::clone() {
    return new PrintContentList(*this);
}



void PrintWatchHistory::act(Session &sess) {
    this->pending();
    if(sess.getactive()->get_history().size()==0){
        const char* error1="there is not watch history for this user";
        error(error1);
    }
    else {
        cout << "Watch history for" << " " << sess.getactive()->getName() << endl;
        for (std::size_t i = 0; i < sess.getactive()->get_history().size(); i++) {
            cout << i+1 << ". " << sess.getactive()->get_history().at(i)->toString() << endl;
        }
        this->complete();
    }
    sess.addaction(this);
}

string PrintWatchHistory::toString() const {
    // string *result=new string;
    string value= "PrintWatchHistory "+ this->statustoString();
    return value;
}

BaseAction *PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}

Watch::Watch():count(0) {
}

void Watch::act(Session &sess) {
    this->pending();
    std::size_t found1 = sess.getCommand().find(" ");
    int convertdata = static_cast<int>(found1);
    string secondword = sess.getCommand().substr(convertdata,sess.getCommand().size());
    stringstream number(secondword);
    std::size_t x = 1;
    number >> x;
    std::size_t i = 0;
    bool found = false;
    Watchable *watch;
    if(x-1<sess.getcontent().size()) {
        while (i < sess.getcontent().size() && !found) {
            if (i == x - 1) {
                watch = sess.getcontent().at(i);
                cout << "Watching" << " " << sess.getcontent().at(i)->toString() << endl;
                found = true;
                sess.getactive()->add_history(sess.getcontent().at(i));
                sess.getactive()->updatepupular(sess.getcontent().at(i));
            }
            i++;
        }
        this->complete();
        sess.addaction(this);
        string rec=watch->getNextWatchable(sess)->toString();
        if(watch->getNextWatchable(sess)!= nullptr) {
            cout << "We recommend watching " << rec << ", continue watching?[y/n]" << endl;
            string next;
            cin >> next;
            if (next == "y") {
                stringstream ee;
                ee << watch->getNextWatchable(sess)->getid();
                sess.setCommand("watch " + ee.str());
                Watch *newWatch = new Watch();
                this->complete();
                sess.addaction(newWatch);
                this->count++;
                this->act(sess);
            }
        }
        else{
            cout << "There are no new recommendations for this user ";
        }
    }
    else{
        const char* error1="the id of the watchable doesnt exist";
        error(error1);
    }

}

string Watch::toString() const {
    // string *result=new string;
    string value= "Watch "+ this->statustoString();
    return value;
}

BaseAction *Watch::clone() {
    return new Watch(*this);
}

void PrintActionsLog::act(Session &sess) {
    this->pending();
    if(sess.getactionsLog().size()==0){
        const char* error1="there is not log activity for this user";
        error(error1);
    }
    else {
        for (int i = sess.getactionsLog().size()-1; i>= 0; i--) {
            cout << sess.getactionsLog().at(i)->toString() << endl;
        }
        this->complete();
    }
    sess.addaction(this);
}

string PrintActionsLog::toString() const {
    // string *result=new string;
    string value= "PrintActionsLog "+ this->statustoString();
    return value;
}

BaseAction *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}


void Exit::act(Session &sess) {
    sess.setexit(true);
    this->complete();
    sess.addaction(this);
}

string Exit::toString() const {
    // string *result=new string;
    string value= "Exit "+ this->statustoString();
    return value;
}

BaseAction *Exit::clone() {
    return new Exit(*this);
}
