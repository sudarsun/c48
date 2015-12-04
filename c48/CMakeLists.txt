include_directories(${CMAKE_SOURCE_DIR})

set(c48_SRCS
AbstractClassifier.cpp
AbstractClassifier.h
C45ModelSelection.cpp
C45ModelSelection.h
C45PruneableClassifierTree.cpp
C45PruneableClassifierTree.h
C45Split.cpp
C45Split.h
C48.cpp
C48.h
Classifier.h
ClassifierSplitModel.cpp
ClassifierSplitModel.h
ClassifierTree.cpp
ClassifierTree.h
Distribution.cpp
Distribution.h
EntropyBasedSplitCrit.cpp
EntropyBasedSplitCrit.h
GainRatioSplitCrit.cpp
GainRatioSplitCrit.h
InfoGainSplitCrit.cpp
InfoGainSplitCrit.h
ModelSelection.cpp
ModelSelection.h
NoSplit.cpp
NoSplit.h
SplitCriterion.cpp
SplitCriterion.h
Stats.cpp
Stats.h
)

add_library(c48 STATIC ${c48_SRCS})

#target_include_directories(c48 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
