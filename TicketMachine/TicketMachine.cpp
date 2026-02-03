#include "TicketMachine.hpp"
#include "../TramParser/TramParser.hpp"
#include "../Payment/Payment.hpp"
#include "../TUI/TUIMenu/TUIMenu.hpp"
#include "../TUI/TUIInputField/TUIInputField.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

/**
 * @brief Allows the user to select a tram line from available files.
 * Loads the tram data using TramParser and resets start/destination indices.
 * @throws std::runtime_error If no tram files are found.
 */
void TicketMachine::selectTram() {
    // Step 1: Retrieve all available tram configuration files from the "data" directory
    std::vector<FileEntry> entries = TramParser::getAvailableLines("data");

    // Check if any tram lines were found
    if (entries.empty()) {
        throw std::runtime_error("No tram available");
    }

    // Step 2: Create a TUI menu for tram selection
    TUIMenu menu("Select a tram:");
    for (const auto& entry : entries) {
        // Add each tram line as an option in the menu
        menu.addOption(entry.displayName, [this, entry]() {
            // Action to perform when a tram is selected:
            // 1. Parse the selected tram file to load stops and price info
            this->currentTram = TramParser::parseTramFile(entry.fileName);
            // 2. Reset the start and destination stop indices for the new tram
            this->selectedStartIndex = 0;
            this->selectedDestinationIndex = 0;
        });
    }
    // Add cancel option
    menu.addCancelationOption();
    // Step 3: Run the menu and wait for user selection
    menu.run();
}

/**
 * @brief Allows the user to select the starting stop.
 * Populates the menu with stops from the currently selected tram.
 * @throws std::runtime_error If no tram has been selected yet.
 */
void TicketMachine::selectStartStop() {
    // Ensure a tram is selected before proceeding
    if (currentTram.stops.empty()) {
        throw std::runtime_error("No tram selected!");
    }

    // Initialize the menu with the price per stop information
    TUIMenu menu("Price per Stop: " + std::to_string(currentTram.pricePerStop) + " Geld\nStart:");

    // Step 1: Add all stops of the current tram as menu options
    for (size_t i = 0; i < currentTram.stops.size(); ++i) {
        const std::string& stop = currentTram.stops.at(i);
        menu.addOption(stop, [this, i]() {
            // Update the selected start index when a stop is chosen
            selectedStartIndex = i;
        });
    }
    // Add cancel option
    menu.addCancelationOption();
    // Step 2: Display the menu to the user
    menu.run();
}

/**
 * @brief Allows the user to select the destination stop.
 * Shows start stop in the menu and populates all stops as options.
 * @throws std::runtime_error If no tram has been selected yet.
 */
void TicketMachine::selectDestinationStop() {
    // Ensure a tram is selected before proceeding
    if (currentTram.stops.empty()) {
        throw std::runtime_error("No tram selected!");
    }

    // Initialize menu, showing price and the already selected start stop
    TUIMenu menu("Price per Stop: " + std::to_string(currentTram.pricePerStop) +
                 " Geld\nStart: " + stopAtIndex(selectedStartIndex) + "\nDestination:");

    // Step 1: Add all stops as menu options
    for (size_t i = 0; i < currentTram.stops.size(); i++) {
        const std::string& stop = currentTram.stops[i];
        menu.addOption(stop, [this, i]() {
            // Update the selected destination index when a stop is chosen
            this->selectedDestinationIndex = i;
        });
    }
    // Add cancel option
    menu.addCancelationOption();
    // Step 2: Display the menu to the user
    menu.run();
}

/**
 * @brief Facilitates the ticket purchase process.
 * Calculates price, asks for payment, handles invalid input, and calculates change.
 * @return A TicketData object containing all details of the purchased ticket.
 * @throws std::runtime_error If no tram is selected.
 */
TicketData TicketMachine::buyTicket() {
    if (currentTram.stops.empty()) {
        throw std::runtime_error("No tram selected! Please select a tram first.");
    }
    if (selectedStartIndex == selectedDestinationIndex) {
        throw std::runtime_error("Invalid stop selection! Please select different stops.");
    }

    TicketData ticket;
    ticket.startStop = stopAtIndex(selectedStartIndex);
    ticket.destinationStop = stopAtIndex(selectedDestinationIndex);
    ticket.tram = currentTram.name;
    ticket.date = getCurrentDate();
    ticket.price = calculatePrice();

    while (true) {
        try {
            const int insertedAmount = processPayment(ticket);
            const int changeAmount = Payment::calculateChange(ticket.price, insertedAmount);
            ticket.change = payment.payOutChange(std::abs(changeAmount));
            return ticket;
        } catch (const std::runtime_error& e) {
            std::string errorMsg = e.what();
            if (errorMsg == "Purchase cancelled by user.") {
                throw;
            }
            if (errorMsg == "Change not available") {
                std::cerr << "Wechselgeld nicht verfügbar! Bitte passend zahlen oder kleineren Betrag wählen.\n";
                std::cout << "Drücken Sie eine Taste um fortzufahren...";
                TUIMenu::waitForKey();
                continue;
            }
            throw;
        }
    }

    return ticket;
}

