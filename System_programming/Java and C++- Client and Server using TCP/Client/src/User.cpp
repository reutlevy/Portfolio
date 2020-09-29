//
// Created by reut on 12/01/2020.
//


#include <vector>
#include "list"
#include "unordered_map"
#include "map"
#include "../include/User.h"

using namespace std;

User::User() {

}

User::User(string name, string password, int id) : Username(name),Password(password),Id(id), Loggedin(true),genres(),Books(){
}

void User::setUsername(const string &username) {
    Username=username;
}

void User::setPassword(const string &password) {
    Password=password;
}

const string User::getName() {
    return Username;
}

const string User::getPassword() {
    return Password;
}


bool User::isLoggedin() const {
    return Loggedin;
}

void User::setLoggedin(bool loggedin) {
    Loggedin=loggedin;
}

map<string, int> &User::getGenres() {
    return *genres;
}

list<Book> User::getBooks() {
    return Books;
}

void User::addBook(Book b) {
    Books.push_back(b);
 //   cout<< "addBook function"<<endl;
}

Book User::findBook(string name) {
    for (Book n : Books) {
        if(n.getname()==name)
            return n;
    }
}
