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
			//numericStats.add(value, count);
			numericStats->add(value, weight);
			numericStats->calculateDerived();
		}
	}
	distinctCount++;
}

std::string AttributeStats::toString() {

	std::string sb = "";
	sb.append(Utils::padLeft("Type", 4)).append(Utils::padLeft("Nom", 5));
	sb.append(Utils::padLeft("Int", 5)).append(Utils::padLeft("Real", 5));
	sb.append(Utils::padLeft("Missing", 12));
	sb.append(Utils::padLeft("Unique", 12));
	sb.append(Utils::padLeft("Dist", 6));
	if (nominalCounts.size() > 0) {
		sb.append(" ");
		for (int i = 0; i < nominalCounts.size(); i++) {
			sb.append(Utils::padLeft(std::string("C[") + std::to_string(i) + std::string("]"), 5));
		}
	}
	sb.append("\n");

	long long percent;
	percent = round(100.0 * intCount / totalCount);
	if (nominalCounts.size() > 0) {
		sb.append(Utils::padLeft("Nom", 4)).append(" ");
		sb.append(Utils::padLeft(std::string("") + std::to_string(percent), 3)).append("% ");
		sb.append(Utils::padLeft(std::string("") + std::to_string(0), 3)).append("% ");
	}
	else {
		sb.append(Utils::padLeft("Num", 4)).append(" ");
		sb.append(Utils::padLeft(std::string("") + std::to_string(0), 3)).append("% ");
		sb.append(Utils::padLeft(std::string("") + std::to_string(percent), 3)).append("% ");
	}
	percent = round(100.0 * realCount / totalCount);
	sb.append(Utils::padLeft(std::string("") + std::to_string(percent), 3)).append("% ");
	sb.append(Utils::padLeft(std::string("") + std::to_string(missingCount), 5)).append(" /");
	percent = round(100.0 * missingCount / totalCount);
	sb.append(Utils::padLeft(std::string("") + std::to_string(percent), 3)).append("% ");
	sb.append(Utils::padLeft(std::string("") + std::to_string(uniqueCount), 5)).append(" /");
	percent = round(100.0 * uniqueCount / totalCount);
	sb.append(Utils::padLeft(std::string("") + std::to_string(percent), 3)).append("% ");
	sb.append(Utils::padLeft(std::string("") + std::to_string(distinctCount), 5)).append(" ");
	if (nominalCounts.size() > 0) {
		for (int i = 0; i < nominalCounts.size(); i++) {
			sb.append(Utils::padLeft(std::string("") + std::to_string(nominalCounts[i]), 5));
		}
	}
	sb.append("\n");

	return sb;
}