/**
 * @brief Handles the payment interaction loop.
 * @param ticket The ticket data containing price and journey details.
 * @return The total valid amount inserted by the user.
 */
int TicketMachine::processPayment(const TicketData& ticket) {
    while (true) {
        std::cout << "\n--- Payment ---\n";
        std::cout << "Tram: " << ticket.tram << "\n";
        std::cout << "From: " << ticket.startStop << "\n";
        std::cout << "To:   " << ticket.destinationStop << "\n";
        std::cout << "Date: " << ticket.date << "\n";
        std::cout << "----------------\n";
        std::cout << "[ESC] Cancel payment\n";
        
        std::string prompt = "Price: " + std::to_string(ticket.price) + " Geld\nAmount paid in: ";
        try {
            int inserted = std::stoi(TUIInputField::getInput(prompt));
            if (inserted < 0) {
                std::cerr << "Amount cannot be negative. Please try again.\n\n";
                continue;
            }
            if (inserted < ticket.price) {
                std::cerr << "Insufficient funds! Needed: " << ticket.price << "\n\n";
                continue;
            }
            return inserted;
        } catch (const InputCancelledException&) {
            throw std::runtime_error("Purchase cancelled by user.");
        } catch (const std::exception&) {
            std::cerr << "Invalid input! Please enter a valid number.\n\n";
        }
    }
}

/**
 * @brief Returns a list of all file names in the given folder path.
 * @param folderPath Path to search for tram files.
 * @return Vector of file names as strings.
 */
std::vector<std::string> TicketMachine::getFileNames(const std::string& folderPath) {
    std::vector<std::string> tramNames;

    try {
        // Check if directory exists and is valid
        if (std::filesystem::exists(folderPath) && std::filesystem::is_directory(folderPath)) {
            // Iterate over all entries in the directory
            for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
                tramNames.push_back(entry.path().filename().string());
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        // Handle filesystem errors (e.g., permission issues)
        std::cerr << "Error while trying to access the folder path: " << e.what() << std::endl;
    }

    return tramNames;
}

/**
 * @brief Calculates the ticket price based on selected start and destination stops.
 * Uses absolute difference between indices multiplied by price per stop.
 * @return Ticket price as integer.
 */
int TicketMachine::calculatePrice() const {
    // Calculate the distance (number of stops) between start and destination
    int routeLength = std::abs(static_cast<int>(selectedStartIndex) - static_cast<int>(selectedDestinationIndex));
    // Multiply by the price per stop
    return routeLength * currentTram.pricePerStop;
}

/**
 * @brief Returns the stop name at the given index.
 * @param index Index of the stop.
 * @return Stop name as string, or error message if invalid.
 */
std::string TicketMachine::stopAtIndex(size_t index) const {
    // Validate that a tram is selected
    if (currentTram.stops.empty()) return "No tram selected";
    // Validate index bounds
    if (index >= currentTram.stops.size()) return "Invalid stop";

    return currentTram.stops.at(index);
}

/**
 * @brief Prints the ticket details to the console.
 * @param ticket The ticket data object to print.
 */
void TicketMachine::printTicket(const TicketData& ticket) {
    std::cout << "\n=== TICKET ===\n";
    std::cout << "Line:          " << ticket.tram << '\n';
    std::cout << "Start:         " << ticket.startStop << '\n';
    std::cout << "Destination:   " << ticket.destinationStop << '\n';
    std::cout << "Price:         " << ticket.price << " Geld\n";
    // Print breakdown of change dispensed
    std::cout << "Change:        " << calculateChangeSum(ticket.change) << " Geld\n";
    for (const auto& [value, count] : ticket.change) {
        std::cout << "  " << count << " x " << value << " Geld" << '\n';
    }

    std::cout << "==============\n";
}

/**
 * @brief Gets the current system date formatted as YYYY-MM-DD.
 * @return The current date as a string.
 */
std::string TicketMachine::getCurrentDate() {
    // Get current time from system clock
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    // Convert to local time structure
    std::tm localTime{};
    localtime_r(&time, &localTime);

    // Format the date using string stream
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d");
    return oss.str();
}

int TicketMachine::calculateChangeSum(const std::map<int, int> &change) {
    int sum = 0;
    for (const auto& [value, count] : change) {
        sum += value * count;
    }
    return sum;
}
