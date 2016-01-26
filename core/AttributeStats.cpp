#include "AttributeStats.h"
#include "statistics.h"
#include "Utils.h"

void AttributeStats::addDistinct(double value, int count, double weight) {

    if (count > 0) {
        if (count == 1) {
            mUniqueCount++;
        }
        if (Utils::eq(value, static_cast<double>(static_cast<int>(value)))) {
            mIntCount += count;
        }
        else {
            mRealCount += count;
        }
        if (mNominalCounts.size() > 0) {
            mNominalCounts[static_cast<int>(value)] = count;
            mNominalWeights[static_cast<int>(value)] = weight;
        }
        if (mNumericStats != nullptr) {
            mNumericStats->add(value, weight);
            mNumericStats->calculateDerived();
        }
    }
    mDistinctCount++;
}
