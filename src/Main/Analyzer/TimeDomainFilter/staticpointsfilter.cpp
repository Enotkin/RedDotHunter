#include "staticpointsfilter.h"
#include "staticpointsfilter/staticpointsfilterprivate.h"

az::StaticPointsFilter::StaticPointsFilter()
    : m_pimpl(std::make_unique<StaticPointsFilterPrivate>())
{
}

az::StaticPointsFilter::~StaticPointsFilter()
{
}

void az::StaticPointsFilter::run(const FramePoints &framePoints)
{
    pimpl()->run(framePoints);
}

const az::FramePoints &az::StaticPointsFilter::filteredPoints() const
{
    return pimpl()->filteredPoints();
}

const az::StaticPointsFilterSettings &az::StaticPointsFilter::settings() const
{
    return pimpl()->settings();
}

void az::StaticPointsFilter::setSettings(const StaticPointsFilterSettings &settings)
{
    pimpl()->setSettings(settings);
}

const az::StaticPointsFilterPrivate *az::StaticPointsFilter::pimpl() const
{
    return m_pimpl.get();
}

az::StaticPointsFilterPrivate *az::StaticPointsFilter::pimpl()
{
    return m_pimpl.get();
}
