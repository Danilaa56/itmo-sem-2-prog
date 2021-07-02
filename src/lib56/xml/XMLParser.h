//
// Created by danila on 27.04.2021.
//

#ifndef PROGACPP_XMLPARSER_H
#define PROGACPP_XMLPARSER_H

#include "XMLObject.h"
#include <algorithm>

namespace XMLParser56 {

    XMLObject* parse(std::string const& documentData);

    bool inline isAlphabetic(char c);
    bool inline isDigit(char c);

    size_t parseAttributes(std::string const& data, size_t i, XMLObject* object);
    size_t skipWhitespaces(std::string const& data, size_t i);

    std::string getField(std::string const& node, std::string const& fieldName);
    size_t indexOf(std::string const& chars, char c, size_t offset);

};



#endif //PROGACPP_XMLPARSER_H
