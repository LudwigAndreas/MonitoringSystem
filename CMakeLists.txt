# <Copyright Holder>. Copyright (C) <Copyright Year(s)>. <License>.

#
# Project Properties
#
CMAKE_MINIMUM_REQUIRED (VERSION 3.10)
PROJECT (MonitoringSystem)
SET (APPLICATION_NAME "MonitoringSystem")
SET (APPLICATION_CODENAME "${PROJECT_NAME}")
SET (APPLICATION_COPYRIGHT_YEARS "2009")
SET (APPLICATION_VERSION_MAJOR 1)
SET (APPLICATION_VERSION_MINOR 0)
SET (APPLICATION_VERSION_PATCH 0)
SET (APPLICATION_VERSION_TYPE SNAPSHOT)
SET (APPLICATION_VERSION_STRING "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}-${APPLICATION_VERSION_TYPE}")
SET (APPLICATION_VENDOR_ID "com.yourcompany")
SET (APPLICATION_VENDOR_NAME "Your Company")
SET (APPLICATION_VENDOR_URL "eternaltimeline.ru")
SET (APPLICATION_ID "${APPLICATION_VENDOR_ID}.${PROJECT_NAME}")


#
# C++ Standard
#
SET (CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_STANDARD_REQUIRED 17)

#
# Debugging Options
#
SET (CMAKE_VERBOSE_MAKEFILE 0) # Use 1 for debugging, 0 for release

#
# Compilation Flags
#

# Add these lines to enable code coverage flags
SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -fPIC")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
SET(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native")

#
# Project Output Paths
#
SET (MAINFOLDER ${PROJECT_SOURCE_DIR})
SET (EXECUTABLE_OUTPUT_PATH "${MAINFOLDER}/bin")
SET (LIBRARY_OUTPUT_PATH "${MAINFOLDER}/lib")

#
# Project Search Paths
#
LIST (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}")
LIST (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}/tools")
LIST (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}/third_party")
SET (CMAKE_MODULE_PATH "${MAINFOLDER}/tools/share/cmake")
INCLUDE_DIRECTORIES("${MAINFOLDER}/src/"
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${MAINFOLDER}/third_party/LogLite/include
    ${MAINFOLDER}/third_party/libs21/include
    ${MAINFOLDER}/third_party/tgbot-cpp/include
    )

#
# Locate Project Prerequisites
#
FIND_PACKAGE(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
FIND_PACKAGE(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

ADD_LIBRARY(LogLite STATIC IMPORTED)

SET_TARGET_PROPERTIES(LogLite PROPERTIES IMPORTED_LOCATION
    ${MAINFOLDER}/third_party/LogLite/lib/libLogLite.a)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED src/parse/converters/LiteralPatternConverter.cc include/logger/parse/converters/LiteralPatternConverter.h third_party/libs21/test/properties_reader_class_test.cc)
target_link_libraries(GTest::GTest INTERFACE gtest_main)
SET (GTest_Library GTest::gtest_main)

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)

#
# Configure Files
#
FILE (GLOB_RECURSE CONFIGINPUTS1 include/*.in.h.cmake)
FILE (GLOB_RECURSE CONFIGINPUTS2 include/*.h.in.cmake)
FILE (GLOB_RECURSE CONFIGINPUTS3 src/*.in.h.cmake)
FILE (GLOB_RECURSE CONFIGINPUTS4 src/*.h.in.cmake)
FILE (GLOB_RECURSE CONFIGINPUTS5 test/*.in.h.cmake)
FILE (GLOB_RECURSE CONFIGINPUTS6 test/*.h.in.cmake)
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS1}")
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS2}")
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS3}")
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS4}")
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS5}")
LIST (APPEND CONFIGINPUTS "${CONFIGINPUTS6}")
FOREACH (CONFIGINPUT ${CONFIGINPUTS})
  STRING (REPLACE ".in.h.cmake" ".h" CONFIGOUTPUT1 "${CONFIGINPUT}")
  STRING (REPLACE ".h.in.cmake" ".h" CONFIGOUTPUT2 "${CONFIGOUTPUT1}")
  CONFIGURE_FILE ("${CONFIGINPUT}" "${CONFIGOUTPUT2}")
ENDFOREACH (CONFIGINPUT ${CONFIGINPUTS})

#
# Add Build Targets
#

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)


#
# Add Install Targets
#
IF (EXISTS "${MAINFOLDER}/include/${PROJECT_NAME}" AND IS_DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}")
  INSTALL(DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}" DESTINATION "include")
ENDIF (EXISTS "${MAINFOLDER}/include/${PROJECT_NAME}" AND IS_DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}")

#
# Add Documentation Targets
#
#INCLUDE (DocumentationTargets)
