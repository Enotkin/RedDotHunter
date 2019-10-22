#ifndef STATICPOINTSFILTER_H
#define STATICPOINTSFILTER_H

#include <memory>
#include "staticpointsfilter/staticpointsfiltersettings.h"
#include "pointsfilter_common.h"

// [A]naly[Z]e
namespace az {

class StaticPointsFilterPrivate;
class StaticPointsFilter
{
public:
    StaticPointsFilter();
    ~StaticPointsFilter();

    void run(const FramePoints &framePoints);
    const FramePoints &filteredPoints() const;

    const StaticPointsFilterSettings &settings() const;
    void setSettings(const StaticPointsFilterSettings &settings);

private:
    const StaticPointsFilterPrivate *pimpl() const;
    StaticPointsFilterPrivate *pimpl();

private:
    std::unique_ptr<StaticPointsFilterPrivate> m_pimpl;
};

} // namespace az

#endif // STATICPOINTSFILTER_H
