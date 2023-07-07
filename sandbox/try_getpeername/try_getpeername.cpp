#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
// Callback function to write received data into a buffer
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    char *buffer = (char *)userp;

    // Append received data to the buffer
    strncpy(buffer, (char *)contents, total_size);
    buffer[total_size] = '\0';

    return total_size;
}

int main() {
    CURL *curl;
    CURLcode res;
    char buffer[4096] = {0}; // Buffer to store received data

    // Initialize libcurl
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize libcurl\n");
        return 1;
    }

    // Set the URL to retrieve
    curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");

    // Set the write callback function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    // Set the buffer as the user-defined data object
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

    // Perform the HTTP request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return 1;
    }

    // Print the received data
    printf("Received data:\n%s\n", buffer);

    // Clean up libcurl
    curl_easy_cleanup(curl);

    return 0;
}


//gcc -o a.out try_getpeername.cpp -lcurl
