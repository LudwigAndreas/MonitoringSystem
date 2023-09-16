//
// Created by Ludwig Andreas on 31.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_APP_H_
#define MONITORINGSYSTEM_SRC_CORE_APP_H_

#include <QApplication>
#include <QMainWindow>
#include <string>

#include "../gui/controller/main_controller.h"
#include "../gui/view/mainwindow.h"
#include "core/core.h"
#include "logger.h"
#include "notifier/notification_controller.h"

namespace s21 {

class App : public QApplication {
  Q_OBJECT
 public:
  App(int& argc, char** argv);
  ~App();

  App* Instance();

  std::string GetProjectName();
  std::string GetProjectCodeName();
  std::string GetProjectVendorID();
  std::string GetProjectVendorName();
  std::string GetProjectID();
  int GetProjectMajorVersion();
  int GetProjectMinorVersion();
  int GetProjectPatchVersion();
  std::string GetProjectVersion();
  std::string GetProjectCopyrightYears();
  std::string GetProjectInvocation();

 private:
  void InitGui();
  void InteractiveMain();
  void ConsoleMain();
  void ConfigureCore();
  void ReadProperties();

  void printHelpMessage();
  void printVersionMessage();
  void printVersionTripletMessage();
  void printApplicationIdentifier();
  void setLogLevel(const std::string& logger, const std::string& level);
  std::string getKeyName(const std::string& key) const;
  std::string getKeyRepr(const std::string& key) const;
  static diagnostic::LoggerPtr getLogger();

  static App* s_instance_;
  //  static diagnostic::LoggerPtr s_logger_;
  std::string invocation_;
  std::string properties_file_ = "application.properties";
  std::shared_ptr<Properties> properties_;
  bool gui_;
  bool interactive_;
  std::shared_ptr<monitor::Core> core_;
  std::shared_ptr<MainWindow> mainwindow_;
  std::shared_ptr<MainController> maincontroller_;
  TelegramSenderPtr telegram_;
  EmailSenderPtr email_;
  std::shared_ptr<NotificationController> notification_controller_;
};

}  // namespace s21

#endif
