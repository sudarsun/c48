#ifndef _CONTINGENCYTABLES_
#define _CONTINGENCYTABLES_

#include <vector>
#include <cmath>

/// <summary>
/// Class implementing some statistical routines for contingency tables.
/// 
/// @author Eibe Frank (eibe@cs.waikato.ac.nz)
/// @version $Revision: 10057 $
/// </summary>
class ContingencyTables {

  /// <summary>
   /// <summary>
  /// Cache of integer logs </summary>
  private:
  static const double MAX_INT_FOR_CACHE_PLUS_ONE;
  static std::vector<double> INT_N_LOG_N_CACHE;

  /// <summary>
  /// Initialize cache </summary>
//JAVA TO C++ CONVERTER TODO TASK: Static constructors are not available in native C++:
  ContingencyTables();

/// <summary>
/// Help method for computing entropy.
/// </summary>
  public:
  static double lnFunc( double num );


};










#endif	//#ifndef _CONTINGENCYTABLES_
