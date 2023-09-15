INCLUDE_DIRECTORIES(${MAINFOLDER}/src/core)

SET (test_LIBS ${GTest_Library} gtest_main gmock_main LogLite)

SET (test_BIN ${PROJECT_NAME}-unittests)

ADD_DEFINITIONS(-fprofile-arcs -ftest-coverage)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
SET(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

SET(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/${test_BIN}.dir/src)
MESSAGE("-- Object directory: ${OBJECT_DIR}")

set(PROJECT_SOURCES
    ../src/modules/include/Metric.cc
    ../src/modules/include/Metric.h
    ../src/modules/include/Agent.h
    ../src/core/config/Properties.cc
    ../src/core/config/Properties.h
    ../src/core/agent/AgentManager.cc
    ../src/core/agent/AgentManager.h
    ../src/core/agent/agent_bundle.cc
    ../src/core/agent/agent_bundle.h
    ../src/core/agent/AgentBundleLoader.cc
    ../src/core/agent/AgentBundleLoader.h
    ../src/core/metric/ConfiguredMetric.cc
    ../src/core/metric/ConfiguredMetric.h
    ../src/core/metric/MetricCriticalValue.cc
    ../src/core/metric/MetricCriticalValue.h
    ../src/core/metric/MetricEvent.cc
    ../src/core/metric/MetricEvent.h
    ../src/core/core/Core.h
    ../src/core/core/Core.cc
    ../src/core/executor/AgentScheduler.cc
    ../src/core/executor/AgentScheduler.h
    ../src/core/analyzer/MetricAnalyzer.cc
    ../src/core/analyzer/MetricAnalyzer.h
    ../src/core/agent/IAgentSubscriber.h
    ../src/core/executor/AgentExecutor.cc
    ../src/core/executor/AgentExecutor.h
    ../src/core/metric/IMetricSubscriber.h
#    ../src/core/notifier/FailedMetric.h
#    ../src/core/notifier/IMessageSender.h
#    ../src/core/notifier/IMessageSender.cc
#    ../src/core/notifier/email/EmailSender.cc
#    ../src/core/notifier/email/EmailSender.h
#    ../src/core/notifier/email/Email.cc
#    ../src/core/notifier/email/Email.h
#    ../src/core/notifier/telegram/TelegramSender.cc
#    ../src/core/notifier/telegram/TelegramSender.h
#    ../src/core/notifier/telegram/TelegramUserRepository.cc
#    ../src/core/notifier/telegram/TelegramUserRepository.h
#    ../src/core/notifier/email/EmailAddress.cc
#    ../src/core/notifier/email/EmailAddress.h
#    ../src/core/notifier/email/Email.h
#    ../src/core/notifier/email/Email.cc
#    ../src/core/notifier/NotificationController.cc
#    ../src/core/notifier/NotificationController.h
    core/agent/AgentManagerTest.cc mock/agent/MockAgentSubscriber.h mock/agent/MockAgentManager.h core/agent/AgentBundleTest.cc mock/agent/MockAgent.h mock/config/MockProperties.h
    core/agent/AgentBundleLoaderTest.cc
    core/analyzer/MetricAnalyzerTest.cc
    core/config/PropertiesTest.cc
    core/core/CoreTest.cc
    core/executor/AgentExecutorTest.cc
    core/executor/AgentSchedulerTest.cc
    core/metric/MetricCriticalValueTest.cc
    core/metric/MetricEventTest.cc

    modules/test_module/test_funcs.cc
    modules/test_module/test_funcs.h
    modules/test_module/TestAgent.cc
    modules/test_module/TestAgent.h
    modules/test_module/agent_factory.cc
    mock/metric/MockMetricSubscriber.h
    mock/metric/MockMetricAnalyzer.h
)

ADD_EXECUTABLE(${test_BIN}
    main.cc
#    core/notifier/email/EmailAddress.cc
#    core/notifier/telegram/TelegramUserRepository.cc
#    core/notifier/FailedMetric.cc

    core/metric/ConfiguredMetricTest.cc
    ${PROJECT_SOURCES}
    )

TARGET_LINK_LIBRARIES(${test_BIN} PRIVATE ${test_LIBS})

TARGET_INCLUDE_DIRECTORIES(${test_BIN} PRIVATE . ${GTest_INCLUDE_DIRS} )

ADD_SUBDIRECTORY(modules/test_module)

ADD_CUSTOM_COMMAND(TARGET ${test_BIN} PRE_BUILD
    COMMAND rm -f ${OBJECT_DIR}/*.gcno
    COMMAND rm -f ${OBJECT_DIR}/*.gcda
)

ADD_CUSTOM_TARGET(init
    COMMAND ${CMAKE_MAKE_PROGRAM} clean
    COMMAND rm -f ${OBJECT_DIR}/*.gcno
    COMMAND rm -f ${OBJECT_DIR}/*.gcda
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

ADD_CUSTOM_TARGET(test "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS} ${PROJECT_SOURCES})