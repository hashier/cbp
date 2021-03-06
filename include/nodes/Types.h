#pragma once

#include <list>
#include <string>
#include <map>

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

        virtual bool isInteger() const {
            return false;
        }

        virtual bool hasSignedBit() const {
            return false;
        }

        virtual Type *clone() = 0;

        virtual ~Type() { }
};

class TypeSimple : public Type
{
    public:
        TypeSimple(BaseType baseType)
            : baseType(baseType) { }

        TypeSimple(TypeSimple *type)
            : baseType(type->baseType) { }

        Type *clone() 
        {
            return new TypeSimple(this);
        }

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

        /** Returns False, if this type is 'uint8' or 'uint16' or 'uint32', otherwise True. */
        virtual bool hasSignedBit() const {
            return baseType != Type_uint8 && baseType != Type_uint16 && baseType != Type_uint32;
        }

        /** Returns whether or not this is a integer type. */
        virtual bool isInteger() const {
            return baseType <= Type_int32;
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

        TypeVoid(TypeVoid *type) {
        }

        Type *clone()
        {
            return new TypeVoid(this);
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
        TypeStruct(std::map<std::string, StructVariable*>* members);

        TypeStruct(TypeStruct *type);

        Type *clone()
        {
            return new TypeStruct(this);
        }

        std::string getString() const;
        inline StructVariable* getVariable(std::string identifier) { return (*members)[identifier]; }

        inline int getSize() { return size; }

    protected:
        std::map<std::string, StructVariable*>* members;
        int size;
};

class TypePointer : public Type
{
    public:
        TypePointer(Type* type) : type(type) {
        }

        TypePointer(TypePointer *t)
        {
            type = t->type->clone();
        }


        Type *clone()
        {
            return new TypePointer(this);
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

