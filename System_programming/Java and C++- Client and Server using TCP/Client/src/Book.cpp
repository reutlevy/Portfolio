//
// Created by danie on 1/14/2020.
//

#include "../include/Book.h"
using namespace std;

#include <string>

Book::Book() {

}

Book::Book(string bookname, string genre, string takenfrom, bool isExist): bookname(bookname), genre(genre), takenFrom(takenfrom), isExist(isExist){

}

void Book::setTaken(const string &username) {
    takenFrom=username;
}

const string Book::getgenre() {
    return genre;
}

const string Book::gettakenFrom() {
    return takenFrom;
}


void Book::setBookname(const string &bookname1) {
    bookname=bookname;
}

void Book::setGenre(const string &genre1) {
    genre=genre;
}

string Book::getname() {
    return bookname;
}

bool Book::getisExist() {
    return isExist;
}

void Book::setisExist(bool exist) {

}