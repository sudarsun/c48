#include "ClassifierTree.h"
#include "ModelSelection.h"
#include "ClassifierSplitModel.h"
#include "core/Instances.h"
#include "Distribution.h"
#include "core/Instance.h"
#include "core/Utils.h"

long long ClassifierTree::PRINTED_NODES = 0;

long long ClassifierTree::nextID() {

	return PRINTED_NODES++;
}

void ClassifierTree::resetID() {

	PRINTED_NODES = 0;
}

ClassifierTree::ClassifierTree(ModelSelection *toSelectLocModel) {

	mtoSelectModel = toSelectLocModel;
}



void ClassifierTree::buildClassifier(Instances *data) throw(std::exception) {


	// remove instances with missing class
	data = new Instances(data);
	data->deleteWithMissingClass();

	buildTree(data, false);
}

void ClassifierTree::buildTree(Instances *data, bool keepData) throw(std::exception) {

	std::vector<Instances*> localInstances;

	if (keepData) {
		mtrain = data;
	}
	//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
	delete mtest;
	misLeaf = false;
	misEmpty = false;
	msons.clear();
	mlocalModel = mtoSelectModel->selectModel(data);
	if (mlocalModel->numSubsets() > 1) {
		//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
		localInstances = mlocalModel->split(data);
		//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
		delete data;
		msons = std::vector<ClassifierTree*>(mlocalModel->numSubsets());
		for (int i = 0; i < msons.size(); i++) {
			msons[i] = getNewTree(localInstances[i]);
			//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
			delete localInstances[i];
		}
	}
	else {
		misLeaf = true;
		if (Utils::eq(data->sumOfWeights(), 0)) {
			misEmpty = true;
		}
		//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
		delete data;
	}
}

void ClassifierTree::buildTree(Instances *train, Instances *test, bool keepData) throw(std::exception) {

	std::vector<Instances*> localTrain, *localTest;
	int i;

	if (keepData) {
		mtrain = train;
	}
	misLeaf = false;
	misEmpty = false;
	msons.clear();
	mlocalModel = mtoSelectModel->selectModel(train, test);
	mtest = new Distribution(test, mlocalModel);
	if (mlocalModel->numSubsets() > 1) {
		//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
		localTrain = mlocalModel->split(train);
		//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
		localTest = mlocalModel->split(test);
		//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
		delete train;
		//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
		delete test;
		msons = std::vector<ClassifierTree*>(mlocalModel->numSubsets());
		for (i = 0; i < msons.size(); i++) {
			msons[i] = getNewTree(localTrain[i], localTest[i]);
			//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
			delete localTrain[i];
			//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
			delete localTest[i];
		}
	}
	else {
		misLeaf = true;
		if (Utils::eq(train->sumOfWeights(), 0)) {
			misEmpty = true;
		}
		//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
		delete train;
		//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
		delete test;
	}
}

double ClassifierTree::classifyInstance(Instance *instance) throw(std::exception) {

	double maxProb = -1;
	double currentProb;
	int maxIndex = 0;
	int j;

	for (j = 0; j < instance->numClasses(); j++) {
		currentProb = getProbs(j, instance, 1);
		if (Utils::gr(currentProb, maxProb)) {
			maxIndex = j;
			maxProb = currentProb;
		}
	}

	return maxIndex;
}

void ClassifierTree::cleanup(Instances *justHeaderInfo) {

	mtrain = justHeaderInfo;
	//JAVA TO C++ CONVERTER WARNING: Java to C++ Converter converted the original 'null' assignment to a call to 'delete', but you should review memory allocation of all pointer variables in the converted code:
	delete mtest;
	if (!misLeaf) {
		for (auto mson : msons) {
			mson->cleanup(justHeaderInfo);
		}
	}
}

std::vector<double> ClassifierTree::distributionForInstance(Instance *instance, bool useLaplace) throw(std::exception) {

	std::vector<double> doubles(instance->numClasses());

	for (int i = 0; i < doubles.size(); i++) {
		if (!useLaplace) {
			doubles[i] = getProbs(i, instance, 1);
		}
		else {
			doubles[i] = getProbsLaplace(i, instance, 1);
		}
	}

	return doubles;
}

int ClassifierTree::assignIDs(int lastID) {

	int currLastID = lastID + 1;

	mid = currLastID;
	if (msons.size() > 0) {
		for (auto mson : msons) {
			currLastID = mson->assignIDs(currLastID);
		}
	}
	return currLastID;
}

int ClassifierTree::graphType() {
	return 0;
}

std::string ClassifierTree::graph() throw(std::exception) {

	StringBuffer *text = new StringBuffer();

	assignIDs(-1);
	text->append(L"digraph J48Tree {\n");
	if (misLeaf) {
		text->append(std::string(L"N") + mid + std::string(L" [label=\"") + Utils::backQuoteChars(mlocalModel->dumpLabel(0, mtrain)) + std::string(L"\" ") + std::string(L"shape=box style=filled "));
		if (mtrain != nullptr && mtrain->numInstances() > 0) {
			text->append(std::string(L"data =\n") + mtrain + std::string(L"\n"));
			text->append(L",\n");

		}
		text->append(L"]\n");
	}
	else {
		text->append(std::string(L"N") + mid + std::string(L" [label=\"") + Utils::backQuoteChars(mlocalModel->leftSide(mtrain)) + std::string(L"\" "));
		if (mtrain != nullptr && mtrain->numInstances() > 0) {
			text->append(std::string(L"data =\n") + mtrain + std::string(L"\n"));
			text->append(L",\n");
		}
		text->append(L"]\n");
		graphTree(text);
	}

	//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
	return text.append("}\n");
}

