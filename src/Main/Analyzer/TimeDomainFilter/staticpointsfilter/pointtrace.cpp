#include "pointtrace.h"

az::PointTrace::PointTrace(const az::MarkedPoint &point)
{
    push_back(point);
}

void az::PointTrace::push_back(const az::MarkedPoint &point)
{
    m_points.push_back(point);
}

const az::MarkedPoint &az::PointTrace::last() const
{
    return back();
}

const az::MarkedPoint &az::PointTrace::back() const
{
    return m_points.back();
}

const az::PointTrace::MarkedPoints &az::PointTrace::points() const
{
    return m_points;
}

void az::PointTrace::clear()
{
    m_points.clear();
}
