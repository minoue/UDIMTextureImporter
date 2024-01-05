#pragma once

#include <chrono>
#include <string>

class Timer {
public:
    Timer();
    ~Timer();
    void start();
    void showDuration(std::string message);
    static std::string getCurrentTime();

private:
    std::chrono::system_clock::time_point start_point;
};
