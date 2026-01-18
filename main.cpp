#include "Header.h"
#include <iostream>
#include <string>
#include <cstdint>

int main() {
    std::cout << "\n========================================\n";
    std::cout << "p(x) = x^191 + x^9 + 1\n";
    std::cout << "========================================\n\n";

    std::string A_str, B_str, N_str;
    std::cout << "\nEnter A: ";
    std::cin >> A_str;
    std::cout << "Enter B: ";
    std::cin >> B_str;
    std::cout << "Enter N: ";
    std::cin >> N_str;

    GF2_191 A = GF2_191::from_string(A_str);
    GF2_191 B = GF2_191::from_string(B_str);

    GF2_191 sum = A + B;
    GF2_191 mul = A * B;
    GF2_191 sqr = A.square();
    GF2_191 inv = A.inverse();
    GF2_191 powN = A.power(N_str);

    std::cout << "A + B = " << sum << std::endl;
    std::cout << "A * B = " << mul << std::endl;
    std::cout << "A^2 = " << sqr << std::endl;
    std::cout << "A^-1 = " << inv << std::endl;
    std::cout << "A^N = " << powN << std::endl;

    if (!A.is_zero()) {
            GF2_191 inv = A.inverse();
            std::cout << "A^-1 = " << inv << std::endl;
            GF2_191 check = A * inv;
            std::cout << "A*A^-1 = " << check;
        } else {
            std::cout << "A^-1 undefined\n";
        }

    timer();
    ticks();

    return 0;
}
