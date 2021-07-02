//
// Created by danila on 27.04.2021.
//

#include "XMLObject.h"

    [[nodiscard]] std::string XMLObject::toString(int depth) const {
        if(isLiteral)
            return name;
        std::string str;
        for(int i = 0; i < depth; i++)
            str += '\t';
        str += '<' + name;

        for (std::pair<const std::basic_string<char>, std::basic_string<char>> const& keyValuePair : attributes) {
            str += ' ' + keyValuePair.first + "=\"" + keyValuePair.second + "\"";
        }
        if(children.empty()){
            str += "/>";
            return str;
        }
        str += '>';
        if(children.size() == 1 && children.begin().operator*()->isLiteral) {
            str += children.begin().operator*()->toString(depth + 1);
        } else {
            for (auto child : children) {
                str += '\n' + child->toString(depth + 1);
            }
            str += '\n';
            for(int i = 0; i < depth; i++)
                str += '\t';
        }
        str += "</" + name + '>';
        return str;
    }

    [[nodiscard]] std::list<XMLObject*> XMLObject::getChildren() const {
        return children;
    }

    [[nodiscard]] std::map<std::string, std::string> XMLObject::getAttributes() const {
        return attributes;
    }

    XMLObject* XMLObject::getChildByTagName(std::string const& tagName) const {
        for(auto child : children) {
            if(child->name == tagName)
                return child;
        }
        return nullptr;
    }

    std::list<XMLObject*> XMLObject::getChildrenByTagName(std::string const& tagName) const {
        std::list<XMLObject *> goodChildren;
        for (auto child : children) {
            if (child->name == tagName)
                goodChildren.push_back(child);
        }
        return goodChildren;
    }

    XMLObject::~XMLObject() {
        for(auto child : children)
            delete child;
    }