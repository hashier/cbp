/*
 * Message.h
 *
 *  Created on: 17.01.2011
 *      Author: Remo
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "Mnemonics.h"
#include "AbstractNodes.h"

// use this to write messages (error, warning, info) to assembler file and console
// (debug is only written to assembler file)
class Message : public DebuggableMnemonic {
public:
    Message();
    explicit Message(const Verbosity& verbosity, const std::string& message, const Node* node);
    std::ostream& write(std::ostream& os, const CodeGen& cg) const;
    std::ostream& writeUnformatted(std::ostream& os, const CodeGen& cg) const;

private:
    Verbosity verbosity;
    std::string message;
    int lineNumber;
};

#endif /* MESSAGE_H_ */
