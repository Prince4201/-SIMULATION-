#include "atmverse/core/BankSystem.h"
#include "atmverse/core/ATMController.h"
#include <iostream>

using namespace atmverse;

int main() {
    try {
        // Initialize the core banking system
        BankSystem bankSystem;
        bankSystem.initialize();

        // Start the ATM interface controller
        ATMController atm(&bankSystem);
        atm.run();

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
