//
// Created by danil on 12.06.2021.
//

#include "Color.h"
#include <GL/gl.h>

Color::Color(std::string const& _name, int code, float r, float g, float b): name(_name), code(code), r(r), g(g), b(b){}

void Color::color() const{
    glColor3f(r, g, b);
}