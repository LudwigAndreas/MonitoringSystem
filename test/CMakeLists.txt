
INCLUDE_DIRECTORIES(${MAINFOLDER}/src)

SET (test_LIBS ${GTest_Library})

SET (test_BIN ${PROJECT_NAME}-unittests)

#FILE (GLOB_RECURSE test_SRCS
#    *.cc
#    )

ADD_EXECUTABLE(${test_BIN}
    main.cc
    core/properties_reader_test.cc
    )

TARGET_LINK_LIBRARIES(${test_BIN} PRIVATE ${PROJECT_NAME}-lib ${test_LIBS} )
TARGET_INCLUDE_DIRECTORIES(${test_BIN} PRIVATE .)

ADD_CUSTOM_TARGET(check ALL "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS} ${project_SRCS})
ADD_CUSTOM_TARGET(test "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS} ${project_SRCS})
