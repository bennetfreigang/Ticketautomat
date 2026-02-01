#include "../Payment/Payment.hpp"
#include <iostream>
#include <cassert>

void test_calc() {
    // Zu wenig bezahlt
    assert(Payment::calculateChange(10, 20) == 10); // fehlen 10
    // Passend
    assert(Payment::calculateChange(20, 20) == 0);
    // Rückgeld
    assert(Payment::calculateChange(25, 20) == -5); // 5 zurück
}

void test_payout() {
    Payment p;
    p.reset(); // Sicherstellen, dass voll

    // 17 Geld zurück (sollte 1x 17 sein)
    auto change = p.payOutChange(17);
    assert(change[17] == 1);
    assert(change[1] == 0);

    // Komplexer Betrag: 6 Geld (5 + 1)
    change = p.payOutChange(6);
    assert(change[5] == 1);
    assert(change[1] == 1);
}

void test_not_enough_money() {
    Payment p;
    // Wir leeren den Automaten künstlich oder fragen absurd viel an
    // Laut Aufgabe sind anfangs je 2 Scheine drin.
    // 2 * 17 = 34. Wenn wir 3x 17 wollen, muss es knallen.

    p.reset();
    try {
        p.payOutChange(17);
        p.payOutChange(17);
        p.payOutChange(17); // Hier sollte Schluss sein
        std::cout << "Fehler: Exception wurde nicht geworfen!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "Korrekt: Automat leer -> " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "Teste Payment..." << std::endl;
    test_calc();
    test_payout();
    test_not_enough_money();
    std::cout << "Payment Tests fertig." << std::endl;
    return 0;
}