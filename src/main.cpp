#include <exception>
#include <iostream>

#include "ui/console_ui.hpp"

int main() {
    try {
        ConsoleUI ui;
        ui.run();
    } catch (const std::exception& error) {
        std::cerr << "fatal error: " << error.what() << "\n";
        return 1;
    }

    return 0;
}
