set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(TS_FILES gui/MonitoringSystem_en_US.ts)

set(PROJECT_SOURCES
    application/main.cc
    application/app.h
    application/app.cc
    modules/include/metric.cc
    modules/include/metric.h
    modules/include/agent.h
    core/config/properties.cc
    core/config/properties.h
    core/agent/agent_manager.cc
    core/agent/agent_manager.h
    core/agent/agent_bundle.cc
    core/agent/agent_bundle.h
    core/agent/agent_bundle_loader.cc
    core/agent/agent_bundle_loader.h
    core/metric/configured_metric.cc
    core/metric/configured_metric.h
    core/metric/metric_critical_value.cc
    core/metric/metric_critical_value.h
    core/metric/metric_event.cc
    core/metric/metric_event.h
    core/core/core.h
    core/core/core.cc
    core/executor/agent_scheduler.cc
    core/executor/agent_scheduler.h
    core/analyzer/metric_analyzer.cc
    core/analyzer/metric_analyzer.h
    core/agent/i_agent_subscriber.h
    core/executor/agent_executor.cc
    core/executor/agent_executor.h
    core/metric/i_metric_subscriber.h
    core/notifier/failed_metric.h
    core/notifier/i_message_sender.h
    core/notifier/i_message_sender.cc
    core/notifier/email/email_sender.cc
    core/notifier/email/email_sender.h
    core/notifier/email/email_address.cc
    core/notifier/email/email.cc
    core/notifier/email/email.h
    core/notifier/telegram/telegram_bot.cc
    core/notifier/telegram/telegram_sender.cc
    core/notifier/telegram/telegram_sender.h
    core/notifier/telegram/telegram_user_repository.cc
    core/notifier/telegram/telegram_user_repository.h
    core/notifier/email/email_address.cc
    core/notifier/email/email_address.h
    core/notifier/email/email.h
    core/notifier/email/email.cc
    gui/view/mainwindow.cc
    gui/view/mainwindow.h
    gui/view/mainwindow.ui
    ${TS_FILES}
    gui/controller/main_controller.cc
    gui/controller/main_controller.h
    core/notifier/notification_controller.cc
    core/notifier/notification_controller.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(${PROJECT_NAME}
      MANUAL_FINALIZATION
      ${PROJECT_SOURCES}
      resources.qrc

      )
  # Define target properties for Android with Qt 6 as:
  #    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
  # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

  qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
  if(ANDROID)
    add_library(${PROJECT_NAME} SHARED
        ${PROJECT_SOURCES}
        )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        )
  endif()

  qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${APPLICATION_VERSION_STRING}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    )

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(${PROJECT_NAME})
endif()


INCLUDE_DIRECTORIES(${MAINFOLDER}/src/core/
    ${MAINFOLDER}/src/
    ${MAINFOLDER}/third_party/LogLite/include
    ${MAINFOLDER}/third_party/libs21/include
    ${MAINFOLDER}/third_party/tgbot-cpp/include)

SET (project_MOC_HEADERS ${MAINFOLDER}/include
    ${MAINFOLDER}/third_party/LogLite/include)

#SET_TARGET_PROPERTIES(TgBot PROPERTIES IMPORTED_LOCATION
#    ${MAINFOLDER}/third_party/tgbot-cpp/build/libTgBot.a)


SET (project_LIBS LogLite ${MAINFOLDER}/third_party/tgbot-cpp/build/libTgBot.a)
SET (project_BIN ${PROJECT_NAME})

#find_package(Boost REQUIRED COMPONENTS filesystem system)

#ADD_LINK_OPTIONS(-L${MAINFOLDER}/third_party/LogLite/lib -lLogLite)

ADD_DEPENDENCIES(${project_BIN} LogLite)

ADD_SUBDIRECTORY(modules)

ADD_DEPENDENCIES(${project_BIN} BonusModule CpuModule MemoryModule NetworkModule)

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
