#pragma once 


class emfRegisterPair {
private:
    emfNumber<uint8_t>& high;
    emfNumber<uint8_t>& low;

public:
    emfRegisterPair(emfNumber<uint8_t>& h, emfNumber<uint8_t>& l)
        : high(h), low(l) {}


    // access individual bytes
    emfNumber<uint8_t>& hi() { return high; }
    emfNumber<uint8_t>& lo() { return low; }

    // read combined 16-bit value
    uint16_t get() const {
        return (static_cast<uint16_t>(static_cast<uint8_t>(high.getUnsigned())) << 8) |
               static_cast<uint8_t>(low.getUnsigned());
    }

    uint16_t getUnsigned() const {
        return get();
    }

    // set combined 16-bit value
    void set(uint16_t v) {
        high=(static_cast<uint8_t>((v >> 8) & 0xFF));
        low=(static_cast<uint8_t>(v & 0xFF));
    }

    void assign(uint16_t v) {
        high=(static_cast<uint8_t>((v >> 8) & 0xFF));
        low=(static_cast<uint8_t>(v & 0xFF));
    }

    emfRegisterPair& operator=(const emfRegisterPair& other) {
        set(other.get());  // copy the combined 16-bit value
        return *this;
    }

    emfRegisterPair& operator=(uint16_t v) {
        set(v);  // split into high and low bytes
        return *this;
    }

    // arithmetic operators
    emfRegisterPair operator+(const emfRegisterPair& other) const {
        emfRegisterPair result = *this;
        result.set(this->get() + other.get());
        return result;
    }

    emfRegisterPair operator-(const emfRegisterPair& other) const {
        emfRegisterPair result = *this;
        result.set(this->get() - other.get());
        return result;
    }

    emfRegisterPair& operator+=(const emfRegisterPair& other) {
        set(get() + other.get());
        return *this;
    }

    emfRegisterPair& operator-=(const emfRegisterPair& other) {
        set(get() - other.get());
        return *this;
    }

    // implicit conversion
    operator uint16_t() const {
        return get();
    }

    // Equality operators
    bool operator==(const emfRegisterPair& other) const {
        return get() == other.get();
    }

    bool operator!=(const emfRegisterPair& other) const {
        return get() != other.get();
    }

    // Optional: compare to raw 16-bit value
    bool operator==(uint16_t v) const { return get() == v; }
    bool operator!=(uint16_t v) const { return get() != v; }

    bool equals(const uint16_t& v) const {
        return getUnsigned() == v;
    }

    bool notEquals(const uint16_t& v) const {
        return getUnsigned() != v;
    }

    bool equals(const emfRegisterPair& other) const {
        return getUnsigned() == other.getUnsigned();
    }

    bool notEquals(const emfRegisterPair& other) const {
        return getUnsigned() != other.getUnsigned();
    }

};

