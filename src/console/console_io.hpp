#pragma once

#include <cstdint>
#include <cstddef>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "core/ordinal.hpp"

// small console helpers; all functions are inline because this file is header-only

inline void clear_console_error_state() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

inline std::string read_line(const std::string& prompt) {
    std::cout << prompt;

    std::string line;
    std::getline(std::cin, line);

    while (!std::cin.good()) {
        clear_console_error_state();
        std::cout << prompt;
        std::getline(std::cin, line);
    }

    return line;
}

inline int read_int(const std::string& prompt) {
    while (true) {
        std::string line = read_line(prompt);
        std::stringstream stream(line);

        int value = 0;
        char extra = '\0';

        if ((stream >> value) && !(stream >> extra)) {
            return value;
        }

        std::cout << "invalid integer input\n";
    }
}

inline int read_non_negative_int(const std::string& prompt) {
    while (true) {
        int value = read_int(prompt);

        if (value >= 0) {
            return value;
        }

        std::cout << "value must be non-negative\n";
    }
}

inline std::size_t read_size_t(const std::string& prompt) {
    while (true) {
        std::string line = read_line(prompt);
        std::stringstream stream(line);

        std::size_t value = 0;
        char extra = '\0';

        if ((stream >> value) && !(stream >> extra)) {
            return value;
        }

        std::cout << "invalid size_t input\n";
    }
}

inline std::uint32_t read_uint32(const std::string& prompt) {
    while (true) {
        std::string line = read_line(prompt);
        std::stringstream stream(line);

        unsigned long value = 0;
        char extra = '\0';

        if ((stream >> value) && !(stream >> extra) && value <= 4294967295UL) {
            return static_cast<std::uint32_t>(value);
        }

        std::cout << "invalid uint32 input\n";
    }
}

inline double read_double(const std::string& prompt) {
    while (true) {
        std::string line = read_line(prompt);
        std::stringstream stream(line);

        double value = 0.0;
        char extra = '\0';

        if ((stream >> value) && !(stream >> extra)) {
            return value;
        }

        std::cout << "invalid double input\n";
    }
}

inline int read_menu_choice(const std::string& prompt, int min_value, int max_value) {
    while (true) {
        int value = read_int(prompt);

        if (value >= min_value && value <= max_value) {
            return value;
        }

        std::cout << "choice must be in range [" << min_value << ", " << max_value << "]\n";
    }
}

inline std::string ordinal_to_string(const Ordinal& ordinal) {
    std::ostringstream out;

    const std::size_t omega_coeff = ordinal.get_omega_coeff();
    const std::size_t finite_part = ordinal.get_finite_part();

    if (omega_coeff == 0) {
        out << finite_part;
        return out.str();
    }

    if (omega_coeff == 1) {
        out << "omega";
    } else {
        out << "omega * " << omega_coeff;
    }

    if (finite_part > 0) {
        out << " + " << finite_part;
    }

    return out.str();
}

inline std::ostream& operator<<(std::ostream& out, const Ordinal& ordinal) {
    out << ordinal_to_string(ordinal);
    return out;
}

inline Ordinal read_ordinal(const std::string& label) {
    std::cout << label << "\n";
    std::cout << "1. finite n\n";
    std::cout << "2. omega\n";
    std::cout << "3. omega * k + n\n";

    int choice = read_menu_choice("choose ordinal type: ", 1, 3);

    if (choice == 1) {
        std::size_t finite = read_size_t("finite part: ");
        return Ordinal(finite);
    }

    if (choice == 2) {
        return Ordinal::omega();
    }

    std::size_t omega_coeff = read_size_t("omega coefficient: ");
    std::size_t finite_part = read_size_t("finite part: ");

    return Ordinal(omega_coeff, finite_part);
}

inline Ordinal read_ordinal_index(const std::string& label) {
    return read_ordinal(label);
}

inline Ordinal read_generator_length(const std::string& label) {
    std::cout << "\n" << label << "\n";
    std::cout << "1. finite n\n";
    std::cout << "2. omega\n";

    int choice = read_menu_choice("choose length type: ", 1, 2);

    if (choice == 1) {
        std::size_t finite = read_size_t("finite length: ");
        return Ordinal(finite);
    }

    return Ordinal::omega();
}

inline void wait_enter() {
    read_line("press enter to continue...");
}

inline void print_exception(const std::exception& error) {
    std::cout << "error: " << error.what() << "\n";
}
