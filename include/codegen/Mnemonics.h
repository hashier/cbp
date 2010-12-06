/// This file provides an abstraction (not very high, just strings) for mnemonics.
/// The purpose for now is to have the same indention everywhere.

#include <ostream>
#include <string>
#include <sstream>

class Mnemonic {
public:
    virtual std::ostream& write(std::ostream& os) const = 0;
    virtual std::ostream& writeUnformatted(std::ostream& os) const = 0;
};

// Tag to chose not to generate a space
struct NoSpace {};
const NoSpace noSpace = NoSpace();

class Nothing : public Mnemonic {
public:
    explicit Nothing(std::string const& nodeId_)
        : nodeId(nodeId_) {}
        
    std::ostream& write(std::ostream& os) const {
        return os << nodeId << ": to be implemented!" << std::endl;
    }
    
    std::ostream& writeUnformatted(std::ostream& os) const {
        return write(os);
    }

private:
    std::string nodeId;
};

class Label : public Mnemonic {
public:
    Label(){}
    
    explicit Label(std::string const& label_)
        : label(label_) {}
    
    std::ostream& write(std::ostream& os) const {
        return os << label << ':' << std::endl;
    }
    
    std::ostream& writeUnformatted(std::ostream& os) const {
        return os << label;
    }

private:
    std::string label;
};

class Command : public Mnemonic {
public:
    Command(){}
        
    explicit Command(std::string const& command_){
        command << command_;    
    }
        
    /// Semantic:
    /// appends param as it is to the current command,
    /// inserting a space between them (nothing more).
    Command& operator()(std::string const& param){
        command << ' ' << param;
        return *this;
    }
    
    /// Semantic:
    /// appends (preArg || param) (concatenation) to the current command.
    template<typename T>
    Command& operator()(T const& preArg, std::string const& param){
        command << ' ' << preArg << param;
        return *this;
    }
    
    /// Semantic:
    /// appends (param || postArg) (concatenation) to the current command.
    template<typename T>
    Command& operator()(std::string const& param, T const& postArg){
        command << ' ' << param << postArg;
        return *this;
    }
    
    /// Semantic:
    /// appends label as it is to the current command,
    /// inserting a space between them (nothing more).
    Command& operator()(Label const& label){
        command << ' ';
        label.writeUnformatted(command);
        return *this;
    }
    
    /// Semantic:
    /// appends param as it is to the current command.
    Command& operator()(NoSpace const& /*noSpace*/, std::string const& param){
        command << param;
        return *this;
    }
    
    std::ostream& write(std::ostream& os) const {
        return os << "    " << command.str() << std::endl;
    }
    
    std::ostream& writeUnformatted(std::ostream& os) const {
        return os << command.str();
    }

private:
    std::stringstream command;
};

