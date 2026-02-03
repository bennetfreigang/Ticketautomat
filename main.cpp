#include "TicketMachine.hpp"
#include "TUIMenu.hpp"
#include <iostream>

void runTicketMachineCycle() {
    try {
        TicketMachine machine;
        machine.selectTram();
        machine.selectStartStop();
        machine.selectDestinationStop();

        auto ticket = machine.buyTicket();
        TicketMachine::printTicket(ticket);

        std::cout << "\nBeliebige Taste für neuen Kauf..." << std::endl;
        TUIMenu::waitForKey();
    } catch (const std::exception& e) {
        std::cerr << "\nFehler: " << e.what() << std::endl;
        std::cout << "Beliebige Taste zum Neustart..." << std::endl;
        TUIMenu::waitForKey();
    }
}

int main() {
    while (true) {
        runTicketMachineCycle();
    }
    return 0;
}