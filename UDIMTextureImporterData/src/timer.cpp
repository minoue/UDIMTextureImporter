
#include "timer.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

Timer::Timer() {};

Timer::~Timer() {};

void Timer::start()
{
    this->start_point = std::chrono::system_clock::now();
}

void Timer::showDuration(std::string message)
{
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<float> fdur = end - start_point;
    std::cout << message << fdur.count() << " seconds" << std::endl;
}

std::string Timer::getCurrentTime()
{
    //
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);

    // std::tm tm = *std::gmtime(&time); //GMT (UTC)
    std::tm tm = *std::localtime(&time); //Locale time-zone, usually UTC by default.
    std::stringstream ss;
    std::string format = "UTC: %Y-%m-%d %H:%M:%S";
    ss << std::put_time( &tm, format.c_str());
    return ss.str();
}
