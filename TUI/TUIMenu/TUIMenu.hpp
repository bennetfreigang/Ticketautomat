#pragma once
#include <vector>
#include <string>
#include <functional>
#include <cstddef>

class TUIMenu {
private:
    struct Option {
        std::string title;
        std::function<void()> action;
    };

    std::vector<Option> options;
    std::string menuTitle;
    std::size_t selected = 0;

    // Enables or disables terminal raw mode (no echo, no canonical input)
    static void setRawMode(bool enable);
    // Clears the screen and renders the menu with the current selection
    void draw() const;
    // Moves the selection one item up (with wrap-around)
    void moveCursorUp();
    // Moves the selection one item down (with wrap-around)
    void moveCursorDown();

public:
    explicit TUIMenu(std::string title);
    void addOption(std::string title, std::function<void()> action);
    void run();
    static void waitForKey();
};