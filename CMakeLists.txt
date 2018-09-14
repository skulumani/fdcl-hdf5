cmake_minimum_required(VERSION 3.9)
project( fdcl_hdf5 VERSION 1.0 DESCRIPTION "FDCL HDF5 Library")

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)
set(DEFAULT_BUILD_TYPE "Release")
set(COVERAGE OFF CACHE BOOL "Coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

################################################################################
# COVERAGE
################################################################################
# we use this to get code coverage
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

################################################################################
# DEPENDENCIES
################################################################################
# HDF5 Setup
if(LINUX)
    set(ENV{HDF5_ROOT} /usr/local/hdf5)
elseif(APPLE)

endif()

find_package(HDF5 REQUIRED COMPONENTS C CXX)
if (NOT HDF5_FOUND)
    message(STATUS "This project requires HDF5 and will not compile")
    return()
else()
    message(STATUS "Found HDF5: ${HDF5_VERSION}")
endif()

if ( HDF5_IS_PARALLEL )
    message(STATUS "HDF5 parallel is installed")
else()
    message(STATUS "HDF5 parallel is NOT installed")
endif()
# message(STATUS "HDF5 Lib: ${HDF5_LIBRARIES}")
include_directories(${HDF5_INCLUDE_DIRS})
set(hdf5_libs hdf5 hdf5_cpp)

# Find Eigen
find_package(Eigen3 REQUIRED)
if (NOT Eigen3_FOUND)
    message(STATUS "This project requires Eigen3 and will not be compiled.")
    return()
endif()
include(${EIGEN3_USE_FILE})

################################################################################
# GOOGLE TESTING
################################################################################
add_subdirectory(${PROJECT_SOURCE_DIR}/extern/googletest/googletest)
ENABLE_TESTING()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
################################################################################
# BUILD TARGETS
################################################################################
# set some paths for including
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include(GNUInstallDirs)

set(fdcl_hdf5_src 
    ${PROJECT_SOURCE_DIR}/src/hdf5_eigen.cpp
    ${PROJECT_SOURCE_DIR}/src/hdf5_file.cpp
    ${PROJECT_SOURCE_DIR}/src/hdf5_group.cpp
    ${PROJECT_SOURCE_DIR}/src/hdf5_dataset.cpp)

add_library(${PROJECT_NAME} SHARED ${fdcl_hdf5_src})
target_link_libraries(${PROJECT_NAME} ${HDF5_CXX_LIBRARIES} ${HDF5_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION}
    SOVERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES
    PUBLIC_HEADER "include/hdf5.hpp;include/hdf5_dataset.hpp;include/hdf5_eigen.hpp;include/hdf5_file.hpp;include/hdf5_group.hpp")
add_executable(main ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(main ${PROJECT_NAME})

################################################################################
# INSTALL 
################################################################################
install(TARGETS ${PROJECT_NAME} EXPORT FDCL_HDF5Config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT FDCL_HDF5Config DESTINATION share/FDCL_HDF5/cmake)

export(TARGETS ${PROJECT_NAME} FILE FDCL_HDF5Config.cmake)

################################################################################
# TESTING
################################################################################
add_executable(test_hdf5 
    ${PROJECT_SOURCE_DIR}/tests/test_hdf5.cpp 
    ${PROJECT_SOURCE_DIR}/tests/test_eigen_sizes.cpp
    ${PROJECT_SOURCE_DIR}/tests/test_all.cpp)
target_link_libraries(test_hdf5 PRIVATE gtest ${PROJECT_NAME})
add_test(NAME test_hdf5 COMMAND test_hdf5
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})


