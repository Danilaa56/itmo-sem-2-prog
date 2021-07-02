//
// Created by danil on 21.03.2021.
//

#ifndef PROGACPP_POLYNOM_H
#define PROGACPP_POLYNOM_H

#include <iostream>

#define POLYNOM_RANGE 16

class Polynom{
public:
    explicit Polynom();
    Polynom(Polynom const& other);

    void increase(int power, double value);
    double calculate(double argument) const;

    bool operator == (Polynom const& other) const;
    bool operator != (Polynom const& other) const;

    Polynom operator + (Polynom const& other) const;
    Polynom operator - (Polynom const& other) const;
    Polynom operator + (double scalar) const;
    Polynom operator - (double scalar) const;
    Polynom operator - () const;

    Polynom operator += (Polynom const& other);
    Polynom operator -= (Polynom const& other);
    Polynom operator += (double scalar);
    Polynom operator -= (double scalar);

    Polynom operator * (Polynom const& other);
    Polynom operator * (double const scalar);
    Polynom operator / (double const scalar);

    Polynom operator *= (Polynom const& other);
    Polynom operator *= (double const scalar);
    Polynom operator /= (double const scalar);

    friend std::ostream& operator << (std::ostream& os, Polynom& polynom);
    friend std::istream& operator >> (std::istream& is, Polynom& polynom);

private:
    double indices[POLYNOM_RANGE]{};
};

std::ostream& operator << (std::ostream& os, Polynom const& polynom);
std::istream& operator >> (std::istream& is, Polynom& polynom);

#endif //PROGACPP_POLYNOM_H
