ALL_FILES    = $(wildcard *.cpp)
SERVER_FILES = $(wildcard SSL_Server*.cpp)
CLIENT_FILES = $(wildcard SSL_Client*.cpp)

ALL_OBJECTS    = $(subst .cpp,.o,$(ALL_FILES))
SERVER_OBJECTS = $(subst .cpp,.o,$(SERVER_FILES))
CLIENT_OBJECTS = $(subst .cpp,.o,$(CLIENT_FILES))

CPP      = g++
CPPFLAGS = -std=c++11 -Wall -O3
LIBRARY  = -lssl

SSL_SERVER_TARGET = SSL_Server
SSL_CLIENT_TARGET = SSL_Client


all: server client
	@echo "SSL Example all"

server:
	$(CPP) -c $(SERVER_FILES)
	$(CPP) $(CPPFLAGS) -o $(SSL_SERVER_TARGET) $(SERVER_OBJECTS) $(LIBRARY)

client:
	$(CPP) -c $(CLIENT_FILES)
	$(CPP) $(CPPFLAGS) -o $(SSL_CLIENT_TARGET) $(CLIENT_OBJECTS) $(LIBRARY)

clean:
	rm -rf $(SSL_SERVER_TARGET) $(SSL_CLIENT_TARGET) $(ALL_OBJECTS)
