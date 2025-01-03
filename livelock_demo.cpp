#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

// Global mutex variables representing shared resources
std::mutex resource1;
std::mutex resource2;

// Shared flags to simulate livelock behavior
volatile bool thread1_released = false;
volatile bool thread2_released = false;

/**
 * Thread 1 function
 * Tries to acquire Resource 1 and Resource 2, but releases Resource 1
 * if it notices Thread 2 is also trying to proceed, creating livelock.
 */
void thread1() 
{
    while (true) 
    {
        std::cout << "Thread 1: Trying to lock Resource 1\n";
        resource1.lock();
        std::cout << "Thread 1: Locked Resource 1\n";

        // Simulate detection of Thread 2 trying to proceed
        if (thread2_released) 
        {
            std::cout << "Thread 1: Detected Thread 2 is waiting. Releasing Resource 1.\n";
            resource1.unlock();
            thread1_released = true; // Indicate that Thread 1 released the resource
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate delay before retrying
            continue;
        }

        std::cout << "Thread 1: Trying to lock Resource 2\n";
        resource2.lock();
        std::cout << "Thread 1: Locked Resource 2\n";

        // If both locks are acquired (unrealistic in livelock), release them
        resource2.unlock();
        resource1.unlock();
        std::cout << "Thread 1: Successfully acquired both resources. Exiting.\n";
        break;
    }
}

/**
 * Thread 2 function
 * Tries to acquire Resource 2 and Resource 1, but releases Resource 2
 * if it notices Thread 1 is also trying to proceed, creating livelock.
 */
void thread2() 
{
    while (true) 
    {
        std::cout << "Thread 2: Trying to lock Resource 2\n";
        resource2.lock();
        std::cout << "Thread 2: Locked Resource 2\n";

        // Simulate detection of Thread 1 trying to proceed
        if (thread1_released) 
        {
            std::cout << "Thread 2: Detected Thread 1 is waiting. Releasing Resource 2.\n";
            resource2.unlock();
            thread2_released = true; // Indicate that Thread 2 released the resource
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate delay before retrying
            continue;
        }

        std::cout << "Thread 2: Trying to lock Resource 1\n";
        resource1.lock();
        std::cout << "Thread 2: Locked Resource 1\n";

        // If both locks are acquired (unrealistic in livelock), release them
        resource1.unlock();
        resource2.unlock();
        std::cout << "Thread 2: Successfully acquired both resources. Exiting.\n";
        break;
    }
}

/**
 * Main function
 * Creates threads and cleans up resources.
 */
int main() 
{
    std::cout << "Initializing resources...\n";

    // Create threads
    std::cout << "Creating threads...\n";
    std::thread t1(thread1);
    std::thread t2(thread2);

    // Wait for threads to finish
    t1.join();
    t2.join();

    std::cout << "Program completed.\n";
    return 0;
}

