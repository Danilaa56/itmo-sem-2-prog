//
// Created by danil on 27.04.2021.
//

#include "XMLParser.h"
#include <iostream>

namespace XMLParser56 {

    size_t min(size_t a, size_t b) {
        return a > b ? b : a;
    }

    size_t max(size_t a, size_t b) {
        return a < b ? b : a;
    }

    void replace(std::string &a, std::string const &from, std::string const &to) {
        size_t pos = 0;
        while ((pos = a.find(from, pos)) != std::string::npos) {
            a.replace(pos, from.length(), to);
            pos += to.length();
        }
    }

    class XMLException : public std::runtime_error {

        explicit XMLException(std::string const &message) : std::runtime_error(message) {

        }

        static std::string stringPreviewWithPointer(std::string const &chars, size_t index) {
            size_t outEnd = min(index + 20, chars.length());
            size_t outBegin = max(outEnd - 40, 0);
            std::string out(chars, outBegin, outEnd - outBegin);
            replace(out, "\r", "\\r");
            replace(out, "\n", "\\n");
            replace(out, "\t", "\\t");

            out = "\n" + out + "\n";
            for (size_t j = outBegin; j < index; j++) {
                out += " ";
                if (chars[j] == '\r' || chars[j] == '\n' || chars[j] == '\t')
                    out += " ";
            }
            out += "^\n";
            return out;
        }

    public:
        static XMLException exceptionAt(std::string const &chars, size_t index, char expected) {
            std::string sb = stringPreviewWithPointer(chars, index);

            sb += "'";
            sb += expected;
            sb += "' was not found\n";

            if (index == chars.length())
                sb += "Out of bounds";
            else {
                sb += "Unexpected character: '";
                sb += chars[index];
                sb += "'";
            }

            return XMLException(sb);
        }

        static XMLException exceptionAt(std::string const &chars, size_t index, std::string const &message) {
            std::string sb = stringPreviewWithPointer(chars, index) + message;
            return XMLException(sb);
        }
    };

    char const whitespaceCharacters[5] = " \r\t\n";
    size_t closeBracketIndex = 0;

    XMLObject* parse(std::string const& data, size_t offset);
    XMLObject* parse(std::string const& documentData) {
        size_t headerEnd = 0;
        if(documentData[0] == '<' && documentData[1] == '?'){
            headerEnd = documentData.find("?>")+2;
            std::string header = documentData.substr(0, headerEnd);

            std::string version = getField(header, "version");
            if(version.length() > 0 && version != "1.0"){
                std::cout << "XML version: " << version;
                return nullptr;
            }
            std::string encoding = getField(header, "encoding");
            std::transform(encoding.begin(), encoding.end(),encoding.begin(), ::toupper);
            if(encoding.length() > 0 && (encoding != "UTF-8" && encoding != "WINDOWS-1251")){
                std::cout << ("Unsupported encoding: " + encoding);
                return nullptr;
            }
        }
        return parse(documentData, headerEnd);
    }

    void checkEndOfString(std::string const& data, size_t index) {
        if(index >= data.length())
            throw XMLException::exceptionAt(data, data.length(), "Unexpected end of string");
    }

