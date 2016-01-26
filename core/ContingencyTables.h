#ifndef _CONTINGENCYTABLES_
#define _CONTINGENCYTABLES_

#include <vector>
#include <cmath>
#include "Typedefs.h"

/**
 * Class implementing some statistical routines for contingency tables.
 *
 */
class ContingencyTables {

private:

    /** Cache of integer logs */
    static const double MAX_INT_FOR_CACHE_PLUS_ONE;
    static double_array INT_N_LOG_N_CACHE;

    /**
     * Initialize cache
     */
    ContingencyTables();

    /**
     * Help method for computing entropy.
     */
public:
    static double lnFunc(const double num);

};

#endif    //#ifndef _CONTINGENCYTABLES_
