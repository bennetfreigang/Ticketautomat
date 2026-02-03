#include "TUIInputField.hpp"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cctype>
#include <fcntl.h>

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
        if (c == '\033') { // ESC or Escape Sequence
            char seq;
            int flags = fcntl(STDIN_FILENO, F_GETFL);
            fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
            ssize_t n = read(STDIN_FILENO, &seq, 1);
            fcntl(STDIN_FILENO, F_SETFL, flags); // Restore blocking

            if (n > 0 && seq == '[') {
                // It is an escape sequence (e.g. arrow key), consume the next char
                read(STDIN_FILENO, &seq, 1);
                continue; // Ignore
            } else {
                // Standalone ESC
                setRawMode(false);
                std::cout << std::endl;
                throw InputCancelledException();
            }
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
            // Only accept printable characters
            if (isprint(c)) {
                input += c;
                std::cout << c << std::flush;
            }
        }
    }

    setRawMode(false);
    return input;
}
