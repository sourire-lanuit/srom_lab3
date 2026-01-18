#include "Header.h"
#include <random>
#include <iostream>
#include <chrono>
#include <stdexcept>

GF2_191::GF2_191() : v(0) {}
GF2_191::GF2_191(const std::bitset<M>& b) : v(b) {}
GF2_191::GF2_191(unsigned long long x) : v(x) {}

GF2_191::GF2_191(const std::string& s) {
    if (s.size() > M) throw std::invalid_argument("String too long for GF2_191");
    v.reset();
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '1') v[M - 1 - i] = 1; 
        else if (s[i] != '0')  throw std::invalid_argument("Invalid character in string");
    }
}

GF2_191 GF2_191::from_string(const std::string& s) {
    std::bitset<M> b;
    b.reset();
    for (size_t i = 0; i < s.size() && i < M; i++) {
        if (s[s.size() - 1 - i] == '1')
            b[i] = 1;
    }
    return GF2_191(b);
}

std::string GF2_191::to_string() const {
    std::string s;
    for (int i = M - 1; i >= 0; i--) 
        s += v[i] ? '1' : '0';
    return s;
}

GF2_191 GF2_191::zero() { 
    return GF2_191(); 
}

GF2_191 GF2_191::one() { 
    return GF2_191(1); 
}

bool GF2_191::is_zero() const { 
    return v.none(); 
}

bool GF2_191::is_one() const { 
    return v[0] && v.count() == 1; 
}

GF2_191 GF2_191::operator+(const GF2_191& b) const {
    return GF2_191(v ^ b.v);
}

GF2_191& GF2_191::operator+=(const GF2_191& other) {
    v ^= other.v;
    return *this;
}

bool GF2_191::operator==(const GF2_191& other) const {
    return v == other.v;
}

bool GF2_191::operator!=(const GF2_191& other) const {
    return v != other.v;
}

std::bitset<M> GF2_191::reduce(const std::bitset<2 * M>& p) {
    std::bitset<2 * M> r = p;
    for (int i = 2 * M - 2; i >= M; i--) {
        if (r[i]) {
            r[i] = 0;
            r[i - M] = r[i - M] ^ 1;     
            r[i - M + 9] = r[i - M + 9] ^ 1;
        }
    }

    std::bitset<M> out;
    for (int i = 0; i < M; i++) out[i] = r[i];
    return out;
}

GF2_191 GF2_191::operator*(const GF2_191& b) const {
    std::bitset<2 * M> p;
    p.reset();
    for (int i = 0; i < M; i++) {
        if (b.v[i]) {
            for (int j = 0; j < M; j++) {
                if (v[j]) {
                    p[i + j] = p[i + j] ^ 1;
                }
            }
        }
    }
    
    return GF2_191(reduce(p));
}

GF2_191 GF2_191::square() const {
    std::bitset<2 * M> p;
    p.reset();
    for (int i = 0; i < M; i++) {
        if (v[i])  p[2 * i] = 1;
    } 
    return GF2_191(reduce(p));
}

GF2_191 GF2_191::power(const std::string& exp_str) const {
    if (is_zero()) {
        bool all_zero = true;
        for (char c : exp_str) {
            if (c != '0') {
                all_zero = false;
                break;
            }
        }
        if (all_zero) throw std::runtime_error("0^0 is undefined");
        return zero();
    }

    GF2_191 r = one();
    GF2_191 b = *this;

    for (int i = exp_str.length() - 1; i >= 0; i--) {
        if (exp_str[i] == '1') r = r * b;
        if (i > 0) b = b.square();
    }
    
    return r;
}

GF2_191 GF2_191::inverse() const {
    if (is_zero()) throw std::runtime_error("Inverse of zero");
    GF2_191 r = *this;
    for (int i = 1; i < M; i++) {
        r = r.square();
        if (i < M - 1) r = r * (*this);
    } 
    return r;
}

bool GF2_191::trace() const {
    GF2_191 t = *this;
    GF2_191 acc = *this;
    for (int i = 1; i < M; i++) {
        t = t.square();
        acc = acc + t;
    }
    return acc.v[0];
}

GF2_191 GF2_191::random() {
    static std::mt19937_64 gen(std::random_device{}());
    std::bitset<M> b;
    for (int i = 0; i < M; i++) b[i] = gen() & 1;
    return GF2_191(b);
}

void GF2_191::print() const {
    std::cout << to_string() << std::endl;
}

void timer() {
    std::cout << "\n=== TIME ===\n";
    
    const int N = 1000;
    GF2_191 a = GF2_191::random();
    GF2_191 b = GF2_191::random();

    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) { GF2_191 temp = a + b; }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Add: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / N << " ns\n";

    t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) { GF2_191 temp = a * b; }
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Mul: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / N << " ns\n";

    t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) { GF2_191 temp = a.square(); }
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Sq: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / N << " ns\n";

    t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; i++) { GF2_191 temp = a.inverse(); }
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Inv: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 100 << " ns\n";
    
    std::cout << std::endl;
}

inline uint64_t rdtsc() {
    unsigned int lo, hi;
    #ifdef _MSC_VER
        __asm {
            rdtsc
            mov lo, eax
            mov hi, edx
        }
    #else
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    #endif
    return ((uint64_t)hi << 32) | lo;
}

void ticks() {
    const int N = 1000;
    GF2_191 a = GF2_191::random();
    GF2_191 b = GF2_191::random();
    for (int i = 0; i < 100; i++) {
        GF2_191 temp = a + b;
        temp = a * b;
    }

    uint64_t t1 = rdtsc();
    for (int i = 0; i < N; i++) {
        GF2_191 temp = a + b;
    }
    uint64_t t2 = rdtsc();
    uint64_t add_cycles = (t2 - t1) / N;
    
    t1 = rdtsc();
    for (int i = 0; i < N; i++) {
        GF2_191 temp = a * b;
    }
    t2 = rdtsc();
    uint64_t mul_cycles = (t2 - t1) / N;

    t1 = rdtsc();
    for (int i = 0; i < N; i++) {
        GF2_191 temp = a.square();
    }
    t2 = rdtsc();
    uint64_t sqr_cycles = (t2 - t1) / N;
    
    t1 = rdtsc();
    for (int i = 0; i < 100; i++) {
        GF2_191 temp = a.inverse();
    }
    t2 = rdtsc();
    uint64_t inv_cycles = (t2 - t1) / 100;
    
    std::cout << "\n=== PROCESSOR TICKS ===\n";
    std::cout << "Add: " << add_cycles << "\n";
    std::cout << "Mul: " << mul_cycles << "\n";
    std::cout << "Sq: " << sqr_cycles << "\n";
    std::cout << "Inv: " << inv_cycles << "\n";

    std::cout << std::endl;
}