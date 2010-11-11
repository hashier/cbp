#include <list>
#include "VariableDef.h"

#pragma once

enum BaseType {
    Type_uint8,
    Type_int8,
    Type_uint16,
    Type_int16,
    Type_uint32,
    Type_int32,
    Type_float32,
    Type_float64,
    Type_void
};

class Type
{
    public:
        virtual std::string getString() const { return "tmp. dummy type"; } //= 0;
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
                case Type_void:
                    return "void";
                default:
                    return "unknown BaseType!";
            }
        }

    protected:
        BaseType baseType;
};

class TypeStruct : public Type
{
    public:
        TypeStruct(const std::list<SymbolTable::VariableDef *> &members)
            : members(members) { }

        std::string getString() const
        {
            std::string result = "Struct of (";
            for (std::list<SymbolTable::VariableDef *>::const_iterator it = members.begin(); it != members.end(); it++)
            {
                if (it != members.begin())
                {
                    result.append(", ");
                }
                result.append((*it)->getType()->getString());

            }
            result.append(")");
            return result;
        }

    protected:
        std::list<SymbolTable::VariableDef *> members;
};

class TypeArray : public Type
{
    public:
        TypeArray(Type *type)
            : type(type) { }

        std::string getString() const
        {
            std::string result = "Array of ";
            result.append(type->getString());
            return result;
        }

    protected:
        Type *type;
};

