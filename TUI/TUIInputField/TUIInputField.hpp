#pragma once
#include <string>
#include <exception>

struct InputCancelledException : public std::exception {
    const char* what() const noexcept override {
        return "Input cancelled by user";
    }
};

class TUIInputField {
public:
    static std::string getInput(const std::string& prompt);

private:
    static void setRawMode(bool enable);
};
