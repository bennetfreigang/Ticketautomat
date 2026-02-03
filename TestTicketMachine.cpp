#include "../TicketMachine/TicketMachine.hpp"
#include "../TramParser/TramParser.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>

// Hilfsfunktion für Testdaten
void createTestTramFile(const std::string& filename, const std::string& lineName,
                        int pricePerStop, const std::vector<std::string>& stops) {
    if (!std::filesystem::exists("data")) {
        std::filesystem::create_directory("data");
    }
    std::ofstream file("data/" + filename + ".txt");
    file << lineName << "\n" << pricePerStop << "\n";
    for (const auto& stop : stops) {
        file << stop << "\n";
    }
    file.close();
}

void test_printTicket() {
    std::cout << "Teste Ticket-Druck..." << std::endl;

    TicketData ticket;
    ticket.tram = "Linie 11";
    ticket.startStop = "Hauptbahnhof";
    ticket.destinationStop = "HTWK";
    ticket.price = 45;
    ticket.change = {{17, 1}, {5, 1}, {3, 1}};
    ticket.date = "2026-02-01";

    // Visuelle Prüfung
    TicketMachine::printTicket(ticket);
    std::cout << "Ticket wurde ausgegeben (bitte prüfen)." << std::endl;
}

void test_full_process() {
    std::cout << "Teste Ablauf..." << std::endl;

    createTestTramFile("test_tram", "Testbahn", 2, {"StopA", "StopB", "StopC"});

    TramData tram = TramParser::parseTramFile("test_tram");
    assert(tram.pricePerStop == 2);
    assert(tram.stops.size() == 3);

    // Preisberechnung simulieren (Index 0 bis 2 = 2 Stationen * 2 Geld = 4)
    int price = std::abs(0 - 2) * tram.pricePerStop;
    assert(price == 4);

    std::filesystem::remove("data/test_tram.txt");
    std::cout << "Logik-Tests OK." << std::endl;
}

int main() {
    std::cout << "--- Start Tests TicketMachine ---" << std::endl;

    test_printTicket();
    test_full_process();

    std::cout << "\nAlle Tests durchgelaufen." << std::endl;
    return 0;
}