std::string ClassifierTree::prefix() throw(std::exception) {

	std::string text;

	text = "";
	if (misLeaf) {
		text.append(std::string("[") + mlocalModel->dumpLabel(0, mtrain) + std::string("]"));
	}
	else {
		prefixTree(text);
	}

	//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
	return text;
}

std::vector<std::string> ClassifierTree::toSource(const std::string &className)  {

	std::vector<StringBuffer*> result(2);
	if (misLeaf) {
		result[0] = new StringBuffer(std::string(L"    p = ") + mlocalModel->distribution().maxClass(0) + std::string(L";\n"));
		result[1] = new StringBuffer(L"");
	}
	else {
		StringBuffer *text = new StringBuffer();
		StringBuffer *atEnd = new StringBuffer();

		long long printID = ClassifierTree::nextID();

		text->append(L"  static double N")->append(int::toHexString(mlocalModel->hashCode()) + printID)->append(L"(Object []i) {\n")->append(L"    double p = Double.NaN;\n");

		text->append(L"    if (")->append(mlocalModel->sourceExpression(-1, mtrain))->append(L") {\n");
		text->append(L"      p = ")->append(mlocalModel->distribution().maxClass(0))->append(L";\n");
		text->append(L"    } ");
		for (int i = 0; i < msons.size(); i++) {
			text->append(std::string(L"else if (") + mlocalModel->sourceExpression(i, mtrain) + std::string(L") {\n"));
			if (msons[i]->misLeaf) {
				text->append(std::string(L"      p = ") + mlocalModel->distribution().maxClass(i) + std::string(L";\n"));
			}
			else {
				std::vector<StringBuffer*> sub = msons[i]->toSource(className);
				text->append(sub[0]);
				atEnd->append(sub[1]);
			}
			text->append(L"    } ");
			if (i == msons.size() - 1) {
				text->append(L'\n');
			}
		}

		text->append(L"    return p;\n  }\n");

		result[0] = new StringBuffer(std::string(L"    p = ") + className + std::string(L".N"));
		result[0]->append(int::toHexString(mlocalModel->hashCode()) + printID)->append(L"(i);\n");
		result[1] = text->append(atEnd);
	}
	return result;
}

int ClassifierTree::numLeaves() {

	int num = 0;
	int i;

	if (misLeaf) {
		return 1;
	}
	else {
		for (i = 0; i < msons.size(); i++) {
			num = num + msons[i]->numLeaves();
		}
	}

	return num;
}

int ClassifierTree::numNodes() {

	int no = 1;
	int i;

	if (!misLeaf) {
		for (i = 0; i < msons.size(); i++) {
			no = no + msons[i]->numNodes();
		}
	}

	return no;
}

std::string ClassifierTree::toString() {

	try {
		StringBuffer *text = new StringBuffer();

		if (misLeaf) {
			text->append(L": ");
			text->append(mlocalModel->dumpLabel(0, mtrain));
		}
		else {
			dumpTree(0, text);
		}
		text->append(std::string(L"\n\nNumber of Leaves  : \t") + numLeaves() + std::string(L"\n"));
		text->append(std::string(L"\nSize of the tree : \t") + numNodes() + std::string(L"\n"));

		//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
		return text->toString();
	}
	catch (std::exception &e) {
		return L"Can't print classification tree.";
	}
}

ClassifierTree *ClassifierTree::getNewTree(Instances *data) throw(std::exception) {

	ClassifierTree *newTree = new ClassifierTree(mtoSelectModel);
	newTree->buildTree(data, false);

	return newTree;
}

ClassifierTree *ClassifierTree::getNewTree(Instances *train, Instances *test) throw(std::exception) {

	ClassifierTree *newTree = new ClassifierTree(mtoSelectModel);
	newTree->buildTree(train, test, false);

	return newTree;
}

void ClassifierTree::dumpTree(int depth, StringBuffer *text) throw(std::exception) {

	int i, j;

	for (i = 0; i < msons.size(); i++) {
		text->append(L"\n");
		;
		for (j = 0; j < depth; j++) {
			text->append(L"|   ");
		}
		text->append(mlocalModel->leftSide(mtrain));
		text->append(mlocalModel->rightSide(i, mtrain));
		if (msons[i]->misLeaf) {
			text->append(L": ");
			text->append(mlocalModel->dumpLabel(i, mtrain));
		}
		else {
			msons[i]->dumpTree(depth + 1, text);
		}
	}
}

