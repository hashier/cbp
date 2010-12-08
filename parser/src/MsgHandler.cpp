#include "MsgHandler.h"

MsgHandler& MsgHandler::getInstance() 
{ 
    static MsgHandler instance; 
    return instance; 
}


void MsgHandler::addMessage(std::string msg, Verbosity verb)
{
    Message elem;
    elem.msg = msg;
    elem.verbLvl = verb;
    messages.push_back(elem);
}

void MsgHandler::addMessageAndPrint(std::string msg, Verbosity verb)
{
    Message elem;
    elem.msg = msg;
    elem.verbLvl = verb;
    messages.push_back(elem);
    std::cout << msg << std::endl;
}

void MsgHandler::printMessage(std::string msg)
{
    std::cout << msg << std::endl;
}
