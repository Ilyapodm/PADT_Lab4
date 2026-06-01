#pragma once

#include <stdexcept>
#include <cstddef>

//  omega * omega_coeff_ + finite_part_
class Ordinal {
public:
    Ordinal() :  omega_coeff_(0), finite_part_(0) {} 

    explicit Ordinal(std::size_t finite_part) 
        :  omega_coeff_(0), finite_part_(finite_part) {}
    
    Ordinal(std::size_t omega_coeff_, std::size_t finite_part) 
        : omega_coeff_(omega_coeff_), finite_part_(finite_part) {}  

    static Ordinal omega() {
        return {1, 0};
    }

    bool is_finite() const {
        return omega_coeff_ == 0;
    }

    bool has_end() const {
        return !(omega_coeff_ > 0 && finite_part_ == 0);
    }

    std::size_t get_omega_coeff() const {
        return omega_coeff_;
    }

    std::size_t get_finite_part() const {
        return finite_part_;
    }

    bool operator==(const Ordinal& other) const {
        return omega_coeff_ == other.omega_coeff_ && finite_part_ == other.finite_part_;
    }

    bool operator!=(const Ordinal& other) const {
        return !(*this == other);
    }

    bool operator<(const Ordinal& other) const {
        if (omega_coeff_ < other.omega_coeff_) 
            return true;
        if (omega_coeff_ > other.omega_coeff_) 
            return false;
        
        return finite_part_ < other.finite_part_;
    }

    bool operator<=(const Ordinal& other) const {
        if (omega_coeff_ < other.omega_coeff_) 
            return true;
        if (omega_coeff_ > other.omega_coeff_) 
            return false;
        
        return finite_part_ <= other.finite_part_;
    }

    bool operator>(const Ordinal& other) const {
        return !(*this <= other);
    }

    bool operator>=(const Ordinal& other) const {
        return !(*this < other);
    }

    Ordinal operator+(const Ordinal& other) const {
        // omega * c1 + f1 + omega * c2 + f2 = omega * (c1 + c2) + f2
        if (other.omega_coeff_ > 0) {
            return {omega_coeff_ + other.omega_coeff_, other.finite_part_};
        }
        return {omega_coeff_, finite_part_ + other.finite_part_};
    }

    Ordinal subtract_prefix(const Ordinal& prefix) const {
        if (prefix.omega_coeff_ > omega_coeff_) {
            throw std::invalid_argument("Can't subtract prefix, argument is too big");
        }

        if (prefix.omega_coeff_ == omega_coeff_) {
            if (prefix.finite_part_ > finite_part_) {
                throw std::invalid_argument("Can't subtract prefix, argument is too big");
            }

            return {0, finite_part_ - prefix.finite_part_};
        }

        return {omega_coeff_ - prefix.omega_coeff_, finite_part_};
    }

private:
    std::size_t omega_coeff_;
    std::size_t finite_part_;
};