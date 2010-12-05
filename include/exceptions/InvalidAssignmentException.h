#pragma once

class InvalidAssignmentException : public std::exception {
    public:
        virtual const char* what() const throw() {
            return "InvalidAssignmentException";
        }
};
