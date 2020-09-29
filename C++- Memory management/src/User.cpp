//
// Created by danielle on 26/11/2019.
//

#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"
#include <vector>
#include <algorithm>
#include <utility>
#include <string>

User::User(const std::string &name):history(), name(name), tags(){;
}


std::string LengthRecommenderUser::getAlgo() const {
    return this->algo;
}

std::string RerunRecommenderUser::getAlgo() const {
    return this->algo;
}

std::string GenreRecommenderUser::getAlgo() const {
    return this->algo;
}


std::string User::getName() const {
    return name;
}

std::vector<Watchable*> User::get_history() const {
    return history;
}

std::vector<Watchable*>* User::getRefhistory(){
    return &history;
}

void User::set_history(std::vector<Watchable*> newhistory)  {
    this->history=newhistory;
}

void User::add_history(Watchable *watchable) {
    this->history.push_back(watchable);
}

vector<pair<string,int>> User::gettag() {
    return tags;
}

bool User::findhistory(Watchable *watchable) { //return false if the user not it the history
    auto position = std::find(history.begin(), history.end(), watchable);
    bool answer=(position==history.end());
    return answer;
}

int User::indexintags(std::string jen) {
    int result=-1;
    bool found=false;
    for(std::size_t i=0;i<tags.size()&& !found;i++){
        if(tags.at(i).first==jen){
            result=i;
            found=true;
        }
    }
    return result;
}

void User::setName(string name1) {
    name=name1;
}

void User::updatepupular(Watchable *watchable) {
    vector<pair<string, int> >::iterator it;
    for (std::vector<int>::size_type i = 0; i != watchable->gettags().size(); i++) {
        int index=indexintags(watchable->gettags().at(i));
        if(index!=-1){
            tags.at(index).second++;
        }
        else{
            std::pair <std::string,int> product1(watchable->gettags().at(i),1);
            tags.push_back(product1);
        }
    }
    sort( tags.begin( ), tags.end( ), [ ]( const pair<string,int> tag1, const pair<string,int> tag2 )
    {
        return tag1.second>tag2.second;
    });

    size_t max=tags.at(0).second;
    std::size_t end=0;
    if(tags.size()>1){
        std::size_t max2=tags.at(1).second;
        end=1;
        while((max2==max)&(end<tags.size())){
            max2=tags.at(end).second;
            end++;       }
    }
    std::sort(std::begin(tags), std::begin(tags)+end, [] (const pair <string,int> & lhs, const pair <string,int> & rhs) {
        return (lhs.first.compare(rhs.first) < 0);
    });
}


LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name), algo("len"), length(){
}

User *LengthRecommenderUser::clone() {

    return new LengthRecommenderUser(*this);
}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    Watchable *watchable;
    long sum=0;
    bool find=false;
    for (std::vector<Watchable *>::size_type i = 0; i != history.size(); i++) {
        sum += history.at(i)->getlength();
    }
    long average = sum / history.size();
    for (size_t i = 0; i < s.getcontent().size(); i++) {
        if(findhistory(s.getcontent().at(i))) {
            pair<long, int> insert;
            insert.first = (s.getcontent().at(i)->getid());
            insert.second = (abs(average - s.getcontent().at(i)->getlength()));
            length.push_back(insert);
        }
    }

    ::sort( length.begin( ), length.end( ), [ ]( const pair<long,int> tag1, const pair<long,int> tag2 )
    {
        return tag1.second<tag2.second;
    });

    for (std::vector<Watchable *>::size_type i = 0; i != length.size()-1 && !find; i++) {
        long index=length.at(i).first-1;
        Watchable *findnext=s.getcontent().at(index);
        for(size_t i=0;i<s.getcontent().size();i++){
            if ((findhistory(s.getcontent().at(index)))&(findnext->getlength()==s.getcontent().at(i)->getlength())&(!find)){
                watchable=s.getcontent().at(i);
                find= true;
            }
        }
    }

    return watchable;
}


RerunRecommenderUser::RerunRecommenderUser(const std::string &name): User(name), algo("rer"), lastRecIndex(0){
}

Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    Watchable* watchable;
    int size = get_history().size();
    watchable = get_history().at((lastRecIndex+1)%size);
    lastRecIndex = (lastRecIndex+1)%size;
    return watchable;
}

User *RerunRecommenderUser::clone() {
    return new RerunRecommenderUser(*this);
}




GenreRecommenderUser::GenreRecommenderUser(const std::string &name): User(name), algo("gen"){
}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    Watchable* watchable;
    std::size_t z=0;
    while(z<s.getcontent().size()) {
        for (std::size_t i = 0; i < s.getcontent().size(); i++){
            for(std::size_t y=0;y< s.getcontent().at(i)->gettags().size();y++){
                if ((s.getcontent().at(i)->gettags().at(y)==this->gettag().at(z).first)&&(findhistory(s.getcontent().at(i)))) {
                    watchable = s.getcontent().at(i);
                    return watchable;
                }
            }
        }
    }
    return nullptr;
}

User *GenreRecommenderUser::clone() {
    return new GenreRecommenderUser(*this);
}



