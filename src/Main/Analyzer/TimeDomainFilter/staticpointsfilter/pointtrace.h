#ifndef POINTTRACE_H
#define POINTTRACE_H

#include "markedpoint.h"

namespace az {

class PointTrace
{
public:
    using MarkedPoints = std::vector<MarkedPoint>;

public:
    PointTrace() = default;
    PointTrace(const MarkedPoint &point);

    void push_back(const MarkedPoint &point);
    const MarkedPoint &last() const;
    const MarkedPoint &back() const;
    const MarkedPoints &points() const;
    void clear();

private:
    MarkedPoints m_points;
};

} // namespace az

#endif // POINTTRACE_H
