
SET (project_MOC_HEADERS ${MAINFOLDER}/include)

SET (project_LIBS ${MAINFOLDER}/third_party/libs21/lib/libs21.a)
SET (project_BIN BonusModule)

SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -fPIC -g -Og")

INCLUDE_DIRECTORIES(${MAINFOLDER}/include ${MAINFOLDER}third_party/libs21/include)

ADD_LIBRARY(${project_BIN} SHARED
    ../include/agent.h
    ../include/metric.h
    bonus_agent.cc
    bonus_agent.h
    agent_factory.cc
    bonus.cc
    bonus.h
    ../include/metric.cc
    ../../../third_party/libs21/src/s21_file_map_reader.cc
    )

TARGET_LINK_OPTIONS(${project_BIN} PRIVATE
    --std=c++17 -fPIC -rdynamic -shared)

ADD_CUSTOM_COMMAND(TARGET ${project_BIN} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${project_BIN}> ${EXECUTABLE_OUTPUT_PATH}/agents/${project_BIN}.agent/$<TARGET_FILE_NAME:${project_BIN}>
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/agent.properties ${EXECUTABLE_OUTPUT_PATH}/agents/${project_BIN}.agent/agent.properties
    COMMENT "Copying ${project_BIN} to ${EXECUTABLE_OUTPUT_PATH}/agents/${project_BIN}.agent")

TARGET_INCLUDE_DIRECTORIES(${project_BIN} PRIVATE ../include/src)
TARGET_LINK_LIBRARIES(${project_BIN} PRIVATE ${project_LIBS})
SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)

#INSTALL(TARGETS ${project_BIN} DESTINATION bin)
