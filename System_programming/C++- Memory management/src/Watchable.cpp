//
// Created by reut on 26/11/2019.
//

#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include <iostream>
#include <sstream>
#include <string>
#include <sstream>


using namespace std;

Watchable::Watchable(long id, int length, const std::vector<std::string> &tags): id(id),length(length),tags(tags) {}


long::Watchable::getid() const {
    return this->id;
}

int Watchable::getlength() const {
    return length;
}

std::vector<std::string> Watchable::gettags() const {
    return this->tags;
}
void Watchable::setlength(int newlength) {
    this->length=newlength;
}


Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags),name(name) {}

std::string Movie::toString() const {
    string result;
    string resulttags;
    for(size_t i=0; i < this->gettags().size()-1; i++) {
        resulttags=resulttags+gettags().at(i)+",";
    }
    resulttags=resulttags+gettags().at(this->gettags().size()-1);
    stringstream mystream;
    mystream << this->getlength();
    result=this->name+" "+ mystream.str() +" minutes "+"["+resulttags+"]";
    return result;
}

Watchable* Movie::getNextWatchable(Session &sess) const {
    return sess.getactive()->getRecommendation(sess);
}

Watchable *Movie::clone() {
    return new Movie(*this);
}

string Movie::getname() const {
    return name;
}

string Episode::getname() const{
    return seriesName;
}

Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags):Watchable(id,length,tags),seriesName(seriesName),season(season),episode(episode),nextEpisodeId() {}

std::string Episode::toString() const {
    string result;
    string resulttags;
    for(std::size_t i=0; i < this->gettags().size()-1; i++) {
        resulttags=resulttags+gettags().at(i)+",";
    }
    resulttags=resulttags+gettags().at(this->gettags().size()-1);
    stringstream mystream;
    mystream << this->getlength();
    stringstream ss;
    ss << this->season;
    stringstream ee;
    ee << this->episode;
    result=this->seriesName +" S"+ss.str()+"E"+ee.str()+" "+mystream.str() +" minutes "+"["+resulttags+"]";
    return result;
}


Watchable* Episode::getNextWatchable(Session &sess) const {
    Watchable* next;
    std::size_t IdLastWatched = sess.getactive()->get_history().at(sess.getactive()->get_history().size()-1)->getid();
    Watchable* watch1= sess.getactive()->get_history().at(sess.getactive()->get_history().size()-1);
    Watchable *watch2;
    if(IdLastWatched<sess.getcontent().size()) {
        string NameLastWatched = sess.getcontent().at(IdLastWatched-1)->toString();
        std::size_t IdNextContent = sess.getcontent().at(IdLastWatched)->getid();
        watch2 = sess.getcontent().at(IdLastWatched);
        string NameNext = sess.getcontent().at(IdNextContent - 1)->toString();
        if(watch1->getname()==watch2->getname()){
            next=watch2;
        }
    }

    else{
        next=sess.getactive()->getRecommendation(sess);
    }
    return next;
}


Watchable *Episode::clone() {
    return new Episode(*this);
}

