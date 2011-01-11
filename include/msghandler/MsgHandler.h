#pragma once

#include<string>
#include<list>
#include<iostream>

enum Verbosity
{
    NONE,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    ALL
};

class MsgHandler
{
private: 
    struct Message
    {
        std::string msg;
        Verbosity verbLvl;
        std::string filename;
        int lineNumber;
        int linePosition;
    };

    MsgHandler() {}
    MsgHandler(const MsgHandler&) {}
    ~MsgHandler() {}
    std::list<Message> messages;

public: 
    static MsgHandler& getInstance(); 
    void addMessage(std::string msg, Verbosity verb, int lineNumber = 0, std::string filename = std::string("DummyFilename.cbp"), int linePosition = 1);
    void addMessageAndPrint(std::string msg, Verbosity verb, int lineNumber = 0, std::string filename = std::string("DummyFilename.cbp"), int linePosition = 1);
    void printMessage(std::string msg);
    void writeLastMessage(std::ostream& os = std::cerr);
};
