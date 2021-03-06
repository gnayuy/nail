
#
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

# sources
SET( nail_sources

# header files
    ${PROJECT_SOURCE_DIR}/src/image.h
    ${PROJECT_SOURCE_DIR}/src/image.hxx
    ${PROJECT_SOURCE_DIR}/src/nail.h

# source files
    ${PROJECT_SOURCE_DIR}/src/nail.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

# gflags
FIND_PACKAGE( GFlags REQUIRED)
IF(GFLAGS_FOUND)
    SET(CMAKE_REQUIRED_LIBRARIES ${GFLAGS_LIBRARIES})
    INCLUDE_DIRECTORIES(${GFLAGS_INCLUDE_DIR})
ENDIF(GFLAGS_FOUND)

# biomedicaldataio
FIND_PACKAGE( BioMedicalDataIO REQUIRED)
IF(BioMedicalDataIO_FOUND)
SET(CMAKE_REQUIRED_LIBRARIES ${BioMedicalDataIO_LIBRARIES})
INCLUDE_DIRECTORIES(${BioMedicalDataIO_INCLUDE_DIR})
ENDIF(BioMedicalDataIO_FOUND)

# Eigen
FIND_PACKAGE(Eigen3 REQUIRED)
IF(EIGEN3_FOUND)
    INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
ENDIF(EIGEN3_FOUND)

#
IF(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
SET(CMAKE_CXX_FLAGS "-std=c++11 -O3 -lm -fopenmp" CACHE STRING "CXX Compiler Flags" FORCE)
ADD_DEFINITIONS(-DUse_OpenMP)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
SET(CMAKE_CXX_FLAGS "-std=c++11 -O3" CACHE STRING "CXX Compiler Flags" FORCE)
SET(CMAKE_MACOSX_RPATH 1)
ENDIF()

#
ADD_EXECUTABLE(nail ${nail_sources})
TARGET_LINK_LIBRARIES(nail ${GFLAGS_LIBRARIES} ${BioMedicalDataIO_LIBRARIES})

