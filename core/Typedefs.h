#ifndef __C48_TYPEDEFS_HPP__
#define __C48_TYPEDEFS_HPP__

#include <vector>

typedef std::vector<double> double_array;
typedef std::vector<std::vector<double>> double_array__array;
typedef std::vector<int> int_array;

#ifdef ARMA_64BIT_WORD
typedef std::vector<unsigned long long> uint_array;
#else
typedef std::vector<unsigned> uint_array;
#endif

typedef std::pair<int, float> weight_t;
typedef std::vector<weight_t> weights_t;


#endif // __C48_TYPEDEFS_HPP__