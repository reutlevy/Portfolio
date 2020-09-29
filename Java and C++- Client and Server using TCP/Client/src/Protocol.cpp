//
// Created by danie on 1/13/2020.
//


#include "unordered_map"
#include "../include/Protocol.h"
#include <sstream>

using namespace std;


Protocol::Protocol(EncoderDecoder encoderDecoder1):encoderDecoder(encoderDecoder1) {

}

StompFrame Protocol::process(StompFrame frame) {
    StompFrame answer;

    //  cout<< "----- get into proccess----"<<endl;
    //   cout<<"---- the type is "+frame.getType()<<endl;
    if (frame.getType() == "CONNECTED") {  //TODO maybe we need to add somthing
   //     cout << "Login successful"<<endl;
        user.setLoggedin(true);
        answer.setType("NOTHING");
        return answer;
    }

    if (frame.getType() == "ERROR") {
        answer.setType("ERROR");
        return answer;
    }

    if (frame.getType()=="RECEIPT"){
        answer.setType("NOTHING");
        return answer;
    }

    if (frame.getType()=="MESSAGE"){
        //     cout<< "I got the MESSAGE!!!!!!!!!!!!!!!!!!" <<endl;
        string genre = frame.getHeaders().at("destination");
        string subscirptionAtMessage = frame.getHeaders().at("subscription");
        string message = frame.getBody();
        //       cout<<"---------> the body is:::"+message<<endl;
        std::vector<std::string> vector;
        vector = encoderDecoder.split(message," ");
        int end = message.size() - 1;
        unordered_map<string,string> headers;

        if(message.find("added") != std::string::npos) {
            //   cout<<"I FOUND ADDED!!!!"<<endl;
            string username = vector[0];
            if(user.getName()==username) {
                string bookName="";
                for(int i=5; (unsigned) i<vector.size(); i++){
                    bookName=bookName+vector[i];
                }
                Book newBook(bookName, genre, username, true);
                user.addBook(newBook);
                list<Book> booklist= user.getBooks();
                for(Book b: booklist){
                    //           cout<<"INSERTED BOOK "+ b.getname()<< endl;
                }
                //         cout<<"IM PUSHED!!"<<endl;
            }
            answer.setType("NOTHING");
            return answer;
        }

        if(message.find("borrow")!= std::string::npos) {
       //     cout << "IFOUND THE BORROW"<<endl;
        //    string bookName = message.substr(4, end);
            string bookName = vector[4];
            if(!user.getBooks().empty()){
                for (Book b : user.getBooks()){
                    if (b.getname()==bookName){
                        if(b.getisExist()) {
                            answer.setType("SEND");
                            answer.setBody(user.getName() + " has " + bookName);
                            headers.insert({"destination", genre});
                            answer.setHeaders(headers);
                        }
                    }
                }
            }
            return answer;
        }

        if(message.find("has")!= std::string::npos && message.find("added") == std::string::npos){
            string bookName = vector[5];
            string Lendername = vector[0];
            int currsubscription = user.getGenres().at(genre);
            stringstream ss;
            ss << currsubscription;
            string str = ss.str();
            if(str==subscirptionAtMessage){
                answer.setType("SEND");
                answer.setBody("Taking " + bookName + " from "+Lendername);
                headers.insert({"destination", genre});
                answer.setHeaders(headers);
            }
            return answer;
        }

        if(message.find("Taking")!= std::string::npos){
            string bookName = vector[0];
            string Lendername = vector[1];
            int currsubscription = user.getGenres().at(genre);
            stringstream ss;
            ss << currsubscription;
            string str = ss.str();
            if(str==subscirptionAtMessage){
                Book newBook(bookName, genre, Lendername, true);
                user.getBooks().push_back(newBook);
            }
            else if(user.getName()==Lendername) {
                if (!user.getBooks().empty()) {
                    for (Book b : user.getBooks()) {
                        if (b.getname() == bookName) {
                            if (b.getisExist()) {
                                b.setisExist(false);
                            }
                        }
                    }
                }
            }
            answer.setType("NOTHING");
            return answer;
        }


        if(message.find("Returning")!= std::string::npos){
            string bookName = vector[1];
            string Lendername = vector[2];
            string nameOfGive;

            if(!user.getBooks().empty() && !(user.getName()== Lendername)) {
                for (Book b : user.getBooks()) {
                    if (b.getname() == bookName) {
                        if (b.getisExist()) {
                            if (b.gettakenFrom()==Lendername) {
                                b.setisExist(false);
                            }
                        }
                    }
                }
            }
            else if(user.getName()==Lendername && !user.getBooks().empty()){
                for (Book b : user.getBooks()) {
                    if (b.getname() == bookName) {
                        if (!b.getisExist()) {
                            b.setisExist(true);
                        }
                    }
                }

            }
            answer.setType("NOTHING");
            return answer;
        }

        if(message.find("status")!= std::string::npos){
            //   cout <<"I know its status" <<endl;
            answer.setType("SEND");
            string begin = user.getName()+":";
            answer.setBody(user.getName()+":");
            headers.insert({"destination", genre});
            answer.setHeaders(headers);


            if(!user.getBooks().empty()){
                //       cout<< "the inventory is not enmpty!!!!!!!"<<endl;
                string bookslist = "";
                for (Book b : user.getBooks()) {
                  //    if(bookslist.find(",")!= std::string::npos)
                          bookslist = bookslist + "," + b.getname();
                    //  else bookslist = bookslist + b.getname();
                }
                answer.setBody(begin + bookslist);
                cout<< "the answer of status--- "<< endl;
                cout<< answer.toString();
            }
            return answer;
        }
        else{
            answer.setType("NOTHING");
            return answer;
        }
    }
}

void Protocol::setUser(User user1) {
    user=user1;
}
