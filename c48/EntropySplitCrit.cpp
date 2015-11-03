#include "EntropySplitCrit.h"
#include "Distribution.h"
#include "core/ContingencyTables.h"
#include "core/Utils.h"

double EntropySplitCrit::splitCritValue(Distribution *bags) {

	return newEnt(bags);
}

double EntropySplitCrit::splitCritValue(Distribution *train, Distribution *test) {

	double result = 0;
	int numClasses = 0;
	int i, j;

	// Find out relevant number of classes
	for (j = 0; j < test->numClasses(); j++) {
		if (Utils::gr(train->perClass(j), 0) || Utils::gr(test->perClass(j), 0)) {
			numClasses++;
		}
	}

	// Compute entropy of test data with respect to training data
	for (i = 0; i < test->numBags(); i++) {
		if (Utils::gr(test->perBag(i), 0)) {
			for (j = 0; j < test->numClasses(); j++) {
				if (Utils::gr(test->perClassPerBag(i, j), 0)) {
					result -= test->perClassPerBag(i, j)* log(train->perClassPerBag(i, j) + 1);
				}
			}
			result += test->perBag(i) * log(train->perBag(i) + numClasses);
		}
	}

	return result / ContingencyTables::log2;
}

