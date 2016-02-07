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

    mToSelectModel = toSelectLocModel;

}

ClassifierTree::~ClassifierTree()
{
    if (mToSelectModel != nullptr)
        delete mToSelectModel;
    if (mLocalModel != nullptr)
        delete mLocalModel;
}

void ClassifierTree::buildClassifier(Instances &data){

    // remove instances with missing class
    data = new Instances(data);
    data.deleteWithMissingClass();

    buildTree(data, false);
}

void ClassifierTree::buildTree(Instances &data, bool keepData) {

    std::vector<Instances*> localInstances;

    if (keepData) {
        mTrain = new Instances(&data);
    }
    mTest = nullptr;;
    mIsLeaf = false;
    mIsEmpty = false;
    mSons.clear();
    mLocalModel = mToSelectModel->selectModel(data);
    if (mLocalModel->numSubsets() > 1) {
        localInstances = mLocalModel->split(data);
        
        mSons = std::vector<ClassifierTree*>(mLocalModel->numSubsets());
        int totalSons = (int)mSons.size();
        for (int i = 0; i < totalSons; i++) {
            mSons[i] = getNewTree(*localInstances[i]);
            //delete (localInstances[i]);
        }
    }
    else {
        mIsLeaf = true;
        if (Utils::eq(data.sumOfWeights(), 0)) {
            mIsEmpty = true;
        }
    }
}

void ClassifierTree::buildTree(Instances &train, Instances &test, bool keepData) {

    std::vector<Instances*> localTrain, localTest;
    int i;

    if (keepData) {
        mTrain = &train;
    }
    mIsLeaf = false;
    mIsEmpty = false;
    mSons.clear();
    mLocalModel = mToSelectModel->selectModel(train, test);
    mTest = new Distribution(test, *mLocalModel);
    if (mLocalModel->numSubsets() > 1) {
        localTrain = mLocalModel->split(train);
        localTest = mLocalModel->split(test);
        //delete train;
        //delete test;
        mSons = std::vector<ClassifierTree*>(mLocalModel->numSubsets());
        for (i = 0; i < mSons.size(); i++) {
            mSons[i] = getNewTree(*localTrain[i], *localTest[i]);
            //delete localTrain[i];
            //delete localTest[i];
        }
    }
    else {
        mIsLeaf = true;
        if (Utils::eq(train.sumOfWeights(), 0)) {
            mIsEmpty = true;
        }
    }
}

double ClassifierTree::classifyInstance(Instance &instance) const{

    double maxProb = -1;
    double currentProb;
    int maxIndex = 0;
    int j;

    for (j = 0; j < instance.numClasses(); j++) {
        currentProb = getProbs(j, instance, 1);
        if (Utils::gr(currentProb, maxProb)) {
            maxIndex = j;
            maxProb = currentProb;
        }
    }

    return maxIndex;
}

void ClassifierTree::cleanup(Instances &justHeaderInfo) {

    mTrain = &justHeaderInfo;
    if (!mIsLeaf) {
        for (auto mSon : mSons) {
            mSon->cleanup(justHeaderInfo);
        }
    }
}

double_array ClassifierTree::distributionForInstance(Instance &instance, bool useLaplace) {

    double_array doubles(instance.numClasses());

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

    mID = currLastID;
    if (mSons.size() > 0) {
        for (auto mSon : mSons) {
            currLastID = mSon->assignIDs(currLastID);
        }
    }
    return currLastID;
}

int ClassifierTree::numLeaves() const{

    int num = 0;
    int i;

    if (mIsLeaf) {
        return 1;
    }
    else {
        for (i = 0; i < mSons.size(); i++) {
            num = num + mSons[i]->numLeaves();
        }
    }

    return num;
}

int ClassifierTree::numNodes() const {

    int no = 1;
    int i;

    if (!mIsLeaf) {
        for (i = 0; i < mSons.size(); i++) {
            no = no + mSons[i]->numNodes();
        }
    }

    return no;
}

string ClassifierTree::toString(bool isDumpTree) const {

    try {
        string text = "";
        if (isDumpTree)
        {
            if (mIsLeaf) {
                text.append(": ");
                text.append(mLocalModel->dumpLabel(0, *mTrain));
            }
            else {
                dumpTree(0, text);
            }
        }
        text.append(string("\n\nNumber of Leaves  : \t") + std::to_string(numLeaves()) + string("\n"));
        text.append(string("\nSize of the tree : \t") + std::to_string(numNodes()) + string("\n"));

        return text;
    }
    catch (std::exception &e) {
        return "Can't print classification tree.";
    }
}

