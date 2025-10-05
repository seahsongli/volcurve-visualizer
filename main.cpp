#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include "DataFetcher.h"

void loadEnvFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open .env file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t equalsPos = line.find('=');
        if (equalsPos != std::string::npos) {
            std::string key = line.substr(0, equalsPos);
            std::string value = line.substr(equalsPos + 1);

            // Remove leading/trailing whitespace from key and value if needed
            // ...

            // Set environment variable
#ifdef _WIN32
            _putenv_s(key.c_str(), value.c_str());
#else
            setenv(key.c_str(), value.c_str(), 1); // 1 to overwrite if exists
#endif
        }
    }
    file.close();
}

int main() {
    std::cout << "Current working directory: " << std::filesystem::current_path() << "\n";
    loadEnvFile(".env");

	DataFetcher dataFetcher{};
    std::string apiKey = getenv("API_KEY");
	std::string url = "https://api.polygon.io/v3/reference/dividends?apiKey=" + apiKey;
	std::string response = dataFetcher.fetchUrl(url);
	std::cout << response;
}