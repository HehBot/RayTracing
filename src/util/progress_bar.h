#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <iostream>
#include <mutex>
#include <sstream>

class progress_bar {
    int const len, max;
    double const incr;
    int const q;

    std::mutex mut; // same mutex for both progress and io
    int progress, last_progress;

public:
    progress_bar(int len, int max, double incr)
        : len(len), max(max), incr(incr), q((incr * max) / len), progress(0), last_progress(0)
    {
    }
    void update()
    {
        std::lock_guard<std::mutex> g(mut);
        if (progress == max)
            return;
        ++progress;
        if (progress >= q + last_progress) {
            last_progress = progress;
            int j = (progress * len) / max;
            double k = (progress * 100.0) / max;

            char buf[10];
            sprintf(buf, "%6.2f", k);

            std::stringstream b;
            b << "\rProgress: [" << buf << "%] [";
            for (int i = 0; i < j; ++i)
                b << '#';
            for (int i = j; i < len; ++i)
                b << '.';
            b << "]\r";
            std::cerr << b.str() << std::flush;
        }
        if (progress == max)
            std::cerr << '\n';
    }
};

#endif // PROGRESS_BAR_H
