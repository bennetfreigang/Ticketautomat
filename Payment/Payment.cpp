#include "Payment.hpp"

/**
 * @brief Default constructor.
 * Initializes the internal change box with default coin/bill counts.
 */
Payment::Payment() {
    setChangeBox();
}

/**
 * @brief Main method to pay out change.
 * Delegates to sub-functions for selecting coins, validating, and updating the storage.
 * @param amount The total amount to dispense as change.
 * @return A map of coin/bill values to the count of each dispensed.
 */
std::map<int, int> Payment::payOutChange(const int& amount) {
    int remainingAmount = amount;

    // Step 1: Determine which coins/bills to give as change
    // This calculates the optimal set of coins/bills without modifying the actual stock yet
    std::map<int, int> payOut = takeFromChangeBox(remainingAmount);

    // Step 2: Validate if the change could be given completely
    // If remainingAmount is not 0, it means we didn't have enough small change
    validateRemainingAmount(remainingAmount);

    // Step 3: Update the changeBox to subtract the coins/bills given
    // Commits the transaction by removing the calculated items from inventory
    updateChangeBox(payOut);

    return payOut;
}

/**
 * @brief Calculates the difference between ticket price and inserted amount.
 * @param ticketPrice Price of the ticket.
 * @param insertedAmount Amount inserted by the user.
 * @return The remaining amount to pay (positive) or change to return (negative logic depending on usage).
 * @note In the current implementation, this returns (Price - Inserted). 
 *       If positive, more money is needed. If negative, change is due.
 */
int Payment::calculateChange(const int &ticketPrice, const int &insertedAmount) {
    // Simply subtract inserted amount from the price
    return ticketPrice - insertedAmount;
}

/**
 * @brief Resets the internal change box to default coin/bill counts.
 */
void Payment::reset() {
    setChangeBox();
}

/**
 * @brief Selects coins/bills from changeBox to satisfy the amount.
 * @param remainingAmount The amount that needs to be paid out as change.
 * @return Map of coin/bill values to counts to give as change.
 */
std::map<int, int> Payment::takeFromChangeBox(int& remainingAmount) {
    std::map<int, int> payOut;

    // Iterate over all coin/bill denominations, starting from largest (due to std::greater in map definition)
    for (const auto& [value, available] : changeBox) {
        int takenNotes = 0;

        // Take as many coins/bills of the current denomination as possible
        // Condition 1: We haven't exhausted the stock of this denomination (takenNotes < available)
        // Condition 2: The value fits into the remaining amount (remainingAmount >= value)
        while (takenNotes < available && remainingAmount >= value) {
            remainingAmount -= value;
            takenNotes++;
        }

        // If we took any notes/coins of this value, record it in the payout map
        if (takenNotes > 0) {
            payOut[value] = takenNotes;
        }

        // Optimization: Stop processing if the entire amount has been covered
        if (remainingAmount == 0) {
            break; 
        }
    }

    return payOut;
}

/**
 * @brief Checks if the remaining amount could be satisfied with available coins/bills.
 * Throws if the change cannot be fully given.
 * @param remainingAmount The amount left after attempting to take change.
 * @throws std::runtime_error if the amount was not fully covered.
 */
void Payment::validateRemainingAmount(const int& remainingAmount) {
    if (remainingAmount > 0) {
        // This case implies we couldn't give enough change back, or the logic for input validation failed earlier
        throw std::runtime_error("Change not available"); // More payment needed than available coins
    }
}

/**
 * @brief Deducts the coins/bills given as change from the changeBox.
 * @param payOut Map of coin/bill values to counts given out.
 */
void Payment::updateChangeBox(const std::map<int, int>& payOut) {
    // Iterate through the calculated payout and decrement the internal stock
    for (const auto& [value, count] : payOut) {
        changeBox[value] -= count; // Subtract the coins/bills that were given out
    }
}

/**
 * @brief Initializes the change box with default coin/bill counts.
 * For simplicity, every denomination starts with 2 units.
 */
void Payment::setChangeBox() {
    // Initialize stock for each denomination
    changeBox[17] = 2;
    changeBox[11] = 2;
    changeBox[7]  = 2;
    changeBox[5]  = 2;
    changeBox[3]  = 2;
    changeBox[2]  = 2;
    changeBox[1]  = 2;
}