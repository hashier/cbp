#pragma once

#include <list>
#include <string>

class Variable;
class StructVariable;

enum BaseType {
    Type_uint8,
    Type_int8,
    Type_uint16,
    Type_int16,
    Type_uint32,
    Type_int32,
    Type_float32,
    Type_float64
};

class Type
{
    public:
        virtual std::string getString() const { return "tmp. dummy type"; } //= 0;

        virtual int getSize() = 0;

        virtual ~Type() { }
};

class TypeSimple : public Type
{
    public:
        TypeSimple(BaseType baseType)
            : baseType(baseType) { }

        std::string getString() const
        {
            switch(baseType) 
            {
                case Type_uint8:
                    return "uint8";
                case Type_int8:
                    return "int8";
                case Type_uint16:
                    return "uint16";
                case Type_int16:
                    return "int16";
                case Type_uint32:
                    return "uint32";
                case Type_int32:
                    return "int32";
                case Type_float32:
                    return "float32";
                case Type_float64:
                    return "float64";
                default:
                    return "unknown BaseType!";
            }
        }

        virtual int getSize()
        {
            switch(baseType)
            {
                case Type_uint8:
                    return 1;
                case Type_int8:
                    return 1;
                case Type_uint16:
                    return 2;
                case Type_int16:
                    return 2;
                case Type_uint32:
                    return 4;
                case Type_int32:
                    return 4;
                case Type_float32:
                    return 4;
                case Type_float64:
                    return 8;
            }
            //fix warning: not all control paths return a value
            return 0;
        }

        /** Returns whether or not this is a floating type. */
        bool isFloating() {
            return (baseType == Type_float32 || baseType == Type_float64);
        }

        bool operator>(TypeSimple& other) {
            return getSize() > other.getSize();
        }

    protected:
        BaseType baseType;
};

class TypeVoid : public Type {
    static TypeVoid* singleton;

    public:
        TypeVoid() {
        }

        virtual int getSize() {
            return 0;
        }

        static TypeVoid* getSingleton() {
            if(!singleton)
                singleton = new TypeVoid();
            return singleton;
        }

};

class TypeStruct : public Type
{
    public:
        TypeStruct(std::list<StructVariable*>* members)
            : members(members) { }

        std::string getString() const;

        virtual int getSize();

    protected:
        std::list<StructVariable*>* members;
};

class TypePointer : public Type
{
    public:
        TypePointer(Type* type) : type(type) {
        }

        std::string getString() const
        {
            std::string result = "Array of ";
            result.append(type->getString());
            return result;
        }

        virtual ~TypePointer() {
            if (type) delete type; type = 0;
        }

        virtual int getSize() {
            // size of a pointer
            return 8;
        }

        Type* getType() {
            return type;
        }

    protected:
        Type* type;
};

