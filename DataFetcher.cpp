
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "DataFetcher.h"

std::string DataFetcher::fetchUrl(const std::string& url) {
	CURL* curl = curl_easy_init();
	std::string response;
	if (curl) { 
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // curl requires const char*
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](char* ptr, std::size_t size, std::size_t nmemb, std::string* data) {
			data->append(ptr, size * nmemb);
			return size * nmemb;
			}); // + converts the lambda to a function pointer
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response); // once data is ready, write to the response
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	
	return response;
}