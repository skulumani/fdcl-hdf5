cmake_minimum_required(VERSION 3.10)
project( fdcl_hdf5 VERSION 0.1 DESCRIPTION "FDCL HDF5 Library")

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)
set(DEFAULT_BUILD_TYPE "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(GNUInstallDirs)

# HDF5 Setup
set(ENV{HDF5_ROOT} /usr/local/hdf5)
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
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

################################################################################
# BUILD TARGETS
################################################################################
# set some paths for including
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_BINARY_DIR})

set(fdcl_hdf5_src 
    ${PROJECT_SOURCE_DIR}/src/hdf5_eigen.cpp
    ${PROJECT_SOURCE_DIR}/src/hdf5_file.cpp
    ${PROJECT_SOURCE_DIR}/src/hdf5_group.cpp
    ${PROJECT_SOURCE_DIR}/src/hdf5_dataset.cpp)
add_library(${PROJECT_NAME} SHARED ${fdcl_hdf5_src})
target_link_libraries(${PROJECT_NAME} ${HDF5_CXX_LIBRARIES} ${HDF5_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION}
    SOVERSION 1)

add_executable(main ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(main ${PROJECT_NAME})

# TESTING
ENABLE_TESTING()
add_executable(test_hdf5 
    ${PROJECT_SOURCE_DIR}/tests/test_hdf5.cpp 
    ${PROJECT_SOURCE_DIR}/tests/test_all.cpp)
target_link_libraries(test_hdf5 gtest ${PROJECT_NAME})
add_test(NAME test_hdf5 COMMAND test_hdf5
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}) 
