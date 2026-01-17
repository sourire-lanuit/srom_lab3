#include "Header.h"
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <vector>

GF2_191::GF2_191() : value(0) {}
GF2_191::GF2_191(const std::bitset<M>& v) : value(v) {}
GF2_191::GF2_191(const std::string& s) : value(s) {}
GF2_191::GF2_191(unsigned long long v) : value(v) {}

GF2_191 GF2_191::zero() {
    return GF2_191();
}

GF2_191 GF2_191::one() {
    return GF2_191(1);
}

GF2_191 GF2_191::operator+(const GF2_191& other) const {
    return GF2_191(value ^ other.value);
}

GF2_191& GF2_191::operator+=(const GF2_191& other) {
    value ^= other.value;
    return *this;
}

GF2_191 GF2_191::operator*(const GF2_191& other) const {
    std::bitset<2*M> product(0);
    for (int i = 0; i < M; i++) {
        if (other.value[i]) {
            for (int j = 0; j < M; j++) {
                if (value[j]) {
                    product[i + j] = product[i + j] ^ 1;
                }
            }
        }
    }
    
    for (int i = 2*M - 2; i >= M; i--) {
        if (product[i]) {
            product[i] = 0;
            product[i - M] = product[i - M] ^ 1;     
            product[i - M + 9] = product[i - M + 9] ^ 1;  
        }
    }
    
    std::bitset<M> result;
    for (int i = 0; i < M; i++) {
        result[i] = product[i];
    }
    
    return GF2_191(result);
}

GF2_191 GF2_191::square() const {
    std::bitset<2*M> squared(0);
    
    for (int i = 0; i < M; i++) {
        if (value[i]) {
            squared[2*i] = 1;
        }
    }

    for (int i = 2*M - 2; i >= M; i--) {
        if (squared[i]) {
            squared[i] = 0;
            squared[i - M] = squared[i - M] ^ 1;
            squared[i - M + 9] = squared[i - M + 9] ^ 1;
        }
    }
    
    std::bitset<M> result;
    for (int i = 0; i < M; i++) {
        result[i] = squared[i];
    }
    
    return GF2_191(result);
}

GF2_191 GF2_191::power(const std::bitset<M>& exp) const {
    if (is_zero()) return zero();
    
    GF2_191 result = one();
    GF2_191 base = *this;
    
    for (int i = 0; i < M; i++) {
        if (exp[i]) {
            result = result * base;
        }
        base = base.square();
    }
    
    return result;
}

GF2_191 GF2_191::inverse() const {
    if (is_zero()) {
        throw std::runtime_error("Cannot invert zero element");
    }
    
    GF2_191 result = *this;
    
    for (int i = 1; i < M; i++) {
        result = result.square();
        if (i < M - 1) {
            result = result * (*this);
        }
    }
    
    return result;
}

std::string GF2_191::to_string() const {
    std::string s;
    for (int i = M - 1; i >= 0; i--) {
        s += (value[i] ? '1' : '0');
    }
    return s;
}

GF2_191 GF2_191::from_string(const std::string& s) {
    if (s.length() > M) {
        throw std::invalid_argument("String too long for field element");
    }
    
    std::string padded = s;
    while (padded.length() < M) {
        padded = "0" + padded;
    }
    
    std::bitset<M> bits;
    for (int i = 0; i < M; i++) {
        bits[i] = (padded[M - 1 - i] == '1');
    }
    
    return GF2_191(bits);
}

bool GF2_191::is_zero() const {
    return value.none();
}

bool GF2_191::is_one() const {
    return value.count() == 1 && value[0] == 1;
}

bool GF2_191::operator==(const GF2_191& other) const {
    return value == other.value;
}

bool GF2_191::operator!=(const GF2_191& other) const {
    return value != other.value;
}

void GF2_191::print() const {
    std::cout << to_string() << std::endl;
}

GF2_191 GF2_191::random() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned long long> dis;
    
    std::bitset<M> bits;
    for (int i = 0; i < M; i++) {
        bits[i] = dis(gen) & 1;
    }
    
    return GF2_191(bits);
}

