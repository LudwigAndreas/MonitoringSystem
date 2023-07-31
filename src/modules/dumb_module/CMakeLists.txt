
SET (project_MOC_HEADERS ${MAINFOLDER}/include)

SET (project_LIBS)
SET (project_BIN DumbModule)

INCLUDE_DIRECTORIES(${MAINFOLDER}/include)

ADD_LIBRARY(${project_BIN} SHARED
    ../include/Agent.h
    ../include/Metric.h
    DumbAgent.cc
    DumbAgent.h
    agent_factory.cc
    dumb_funcs.cc
    dumb_funcs.h
    ../include/Metric.cc
    )

TARGET_LINK_OPTIONS(${project_BIN} PRIVATE
    --std=c++17 -fPIC -rdynamic -shared)

TARGET_INCLUDE_DIRECTORIES(${project_BIN} PRIVATE ../include/src)
TARGET_LINK_LIBRARIES(${project_BIN} PRIVATE ${project_LIBS})
SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)

#INSTALL(TARGETS ${project_BIN} DESTINATION bin)
