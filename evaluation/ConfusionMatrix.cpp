#include "ConfusionMatrix.h"
#include "CostMatrix.h"
#include "TwoClassStats.h"
#include "core/Utils.h"

ConfusionMatrix::ConfusionMatrix(std::vector<std::string> classNames) :
    Matrix((int)classNames.size(), (int)classNames.size()) {
	mClassNames = classNames;
}

ConfusionMatrix *ConfusionMatrix::makeWeighted(CostMatrix *costs) {

	if (costs->size() != size()) {
		throw "Cost and confusion matrices must be the same size";
	}
	ConfusionMatrix *weighted = new ConfusionMatrix(mClassNames);
	for (int row = 0; row < size(); row++) {
		for (int col = 0; col < size(); col++) {
			weighted->set(row, col, get(row, col) * costs->getElement(row, col));
		}
	}
	return weighted;
}

int ConfusionMatrix::size() {

	return (int)mClassNames.size();
}

std::string ConfusionMatrix::className(int index) {

	return mClassNames[index];
}

void ConfusionMatrix::addPrediction(NominalPrediction *pred) {

	if (pred->predicted() == NominalPrediction::MISSING_VALUE) {
		throw "No predicted value given.";
	}
	if (pred->actual() == NominalPrediction::MISSING_VALUE) {
		throw "No actual value given.";
	}
	set(static_cast<int>(pred->actual()), static_cast<int>(pred->predicted()), get(static_cast<int>(pred->actual()), static_cast<int>(pred->predicted())) + pred->weight());

}

void ConfusionMatrix::addPredictions(std::vector<Prediction*> &predictions) {

	for (int i = 0; i < predictions.size(); i++) {
		addPrediction(static_cast<NominalPrediction*>(predictions[i]));
	}
}

TwoClassStats *ConfusionMatrix::getTwoClassStats(int classIndex) {

	double fp = 0, tp = 0, fn = 0, tn = 0;
	for (int row = 0; row < size(); row++) {
		for (int col = 0; col < size(); col++) {
			if (row == classIndex) {
				if (col == classIndex) {
					tp += get(row, col);
				}
				else {
					fn += get(row, col);
				}
			}
			else {
				if (col == classIndex) {
					fp += get(row, col);
				}
				else {
					tn += get(row, col);
				}
			}
		}
	}
	return new TwoClassStats(tp, fp, tn, fn);
}

double ConfusionMatrix::correct() {

	double correct = 0;
	for (int i = 0; i < size(); i++) {
		correct += get(i, i);
	}
	return correct;
}

double ConfusionMatrix::incorrect() {

	double incorrect = 0;
	for (int row = 0; row < size(); row++) {
		for (int col = 0; col < size(); col++) {
			if (row != col) {
				incorrect += get(row, col);
			}
		}
	}
	return incorrect;
}

double ConfusionMatrix::total() {

	double total = 0;
	for (int row = 0; row < size(); row++) {
		for (int col = 0; col < size(); col++) {
			total += get(row, col);
		}
	}
	return total;
}

double ConfusionMatrix::errorRate() {

	return incorrect() / total();
}

std::string ConfusionMatrix::toString() {

	return toString("=== Confusion Matrix ===\n");
}

std::string ConfusionMatrix::toString(const std::string &title) {

	std::string text = "";
	std::vector<char> IDChars = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
	int IDWidth;
	bool fractional = false;

	// Find the maximum value in the matrix
	// and check for fractional display requirement
	double maxval = 0;
	for (int i = 0; i < size(); i++) {
		for (int j = 0; j < size(); j++) {
			double current = get(i, j);
			if (current < 0) {
				current *= -10;
			}
			if (current > maxval) {
				maxval = current;
			}
			double fract = current - std::rint(current);
			if (!fractional && ((log(fract) / log(10)) >= -2)) {
				fractional = true;
			}
		}
	}
	int value1 = static_cast<int>(log(maxval) / log(10) + (fractional ? 3 : 0));
	int value2 = static_cast<int>(log(size()) / log(IDChars.size()));
	IDWidth = 1 + value1 > value2 ? value1 : value2;
	text.append(title).append("\n");
	for (int i = 0; i < size(); i++) {
		if (fractional) {
			text.append(" ").append(num2ShortID(i, IDChars, IDWidth - 3)).append("   ");
		}
		else {
			text.append(" ").append(num2ShortID(i, IDChars, IDWidth));
		}
	}
	text.append("     actual class\n");
	for (int i = 0; i < size(); i++) {
		for (int j = 0; j < size(); j++) {
			text.append(" ").append(Utils::doubleToString(get(i, j), IDWidth, (fractional ? 2 : 0)));
		}
		text.append(" | ").append(num2ShortID(i, IDChars, IDWidth)).append(" = ").append(mClassNames[i]).append("\n");
	}

	return text;
}

std::string ConfusionMatrix::num2ShortID(int num, std::vector<char> &IDChars, int IDWidth) {

	std::vector<char> ID(IDWidth);
	int i;

	for (i = IDWidth - 1; i >= 0; i--) {
		ID[i] = IDChars[num % IDChars.size()];
		num = num / (int)IDChars.size() - 1;
		if (num < 0) {
			break;
		}
	}
	for (i--; i >= 0; i--) {
		ID[i] = ' ';
	}

	return std::string(ID.begin(),ID.end());
}


void ConfusionMatrix::set(int i, int j, double s)
{
  set(i,j,s);
}
int ConfusionMatrix::getRowDimension() {
	return getRowDimension();
}

int ConfusionMatrix::getColumnDimension() {
	return getColumnDimension();
}
std::vector<std::vector<double>> ConfusionMatrix::getArray() {
  return getArray();
}


double ConfusionMatrix::get( int i, int j ) {
  return get( i, j );
}