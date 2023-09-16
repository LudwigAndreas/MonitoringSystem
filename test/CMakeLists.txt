INCLUDE_DIRECTORIES(${MAINFOLDER}/src/core)

SET (test_LIBS ${GTest_Library} gtest_main gmock_main LogLite)

SET (test_BIN ${PROJECT_NAME}-unittests)

ADD_DEFINITIONS(-fprofile-arcs -ftest-coverage)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
SET(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

SET(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/${test_BIN}.dir/src)
MESSAGE("-- Object directory: ${OBJECT_DIR}")

set(PROJECT_SOURCES
    ../src/modules/include/metric.cc
    ../src/modules/include/metric.h
    ../src/modules/include/agent.h
    ../src/core/config/properties.cc
    ../src/core/config/properties.h
    ../src/core/agent/agent_manager.cc
    ../src/core/agent/agent_manager.h
    ../src/core/agent/agent_bundle.cc
    ../src/core/agent/agent_bundle.h
    ../src/core/agent/agent_bundle_loader.cc
    ../src/core/agent/agent_bundle_loader.h
    ../src/core/metric/configured_metric.cc
    ../src/core/metric/configured_metric.h
    ../src/core/metric/metric_critical_value.cc
    ../src/core/metric/metric_critical_value.h
    ../src/core/metric/metric_event.cc
    ../src/core/metric/metric_event.h
    ../src/core/core/core.h
    ../src/core/core/core.cc
    ../src/core/executor/agent_scheduler.cc
    ../src/core/executor/agent_scheduler.h
    ../src/core/analyzer/metric_analyzer.cc
    ../src/core/analyzer/metric_analyzer.h
    ../src/core/agent/i_agent_subscriber.h
    ../src/core/executor/agent_executor.cc
    ../src/core/executor/agent_executor.h
    ../src/core/metric/i_metric_subscriber.h
#    ../src/core/notifier/failed_metric.h
#    ../src/core/notifier/imessage_sender.h
#    ../src/core/notifier/imessage_sender.cc
#    ../src/core/notifier/email/email_sender.cc
#    ../src/core/notifier/email/email_sender.h
#    ../src/core/notifier/email/email.cc
#    ../src/core/notifier/email/email.h
#    ../src/core/notifier/telegram/telegram_sender.cc
#    ../src/core/notifier/telegram/telegram_sender.h
#    ../src/core/notifier/telegram/telegram_user_repository.cc
#    ../src/core/notifier/telegram/telegram_user_repository.h
#    ../src/core/notifier/email/email_address.cc
#    ../src/core/notifier/email/email_address.h
#    ../src/core/notifier/email/email.h
#    ../src/core/notifier/email/email.cc
#    ../src/core/notifier/notification_controller.cc
#    ../src/core/notifier/notification_controller.h
    mock/agent/mock_agent_subscriber.h
    mock/agent/mock_agent_manager.h
    core/agent/agent_bundle_test.cc
    mock/agent/mock_agent.h
    mock/config/mock_properties.h
    core/agent/agent_bundle_loader_test.cc
    core/analyzer/metric_analyzer_test.cc
    core/config/properties_test.cc
    core/core/core_test.cc
    core/executor/agent_executor_test.cc
    core/executor/agent_scheduler_test.cc
    core/metric/metric_critical_value_test.cc
    core/metric/metric_event_test.cc

    modules/test_module/test_funcs.cc
    modules/test_module/test_funcs.h
    modules/test_module/test_agent.cc
    modules/test_module/test_agent.h
    modules/test_module/agent_factory.cc
    mock/metric/mock_metric_subscriber.h
    mock/metric/mock_metric_analyzer.h
)

ADD_EXECUTABLE(${test_BIN}
    main.cc
#    core/notifier/email/email_address.cc
#    core/notifier/telegram/telegram_user_repository.cc
#    core/notifier/failed_metric.cc

    core/metric/configured_metric_test.cc
    ${PROJECT_SOURCES}
    )

TARGET_LINK_LIBRARIES(${test_BIN} PRIVATE ${test_LIBS})

TARGET_INCLUDE_DIRECTORIES(${test_BIN} PRIVATE . ${GTest_INCLUDE_DIRS} )

ADD_CUSTOM_COMMAND(TARGET ${test_BIN} PRE_BUILD
    COMMAND rm -f ${OBJECT_DIR}/*.gcno
    COMMAND rm -f ${OBJECT_DIR}/*.gcda
)

ADD_SUBDIRECTORY(modules/test_module)

ADD_CUSTOM_TARGET(init
    COMMAND ${CMAKE_MAKE_PROGRAM} clean
    COMMAND rm -f ${OBJECT_DIR}/*.gcno
    COMMAND rm -f ${OBJECT_DIR}/*.gcda
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

ADD_DEPENDENCIES(${test_BIN} TestModule)
ADD_CUSTOM_TARGET(test "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS} ${PROJECT_SOURCES})