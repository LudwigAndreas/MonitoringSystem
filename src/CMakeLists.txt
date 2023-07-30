
INCLUDE_DIRECTORIES(${MAINFOLDER}/include)

SET (project_MOC_HEADERS ../include)
SET (project_LIBS)
SET (project_BIN ${PROJECT_NAME})

ADD_EXECUTABLE(${project_BIN}
    core/ConfigReader.cc
    core/main.cc
    )

TARGET_LINK_LIBRARIES(${project_BIN} ${project_LIBS})
SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)
TARGET_INCLUDE_DIRECTORIES(${project_BIN} PRIVATE ../include/src)

ADD_CUSTOM_TARGET(cpu make -C ${MAINFOLDER}/src/modules/cpu)
ADD_CUSTOM_TARGET(memory make -C ${MAINFOLDER}/src/modules/memory)
ADD_CUSTOM_TARGET(network make -C ${MAINFOLDER}/src/modules/network)

ADD_DEPENDENCIES(${project_BIN} cpu memory network)

INSTALL(TARGETS ${project_BIN} DESTINATION bin)
