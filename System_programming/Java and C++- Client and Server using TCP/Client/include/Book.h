//
// Created by danie on 1/14/2020.
//

#ifndef BOOST_ECHO_CLIENT_2_BOOK_H
#define BOOST_ECHO_CLIENT_2_BOOK_H

#include <string>
using namespace std;

class Book {
private:
    string bookname;
    string genre;
    string takenFrom;
    bool isExist = false;

public:
    Book();

    Book(string bookname, string genre, string takenfrom, bool isExist);

    void setTaken(const string &username);

    void setBookname (const string &bookname);

    void setGenre (const string &genre);

    const string getgenre();

    string getname();

    const string gettakenFrom();

    bool getisExist ();

    void setisExist(bool exist);


};


#endif //BOOST_ECHO_CLIENT_2_BOOK_H