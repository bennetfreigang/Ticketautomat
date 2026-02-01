#include "TUIInputField.hpp"
#include <iostream>

/**
 * @brief Pauses program execution and waits for text input.
 *
 * Displays a prompt to the user, reads a full line of text from standard
 * input, and returns it as a string.
 *
 * @param prompt The text displayed before the user input.
 * @return The string entered by the user, or an empty string if input fails.
 */
std::string TUIInputField::getInput(const std::string &prompt) {
    std::string input;
    // Step 1: Display the prompt message to the console
    std::cout << prompt;

    // Step 2: Read a full line of input from stdin into the string variable
    // This handles spaces correctly, unlike std::cin >> input
    if (!(std::getline(std::cin, input))) {
        return ""; // Return empty string on EOF or error
    }

    return input;
}