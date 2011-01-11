#include "MsgHandler.h"

MsgHandler& MsgHandler::getInstance() 
{ 
    static MsgHandler instance; 
    return instance; 
}


void MsgHandler::addMessage(std::string msg, Verbosity verb, int lineNumber, std::string filename, int linePosition)
{
    Message elem;
    elem.msg = msg;
    elem.verbLvl = verb;
    elem.lineNumber = lineNumber;
    elem.linePosition = linePosition;
    elem.filename = filename;
    messages.push_back(elem);
}

void MsgHandler::addMessageAndPrint(std::string msg, Verbosity verb, int lineNumber, std::string filename, int linePosition)
{
    addMessage(msg, verb, lineNumber, filename, linePosition);
    writeLastMessage(std::cerr);
}

void MsgHandler::printMessage(std::string msg)
{
    std::cout << msg << std::endl;
}

void MsgHandler::writeLastMessage(std::ostream& os)
{
    Message msg = messages.back();
    os << msg.filename << ":" << msg.lineNumber << ":" << msg.linePosition << ": ";
    switch (msg.verbLvl) {
        case NONE:    os << "error: NONE"; break;
        case ERROR:   os << "error"; break;
        case WARNING: os << "warning"; break;
        case INFO:    os << "info"; break;
        case DEBUG:   os << "info: DEBUG"; break;
        case ALL:
        default:      os << "error: ALL";
    }
    os << ": "<< msg.msg << std::endl;
}
