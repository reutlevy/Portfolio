#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>


class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

using namespace std;
class BaseAction{
public:
    BaseAction();

    virtual  ~BaseAction()= default;

    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    std::string statustoString() const;
    virtual BaseAction* clone()=0;

protected:
    void complete();
    void pending();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;

};


class CreateUser  : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();
};

class ChangeActiveUser : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();
};

class DeleteUser : public BaseAction {
public:
    virtual void act(Session & sess);
    virtual std::string toString() const;
    BaseAction* clone();
};


class DuplicateUser : public BaseAction {
public:
    virtual void act(Session & sess);
    virtual std::string toString() const;
    BaseAction* clone();
};

class PrintContentList : public BaseAction {
public:
    virtual void act (Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();
};

class PrintWatchHistory : public BaseAction {
public:
    virtual void act (Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();
};


class Watch : public BaseAction {
public:
    Watch();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();

private:
    int count;
};


class PrintActionsLog : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();
};

class Exit : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    BaseAction* clone();
};
#endif