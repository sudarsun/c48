#include "CostMatrix.h"
#include "core/Instances.h"
#include "core/Instance.h"


string CostMatrix::FILE_EXTENSION = ".cost";

CostMatrix::CostMatrix(int numOfClasses = 0) {
    mSize = numOfClasses;
    initialize();
}

CostMatrix::CostMatrix(CostMatrix *toCopy) {
    if (toCopy != nullptr)
    {
        CostMatrix(toCopy->size());
        for (int i = 0; i < mSize; i++) {
            for (int j = 0; j < mSize; j++) {
                setCell(i, j, toCopy->getCell(i, j));
            }
        }
    }
}

void CostMatrix::initialize() {

    //ORIGINAL LINE: mMatrix = new Object[mSize][mSize];
    mMatrix.resize(mSize, std::vector<void*>(mSize, 0));

    for (int i = 0; i < mSize; i++) {
        for (int j = 0; j < mSize; j++) {
            double p = i == j ? 0.0 : 1.0;
            setCell(i, j, static_cast<void *>(&p));
        }
    }
}

int CostMatrix::size() {
    return mSize;
}

int CostMatrix::numColumns() {
    return size();
}

int CostMatrix::numRows() {
    return size();
}

bool CostMatrix::replaceStrings(Instances *dataset) {
    bool nonDouble = false;

    for (int i = 0; i < mSize; i++) {
        for (int j = 0; j < mSize; j++) {
            //if( dynamic_cast<string>( getCell( i, j ) ) != nullptr ) {
           //      setCell(i, j, new InstanceExpression(static_cast<string>(getCell(i, j)), dataset));
            //  nonDouble = true;
            //} else
            double *p = static_cast<double*>(getCell(i, j));
            if (p != nullptr) {
                nonDouble = true;
            }
        }
    }

    return nonDouble;
}

/*
Instances *CostMatrix::applyCostMatrix( Instances *data, Random *random ) {

  double sumOfWeightFactors = 0, sumOfMissClassWeights, sumOfWeights;
  double_array weightOfInstancesInClass, weightFactor, weightOfInstances;

  if( data->classIndex() < 0 ) {
    throw std::exception( "Class index is not set!" );
  }

  if( Size() != data->numClasses() ) {
    throw std::exception( "Misclassification cost matrix has wrong format!" );
  }

  // are there any non-fixed, per-instance costs defined in the matrix?
  if( replaceStrings( data ) ) {
    // could reweight in the two class case
    if( data->classAttribute()->numValues() > 2 ) {
      throw std::exception("Can't resample/reweight instances using non-fixed cost values when there are more than two classes!");
    } else {
      // Store new weights
      weightOfInstances = double_array( data->numInstances() );
      for( int i = 0; i < data->numInstances(); i++ ) {
        Instance *inst = data->instance( i );
        int classValIndex = static_cast<int>( inst->classValue() );
        double factor = 1.0;
        auto element = ( classValIndex == 0 ) ? getCell( classValIndex, 1 ) : getCell( classValIndex, 0 );
        if( element != nullptr ) {
          factor = ( static_cast<double>( element ) );
        } else {
          factor = ( static_cast<InstanceExpression*>( element ) )->evaluate( inst );
        }
        weightOfInstances[i] = inst->weight() * factor;
      }

      // Change instances weight or do resampling
      if( random != nullptr ) {
        return data->resampleWithWeights( random, weightOfInstances );
      } else {
        Instances *instances = new Instances( data );
        for( int i = 0; i < data->numInstances(); i++ ) {
          instances->instance( i )->setWeight( weightOfInstances[i] );
        }
        return instances;
      }
    }
  }

  weightFactor = double_array( data->numClasses() );
  weightOfInstancesInClass = double_array( data->numClasses() );
  for( int j = 0; j < data->numInstances(); j++ ) {
    weightOfInstancesInClass[static_cast<int>( data->instance( j ).classValue() )] += data->instance(j).weight();
  }
  sumOfWeights = Utils::sum( weightOfInstancesInClass );

  // normalize the matrix if not already
  for( int i = 0; i < mSize; i++ ) {
    if( !Utils::eq( ( static_cast<double>( getCell( i, i ) ) ), 0 ) ) {
      CostMatrix *normMatrix = new CostMatrix( this );
      normMatrix->normalize();
      return normMatrix->applyCostMatrix( data, random );
    }
  }

  for( int i = 0; i < data->numClasses(); i++ ) {
    // Using Kai Ming Ting's formula for deriving weights for
    // the classes and Breiman's heuristic for multiclass
    // problems.

    sumOfMissClassWeights = 0;
    for( int j = 0; j < data->numClasses(); j++ ) {
      if( Utils::sm( ( static_cast<double>( getCell( i, j ) ) ), 0 ) ) {
        throw std::exception( "Neg. weights in misclassification cost matrix!" );
      }
      sumOfMissClassWeights += ( static_cast<double>( getCell( i, j ) ) );
    }
    weightFactor[i] = sumOfMissClassWeights * sumOfWeights;
    sumOfWeightFactors += sumOfMissClassWeights * weightOfInstancesInClass[i];
  }
  for( int i = 0; i < data->numClasses(); i++ ) {
    weightFactor[i] /= sumOfWeightFactors;
  }

  // Store new weights
  weightOfInstances = double_array( data->numInstances() );
  for( int i = 0; i < data->numInstances(); i++ ) {
    weightOfInstances[i] = data->instance( i )->weight() * weightFactor[static_cast<int>(data->instance(i)->classValue())];
  }

  // Change instances weight or do resampling
  if( random != nullptr ) {
    return data->resampleWithWeights( random, weightOfInstances );
  } else {
    Instances *instances = new Instances( data );
    for( int i = 0; i < data->numInstances(); i++ ) {
      instances->instance( i )->setWeight( weightOfInstances[i] );
    }
    return instances;
  }
}*/

