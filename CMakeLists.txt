cmake_minimum_required(VERSION 4.1)
project(21_Ticketautomat)

set(CMAKE_CXX_STANDARD 17)

add_executable(21_Ticketautomat main.cpp
        TUI/TUIMenu/TUIMenu.hpp
        TUI/TUIMenu/TUIMenu.cpp
        TramParser/TramParser.hpp
        TramParser/TramParser.cpp
        TicketMachine/TicketMachine.hpp
        TicketMachine/TicketMachine.cpp
        TUI/TUIInputField/TUIInputField.hpp
        TUI/TUIInputField/TUIInputField.cpp
        Payment/Payment.hpp
        Payment/Payment.cpp
        Tests/TestPayment.cpp
        Tests/TestTramParser.cpp
        Tests/TestTicketMachine.cpp
)
