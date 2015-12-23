#include "CostMatrix.h"
#include "core/Instances.h"
//#include "core/InstanceExpression.h"
#include "core/Instance.h"


string CostMatrix::FILE_EXTENSION = ".cost";

CostMatrix::CostMatrix( int numOfClasses = 0 ) {
  m_size = numOfClasses;
  initialize();
}

CostMatrix::CostMatrix( CostMatrix *toCopy )  {
	if (toCopy != nullptr)
	{
		CostMatrix(toCopy->size());
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				setCell(i, j, toCopy->getCell(i, j));
			}
		}
	}
}

void CostMatrix::initialize() {

//ORIGINAL LINE: m_matrix = new Object[m_size][m_size];
  m_matrix.resize(m_size, std::vector<void*>(m_size, 0));
  
  for( int i = 0; i < m_size; i++ ) {
    for( int j = 0; j < m_size; j++ ) {
		double p = i == j ? 0.0: 1.0;
      setCell( i, j, static_cast<void *>(&p));
    }
  }
}

int CostMatrix::size() {
  return m_size;
}

int CostMatrix::numColumns() {
  return size();
}

int CostMatrix::numRows() {
  return size();
}

bool CostMatrix::replaceStrings( Instances *dataset ) {
  bool nonDouble = false;

  for( int i = 0; i < m_size; i++ ) {
    for( int j = 0; j < m_size; j++ ) {
      //if( dynamic_cast<string>( getCell( i, j ) ) != nullptr ) {
	 //	  setCell(i, j, new InstanceExpression(static_cast<string>(getCell(i, j)), dataset));
      //  nonDouble = true;
      //} else
		double *p = static_cast<double*>(getCell(i, j));
		if( p != nullptr ) {
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
  for( int i = 0; i < m_size; i++ ) {
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

double_array CostMatrix::expectedCosts( double_array &classProbs ) {

  if( classProbs.size() != m_size ) {
    throw "Length of probability estimates don't match cost matrix";
  }

  double_array costs( m_size );

  for( int x = 0; x < m_size; x++ ) {
    for( int y = 0; y < m_size; y++ ) {
      auto element = static_cast<double*>(getCell( y, x ));
      if( !( element  != nullptr ) ) {
        throw "Can't use non-fixed costs in computing expected costs.";
      }
      costs[x] += classProbs[y] * (*element) ;
    }
  }

  return costs;
}

/*double_array CostMatrix::expectedCosts( double_array &classProbs, Instance *inst ) {

  if( classProbs.size() != m_size ) {
    throw std::exception( "Length of probability estimates don't match cost matrix");
  }

  if( !replaceStrings( inst->dataset() ) ) {
    return expectedCosts( classProbs );
  }

  double_array costs( m_size );

  for( int x = 0; x < m_size; x++ ) {
    for( int y = 0; y < m_size; y++ ) {
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

double CostMatrix::getMaxCost( int classVal ) {

  double maxCost = -std::numeric_limits<double>::infinity();

  for( int i = 0; i < m_size; i++ ) {
    auto element = static_cast<double*>(getCell( classVal, i ));
    if( !(  element != nullptr ) ) {
      throw "Can't use non-fixed costs when getting max cost.";
    }
    double cost = *element;
    if( cost > maxCost ) {
      maxCost = cost;
    }
  }

  return maxCost;
}

double CostMatrix::getMaxCost( int classVal, Instance *inst ) {

  if( !replaceStrings( inst->getDataset() ) ) {
    return getMaxCost( classVal );
  }

  double maxCost = -std::numeric_limits<double>::infinity();
  double cost;
  for( int i = 0; i < m_size; i++ ) {
    auto element = static_cast<double*>(getCell( classVal, i ));
    if( !( element != nullptr ) ) {
		;// cost = (static_cast<InstanceExpression*>(element))->evaluate(inst);
    } else {
      cost = ( *element );
    }
    if( cost > maxCost ) {
      maxCost = cost;
    }
  }

  return maxCost;
}

void CostMatrix::normalize() {

  for( int y = 0; y < m_size; y++ ) {
    double *diag = ( static_cast<double*>( getCell( y, y ) ) );
    for( int x = 0; x < m_size; x++ ) {
		double *val = static_cast<double*>(getCell(x, y));
		double cellvalue = *val - *diag;		
      setCell( x, y, static_cast<void*>(&cellvalue) );
    }
  }
}

void CostMatrix::setCell( int rowIndex, int columnIndex, void *value ) {
  m_matrix[rowIndex][columnIndex] = value;
}

void *CostMatrix::getCell( int rowIndex, int columnIndex ) {
  return m_matrix[rowIndex][columnIndex];
}

double CostMatrix::getElement( int rowIndex, int columnIndex ) {
	auto cell = m_matrix[rowIndex][columnIndex];
  if( !(static_cast<double*>(cell) != nullptr ) ) {
    throw "Cost matrix contains non-fixed costs!";
  }
  return *( static_cast<double*>(cell ) );
}

double CostMatrix::getElement( int rowIndex, int columnIndex, Instance *inst ) {

	auto cell = m_matrix[rowIndex][columnIndex];
	if(static_cast<double*>(cell) != nullptr ) {
    return *(static_cast<double*>( cell ) );
  } else if( static_cast<char *>( cell ) != nullptr ) {
	  ;// replaceStrings(inst->dataset());
  }

  //return ((InstanceExpression) cell.evaluate(inst);
  return 0.0;
}

void CostMatrix::setElement( int rowIndex, int columnIndex, double value ) {
  m_matrix[rowIndex][columnIndex] = static_cast<void *>( &value );
}
