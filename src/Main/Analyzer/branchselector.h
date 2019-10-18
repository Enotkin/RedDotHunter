#ifndef BRANCHESELECTOR_H
#define BRANCHESELECTOR_H
#include <functional>

#include "branch.h"

using BrancheRef = std::reference_wrapper<Branch>;

class BranchSelector
{
public:
    BranchSelector(const cv::Point &point);
    void addBrancheRef(BrancheRef newBranche);
    void selectionBranch();
    cv::Point getPoint() const;

    bool isSelectingEnd() const;

    std::list<BrancheRef> getBranches() const;

private:
    cv::Point point;
    std::list<BrancheRef> branches;
    bool selectingEnd = false;
    template<class Comparator>
    constexpr auto branchesCmp(Comparator cmp){
        return [cmp](const BrancheRef &l, const BrancheRef &r){
            return cmp(l.get().length(), r.get().length());
        };
    }

};

#endif // BRANCHESELECTOR_H