void ClassifierTree::graphTree(StringBuffer *text) throw(std::exception) {

	for (int i = 0; i < msons.size(); i++) {
		text->append(std::string(L"N") + mid + std::string(L"->") + std::string(L"N") + msons[i]->mid + std::string(L" [label=\"") + Utils::backQuoteChars(mlocalModel->rightSide(i, mtrain)->trim()) + std::string(L"\"]\n"));
		if (msons[i]->misLeaf) {
			text->append(std::string(L"N") + msons[i]->mid + std::string(L" [label=\"") + Utils::backQuoteChars(mlocalModel->dumpLabel(i, mtrain)) + std::string(L"\" ") + std::string(L"shape=box style=filled "));
			if (mtrain != nullptr && mtrain->numInstances() > 0) {
				text->append(std::string(L"data =\n") + msons[i]->mtrain + std::string(L"\n"));
				text->append(L",\n");
			}
			text->append(L"]\n");
		}
		else {
			text->append(std::string(L"N") + msons[i]->mid + std::string(L" [label=\"") + Utils::backQuoteChars(msons[i]->mlocalModel.leftSide(mtrain)) + std::string(L"\" "));
			if (mtrain != nullptr && mtrain->numInstances() > 0) {
				text->append(std::string(L"data =\n") + msons[i]->mtrain + std::string(L"\n"));
				text->append(L",\n");
			}
			text->append(L"]\n");
			msons[i]->graphTree(text);
		}
	}
}

void ClassifierTree::prefixTree(StringBuffer *text) throw(std::exception) {

	text->append(L"[");
	text->append(mlocalModel->leftSide(mtrain) + std::string(L":"));
	for (int i = 0; i < msons.size(); i++) {
		if (i > 0) {
			text->append(L",\n");
		}
		text->append(mlocalModel->rightSide(i, mtrain));
	}
	for (int i = 0; i < msons.size(); i++) {
		if (msons[i]->misLeaf) {
			text->append(L"[");
			text->append(mlocalModel->dumpLabel(i, mtrain));
			text->append(L"]");
		}
		else {
			msons[i]->prefixTree(text);
		}
	}
	text->append(L"]");
}

double ClassifierTree::getProbsLaplace(int classIndex, Instance *instance, double weight) throw(std::exception) {

	double prob = 0;

	if (misLeaf) {
		return weight * localModel()->classProbLaplace(classIndex, instance, -1);
	}
	else {
		int treeIndex = localModel()->whichSubset(instance);
		if (treeIndex == -1) {
			std::vector<double> weights = localModel()->weights(instance);
			for (int i = 0; i < msons.size(); i++) {
				if (!son(i)->misEmpty) {
					prob += son(i)->getProbsLaplace(classIndex, instance, weights[i] * weight);
				}
			}
			return prob;
		}
		else {
			if (son(treeIndex)->misEmpty) {
				return weight * localModel()->classProbLaplace(classIndex, instance, treeIndex);
			}
			else {
				return son(treeIndex)->getProbsLaplace(classIndex, instance, weight);
			}
		}
	}
}

double ClassifierTree::getProbs(int classIndex, Instance *instance, double weight) throw(std::exception) {

	double prob = 0;

	if (misLeaf) {
		return weight * localModel()->classProb(classIndex, instance, -1);
	}
	else {
		int treeIndex = localModel()->whichSubset(instance);
		if (treeIndex == -1) {
			std::vector<double> weights = localModel()->weights(instance);
			for (int i = 0; i < msons.size(); i++) {
				if (!son(i)->misEmpty) {
					prob += son(i)->getProbs(classIndex, instance, weights[i] * weight);
				}
			}
			return prob;
		}
		else {
			if (son(treeIndex)->misEmpty) {
				return weight * localModel()->classProb(classIndex, instance, treeIndex);
			}
			else {
				return son(treeIndex)->getProbs(classIndex, instance, weight);
			}
		}
	}
}

ClassifierSplitModel *ClassifierTree::localModel() {

	return mlocalModel;
}

ClassifierTree *ClassifierTree::son(int index) {

	return msons[index];
}

std::vector<double> ClassifierTree::getMembershipValues(Instance *instance) throw(std::exception) {

	// Set up array for membership values
	std::vector<double> a(numNodes());

	// Initialize queues
	std::list<double> queueOfWeights;
	std::list<ClassifierTree*> queueOfNodes;
	queueOfWeights.push_back(instance->weight());
	queueOfNodes.push_back(this);
	int index = 0;

	// While the queue is not empty
	while (!queueOfNodes.empty()) {

		a[index++] = queueOfWeights.pop_front();
		ClassifierTree *node = queueOfNodes.pop_front();

		// Is node a leaf?
		if (node->misLeaf) {
			continue;
		}

		// Which subset?
		int treeIndex = node->localModel()->whichSubset(instance);

		// Space for weight distribution
		std::vector<double> weights(node->msons.size());

		// Check for missing value
		if (treeIndex == -1) {
			weights = node->localModel()->weights(instance);
		}
		else {
			weights[treeIndex] = 1.0;
		}
		for (int i = 0; i < node->msons.size(); i++) {
			queueOfNodes.push_back(node->son(i));
			queueOfWeights.push_back(a[index - 1] * weights[i]);
		}
	}
	return a;
}

