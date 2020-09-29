CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system
CFLAGS += -pthread
LDFLAGS += -pthread

all: StompBookClubClient
	g++ -o bin/StompBookClubClient bin/Book.o bin/Client.o bin/connectionHandler.o bin/EncoderDecoder.o bin/Protocol.o bin/StompconnectionHandler.o bin/StompFrame.o bin/User.o $(LDFLAGS) 

StompBookClubClient: bin/Book.o bin/Client.o bin/connectionHandler.o bin/EncoderDecoder.o bin/Protocol.o bin/StompconnectionHandler.o bin/StompFrame.o bin/User.o

bin/Book.o: src/Book.cpp include/Book.h
	g++ $(CFLAGS) -o bin/Book.o src/Book.cpp

bin/Client.o: src/Client.cpp include/Client.h
	g++ $(CFLAGS) -o bin/Client.o src/Client.cpp
	
bin/connectionHandler.o: src/connectionHandler.cpp include/connectionHandler.h
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/EncoderDecoder.o: src/EncoderDecoder.cpp include/EncoderDecoder.h
	g++ $(CFLAGS) -o bin/EncoderDecoder.o src/EncoderDecoder.cpp
	
bin/Protocol.o: src/Protocol.cpp include/Protocol.h
	g++ $(CFLAGS) -o bin/Protocol.o src/Protocol.cpp

bin/StompconnectionHandler.o: src/StompconnectionHandler.cpp include/StompconnectionHandler.h
	g++ $(CFLAGS) -o bin/StompconnectionHandler.o src/StompconnectionHandler.cpp

bin/StompFrame.o: src/StompFrame.cpp include/StompFrame.h
	g++ $(CFLAGS) -o bin/StompFrame.o src/StompFrame.cpp

bin/User.o: src/User.cpp include/User.h
	g++ $(CFLAGS) -o bin/User.o src/User.cpp

.PHONY: clean
clean:
	rm -f bin/*