void test() {
    std::cout << "\n=== TESTS===\n";

    GF2_191 a = GF2_191::random();
    GF2_191 b = GF2_191::random();
    GF2_191 c = GF2_191::random();
    GF2_191 d = GF2_191::random();
    while (d.is_zero()) d = GF2_191::random();
    
    std::cout << "Test elements generated\n";
    std::cout << "a = " << a.to_string().substr(0, 13) << "...\n";
    std::cout << "b =  " << b.to_string().substr(0, 13) << "...\n";
    std::cout << "c = " << c.to_string().substr(0, 13) << "...\n";
    std::cout << "d = " << d.to_string().substr(0, 13) << "...\n";
    
    GF2_191 zero = GF2_191::zero();
    GF2_191 zero_res = a + zero;
    std::cout << "a + 0 = " << zero_res.to_string().substr(0, 13) << std::endl;
    
    GF2_191 one = GF2_191::one();
    GF2_191 one_res = a * one;
    std::cout << "a * 1 = " << one_res.to_string().substr(0, 13) << std::endl;

    GF2_191 aadd = a + b;
    GF2_191 badd = b + a;
    std::cout << "a + b = " << aadd.to_string().substr(0, 13) << std::endl;
    std::cout << "b + a = " << badd.to_string().substr(0, 13) << std::endl;

    GF2_191 addc = (a + b) + c;
    GF2_191 aaddc = a + (b + c);
    std::cout << "(a + b) + c = " << addc.to_string().substr(0, 13) << std::endl;
    std::cout << "a + (b + c) = " << aaddc.to_string().substr(0, 13) << std::endl;

    GF2_191 mulc = (a + b) * c;
    GF2_191 cmulc = (a * c) + (b * c);
    std::cout << "(a + b) * c = " << mulc.to_string().substr(0, 13) << std::endl;
    std::cout << "a*c + b*c = " << cmulc.to_string().substr(0, 13) << std::endl;
    
    GF2_191 d_inv = d.inverse();
    GF2_191 inv = d * d_inv;
    std::cout << "d*d^(-1) = " << inv.to_string().substr(0, 13) << std::endl;
    
    std::bitset<M> exp_max;
    exp_max.set(); 
    GF2_191 d_power = d.power(exp_max);
    GF2_191 pow = d.power(exp_max);
    std::cout << "d^(2^m-1) = " << pow.to_string().substr(0, 13) << std::endl;
    
    GF2_191 aa_mult = a * a;
    GF2_191 a_squared = a.square();
    std::cout << "a * a = " << aa_mult.to_string().substr(0, 13) << std::endl;
    std::cout << "a^2 = " << a_squared.to_string().substr(0, 13) << std::endl;
    
    std::string a_str = a.to_string();
    GF2_191 str = GF2_191::from_string(a_str);
    std::cout << "Original: " << a.to_string().substr(0, 13) << std::endl;
    std::cout << "After convert : " << str.to_string().substr(0, 13) << std::endl;

}

void timer() {
    std::cout << "\n=== TIME ===\n";
    
    const int ITERATIONS = 1000;
    
    std::vector<GF2_191> elements;
    for (int i = 0; i < 100; i++) {
        elements.push_back(GF2_191::random());
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        GF2_191 result = elements[i % 100] + elements[(i + 1) % 100];
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_add = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / ITERATIONS;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        GF2_191 result = elements[i % 100] * elements[(i + 1) % 100];
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration_mul = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / ITERATIONS;
    

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        GF2_191 result = elements[i % 100].square();
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration_sqr = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / ITERATIONS;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; i++) {
        GF2_191 result = elements[i].inverse();
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration_inv = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 100;
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Operation          | Avg Time (ns) | Ticks\n";
    std::cout << "-------------------+---------------+--------------- \n" ;
    std::cout << "Addition           | " << std::setw(13) << duration_add << " | " << std::setw(13) << duration_add/1000.0 << std::endl;
    std::cout << "Multiplication     | " << std::setw(13) << duration_mul << " | " << std::setw(13) << duration_mul/1000.0 << std::endl;
    std::cout << "Squaring           | " << std::setw(13) << duration_sqr << " | " << std::setw(13) << duration_sqr/1000.0 << std::endl;
    std::cout << "Inverse            | " << std::setw(13) << duration_inv << " | " << std::setw(13) << duration_inv/1000.0 << std::endl;
    std::cout << std::endl;
}