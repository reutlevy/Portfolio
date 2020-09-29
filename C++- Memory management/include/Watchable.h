#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
// #include "../src/Watchable.cpp"

class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);

    virtual ~Watchable()= default; //destructor

    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session &sess) const = 0;
    long getid() const;
    int getlength() const;
    std::vector<std::string> gettags() const;
    void setlength(int newlength) ;
    virtual string getname() const=0;
    virtual Watchable* clone()=0;

private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session &sess) const;
    string getname() const;
    Watchable* clone();
private:
    std::string name;
};

class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session &sess) const;
    string getname() const;
    Watchable* clone();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
