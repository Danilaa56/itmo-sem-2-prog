//
// Created by danil on 21.03.2021.
//

#include "Polynom.h"

Polynom::Polynom(){
    for(double & index : indices)
        index = 0;
}
Polynom::Polynom(Polynom const& other){
    for(int i = 0; i < POLYNOM_RANGE; i++){
        indices[i] = other.indices[i];
    }
}

void Polynom::increase(int power, double value){
    indices[power+POLYNOM_RANGE/2] += value;
}
double Polynom::calculate(double argument) const{
    double result = 0;
    for(int i = POLYNOM_RANGE-1; i >= POLYNOM_RANGE/2; i--){
        result = result*argument + indices[i];
    }
    return result;
}

bool Polynom::operator == (Polynom const& other) const{
    for(int i = 0; i < POLYNOM_RANGE; i++)
        if(indices[i] != other.indices[i])
            return false;
    return true;
}
bool Polynom::operator != (Polynom const& other) const{
    return !(*this == other);
}

Polynom Polynom::operator + (Polynom const& other) const{
    Polynom p;
    for(int i = 0; i < POLYNOM_RANGE; i++)
        p.indices[i] = indices[i] + other.indices[i];
    return p;
}
Polynom Polynom::operator - (Polynom const& other) const{
    Polynom p;
    for(int i = 0; i < POLYNOM_RANGE; i++)
        p.indices[i] = indices[i] - other.indices[i];
    return p;
}
Polynom Polynom::operator + (double scalar) const{
    Polynom p(*this);
    p.indices[POLYNOM_RANGE/2] += scalar;
    return p;
}
Polynom Polynom::operator - (double scalar) const{
    Polynom p(*this);
    p.indices[POLYNOM_RANGE/2] -= scalar;
    return p;
}
Polynom Polynom::operator - () const{
    Polynom p;
    for(int i = 0; i < POLYNOM_RANGE; i++)
        p.indices[i] = -indices[i];
    return p;
}
Polynom Polynom::operator += (Polynom const& other){
    for(int i = 0; i < POLYNOM_RANGE; i++)
        indices[i] += other.indices[i];
    return *this;
}
Polynom Polynom::operator -= (Polynom const& other){
    for(int i = 0; i < POLYNOM_RANGE; i++)
        indices[i] -= other.indices[i];
    return *this;
}
Polynom Polynom::operator += (double scalar){
    indices[POLYNOM_RANGE/2] += scalar;
    return *this;
}
Polynom Polynom::operator -= (double scalar){
    indices[POLYNOM_RANGE/2] -= scalar;
    return *this;
}

Polynom Polynom::operator * (Polynom const& other){
    Polynom p;
    for(int i = -POLYNOM_RANGE/2; i < POLYNOM_RANGE/2; i++){
        for(int j = std::max(-POLYNOM_RANGE/2-i, -POLYNOM_RANGE/2); j+i < POLYNOM_RANGE/2 && j < POLYNOM_RANGE/2; j++){
            p.indices[i+j + POLYNOM_RANGE/2] += indices[i+POLYNOM_RANGE/2]*other.indices[j+POLYNOM_RANGE/2];
        }
    }
    return p;
}
Polynom Polynom::operator * (double const scalar){
    Polynom p;
    for(int i = 0; i < POLYNOM_RANGE; i++)
        p.indices[i] = indices[i]*scalar;
    return p;
}
Polynom Polynom::operator / (double const scalar){
    Polynom p;
    for(int i = 0; i < POLYNOM_RANGE; i++)
        p.indices[i] = indices[i]/scalar;
    return p;
}
Polynom Polynom::operator *= (Polynom const& other){
    *this = *this*other;
    return *this;
}
Polynom Polynom::operator *= (double const scalar){
    for(double & index : indices)
        index *= scalar;
    return *this;
}
Polynom Polynom::operator /= (double const scalar){
    for(double & index : indices)
        index /= scalar;
    return *this;
}

std::ostream& operator << (std::ostream& os, Polynom& polynom){
    bool wasFirst = false;
    for(int i = 0; i < POLYNOM_RANGE; i++){
        if(polynom.indices[i] != 0){
            if(wasFirst){
                os << (polynom.indices[i] < 0 ? " - " : " + ");
            } else {
                wasFirst = true;
                if(polynom.indices[i] < 0){
                    os << '-';
                }
            }

            if(i == POLYNOM_RANGE/2){
                os << (polynom.indices[i] < 0 ? -polynom.indices[i] : polynom.indices[i]);
            } else {
                if(polynom.indices[i] != 1 && polynom.indices[i] != -1){
                    os << (polynom.indices[i] < 0 ? -polynom.indices[i] : polynom.indices[i]);
                }
                os << 'x';
                if(i != POLYNOM_RANGE/2+1)
                    os << '^' << i-POLYNOM_RANGE/2;
            }
        }
    }
    if(!wasFirst)
        os << '0';

    return os;
}
std::istream& operator >> (std::istream& is, Polynom& polynom){
    std::string str;
    is >> str;

    int state = 0;

    bool isMinus = false;

    int k = 0;
    int degree = 0;

    int length = str.length();
    for(int i = 0; i < length; i++){
        char c = str[i];

        bool tokenReady = false;

        switch(state){
            case 0:
                if(c == '-'){
                    isMinus = !isMinus;
                } else if(c >= '0' && c <= '9') {
                    k = c-48;
                    degree = 0;
                    state = 1;
                } else if(c == 'x') {
                    state = 2;
                    k = 1;
                    if(isMinus)
                        k = -k;
                    isMinus = false;
                }
                break;
            case 1:
                if(c >= '0' && c <= '9'){
                    k = k*10 + c-48;
                } else if(c == 'x'){
                    degree = 1;
                    state = 2;
                    if(isMinus)
                        k = -k;
                    isMinus = false;
                } else if(c == '-' || c == '+'){
                    state = 0;
                    if(isMinus)
                        k = -k;
                    degree = 0;
                    tokenReady = true;
                    isMinus = c == '-';
                }
                break;
            case 2:
                if(c == '^'){
                    state = 3;
                    degree = 0;
                    isMinus = false;
                } else if(c == '-' || c == '+'){
                    state = 0;
                    degree = 1;
                    tokenReady = true;
                    isMinus = c == '-';
                }
                break;
            case 3:
                if(c == '-'){
                    isMinus = !isMinus;
                } else if(c >= 48 && c <= '9'){
                    degree = c-48;
                    state = 4;
                }
                break;
            case 4:
                if(c >= '0' && c <= '9'){
                    degree = k*10 + c-48;
                } else if(c == '-' || c == '+'){
                    state = 0;
                    if(isMinus)
                        degree = -degree;
                    tokenReady = true;
                    isMinus = c == '-';
                }
                break;
        }

        if(i + 1 == length){
            tokenReady = true;
            switch(state){
                case 1:
                    if (isMinus)
                        k = -k;
                    degree = 0;
                    break;
                case 2:
                    degree = 1;
                    break;
                case 4:
                    if (isMinus)
                        degree = -degree;
            }
        }
        if(tokenReady){
            polynom.increase(degree, k);
        }
    }
    return is;
}