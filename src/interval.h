#ifndef INTERVAL_H
#define INTERVAL_H

#include <cmath>
#include <constants.h>

class interval {
public:
    double min, max;

    interval()
        : min(+infinity), max(-infinity)
    {
    }
    interval(double m, double M)
        : min(m), max(M)
    {
    }
    interval(interval const& a, interval const& b)
        : min(std::fmin(a.min, b.min)), max(std::fmax(a.max, b.max))
    {
    }

    bool contains(double x) const
    {
        return (min <= x) && (max >= x);
    }
    interval expand(double delta) const
    {
        double padding = delta / 2;
        return interval(min - padding, max + padding);
    }
    interval offset(double off) const
    {
        return interval(min + off, max + off);
    }
    interval scale(double sc) const
    {
        double m1 = min * sc;
        double m2 = max * sc;
        if (sc < 0.0)
            std::swap(m1, m2);
        return interval(m1, m2);
    }

    double size() const
    {
        return (min < max ? (max - min) : 0.0);
    }
};

#endif // INTERVAL_H
