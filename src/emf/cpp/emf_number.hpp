#pragma once 


template<typename T>
class emfNumber {
protected:
    T value;

public:
    emfNumber(T v = 0) : value(v) {}

    // Trad EMF

    T getUnsigned() const {
        return value;
    }

    void assign(T v) {
        value = v;
    }
    // add raw value
    void add(T v) {
        value += v;
    }

    // subtract raw value
    void sub(T v) {
        value -= v;
    }
    
    T abs() {
        value = value>0 ? value : -value;
        return value;
    }

    // Vibed, std, for native type & class type crossover

    // assignment from raw value
    emfNumber& operator=(T v) {
        value = v;
        return *this;
    }

    // addition
    emfNumber operator+(const emfNumber& other) const {
        return emfNumber(value + other.value);
    }

    // subtraction
    emfNumber operator-(const emfNumber& other) const {
        return emfNumber(value - other.value);
    }

    // multiplication
    emfNumber operator*(const emfNumber& other) const {
        return emfNumber(value * other.value);
    }

    // division
    emfNumber operator/(const emfNumber& other) const {
        return emfNumber(value / other.value);
    }

    // compound operators
    emfNumber& operator+=(const emfNumber& other) {
        value += other.value;
        return *this;
    }

    emfNumber& operator-=(const emfNumber& other) {
        value -= other.value;
        return *this;
    }

    // implicit conversion
    operator T() const {
        return value;
    }


    // Equality
    bool operator==(const emfNumber& other) const {
        return getUnsigned() == other.getUnsigned();
    }

    bool operator!=(const emfNumber& other) const {
        return getUnsigned() != other.getUnsigned();
    }
    
    bool equals(const emfNumber& other) const {
        return getUnsigned() == other.getUnsigned();
    }

    bool notEquals(const emfNumber& other) const {
        return getUnsigned() != other.getUnsigned();
    }

    // Optional: compare to raw 16-bit value
    bool operator==(uint16_t v) const { return getUnsigned() == v; }
    bool operator!=(uint16_t v) const { return getUnsigned() != v; }
};

