#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// For convenience
using json = nlohmann::json;

// Callback function for handling the data received
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
    size_t totalSize = size * nmemb;
    userData->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int main() {
    CURL* curl;
    CURLcode res;

    std::string response; // To store the response

    curl = curl_easy_init(); // Initialize cURL
    if (curl) {
        // Set the URL for the GET request
        curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts/1");

        // Specify the callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Pass the response string to the callback
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Raw Response: " << response << std::endl;

            try {
                // Parse the JSON response
                json jsonResponse = json::parse(response);

                // Iterate over all fields and print
                for (auto it = jsonResponse.begin(); it != jsonResponse.end(); ++it) {
                    std::cout << it.key() << ": " << it.value() << std::endl;
                }
            } catch (const json::parse_error& e) {
                std::cerr << "JSON Parse Error: " << e.what() << std::endl;
            }
        }

        // Cleanup
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize cURL" << std::endl;
    }

    return 0;
}
