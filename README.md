# Ticketautomat "Le" (Aufgabe 21)

Dies ist die Umsetzung eines interaktiven Ticketautomaten für den ÖPNV. Das Programm berechnet Preise basierend auf der Anzahl der Haltestellen und wickelt Zahlungen mit automatischer Wechselgeldausgabe ab.

## Features

* **Dynamischer Import:** Lädt Tram-Linien direkt aus `.txt`-Dateien im `data/`-Ordner.
* **Preislogik:** Berechnet Kosten anhand der Haltestellen-Distanz.
* **Wechselgeld-Algo:** Nutzt ein Greedy-Verfahren für die Stückelung (Werte: 17, 5, 3, 1).
* **TUI:** Schlanke Menüführung über die Konsole.

## Kompilieren & Ausführen

Zum Bauen des Hauptprogramms folgenden Befehl nutzen:

```bash
clang++ main.cpp Payment.cpp TicketMachine.cpp TramParser.cpp TUIMenu.cpp TUIInputField.cpp -o ticketautomat -std=c++17 -Wall -Wextra
```

Programm starten:

```bash
./ticketautomat
```

## Tests

Die Module können einzeln mit den Test-Files geprüft werden, z.B. für das Zahlungsmodul:

```bash
clang++ TestPayment.cpp Payment/Payment.cpp -o test_payment -std=c++17
./test_payment
```

## Wichtige Hinweise

* Mindestens eine gültige `.txt`-Datei muss enthalten sein.
* Das Programm setzt einen C++17-fähigen Compiler voraus.