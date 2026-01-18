#ifndef HEADER_H
#define HEADER_H

#include <bitset>
#include <string>
#include <cstdint>

const int M = 191; 

class GF2_191 {
private:
    std::bitset<M> v; 
    static std::bitset<M> reduce(const std::bitset<2 * M>&);

public:
    GF2_191();
    GF2_191(const std::bitset<M>& v);
    GF2_191(const std::string& s);
    GF2_191(unsigned long long v);

    static GF2_191 zero();
    static GF2_191 one();

    GF2_191 operator+(const GF2_191& other) const;
    GF2_191& operator+=(const GF2_191& other);
    GF2_191 operator*(const GF2_191& other) const;
    bool operator==(const GF2_191& other) const;
    bool operator!=(const GF2_191& other) const;

    GF2_191 square() const;
    GF2_191 inverse() const;
    GF2_191 power(const std::string& exp_str) const;
    std::string to_string() const;
    static GF2_191 from_string(const std::string& s);

    bool is_zero() const;
    bool is_one() const;

    void print() const;
    bool trace() const;

    static GF2_191 random();

    friend std::ostream& operator<<(std::ostream& os, const GF2_191& obj) {
        os << obj.to_string(); 
        return os;
    }
};

void timer();
void ticks();

#endif