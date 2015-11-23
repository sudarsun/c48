include_directories(${CMAKE_SOURCE_DIR})

set(evaluation_SRCS
BatchPredictor.h
CostMatrix.cpp
CostMatrix.h
Evaluation.cpp
Evaluation.h
NominalPrediction.cpp
NominalPrediction.h
NumericPrediction.cpp
NumericPrediction.h
Prediction.cpp
Prediction.h
)

add_library(evaluation STATIC ${evaluation_SRCS})
#target_include_directories(evaluation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
