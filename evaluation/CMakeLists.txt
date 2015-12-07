include_directories(${CMAKE_SOURCE_DIR})

set(evaluation_SRCS
BatchPredictor.h
ConfusionMatrix.cpp
ConfusionMatrix.h
CostMatrix.cpp
CostMatrix.h
Estimator.cpp
Estimator.h
EstimatorUtils.cpp
EstimatorUtils.h
Evaluation.cpp
Evaluation.h
IncrementalEstimator.h
KernelEstimator.cpp
KernelEstimator.h
NominalPrediction.cpp
NominalPrediction.h
NumericPrediction.cpp
NumericPrediction.h
Prediction.h
ThresholdCurve.cpp
ThresholdCurve.h
TwoClassStats.cpp
TwoClassStats.h
)

add_library(evaluation STATIC ${evaluation_SRCS})
#target_include_directories(evaluation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
