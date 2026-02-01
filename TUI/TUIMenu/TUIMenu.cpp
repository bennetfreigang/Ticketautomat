#include "TUIMenu.hpp"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <utility>

/**
 * @brief Constructs a new TUIMenu instance.
 *
 * Initializes the menu with a title that will be displayed
 * at the top of the terminal menu.
 *
 * @param title The title shown at the top of the menu.
 */
TUIMenu::TUIMenu(std::string title) : menuTitle(std::move(title)) {}

/**
 * @brief Adds a new option to the menu.
 *
 * Each option consists of a display title and an action
 * that is executed when the option is selected and confirmed.
 *
 * @param title  The text displayed for this menu option.
 * @param action The function executed when the option is selected.
 */
void TUIMenu::addOption(std::string title, std::function<void()> action) {
    options.push_back({std::move(title), std::move(action)});
}

/**
 * @brief Enables or disables terminal raw mode.
 *
 * When enabled, canonical input and echo are disabled so that key presses
 * can be read byte-by-byte. When disabled, the original terminal settings
 * are restored.
 *
 * @param enable Set to true to enable raw mode, false to restore normal mode.
 */
void TUIMenu::setRawMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        // Step 1: Save current terminal attributes to restore later
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        // Step 2: Disable canonical mode (line-by-line input) and echo
        newt.c_lflag &= ~(ICANON | ECHO);
        // Step 3: Apply new terminal attributes immediately
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        // Step 4: Hide the cursor to improve UI appearance
        std::cout << "\033[?25l" << std::flush; // Cursor off
    } else {
        // Restore saved terminal attributes
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        // Show the cursor again
        std::cout << "\033[?25h" << std::flush; // Cursor on
    }
}

/**
 * @brief Draws the menu to the terminal.
 *
 * Clears the screen, prints the menu title, and renders all menu options.
 * The currently selected option is highlighted.
 */
void TUIMenu::draw() const {
    // Step 1: Clear the screen and move cursor to home position
    std::cout << "\033[H\033[J"; // Screen Clear
    
    // Step 2: Render the menu title with cyan color
    std::cout << "\033[1;36m" << menuTitle << "\033[0m\n";
    std::cout << "============================\n\n";

    // Step 3: Loop over all options to render them
    for (std::size_t i = 0; i < options.size(); ++i) {
        if (i == selected) {
            // Highlight the selected option with a yellow bullet
            std::cout << "  \033[1;33m● " << options[i].title << "\033[0m\n";
        } else {
            // Render unselected options with a hollow bullet
            std::cout << "  ○ " << options[i].title << "\n";
        }
    }
}

/**
 * @brief Starts the menu event loop.
 *
 * Handles keyboard input (arrow keys and Enter), updates the selection,
 * and executes the selected action when Enter is pressed.
 */
void TUIMenu::run() {
    if (options.empty()) return;
    bool running = true;

    // Step 1: Enable raw mode to read input byte-by-byte immediately
    // This is moved outside the loop to prevent saving the already modified state
    setRawMode(true);

    // Step 2: Main Event loop
    while (running) {
        // Draw the menu with current selection
        draw();

        char c;
        // Read one character from stdin
        read(STDIN_FILENO, &c, 1); // keyboard input

        // Check if input was an escape sequence (likely arrow keys)
        if (c == '\033') {
            char seq[2];
            // Read the next two bytes to determine the sequence
            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);
            
            // 'A' is Up Arrow, 'B' is Down Arrow
            if (seq[1] == 'A') moveCursorUp(); 
            else if (seq[1] == 'B') moveCursorDown();
        }
        // Check if input was Enter key (newline)
        else if (c == '\n') {
            // Step 3: Execute Action
            // Disable raw mode before executing action to allow normal input if needed
            setRawMode(false);
            // Clear screen before running action
            std::cout << "\033[H\033[J";
            
            // Perform action of selected option safely
            try {
                options[selected].action();
            } catch (...) {
                // If an exception occurs, ensure we aren't stuck in a weird state
                setRawMode(false); // Ensure raw mode is definitely off
                throw; // Re-throw to be handled by main
            }
            running = false; // Exit the loop
        }
    }
    // Final cleanup: Ensure raw mode is disabled when exiting
    setRawMode(false);
}

/**
 * @brief Pauses execution and waits for a single key press.
 * 
 * Uses raw mode to detect a key press without requiring Enter.
 */
void TUIMenu::waitForKey() {
    // Enable raw mode to catch single character
    setRawMode(true);
    char c;
    // Wait for one byte of input
    read(STDIN_FILENO, &c, 1);
    // Restore normal terminal settings
    setRawMode(false);
}

/**
 * @brief Moves the cursor down.
 * Jumps back to the first element if at the end.
 *
 * Uses modulo arithmetic to implement wrap-around behavior.
 */
void TUIMenu::moveCursorDown() {
    // Increment selected index with wrap-around using modulo
    selected = (selected + 1) % options.size();
}

/**
 * @brief Moves the cursor up.
 * Jumps to the last element if at the beginning.
 *
 * Uses modulo arithmetic and avoids unsigned underflow.
 */
void TUIMenu::moveCursorUp() {
    // Decrement selected index with wrap-around.
    // Adding options.size() before subtracting 1 prevents unsigned underflow.
    selected = (selected + options.size() - 1 ) % options.size();
}