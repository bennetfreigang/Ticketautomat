#pragma once
#include <string>
#include "../TramParser/TramParser.hpp"
#include "../Payment/Payment.hpp"
#include <map>

struct TicketData {
    std::string tram;
    std::string startStop;
    std::string destinationStop;
    int price;
    std::map<int, int> change;
    std::string date;
};

class TicketMachine {
public:
    TicketMachine()
        : selectedStartIndex(0), selectedDestinationIndex(0) {
        currentTram.pricePerStop = 0;
        payment = Payment();
    }

    void selectTram();
    void selectStartStop();
    void selectDestinationStop();
    TicketData buyTicket();
    static void printTicket(const TicketData& ticket);

private:
    TramData currentTram;
    Payment payment;
    size_t selectedStartIndex;
    size_t selectedDestinationIndex;

    static std::vector<std::string> getFileNames(const std::string& folderPath);
    [[nodiscard]] int calculatePrice() const;
    [[nodiscard]] std::string stopAtIndex(size_t index) const;
    static std::string getCurrentDate();
    static int processPayment(const TicketData& ticket);
    static int calculateChangeSum(const std::map<int, int>& change);
};