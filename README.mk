# Ticketautomat "Le" (Aufgabe 21)

Dies ist die Umsetzung eines interaktiven Ticketautomaten für den ÖPNV. Das Programm berechnet Preise basierend auf der Anzahl der Haltestellen und wickelt Zahlungen mit automatischer Wechselgeldausgabe ab.

## Features

* **Dynamischer Import:** Lädt Tram-Linien direkt aus `.txt`-Dateien im `data/`-Ordner.
* **Preislogik:** Berechnet Kosten anhand der Haltestellen-Distanz.
* **Wechselgeld-Algo:** Nutzt ein Greedy-Verfahren für die Stückelung (Werte: 17, 5, 3, 1).
* **TUI:** Schlanke Menüführung über die Konsole.

## Projektstruktur

* `main.cpp` – Startpunkt des Programms.
* `data/` – Verzeichnis für Linien-Konfigurationen (erforderlich).
* `Payment/`, `TramParser/`, `TicketMachine/` – Logik-Module.
* `TUI/` – Hilfsklassen für die Konsolendarstellung.
* `test_*.cpp` – Unittests für die einzelnen Komponenten.

## Kompilieren & Ausführen

Zum Bauen des Hauptprogramms folgenden Befehl nutzen:

```bash
clang++ main.cpp Payment/Payment.cpp TicketMachine/TicketMachine.cpp \
TramParser/TramParser.cpp TUI/TUIMenu/TUIMenu.cpp \
TUI/TUIInputField/TUIInputField.cpp -o ticketautomat -std=c++17

```

Programm starten:

```bash
./ticketautomat

```

## Tests

Die Module können einzeln mit den Test-Files geprüft werden, z.B. für das Zahlungsmodul:

```bash
clang++ test_payment.cpp Payment/Payment.cpp -o test_payment -std=c++17
./test_payment

```

## Wichtige Hinweise

* Der Ordner `data/` muss vorhanden sein und mindestens eine gültige `.txt`-Datei enthalten.
* Das Programm setzt einen C++17-fähigen Compiler voraus.