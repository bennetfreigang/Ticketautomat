#pragma once
#include <map>
#include <functional>

class Payment {
public:
    Payment();
    std::map<int, int> payOutChange(const int& amount);
    static int calculateChange(const int& ticketPrice, const int& insertedAmount);
    void reset();

private:
    std::map<int, int, std::greater<>> changeBox;

    std::map<int, int> takeFromChangeBox(int& remainingAmount);
    static void validateRemainingAmount(const int& remainingAmount);
    void updateChangeBox(const std::map<int, int>& payOut);
    void setChangeBox();
};