#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;
using namespace std;

class User{
public:
    User(const std::string& name);

    virtual ~User()= default; //destructor
    User(const User &other)= default; //copy constructor
    User(User &&other) = default; //move constructor
    User& operator=(const User& other) = default; //copy Assignment operator
    User&operator=(User&& other) = default; //move Assignment operator

    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    std::vector<Watchable*>* getRefhistory();
    void set_history(std::vector<Watchable*> newhistory);
    void add_history(Watchable *watchable);
    virtual std::string getAlgo() const=0;
    void updatepupular(Watchable *watchable);
    vector<pair <string,int>> gettag();
    bool findhistory(Watchable *watchable);
    int indexintags(std::string);
    void setName(string);
    virtual User *clone()=0;
protected:
    std::vector<Watchable*> history;
private:
    std::string name;
    vector< pair <string,int> > tags;

};

class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getAlgo() const;
    User *clone();
private:
    std::string algo;
    vector< pair <long,int> > length;
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getAlgo() const;
    User* clone();
private:
    std::string algo;
    int lastRecIndex;
};

class GenreRecommenderUser : public User{
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getAlgo() const;
    User* clone();
private:
    std::string algo;


};

#endif