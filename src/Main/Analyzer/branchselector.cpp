#include "branchselector.h"
#include <QtDebug>

BranchSelector::BranchSelector(const cv::Point &point) : point(point) {}

void BranchSelector::addBrancheRef(BrancheRef newBranche)
{
    branches.push_back(newBranche);
}

void BranchSelector::selectionBranch()
{
    for (auto &branche : branches) {
        if(!branche.get().isPairFound()){
            branche.get().addPoint(point);
            selectingEnd = true;
            break;
        }
    }
}

bool BranchSelector::isSelectingEnd() const
{
    return selectingEnd;
}

std::list<BrancheRef> BranchSelector::getBranches() const
{
    return branches;
}

cv::Point BranchSelector::getPoint() const
{
    return point;
}
