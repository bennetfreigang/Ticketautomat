#include "../TramParser/TramParser.hpp"
#include <iostream>
#include <cassert>
#include <fstream>
#include <filesystem>
#include <vector>

void write_file(std::string name, std::string content) {
    if (!std::filesystem::exists("data")) std::filesystem::create_directory("data");
    std::ofstream f("data/" + name);
    f << content;
    f.close();
}

void test_parser() {
    std::cout << "Teste Parser..." << std::endl;

    // Testdatei anlegen
    std::string content = "Linie 10\n5\nStop A\nStop B\nStop C";
    write_file("linie10.txt", content);

    // Einlesen testen
    TramData data = TramParser::parseTramFile("linie10");

    assert(data.name == "Linie 10");
    assert(data.pricePerStop == 5);
    assert(data.stops.size() == 3);
    assert(data.stops[0] == "Stop A");
    assert(data.stops[2] == "Stop C");

    std::cout << "Parser OK." << std::endl;
    std::filesystem::remove("data/linie10.txt");
}

void test_error() {
    std::cout << "Teste Fehlerbehandlung..." << std::endl;
    try {
        TramParser::parseTramFile("gibts_nicht");
        assert(false); // Sollte hier nicht hinkommen
    } catch (std::exception& e) {
        std::cout << "Erwarteter Fehler abgefangen: " << e.what() << std::endl;
    }
}

int main() {
    test_parser();
    test_error();
    std::cout << "TramParser Tests fertig." << std::endl;
    return 0;
}