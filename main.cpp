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

        std::cout << "\nPress any key to restart..." << std::endl;
        TUIMenu::waitForKey();
    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << std::endl;
        std::cout << "Press any key to restart..." << std::endl;
        TUIMenu::waitForKey();
    }
}

int main() {
    while (true) {
        runTicketMachineCycle();
    }
    return 0;
}