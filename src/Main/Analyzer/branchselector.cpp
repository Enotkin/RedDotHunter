#include "branchselector.h"
#include <QtDebug>

BranchSelector::BranchSelector(const Contour &contour) : contour(contour)
{
}

void BranchSelector::addBranche(BrancheRef newBranche)
{
    branches.push_back(newBranche);
}

void BranchSelector::selectionBranch()
{
    branchSort();
    for (auto &branche : branches) {
        if(!branche.get().isPairFound()){
            branche.get().addContour(contour);
            selectingEnd = true;
            break;
        }
    }
}

void BranchSelector::branchSort()
{
    auto cmpBrancheArea = [](const BrancheRef &l, const BrancheRef &r){
        return l.get().getAverageArea() > r.get().getAverageArea();};

    if (auto [min, max] = std::minmax_element(branches.begin(), branches.end(), branchesCmp(std::less<int>{})) ; min == max){
        branches.sort(cmpBrancheArea);
    } else {
        branches.sort(branchesCmp(std::greater<int>{}));
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

Contour BranchSelector::getContour() const
{
    return contour;
}
