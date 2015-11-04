#include "ContingencyTables.h"

const double ContingencyTables::MAX_INT_FOR_CACHE_PLUS_ONE = 10000;
std::vector<double> ContingencyTables::INT_N_LOG_N_CACHE = std::vector<double>( static_cast<int>( MAX_INT_FOR_CACHE_PLUS_ONE ) );

//JAVA TO C++ CONVERTER TODO TASK: Static constructors are not available in native C++:
ContingencyTables::ContingencyTables() {
  for( int i = 1; i < MAX_INT_FOR_CACHE_PLUS_ONE; i++ ) {
    double d = static_cast<double>( i );
    INT_N_LOG_N_CACHE[i] = d * log( d );
  }
}

double ContingencyTables::lnFunc( double num ) {

  if( num <= 0 ) {
    return 0;
  } else {

    // Use cache if we have a sufficiently small integer
    if( num < MAX_INT_FOR_CACHE_PLUS_ONE ) {
      int n = static_cast<int>( num );
      if( static_cast<double>( n ) == num ) {
        return INT_N_LOG_N_CACHE[n];
      }
    }
    return num * log( num );
  }
}
