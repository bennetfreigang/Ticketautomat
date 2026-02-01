#pragma once
#include <string>
#include <vector>

struct TramData {
    std::string name;
    int pricePerStop;
    std::vector<std::string> stops;
};

struct FileEntry {
    std::string displayName;
    std::string fileName;
};

class TramParser {
public:
    static TramData parseTramFile(const std::string& filename);
    static std::string getDisplayNameFromFile(const std::string& filename);
    static std::vector<FileEntry> getAvailableLines(const std::string& folderPath);

private:
    static void extractData(std::ifstream& file, TramData& data);
    static std::string createFilePath(const std::string& filename);
    static void validateFilename(const std::string& filename);
    static void validateDirectory(const std::string& folderPath);
    static void extractStops(std::ifstream& file, TramData& data);
};