#include "AttributeStats.h"
#include "statistics.h"
#include "Utils.h"

void AttributeStats::addDistinct(double value, int count, double weight) {

	if (count > 0) {
		if (count == 1) {
			uniqueCount++;
		}
		if (Utils::eq(value, static_cast<double>(static_cast<int>(value)))) {
			intCount += count;
		}
		else {
			realCount += count;
		}
		if (nominalCounts.size() > 0) {
			nominalCounts[static_cast<int>(value)] = count;
			nominalWeights[static_cast<int>(value)] = weight;
		}
		if (numericStats != nullptr) {
			numericStats->add(value, weight);
			numericStats->calculateDerived();
		}
	}
	distinctCount++;
}
