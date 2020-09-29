//
// Created by reut on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USER_H
#define BOOST_ECHO_CLIENT_USER_H

#endif //BOOST_ECHO_CLIENT_USER_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include "Book.h"
#include "map"

using namespace std;

class User {

private:
    string Username;
    string Password;
    int Id;
    map<string, int>* genres;
    list<Book> Books;
    bool Loggedin;


public:
    User();

    User(string name, string password, int id);

    void setUsername(const string &username);

    void setPassword(const string &password);

    const string getName();

    const string getPassword();

    map<string, int> &getGenres();

    bool isLoggedin() const;

    void setLoggedin(bool loggedin);

    list<Book> getBooks();

    void addBook(Book);

    Book findBook(string name);

    virtual ~User()= default;

    virtual User& operator=(const User& packet)= default;

};