ClassifierTree *ClassifierTree::getNewTree(Instances &data) const{

    ClassifierTree *newTree = new ClassifierTree(mToSelectModel);
    newTree->buildTree(data, false);

    return newTree;
}

ClassifierTree *ClassifierTree::getNewTree(Instances &train, Instances &test) const{

    ClassifierTree *newTree = new ClassifierTree(mToSelectModel);
    newTree->buildTree(train, test, false);

    return newTree;
}

void ClassifierTree::dumpTree(int depth, string &text) const {

    int i, j;

    for (i = 0; i < mSons.size(); i++) {
        text.append("\n");
        ;
        for (j = 0; j < depth; j++) {
            text.append("|   ");
        }
        text.append(mLocalModel->leftSide(*mTrain));
        text.append(mLocalModel->rightSide(i, *mTrain));
        if (mSons[i]->mIsLeaf) {
            text.append(": ");
            text.append(mLocalModel->dumpLabel(i, *mTrain));
        }
        else {
            mSons[i]->dumpTree(depth + 1, text);
        }
    }
}

double ClassifierTree::getProbsLaplace(int classIndex, Instance &instance, double weight) const{

    double prob = 0;

    if (mIsLeaf) {
        return weight * localModel()->classProbLaplace(classIndex, instance, -1);
    }
    else {
        int treeIndex = localModel()->whichSubset(instance);
        if (treeIndex == -1) {
            double_array weights = localModel()->weights(instance);
            for (int i = 0; i < mSons.size(); i++) {
                if (!mSons[i]->mIsEmpty) {
                    prob += mSons[i]->getProbsLaplace(classIndex, instance, weights[i] * weight);
                }
            }
            return prob;
        }
        else {
            if (mSons[treeIndex]->mIsEmpty) {
                return weight * localModel()->classProbLaplace(classIndex, instance, treeIndex);
            }
            else {
                return mSons[treeIndex]->getProbsLaplace(classIndex, instance, weight);
            }
        }
    }
}

double ClassifierTree::getProbs(int classIndex, Instance &instance, double weight) const{

    double prob = 0;

    if (mIsLeaf) {
        return weight * localModel()->classProb(classIndex, instance, -1);
    }
    else {
        int treeIndex = localModel()->whichSubset(instance);
        if (treeIndex == -1) {
            double_array weights = localModel()->weights(instance);
            for (int i = 0; i < mSons.size(); i++) {
                if (!mSons[i]->mIsEmpty) {
                    prob += mSons[i]->getProbs(classIndex, instance, weights[i] * weight);
                }
            }
            return prob;
        }
        else {
            if (mSons[treeIndex]->mIsEmpty) {
                return weight * localModel()->classProb(classIndex, instance, treeIndex);
            }
            else {
                return mSons[treeIndex]->getProbs(classIndex, instance, weight);
            }
        }
    }
}

ClassifierSplitModel *ClassifierTree::localModel() const{

    return mLocalModel;
}

ClassifierTree *ClassifierTree::son(int index) {

    return mSons[index];
}

double_array ClassifierTree::getMembershipValues(Instance &instance) {

    // Set up array for membership values
    double_array a(numNodes());

    // Initialize queues
    std::list<double> queueOfWeights;
    std::list<ClassifierTree*> queueOfNodes;
    queueOfWeights.push_back(instance.weight());
    queueOfNodes.push_back(this);
    int index = 0;

    // While the queue is not empty
    while (!queueOfNodes.empty()) {

        a[index++] = queueOfWeights.front();
        queueOfWeights.pop_front();
        ClassifierTree *node = queueOfNodes.front();
        queueOfNodes.pop_front();

        // Is node a leaf?
        if (node->mIsLeaf) {
            continue;
        }

        // Which subset?
        int treeIndex = node->localModel()->whichSubset(instance);

        // Space for weight distribution
        double_array weights(node->mSons.size());

        // Check for missing value
        if (treeIndex == -1) {
            weights = node->localModel()->weights(instance);
        }
        else {
            weights[treeIndex] = 1.0;
        }
        for (int i = 0; i < node->mSons.size(); i++) {
            queueOfNodes.push_back(node->mSons[i]);
            queueOfWeights.push_back(a[index - 1] * weights[i]);
        }
    }
    return a;
}

