INCLUDE_DIRECTORIES(${MAINFOLDER}/include)

SET (test_LIBS ${GTest_Library})

SET (test_BIN ${PROJECT_NAME}-unittests)

ADD_EXECUTABLE(${test_BIN} ${project_SRCS}
    main.cc
    )

TARGET_LINK_LIBRARIES(${test_BIN} ${test_LIBS} ${project_BIN})
TARGET_INCLUDE_DIRECTORIES(${test_BIN} PRIVATE .)

ADD_CUSTOM_TARGET(check ALL "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS} ${project_SRCS})
ADD_CUSTOM_TARGET(test "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS} ${project_SRCS})
