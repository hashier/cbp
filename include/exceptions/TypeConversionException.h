#pragma once

class TypeConversionException : public std::exception {
    public:
        virtual const char* what() const throw() {
            return "TypeConversionException";
        }
};
