cmake_minimum_required(VERSION 3.5)

project(SystemMonitoring VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES gui/MonitoringSystem_en_US.ts)

set(PROJECT_SOURCES
    application/main.cc
    application/App.h
    application/App.cc
    modules/include/Metric.cc
    modules/include/Metric.h
    modules/include/Agent.h
    core/config/Properties.cc
    core/config/Properties.h
    core/agent/AgentManager.cc
    core/agent/AgentManager.h
    core/agent/agent_bundle.cc
    core/agent/agent_bundle.h
    core/agent/AgentBundleLoader.cc
    core/agent/AgentBundleLoader.h
    core/metric/ConfiguredMetric.cc
    core/metric/ConfiguredMetric.h
    core/metric/MetricCriticalValue.cc
    core/metric/MetricCriticalValue.h
    core/metric/MetricEvent.cc
    core/metric/MetricEvent.h
    core/core/Core.h
    core/core/Core.cc
    core/executor/AgentScheduler.cc
    core/executor/AgentScheduler.h
    core/analyzer/MetricAnalyzer.cc
    core/analyzer/MetricAnalyzer.h
    core/agent/IAgentSubscriber.h
    core/executor/AgentExecutor.cc
    core/executor/AgentExecutor.h
    core/metric/IMetricSubscriber.h
    core/notifier/FailedMetric.h
    core/notifier/IMessageSender.h
    core/notifier/IMessageSender.cc
    core/notifier/email/EmailSender.cc
    core/notifier/email/EmailSender.h
    core/notifier/email/Email.cc
    core/notifier/email/Email.h
    core/notifier/telegram/TelegramSender.cc
    core/notifier/telegram/TelegramSender.h
    core/notifier/telegram/TelegramUserRepository.cc
    core/notifier/telegram/TelegramUserRepository.h
    core/notifier/email/EmailAddress.cc
    core/notifier/email/EmailAddress.h
    core/notifier/email/Email.h
    core/notifier/email/Email.cc
    gui/view/mainwindow.cpp
    gui/view/mainwindow.h
    gui/view/mainwindow.ui
    ${TS_FILES}
    gui/controller/MainController.cc
    gui/controller/MainController.h
    core/notifier/NotificationController.cc core/notifier/NotificationController.h)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(SystemMonitoring
      MANUAL_FINALIZATION
      ${PROJECT_SOURCES}
      resources.qrc

      )
  # Define target properties for Android with Qt 6 as:
  #    set_property(TARGET SystemMonitoring APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
  # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

  qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
  if(ANDROID)
    add_library(SystemMonitoring SHARED
        ${PROJECT_SOURCES}
        )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(SystemMonitoring
        ${PROJECT_SOURCES}
        )
  endif()

  qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(SystemMonitoring PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SystemMonitoring)
endif()
set_target_properties(SystemMonitoring PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    )

include(GNUInstallDirs)
install(TARGETS SystemMonitoring
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(SystemMonitoring)
endif()


INCLUDE_DIRECTORIES(${MAINFOLDER}/src/core
    ${MAINFOLDER}/third_party/LogLite/include
    ${MAINFOLDER}/third_party/libs21/include
    ${MAINFOLDER}/third_party/tgbot-cpp/include)

SET (project_MOC_HEADERS ${MAINFOLDER}/include
    ${MAINFOLDER}/third_party/LogLite/include)


ADD_LIBRARY(LogLite STATIC IMPORTED)

SET_TARGET_PROPERTIES(LogLite PROPERTIES IMPORTED_LOCATION
    ${MAINFOLDER}/third_party/LogLite/lib/libLogLite.a)

#SET_TARGET_PROPERTIES(TgBot PROPERTIES IMPORTED_LOCATION
#    ${MAINFOLDER}/third_party/tgbot-cpp/build/libTgBot.a)


SET (project_LIBS LogLite ${MAINFOLDER}/third_party/tgbot-cpp/build/libTgBot.a)
SET (project_BIN ${PROJECT_NAME})

#find_package(Boost REQUIRED COMPONENTS filesystem system)

#ADD_LINK_OPTIONS(-L${MAINFOLDER}/third_party/LogLite/lib -lLogLite)

ADD_SUBDIRECTORY(modules)

TARGET_LINK_OPTIONS(${project_BIN} PRIVATE -rdynamic)

TARGET_INCLUDE_DIRECTORIES(${project_BIN} PRIVATE ../include/src
    $<TARGET_PROPERTY:LogLite,INTERFACE_INCLUDE_DIRECTORIES> )

TARGET_LINK_LIBRARIES(${project_BIN} PRIVATE ${project_LIBS}
    ${Boost_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${CURL_LIBRARIES}
    )
SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)

INSTALL(TARGETS ${project_BIN} DESTINATION bin)
