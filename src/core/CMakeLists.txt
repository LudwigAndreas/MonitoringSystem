
INCLUDE_DIRECTORIES(${MAINFOLDER}/src/core
    ${MAINFOLDER}/third_party/LogLite/include
    ${MAINFOLDER}/third_party/libs21/include)

SET (project_MOC_HEADERS ${MAINFOLDER}/include
    ${MAINFOLDER}/third_party/LogLite/include)


ADD_LIBRARY(LogLite STATIC IMPORTED)

SET_TARGET_PROPERTIES(LogLite PROPERTIES IMPORTED_LOCATION
    ${MAINFOLDER}/third_party/LogLite/lib/libLogLite.a)

SET (project_LIBS LogLite)
SET (project_BIN ${PROJECT_NAME})

ADD_LINK_OPTIONS(-L${MAINFOLDER}/third_party/LogLite/lib -lLogLite)

#ADD_LIBRARY(${project_BIN}-lib
#    )

ADD_EXECUTABLE(${project_BIN}
    main.cc
    app/App.h
    app/App.cc
    ../modules/include/Metric.cc
    ../modules/include/Metric.h
    ../modules/include/Agent.h
    config/Properties.cc
    config/Properties.h
    agent/AgentManager.cc
    agent/AgentManager.h
    agent/agent_bundle.cc
    agent/agent_bundle.h
    agent/AgentBundleLoader.cc
    agent/AgentBundleLoader.h
    metric/ConfiguredMetric.cc
    metric/ConfiguredMetric.h
    metric/MetricCriticalValue.cc
    metric/MetricCriticalValue.h
    metric/MetricEvent.cc
    metric/MetricEvent.h
    app/Core.h
    app/Core.cc
    executor/AgentScheduler.cc
    executor/AgentScheduler.h
    analyzer/MetricAnalyzer.cc
    analyzer/MetricAnalyzer.h
    agent/IAgentSubscriber.h
    executor/AgentExecutor.cc
    executor/AgentExecutor.h
    notifier/IMetricNotifier.h
    )

TARGET_LINK_OPTIONS(${project_BIN} PRIVATE -rdynamic)

TARGET_INCLUDE_DIRECTORIES(${project_BIN} PRIVATE ../include/src
    $<TARGET_PROPERTY:LogLite,INTERFACE_INCLUDE_DIRECTORIES>)

TARGET_LINK_LIBRARIES(${project_BIN} PRIVATE ${project_LIBS})
SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)

INSTALL(TARGETS ${project_BIN} DESTINATION bin)
