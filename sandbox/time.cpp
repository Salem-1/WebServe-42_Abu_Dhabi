#include <iostream>
#include <ctime>
#include <unistd.h>
int main() {
    // Get the current time
    std::time_t currentTime = std::time(nullptr);

    // Convert the time to a string representation

    sleep (1);
    std::time_t secondTime = std::time(nullptr);
    std::time_t total = secondTime - currentTime;
    std::string timeString = std::ctime(&currentTime);
    // Output the current time
    std::cout << "Current time: " << timeString;
    timeString = std::ctime(&total);
    std::cout << "Current time: " << total;


    std::string input;

    // Start the timer
    clock_t start = clock();

    // Call the getline() function
    std::getline(std::cin, input);

    // Stop the timer
    clock_t end = clock();

    // Calculate the duration
    double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    // Output the execution time
    std::cout << "Execution time: " << duration << " seconds" << std::endl;

    return 0;
}
