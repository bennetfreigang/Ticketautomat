cmake_minimum_required(VERSION 4.1)
project(21_Ticketautomat)

set(CMAKE_CXX_STANDARD 17)

add_executable(21_Ticketautomat main.cpp
        TUIMenu.hpp
        TUIMenu.cpp
        TramParser.hpp
        TramParser.cpp
        TicketMachine.hpp
        TicketMachine.cpp
        TUIInputField.hpp
        TUIInputField.cpp
        Payment.hpp
        Payment.cpp
        TestPayment.cpp
        TestTramParser.cpp
        TestTicketMachine.cpp
)
