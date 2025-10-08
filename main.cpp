#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <format>
#include "nlohmann/json.hpp"
#include "DataFetcher.h"

void loadEnvFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open .env file: " << filename << std::endl;
        return;
    }
    // to load the env file
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
    //std::cout << "Current working directory: " << std::filesystem::current_path() << "\n";
    loadEnvFile(".env");

	DataFetcher dataFetcher{};
    std::string apiKey = getenv("API_KEY");
    std::string underlying_ticker = "ES";
	/*std::string url = "https://api.polygon.io/v3/reference/options/contracts?apiKey=" + apiKey;*/
    std::string url = std::format("https://api.polygon.io/v3/reference/options/contracts?underlying_ticker={}&apiKey={}", underlying_ticker, apiKey);
	std::string response = dataFetcher.fetchUrl(url);
	std::cout << response;

    try {
        nlohmann::json option_data = nlohmann::json::parse(response);
        // while there is more data to be fetched
        while (option_data["next_url"] != "") {
            std::string next_page_url = std::format("{}&underlying_ticker={}&apiKey={}", option_data["next_url"].get<std::string>(), underlying_ticker, apiKey);
            std::string next_page = dataFetcher.fetchUrl(next_page_url);
            std::cout << next_page;
            try {
                nlohmann::json next_option_data = nlohmann::json::parse(next_page);
                // To do: Determine how to store the data, do we put it in a data structure? 
            }
            catch (const nlohmann::json::parse_error& e) {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
            }

        }
        // After fetching all the data, we should do something like taking in all the prices of the option and churn out IV. (Black scholes)
        // Plot market IV against Option chain IV.
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
    return 0;
}