#ifndef TIMER_5324532_H
#define TIMER_5324532_H

#include <chrono>   // for std::chrono functions
#include <iostream> // for printing results

class Timer
{
private:
    // Type aliases to make accessing nested type easier
    using clock_type = std::chrono::steady_clock;
    using second_type = std::chrono::duration<double, std::ratio<1>>;

    std::chrono::time_point<clock_type> m_beg{clock_type::now()};

public:
    void reset();
    double elapsed() const;
    void start();
    void stop();
};

#endif
