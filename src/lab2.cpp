#include <iostream>
#include "../Polynom.h"

int main(){

    Polynom p;
    p.increase(1, 1);

    std::cout << p << std::endl;
    std::cout << p.calculate(0) << std::endl;
    std::cout << p.calculate(1) << std::endl;
    std::cout << p.calculate(2) << std::endl;
    std::cout << p.calculate(3) << std::endl;

    p = p*p + p*3 -1;
    std::cout << std::endl;

    std::cout << p << std::endl;
    std::cout << p.calculate(0) << std::endl;
    std::cout << p.calculate(1) << std::endl;
    std::cout << p.calculate(2) << std::endl;
    std::cout << p.calculate(3) << std::endl;

    std::cout << "Write some polynoms to check their parsing (without spaces)" << std::endl;

    for(int i = 0; i < 100; i++){
        Polynom p2;
        std::cin >> p2;
        std::cout << p2 << std::endl;
    }

    return 0;
}