#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include "lab4algo.h"

bool is_positive(int x){
    return x > 0;
}

bool not_negative(int x){
    return x >= 0;
}

bool is_even(int x){
    return (x&1) == 0;
}

class CPoint {

public:
    CPoint(double x, double y) : x(x), y(y) {

    }

    bool operator !=(CPoint const& other) const {
        return x != other.x || y != other.y;
    }

private:
    double x, y;
};

int main(){
    std::vector<int> pool;
    std::list<int> list_pool;

    for(int i = 0; i < 5; i++) {
        if(i == 1)
            continue;
        pool.push_back(i);
        list_pool.push_back(i);
    }

    std::cout << "Are all in vector positive: " << lab4::all_of(pool.begin(), pool.end(), is_positive)
        << ", not negative: " << lab4::all_of(pool.begin(), pool.end(), not_negative) << std::endl;

    std::cout << "Are all in list positive: " << lab4::all_of(list_pool.begin(), list_pool.end(), is_positive)
         << ", not negative: " << lab4::all_of(list_pool.begin(), list_pool.end(), not_negative) << std::endl;

    std::cout << "Is vector sorted: " << lab4::is_sorted(pool.begin(), pool.end(), std::less<>()) << std::endl;
    std::cout << "Is vector sorted: " << lab4::is_sorted(pool.begin(), pool.end(), std::greater<>()) << std::endl;

    pool.push_back(1);

    std::cout << "Is vector sorted: " << lab4::is_sorted(pool.begin(), pool.end(), std::less<>()) << std::endl;
    std::cout << "Is vector sorted: " << lab4::is_sorted(pool.begin(), pool.end(), std::greater<>()) << std::endl;

    std::cout << "Is vector partitioned in positives and not: " << lab4::is_partitioned(pool.begin(), pool.end(), is_positive) << std::endl;
    std::cout << "Is vector partitioned in even and not: " << lab4::is_partitioned(pool.begin(), pool.end(), is_even) << std::endl;

    std::cout << "First number not equal to 0 in vector: " << *lab4::find_not(pool.begin(), pool.end(), 0) << std::endl;
    std::cout << "Number before last 1 in vector: " << *(lab4::find_backward(pool.begin(), pool.end(), 1)-1) << std::endl;

    std::vector<int> palindromeVector{1, 2, 3, 2, 1};
    std::cout << "Is 12321 palindrome: " << lab4::is_palindrome(palindromeVector.begin(), palindromeVector.end()) << std::endl;
    std::vector<int> palindromeVector2{1, 2, 2, 1};
    std::cout << "Is 12321 palindrome: " << lab4::is_palindrome(palindromeVector2.begin(), palindromeVector2.end()) << std::endl;
    std::cout << "Is 02341 palindrome: " << lab4::is_palindrome(list_pool.begin(), list_pool.end()) << std::endl;

    std::vector<CPoint> pointsPalindrome{{1,1}, {1,2},{0,0}, {1, 2}};
    std::cout << "Is (1, 1),(1, 2), (0, 0), (1, 2) palindrome: " << lab4::is_palindrome(pointsPalindrome.begin(), pointsPalindrome.end()) << std::endl;
    pointsPalindrome.emplace_back(1,1);
    std::cout << "Is (1, 1),(1, 2), (0, 0), (1, 2), (1, 1) palindrome: " << lab4::is_palindrome(pointsPalindrome.begin(), pointsPalindrome.end()) << std::endl;
}