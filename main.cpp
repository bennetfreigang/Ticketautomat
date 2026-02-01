#include "TicketMachine/TicketMachine.hpp"
#include "TUI/TUIMenu/TUIMenu.hpp"
#include <iostream>

/**
 * @brief Executes one complete cycle of the ticket machine operation.
 * Handles tram selection, ticket purchase, and error reporting.
 */
void runTicketMachineCycle() {
    try {
        TicketMachine machine;
        machine.selectTram();
        machine.selectStartStop();
        machine.selectDestinationStop();

        const TicketData ticket = machine.buyTicket();
        TicketMachine::printTicket(ticket);
        
        std::cout << "\nPress any key to start a new purchase..." << std::endl;
        TUIMenu::waitForKey();
    } catch (const std::exception& e) {
        std::cerr << "\nAn error occurred: " << e.what() << std::endl;
        std::cout << "Press any key to restart..." << std::endl;
        TUIMenu::waitForKey();
    }
}

/**
 * @brief Main entry point of the Ticket Automaton application.
 * Starts the infinite application loop.
 * @return 0.
 */
int main() {
    while (true) {
        runTicketMachineCycle();
    }
    return 0;
}
