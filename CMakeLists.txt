cmake_minimum_required(VERSION 3.16)

project(cppproject VERSION 2024.09
              LANGUAGES CXX
              HOMEPAGE_URL "...")


option(LIBONLY "Activate to build library only" OFF)
set(CMAKE_CXX_STANDARD 17)

include(cmake_macros.txt)


# set constants:

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(DIR_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/")
set(DIR_EXTERNAL "${DIR_PROJECT}lib")
set(DIR_BUILD "${DIR_PROJECT}build")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DIR_BUILD}")
set(EXECUTABLE_OUTPUT_PATH "${DIR_BUILD}")
set(LIBRARY_OUTPUT_PATH "${DIR_BUILD}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${DIR_PROJECT})
include(CTest)


# find and add dependencies

include(FetchContent)
set(BUILD_MY_TESTS ${BUILD_TESTING})
set(BUILD_TESTING OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED OFF)

##############################
# -- Eigen
##############################
#FetchContent_Declare(
#  Eigen
#  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
#  GIT_TAG 3.4.0
#  GIT_SHALLOW TRUE
#  GIT_PROGRESS TRUE
#  )
#set(EIGEN_BUILD_TESTING OFF)
#set(EIGEN_MPL2_ONLY ON)
#set(EIGEN_BUILD_PKGCONFIG OFF)
#set(EIGEN_BUILD_DOC ON)
#FetchContent_MakeAvailable(Eigen)


# -- Eigen
#set(EIGEN_DIR "${DIR_EXTERNAL}/eigen-3.4.0")

find_package(Eigen3 3.4 REQUIRED)
link_directories(${Eigen_INCLUDE_DIRS})
include_directories(${EIGEN_DIR})


##############################
# -- googletest
##############################
FetchContent_Declare(
  googletest
  GIT_REPOSITORY  https://github.com/google/googletest.git
  GIT_TAG         v1.17.0
  GIT_PROGRESS TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

##############################
# -- autodiff
##############################
#FetchContent_Declare(
#  autodiff
#  GIT_REPOSITORY  https://github.com/krippner/auto-diff.git
#  GIT_TAG         v0.4.0
#  GIT_SHALLOW TRUE
#  GIT_PROGRESS TRUE
#)
#set(AUTODIFF_BUILD_TESTS OFF CACHE BOOL "Build autodiff tests")
#set(AUTODIFF_BUILD_EXAMPLES OFF CACHE BOOL "Build autodiff examples")
#set(AUTODIFF_BUILD_PYTHON OFF CACHE BOOL "Build autodiff Python bindings")
#set(AUTODIFF_BUILD_DOCS ON CACHE BOOL "Build autodiff documentation")
#FetchContent_MakeAvailable(autodiff)

include_directories(${DIR_PROJECT}/extern/autodiff)

#find_package( Boost 1.46.0 COMPONENTS serialization filesystem REQUIRED)
#include_directories(${Boost_INCLUDE_DIR})

set(BUILD_TESTING ${BUILD_MY_TESTS})


# output directories and constants:

message("Directories: ")
message("  MYCODE_BASE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("  DIR_PROJECT = ${DIR_PROJECT}")
message("  DIR_EXTERNAL = ${DIR_EXTERNAL}")
message("  DIR_BUILD = ${DIR_BUILD}")
message("  EIGEN_DIR = ${EIGEN_DIR}")
message("  Boost_INCLUDE_DIR = ${Boost_INCLUDE_DIR}")


# build all libraries and executables:

add_subdirectory(cppgp)
