#include "staticpointsfiltersettings.h"

az::StaticPointsFilterSettings::StaticPointsFilterSettings()
    : m_maxPointTraceGap(3)
    , m_minPointTraceSize(10)
    , m_maxXDifference(5)
    , m_maxYDifference(5)
{
}

void az::StaticPointsFilterSettings::setMaxPointTraceGap(int maxPointTraceGap)
{
    m_maxPointTraceGap = maxPointTraceGap;
}

void az::StaticPointsFilterSettings::setMinPointTraceSize(int minPointTraceSize)
{
    m_minPointTraceSize = minPointTraceSize;
}

void az::StaticPointsFilterSettings::setMaxXDifference(int maxXDifference)
{
    m_maxXDifference = maxXDifference;
}

void az::StaticPointsFilterSettings::setMaxYDifference(int maxYDifference)
{
    m_maxYDifference = maxYDifference;
}

int az::StaticPointsFilterSettings::maxPointTraceGap() const
{
    return m_maxPointTraceGap;
}

int az::StaticPointsFilterSettings::minPointTraceSize() const
{
    return m_minPointTraceSize;
}

int az::StaticPointsFilterSettings::maxXDifference() const
{
    return m_maxXDifference;
}

int az::StaticPointsFilterSettings::maxYDifference() const
{
    return m_maxYDifference;
}
