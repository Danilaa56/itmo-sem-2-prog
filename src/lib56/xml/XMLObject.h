//
// Created by danila on 27.04.2021.
//

#ifndef PROGACPP_XMLOBJECT_H
#define PROGACPP_XMLOBJECT_H

#include <string>
#include <list>
#include <map>


class XMLObject {
public:
    std::string name;
    XMLObject* parent = nullptr;
    std::list<XMLObject*> children;
    std::map<std::string, std::string> attributes;
    bool isLiteral = false;

    [[nodiscard]] std::string toString(int depth) const;

    [[nodiscard]] std::list<XMLObject*> getChildren() const;

    [[nodiscard]] std::map<std::string, std::string> getAttributes() const;

    [[nodiscard]] XMLObject* getChildByTagName(std::string const& tagName) const;

    [[nodiscard]] std::list<XMLObject*> getChildrenByTagName(std::string const& tagName) const;

    ~XMLObject();
};

#endif //PROGACPP_XMLOBJECT_H
