#include "Header.h"
#include <iostream>

int main() {
     std::cout << "\n========================================\n";
    std::cout << "p(x) = x^191 + x^9 + 1\n";
    std::cout << "========================================\n\n";
    
    test();
    timer();

    GF2_191 zero = GF2_191::zero();
    GF2_191 one = GF2_191::one();
    
    std::cout << "\nZero element =" << zero.to_string() << std::endl;
    std::cout << "One element = " << one.to_string() << std::endl;
    
    GF2_191 x = GF2_191::from_string("10"); 
    std::cout << "Element x = " << x.to_string() << std::endl;
    
    GF2_191 x_squared = x.square();
    std::cout << "x^2 =" << x_squared.to_string() << std::endl;
    
    GF2_191 x_cubed = x * x * x;
    std::cout << "x^3 = " << x_cubed.to_string() << std::endl;
    
    return 0;
}