double_array CostMatrix::expectedCosts(double_array &classProbs) {

    if (classProbs.size() != mSize) {
        throw "Length of probability estimates don't match cost matrix";
    }

    double_array costs(mSize);

    for (int x = 0; x < mSize; x++) {
        for (int y = 0; y < mSize; y++) {
            auto element = static_cast<double*>(getCell(y, x));
            if (!(element != nullptr)) {
                throw "Can't use non-fixed costs in computing expected costs.";
            }
            costs[x] += classProbs[y] * (*element);
        }
    }

    return costs;
}

/*double_array CostMatrix::expectedCosts( double_array &classProbs, Instance *inst ) {

  if( classProbs.size() != mSize ) {
    throw std::exception( "Length of probability estimates don't match cost matrix");
  }

  if( !replaceStrings( inst->dataset() ) ) {
    return expectedCosts( classProbs );
  }

  double_array costs( mSize );

  for( int x = 0; x < mSize; x++ ) {
    for( int y = 0; y < mSize; y++ ) {
      auto element = static_cast<double*>(getCell( y, x ));
      double costVal;
      if( !(  element != nullptr ) ) {
          ;// costVal = (static_cast<InstanceExpression*>(element))->evaluate(inst);
      } else {
        costVal =  *element ;
      }
      costs[x] += classProbs[y] * costVal;
    }
  }

  return costs;
}*/

double CostMatrix::getMaxCost(int classVal) {

    double maxCost = -std::numeric_limits<double>::infinity();

    for (int i = 0; i < mSize; i++) {
        auto element = static_cast<double*>(getCell(classVal, i));
        if (!(element != nullptr)) {
            throw "Can't use non-fixed costs when getting max cost.";
        }
        double cost = *element;
        if (cost > maxCost) {
            maxCost = cost;
        }
    }

    return maxCost;
}

double CostMatrix::getMaxCost(int classVal, Instance *inst) {

    if (!replaceStrings(inst->getDataset())) {
        return getMaxCost(classVal);
    }

    double maxCost = -std::numeric_limits<double>::infinity();
    double cost;
    for (int i = 0; i < mSize; i++) {
        auto element = static_cast<double*>(getCell(classVal, i));
        if (!(element != nullptr)) {
            ;// cost = (static_cast<InstanceExpression*>(element))->evaluate(inst);
        }
        else {
            cost = (*element);
        }
        if (cost > maxCost) {
            maxCost = cost;
        }
    }

    return maxCost;
}

void CostMatrix::normalize() {

    for (int y = 0; y < mSize; y++) {
        double *diag = (static_cast<double*>(getCell(y, y)));
        for (int x = 0; x < mSize; x++) {
            double *val = static_cast<double*>(getCell(x, y));
            double cellvalue = *val - *diag;
            setCell(x, y, static_cast<void*>(&cellvalue));
        }
    }
}

void CostMatrix::setCell(int rowIndex, int columnIndex, void *value) {
    mMatrix[rowIndex][columnIndex] = value;
}

void *CostMatrix::getCell(int rowIndex, int columnIndex) {
    return mMatrix[rowIndex][columnIndex];
}

double CostMatrix::getElement(int rowIndex, int columnIndex) {
    auto cell = mMatrix[rowIndex][columnIndex];
    if (!(static_cast<double*>(cell) != nullptr)) {
        throw "Cost matrix contains non-fixed costs!";
    }
    return *(static_cast<double*>(cell));
}

double CostMatrix::getElement(int rowIndex, int columnIndex, Instance *inst) {

    auto cell = mMatrix[rowIndex][columnIndex];
    if (static_cast<double*>(cell) != nullptr) {
        return *(static_cast<double*>(cell));
    }
    else if (static_cast<char *>(cell) != nullptr) {
        ;// replaceStrings(inst->dataset());
    }

    //return ((InstanceExpression) cell.evaluate(inst);
    return 0.0;
}

void CostMatrix::setElement(int rowIndex, int columnIndex, double value) {
    mMatrix[rowIndex][columnIndex] = static_cast<void *>(&value);
}
