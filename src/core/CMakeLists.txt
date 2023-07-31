
INCLUDE_DIRECTORIES(${MAINFOLDER}/include
    ${MAINFOLDER}/third_party/LogLite/include
    ${MAINFOLDER}/third_party/libs21/include)

SET (project_MOC_HEADERS ${MAINFOLDER}/include)

ADD_LIBRARY(LogLite STATIC IMPORTED)

SET_TARGET_PROPERTIES(LogLite PROPERTIES IMPORTED_LOCATION
    ${MAINFOLDER}/third_party/LogLite/lib/libLogLite.a)

SET (project_LIBS LogLite)
SET (project_BIN ${PROJECT_NAME})

ADD_LINK_OPTIONS(-L${MAINFOLDER}/third_party/LogLite/lib -lLogLite)

ADD_EXECUTABLE(${project_BIN}
    main.cc
    App.h
    App.cc
    Core.h
    Core.cc
    ConfigReader.cc
    ConfigReader.h
    AgentManager.cc
    AgentManager.h
    ../modules/include/Metric.cc
    ../modules/include/Metric.h
    ../modules/include/Agent.h)

TARGET_LINK_OPTIONS(${project_BIN} PRIVATE -rdynamic)

TARGET_INCLUDE_DIRECTORIES(${project_BIN} PRIVATE ../include/src
    $<TARGET_PROPERTY:LogLite,INTERFACE_INCLUDE_DIRECTORIES>)

TARGET_LINK_LIBRARIES(${project_BIN} PRIVATE ${project_LIBS})
SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)

INSTALL(TARGETS ${project_BIN} DESTINATION bin)
