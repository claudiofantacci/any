#ifndef CPUTIMER_H
#define CPUTIMER_H

#include <chrono>

namespace utils
{

template<typename timetype = std::chrono::milliseconds>
struct CpuTimer
{
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point stop;

    void Start()
    {
        start = std::chrono::steady_clock::now();
    }

    void Stop()
    {
        stop = std::chrono::steady_clock::now();
    }

    double Elapsed()
    {
        std::chrono::steady_clock::duration time_span = stop - start;

        return static_cast<double>(time_span.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den * timetype::period::den;
    }
};

}

#endif  /* CPUTIMER_H */
