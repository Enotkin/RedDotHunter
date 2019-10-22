#ifndef STATICPOINTSFILTERSETTINGS_H
#define STATICPOINTSFILTERSETTINGS_H

namespace az {

class StaticPointsFilterSettings
{
public:
    StaticPointsFilterSettings();

    void setMaxPointTraceGap(int maxPointTraceGap);
    void setMinPointTraceSize(int minPointTraceSize);
    void setMaxXDifference(int maxXDifference);
    void setMaxYDifference(int maxYDifference);

    int maxPointTraceGap() const;
    int minPointTraceSize() const;
    int maxXDifference() const;
    int maxYDifference() const;

private:
    int m_maxPointTraceGap;
    int m_minPointTraceSize;
    int m_maxXDifference;
    int m_maxYDifference;
};

} // namespace az

#endif // STATICPOINTSFILTERSETTINGS_H
