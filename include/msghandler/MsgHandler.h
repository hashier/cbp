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
    };

    MsgHandler() {}
    MsgHandler(const MsgHandler&) {}
    ~MsgHandler() {}
    std::list<Message> messages;

public: 
    static MsgHandler& getInstance(); 
    void addMessage(std::string msg, Verbosity verb);
    void addMessageAndPrint(std::string msg, Verbosity verb);
    void printMessage(std::string msg);
};
