# c48
C++ implementation of J4.8 algorithm

*c48 <source> <flag> <options>*

SOURCE:

-*train <training data>* - fileStem.names|fileStem.data

-*test <testing data>* - fileStem.test (optional)

FLAG:

-*D* - To dump tree structure for classification

-*U* - To use unpruned tree

-*R* - To use reduced error pruning

-*S* - Don't perform subtree raising

-*L* - Do not clean up after the tree has been built

-*A* - Laplace smoothing for predicted probabilities

OPTIONS:

-*C <pruning confidence>* - Set confidence threshold for pruning (default .25)

-*M <minimum number of instances>* - Set minimum number of instances per leaf (default 2)

-*N <number of folds>* - Set number of folds for reduced error pruning. One fold is used as pruning set (default 3)
