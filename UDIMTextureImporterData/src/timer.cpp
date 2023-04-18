
#include "timer.hpp"
#include <iostream>

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
