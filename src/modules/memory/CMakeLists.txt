
SET (project_MOC_HEADERS ${MAINFOLDER}/include)

SET (project_LIBS)
SET (project_BIN MemoryModule)

INCLUDE_DIRECTORIES(${MAINFOLDER}/include)

ADD_LIBRARY(${project_BIN} SHARED
    ../include/Agent.h
    ../include/Metric.h
    MemoryAgent.cc
    MemoryAgent.h
    agent_factory.cc
    memory.cc
    memory.h
    ../include/Metric.cc)

TARGET_LINK_OPTIONS(${project_BIN} PRIVATE
    --std=c++17 -fPIC -rdynamic -shared)

ADD_CUSTOM_COMMAND(TARGET ${project_BIN} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${project_BIN}> ${EXECUTABLE_OUTPUT_PATH}/agents/${project_BIN}.agent/$<TARGET_FILE_NAME:${project_BIN}>
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/agent.properties ${EXECUTABLE_OUTPUT_PATH}/agents/${project_BIN}.agent/agent.properties
    COMMENT "Copying ${project_BIN} to ${EXECUTABLE_OUTPUT_PATH}/bin/agents/${project_BIN}.agent")

TARGET_INCLUDE_DIRECTORIES(${project_BIN} PRIVATE ../include/src)
TARGET_LINK_LIBRARIES(${project_BIN} PRIVATE ${project_LIBS})
SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)

#INSTALL(TARGETS ${project_BIN} DESTINATION bin)
