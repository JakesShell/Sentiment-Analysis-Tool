#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>

// Function to handle the response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to perform sentiment analysis
std::string analyzeSentiment(const std::string& text, const std::string& apiKey) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        std::string url = "https://api.example.com/sentiment"; // Replace with your API endpoint
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ("text=" + text).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, nullptr);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, nullptr);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(nullptr, ("x-api-key: " + apiKey).c_str()));

        // Perform the request
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return readBuffer; // Return the response (JSON)
}

int main() {
    std::string apiKey = "YOUR_API_KEY"; // Replace with your API key
    std::string text;

    std::cout << "Enter text for sentiment analysis: ";
    std::getline(std::cin, text);

    std::string response = analyzeSentiment(text, apiKey);

    // Parse the JSON response (assuming a specific format)
    Json::Value jsonData;
    Json::CharReaderBuilder readerBuilder;
    std::string errs;

    std::istringstream s(response);
    Json::parseFromStream(readerBuilder, s, &jsonData, &errs);

    std::cout << "Sentiment: " << jsonData["sentiment"].asString() << std::endl;

    return 0;
}
