#include "sleep_utils.h"
#include <thread>
#include <chrono>
#include <cstdint>

// Freeze program in seconds
void sleepSeconds(int16_t seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

// Freeze program in milliseconds
void sleep(int16_t milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
