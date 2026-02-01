#include "TUIInputField.hpp"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cctype>

/**
 * @brief Enables or disables terminal raw mode.
 *
 * @param enable Set to true to enable raw mode, false to restore normal mode.
 */
void TUIInputField::setRawMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

/**
 * @brief Pauses program execution and waits for text input.
 *
 * Displays a prompt to the user, reads characters one by one.
 * Supports:
 * - Backspace
 * - ESC to cancel (throws InputCancelledException)
 * - Enter to confirm
 *
 * @param prompt The text displayed before the user input.
 * @return The string entered by the user.
 * @throws InputCancelledException if the user presses ESC.
 */
std::string TUIInputField::getInput(const std::string &prompt) {
    std::string input;
    std::cout << prompt << std::flush;

    setRawMode(true);

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == '\033') { // ESC
            // Check if it's an escape sequence (like arrow keys) or just ESC
            // This is a simple check: we assume if ESC is pressed, it's a cancel
            // unless we can immediately read more.
            // For now, let's treat ESC as Cancel.
            setRawMode(false);
            std::cout << std::endl;
            throw InputCancelledException();
        } else if (c == '\n' || c == '\r') { // Enter
            setRawMode(false);
            std::cout << std::endl;
            return input;
        } else if (c == 127 || c == '\b') { // Backspace
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        } else {
            // Only accept printable characters (optional)
            if (isprint(c)) {
                input += c;
                std::cout << c << std::flush;
            }
        }
    }

    setRawMode(false);
    return input;
}
