include_directories(${CMAKE_SOURCE_DIR})

set(core_SRCS
Attribute.cpp
Attribute.h
AttributeInfo.h
AttributeMetaInfo.cpp
AttributeMetaInfo.h
AttributeStats.cpp
AttributeStats.h
Consts.h
DataSource.cpp
DataSource.h
DenseInstance.cpp
DenseInstance.h
Instance.h
Instances.cpp
Instances.h
NominalAttributeInfo.cpp
NominalAttributeInfo.h
statistics.cpp
statistics.h
Utils.cpp
Utils.h
ContingencyTables.cpp
ContingencyTables.h
)

add_library(core STATIC ${core_SRCS})
#target_include_directories(c48 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
