//
// Created by danil on 12.06.2021.
//

#ifndef PROGACPP_COLOR_H
#define PROGACPP_COLOR_H

#include <iostream>

class Color{
public:
    std::string name;
    int code;
    float r, g, b;
    Color(std::string const& _name, int code, float r, float g, float b);
    void color() const;
};

#endif //PROGACPP_COLOR_H
