#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <future> // For std::async

// For convenience
using json = nlohmann::json;

// Helper function to handle the response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
    size_t totalSize = size * nmemb;
    userData->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// REST API Client Class
class RestClient {
public:
    RestClient(const std::string& url) : url_(url) {}

    // Perform the GET request and return the parsed JSON
    json performRequest() {
        CURL* curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize cURL");
        }

        std::string response;
        CURLcode res;

        // Set cURL options
        curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            throw std::runtime_error("cURL Error: " + std::string(curl_easy_strerror(res)));
        }

        // Clean up
        curl_easy_cleanup(curl);

        // Parse the JSON response
        return json::parse(response);
    }

private:
    std::string url_;
};

// Function to process the REST API request asynchronously
void asyncRequest(const std::string& url) {
    try {
        RestClient client(url);

        // Perform the request asynchronously
        auto future = std::async(std::launch::async, [&client]() {
            return client.performRequest();
        });

        std::cout<<"jdjdjd0;\n";


        // Get the response
        json response = future.get();

        // Print the fields
        std::cout << "Response Fields:\n";
        for (auto it = response.begin(); it != response.end(); ++it) {
            std::cout << it.key() << ": " << it.value() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    const std::string url = "https://jsonplaceholder.typicode.com/posts/1";
    asyncRequest(url);
    return 0;
}
