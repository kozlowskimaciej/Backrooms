#include "timer.h"

void Timer::reset()
{
    m_beg = clock_type::now();
}

double Timer::elapsed() const
{
    return std::chrono::duration_cast<second_type>(clock_type::now() - m_beg).count();
}

void Timer::start()
{
    reset();
}

void Timer::stop()
{
    static int counter{0};
    static double measuredTime{0.0};
    ++counter;
    measuredTime += elapsed();
    if (counter % 10000 == 0)
    {
        std::cerr << "fps :" << counter / measuredTime / 100.0 << '\n';
    }
}