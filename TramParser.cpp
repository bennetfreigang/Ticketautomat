#include "TramParser.hpp"
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <iostream>

/**
 * @brief Main entry point for parsing a tram configuration file.
 *
 * This function handles the entire process of opening a file, extracting tram data,
 * and returning a populated TramData object.
 *
 * @param filename The name of the file (without extension).
 * @return A populated TramData object containing the tram's name, stops, and price info.
 * @throws std::runtime_error If the file cannot be opened.
 */
TramData TramParser::parseTramFile(const std::string& filename) {
    validateFilename(filename);
    std::string path = createFilePath(filename);
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    TramData data;
    extractData(file, data);
    file.close();
    return data;
}

/**
 * @brief Retrieves the display name of a tram line from its file.
 *
 * Reads the first line of the file which serves as the display name.
 * If reading fails, it defaults to the filename.
 *
 * @param filename The base name of the file to read.
 * @return The display name found in the file, or the filename as a fallback.
 */
std::string TramParser::getDisplayNameFromFile(const std::string &filename) {
    validateFilename(filename);
    std::string path = createFilePath(filename);

    std::ifstream file(path);
    if (!file.is_open()) {
        return filename; // Fallback: return filename if file is not readable
    }

    std::string displayName;
    // Attempt to read the first line
    if (!std::getline(file, displayName)) {
        return filename; // Fallback: return filename if file is empty
    }

    return displayName;
}

/**
 * @brief Scans a directory for available tram line configuration files.
 *
 * It looks for `.txt` files in the specified directory and extracts their
 * display names to create a list of available options for the user.
 *
 * @param folderPath The directory path to search in.
 * @return A vector of FileEntry objects containing filenames and display names.
 */
std::vector<FileEntry> TramParser::getAvailableLines(const std::string& folderPath) {
    std::vector<FileEntry> entries;

    std::cout << "Suche Tramlinien in: " << folderPath << std::endl;

    try {
        // Verify the directory exists and is valid
        if (!std::filesystem::exists(folderPath)) {
            std::cerr << "Warnung: Verzeichnis existiert nicht: " << folderPath << std::endl;
            return entries;
        }

        if (!std::filesystem::is_directory(folderPath)) {
            std::cerr << "Warnung: Pfad ist kein Verzeichnis: " << folderPath << std::endl;
            return entries;
        }

        // Iterate through files in the directory
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            // Filter for .txt files that start with "Linie"
            std::string fileName = entry.path().filename().string();
            if (entry.path().extension() == ".txt" && fileName.rfind("Linie", 0) == 0) {
                std::string baseName = entry.path().stem().string();
                std::cout << "  Gefunden: " << baseName << std::endl;
                
                // Add the file to the list with its display name
                entries.push_back({
                    getDisplayNameFromFile(baseName),
                    baseName
                });
            }
        }

        std::cout << "Insgesamt " << entries.size() << " Linien gefunden." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Fehler beim Lesen des Verzeichnisses: " << e.what() << std::endl;
    }

    return entries;
}

/**
 * @brief Core extraction logic to parse a tram data file.
 *
 * Reads the tram name, price per stop, and the list of stops from the file stream.
 *
 * @param file Open input file stream.
 * @param data Target TramData struct to populate.
 * @throws std::runtime_error If reading critical data (name, price) fails.
 */
void TramParser::extractData(std::ifstream& file, TramData& data) {
    // Read the tram line name (first line)
    if (!std::getline(file, data.name)) {
        throw std::runtime_error("Failed to read tram line name");
    }

    //  Read the price per stop (second line, numeric)
    if (!(file >> data.pricePerStop)) {
        throw std::runtime_error("Failed to read price per stop");
    }

    // Buffer cleanup: consumes the leftover newline after reading a number
    std::string dummy;
    std::getline(file, dummy);

    // Read all subsequent lines as tram stops
    std::string stop;
    while (std::getline(file, stop)) {
        if (!stop.empty()) {
            data.stops.push_back(stop);
        }
    }

    std::cout << "Tram geladen: " << data.name << " (" << data.stops.size()
              << " Haltestellen, " << data.pricePerStop << " Geld/Stop)" << std::endl;
}

/**
 * @brief Constructs the relative path for the configuration file.
 * @param filename The base name of the file.
 * @return A string representing "filename.txt".
 */
std::string TramParser::createFilePath(const std::string &filename) {
    return filename + ".txt";
}

/**
 * @brief Ensures the filename is not empty.
 * @param filename The filename to validate.
 * @throws std::invalid_argument If the filename is empty.
 */
void TramParser::validateFilename(const std::string& filename) {
    if (filename.empty()) {
        throw std::invalid_argument("Provided filename is empty.");
    }
}