    XMLObject* parse(std::string const& data, size_t offset) {
        auto object = new XMLObject();
        try {
            size_t i = offset;
            char c;

            i = skipWhitespaces(data, i);
            checkEndOfString(data, i);
            if (data[i] != '<')
                throw XMLException::exceptionAt(data, i, "'<' was expected");

            i++;
            checkEndOfString(data, i);
            c = data[i];
            size_t nameStartIndex = i;
            while ((('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_') && i < data.length()) {
                i++;
                c = data[i];
            }
            checkEndOfString(data, i);
            size_t nameLength = i - nameStartIndex;

            object->name = std::string(data, nameStartIndex, nameLength);

            i = parseAttributes(data, i, object);
            checkEndOfString(data, i);

            if (data[i] == '/') {
                i++;
                checkEndOfString(data, i);
                if (data[i] == '>') {
                    closeBracketIndex = i;
                    return object;
                } else
                    throw XMLException::exceptionAt(data, i, "'>' was expected");
            }
            if (data[i] == '>') {
                while (true) {
                    i = skipWhitespaces(data, i + 1);
                    checkEndOfString(data, i);
                    size_t firstPrintableCharacter = i;
                    if (data[i] != '<') {
                        i = indexOf(data, '<' , i);
                        size_t lastPrintableCharacter = i;
                        auto stringChild = new XMLObject();
                        stringChild->name = std::string (data, firstPrintableCharacter, lastPrintableCharacter - firstPrintableCharacter);
                        stringChild->isLiteral = true;
                        object->children.push_back(stringChild);
                    }
                    if (data[i] != '<')
                        throw XMLException::exceptionAt(data, i, "'<' was expected");
                    checkEndOfString(data, i + 1);
                    if (data[i + 1] == '/') {
                        i += 2;
                        for (int j = 0; j < nameLength; j++) {
                            checkEndOfString(data, i + j);
                            if (data[nameStartIndex + j] != data[i + j]) {
                                throw XMLException::exceptionAt(data, i + j, "Closing tag </" + object->name + "> was expected");
                            }
                        }

                        i = skipWhitespaces(data, i + nameLength);
                        checkEndOfString(data, i);
                        if (data[i] == '>') {
                            closeBracketIndex = i;
                            return object;
                        } else
                            throw XMLException::exceptionAt(data, i, "'>' was expected");
                    } else {
                        auto child = parse(data, i);
                        child->parent = object;
                        object->children.push_back(child);
                        i = closeBracketIndex;
                    }
                }
            }
            throw XMLException::exceptionAt(data, i, "Unexpected character");
        } catch (XMLException const& e) {
            std::cerr << e.what() << std::endl;
            delete object;
            return nullptr;
        }
    }

    bool inline isAlphabetic(char c) {
        return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
    }

    bool inline isDigit(char c) {
        return '0' <= c && c <= '9';
    }

    size_t parseAttributes(std::string const& data, size_t i, XMLObject* object) {
        size_t length = data.length();
        while(i < length){
            i = skipWhitespaces(data, i);
            if(i == length)
                break;
            if(data[i] == '/' || data[i] == '>')
                return i;

            size_t attributeNameStart = i;
            if(!isAlphabetic(data[i]) && data[i] != '_'){
                std::string message = "Letter or '_' was expected. Got: 'c'";
                message[34] = data[i];
                throw XMLException::exceptionAt(data, i, message);
            }
            i++;
            while (isAlphabetic(data[i]) || data[i] == '-' || data[i] == ':' || data[i] == '_' || isDigit(data[i])) {
                if (++i == length)
                    throw XMLException::exceptionAt(data, i, "End of string. '=' was expected");
            }
            std::string attributeName(data, attributeNameStart, i-attributeNameStart);

            i = skipWhitespaces(data, i);
            if(i == length)
                throw XMLException::exceptionAt(data, i, "End of string. '=' was expected");
            if(data[i] != '=')
                throw XMLException::exceptionAt(data, i, "'=' was expected");

            i = skipWhitespaces(data, i + 1);
            if(i == length)
                throw XMLException::exceptionAt(data, i, "End of string. Value was expected");

            if(data[i] != '"')
                throw XMLException::exceptionAt(data, i, "Opening '\"' was expected");

            size_t firstQuot = i;
            size_t secondQuot = indexOf(data, '"', i + 1);
            if(secondQuot == -1)
                throw XMLException::exceptionAt(data, length, "End of string. Closing '\"' was expected");

            std::string value(data, firstQuot+1, secondQuot-firstQuot-1);
            object->attributes[attributeName] = value;
            i = secondQuot + 1;
        }
        throw XMLException::exceptionAt(data, length, "End of string. \"/>\" was expected");
    }

    size_t skipWhitespaces(std::string const& data, size_t i) {
        size_t length = data.length();
        for(; i < length; i++){
            bool notSkip = true;
            for (char c : whitespaceCharacters) {
                if (data[i] == c) {
                    notSkip = false;
                    break;
                }
            }
            if(notSkip)
                return i;
        }
        return i;
    }

    std::string getField(std::string const& node, std::string const& fieldName) {
        size_t indexOfFieldName = node.find(fieldName);
        if(indexOfFieldName == -1)
            return "";

        size_t index = indexOfFieldName + fieldName.length();
        size_t length = node.length();

        while(index < length){
            if(node[index] == '=') {
                if(++index == length)
                    return "";
                break;
            }
            if(node[index] != ' ' || ++index == length) {
                std::cout << (node);
                for(int i = 0; i < index; i++)
                    std::cout << ' ';
                std::cout << ('^');
                std::cout << ("Looking for '='");
                std::cout << ("Failed to get field: " + fieldName);
                return "";
            }
        }
        size_t firstQuot = 0, secondQuot;
        while(index < length){
            if(node[index] == '"' || node[index] == '\''){
                firstQuot = index;
                index++;
                if(++index == length)
                    return "";
                break;
            }
            if(node[index] != ' ' || ++index == length) {
                std::cout << (node);
                for(int i=0; i<index; i++)
                    std::cout << ' ';
                std::cout << ('^');
                std::cout << ("Looking for the first '\"'");
                std::cout << ("Failed to get field: " + fieldName);
                return "";
            }
        }
        while(true) {
            if(node[index] == '"' || node[index] == '\'') {
                secondQuot = index;
                break;
            }
            if(++index == length) {
                std::cout << (node);
                for(int i=0; i<index; i++)
                    std::cout << ' ';
                std::cout << ('^');
                std::cout << ("Looking for the second '\"'");
                std::cout << ("Failed to get field: " + fieldName);
                return "";
            }
        }
        return node.substr(firstQuot + 1, secondQuot - firstQuot - 1);
    }

    size_t indexOf(std::string const& chars, char c, size_t offset) {
        size_t length = chars.length();
        for(size_t i = offset; i < length; i++){
            if(chars[i] == c)
                return i;
        }
        return -1;
    }
};