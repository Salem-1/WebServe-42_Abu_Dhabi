#include <iostream>
#include <thread>

// Function to be executed in a separate thread
void threadFunction()
{
    std::cout << "This is a separate thread!" << std::endl;
}

int main()
{
    std::cout << "This is the main thread!" << std::endl;

    // Create a thread and launch it
    std::thread myThread(threadFunction);

    // Wait for the thread to finish
    myThread.join();

    return 0;
}

