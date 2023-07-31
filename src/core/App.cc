//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "App.h"

#include <iostream>
#include <string>

#include "appinfo.h"
#include "Core.h"

namespace s21 {

namespace {
  bool matches_option(const std::string& givenoption, const std::string& expectedoption, int mindashes = 1, int maxdashes = 2) {
    int dashes = 0;
    if (givenoption.length() > 0) {
      while ((dashes < int(givenoption.length())) && givenoption[dashes] == '-') {
        dashes++;
      }
      if ((dashes < mindashes) || (dashes > maxdashes)) {
        return false;
      }
    }
    std::string substr = givenoption.substr(dashes, givenoption.length());
    return expectedoption == substr;
  }
}

App::App(int &argc, char **argv) : invocation_(argv[0]), gui_(false), interactive_(
    false) {
  if (s_instance_) {
//    Only one instance of App allowed.
    std::exit(0);
  }

  bool done = false;

  s_instance_ = this;

//  setApplication

  diagnostic::LoggerPtr rootlogger = diagnostic::Logger::getRootLogger();
  rootlogger->SetPatternLayout(diagnostic::PatternLayout("%d{%Y-%m-%d %X} %Y%5.5p%y \x1B[35m%-5P%y --- [%M] \x1B[36m%-25.40F%y : %m%n"));
  rootlogger->AddOutputStream(std::cout, false);


//  Parse the commandline
  int idx = 1;
  while (idx < argc) {
    std::string arg(argv[idx]);
    if (matches_option(arg, "help", 0) || matches_option(arg, "h") || matches_option(arg, "?", 0)) {
      printHelpMessage();
      std::exit(0);
    } else if (matches_option(arg, "version", 0)) {
      printVersionMessage();
      std::exit(0);
    } else if (matches_option(arg, "version-triplet")) {
      printVersionTripletMessage();
      std::exit(0);
    } else if (matches_option(arg, "prefset")) {
      if ( (idx + 1) >= argc ) {
        LOG_FATAL(s_logger_, "Option \"" << arg << "\" requires a parameter");
        std::exit(1);
      }

      // Increment the index
      idx++;

      // Get the next parameter
      std::string param(argv[idx]);

      // Determine if there is an equals sign
      // If there is, set the preference;
      // Otherwise, remove the preference
      size_t eqidx = param.find('=');
      if (eqidx != std::string::npos) {
        std::string key = param.substr(0, eqidx);
        std::string value = param.substr(eqidx + 1);
        setPreference(key, value);
      } else {
        unsetPreference(param);
      }
      done = true;
    } else if (matches_option(arg, "prefdel")) {
      // Verify that there another argument
      if ( (idx + 1) >= argc ) {
        LOG_FATAL(s_logger_, "Option \"" << arg << "\" requires a parameter");
        std::exit(1);
      }

      // Increment the index
      idx++;

      // Get the next parameter
      std::string param(argv[idx]);
      unsetPreference(param);
      done = true;
    } else if (matches_option(arg, "preflist")) {
      printAllPreferences();
      done = true;
    } else if (matches_option(arg, "prefget")) {
      // Verify that there another argument
      if ( (idx + 1) >= argc ) {
        LOG_FATAL(s_logger_, "Option \"" << arg << "\" requires a parameter");
        std::exit(1);
      }

      // Increment the index
      idx++;

      // Get the next parameter
      std::string param(argv[idx]);
      printPreference(param);
      done = true;
    } else if (matches_option(arg, "loglevel")) {
      // Verify that there another argument
      if ( (idx + 1) >= argc ) {
        LOG_FATAL(s_logger_, "Option \"" << arg << "\" requires a parameter");
        std::exit(1);
      }

      // Increment the index
      idx++;

      // Get the next parameter
      std::string param(argv[idx]);

      size_t eqidx = param.find('=');
      if (eqidx != std::string::npos) {
        std::string logger = param.substr(0, eqidx);
        std::string level = param.substr(eqidx + 1);
        setLogLevel(logger, level);
      } else {
        setLogLevel("", param);
      }
    } else if (matches_option(arg, "appid") || matches_option(arg, "application-identifier")) {
      printApplicationIdentifier();
      std::exit(0);
    } else if (matches_option(arg, "gui")) {
      if (interactive_) {
        LOG_FATAL(s_logger_, "Cannot specify both \"--gui\" and \"--interactive\" simultaneously.");
        std::exit(1);
      }
      if (gui_) {
        LOG_WARN(s_logger_, "Option \"" << arg << "\" already specified. Ignoring.");
      }
      gui_ = true;
    } else if (matches_option(arg, "interactive")) {
      if (gui_) {
        LOG_FATAL(s_logger_, "Cannot specify both \"--gui\" and \"--interactive\" simultaneously.");
        std::exit(1);
      }
      if (interactive_) {
        LOG_WARN(s_logger_, "Option \"" << arg << "\" already specified. Ignoring.");
      }
      interactive_ = true;
    } else {
      LOG_WARN(s_logger_, "Unrecognized option \"" << arg << "\". Ignoring");
    }
    idx++;
  }
  if (done) {
    std::exit(0);
  }

  if (gui_) {
    InitGui();
  } else if (interactive_) {
    InteractiveMain();
    std::exit(0);
  } else {
    ConsoleMain();
    std::exit(0);
  }
}

App::~App() {

}

App* App::Instance() {
  return s_instance_;
}

void App::InitGui() {
  LOG_FATAL(s_logger_, "Gui has not been implemented yet.");
}

void App::InteractiveMain() {
  std::cout << "Hello world!" << std::endl;
}

void App::ConsoleMain() {
  monitor::Core("./agents/");
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(10));
  }
}

void
App::printHelpMessage()
{
  std::cout << "Usage: " << GetProjectInvocation() << " [options]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "    --help                       Displays this help message." << std::endl;
  std::cout << "    --version                    Prints the program version." << std::endl;
  std::cout << "    --version-triplet            Prints the undecorated program version." << std::endl;
  std::cout << "    --appid                      Prints the unique application identifier." << std::endl;
  std::cout << "    --prefset <key>=<val>        Sets the given preference." << std::endl;
  std::cout << "    --prefdel <key>              Unsets the given preference." << std::endl;
  std::cout << "    --prefget <key>              Prints the given preference." << std::endl;
  std::cout << "    --preflist                   Lists all preferences that are set." << std::endl;
  std::cout << "    --loglevel <level>           Sets the current logging level." << std::endl;
  std::cout << "    --loglevel <logger>=<level>  Sets the logging level for the given logger." << std::endl;
  std::cout << "    --gui                        Run in graphical user interface mode." << std::endl;
  std::cout << "    --interactive                Run in interactive commandline mode." << std::endl;
  std::cout << "Log Levels:" << std::endl;
//  std::cout << "    all" << std::endl;
  std::cout << "    trace" << std::endl;
  std::cout << "    debug" << std::endl;
  std::cout << "    info" << std::endl;
  std::cout << "    warn" << std::endl;
  std::cout << "    error" << std::endl;
  std::cout << "    fatal" << std::endl;
//  std::cout << "    off" << std::endl;
}

void App::printVersionMessage() {
  std::cout << GetProjectName() << " v" << GetProjectVersion() << std::endl;
  std::cout << GetProjectVendorName() << "; Copyright" << GetProjectCopyrightYears() << std::endl;
}

void App::printVersionTripletMessage() {
  std::cout << GetProjectVersion() << std::endl;
}

void App::printApplicationIdentifier() {
  std::cout << GetProjectID() << std::endl;
}

std::string App::GetProjectName() {
  return APPLICATION_NAME;
}

std::string App::GetProjectCodeName() {
  return APPLICATION_CODENAME;
}

std::string App::GetProjectVendorID() {
  return APPLICATION_VENDOR_ID;
}

std::string App::GetProjectVendorName() {
  return APPLICATION_VENDOR_NAME;
}

std::string App::GetProjectID() {
  return APPLICATION_ID;
}

int App::GetProjectMinorVersion() {
  return APPLICATION_VERSION_MINOR;
}

int App::GetProjectMajorVersion() {
  return APPLICATION_VERSION_MAJOR;
}

int App::GetProjectPatchVersion() {
  return APPLICATION_VERSION_PATCH;
}

std::string App::GetProjectVersion() {
  return APPLICATION_VERSION_STRING;
}

std::string App::GetProjectCopyrightYears() {
  return APPLICATION_COPYRIGHT_YEARS;
}

std::string App::GetProjectInvocation() {
  return invocation_;
}

std::string App::getKeyName(const std::string &key) const {
  std::string result(key);
  for (size_t i = 0; i < result.size(); i++) {
    if (result[i] == '/' || (result[i] == '\\')) {
      result[i] = '.';
    }
  }
  return result;
}

std::string App::getKeyRepr(const std::string &key) const {
  std::string result(key);
  for (size_t i = 0; i < result.size(); i++) {
    if (result[i] == '/' || (result[i] == '\\')) {
      result[i] = '/';
    }
  }
  return result;
}

void App::setPreference(const std::string &key, const std::string &val) {
  (void) key;
  (void) val;
//  QSettings settings;
//  std::string keyrep(getKeyRepr(key));
//  QString qkeyrep(keyrep.c_str());
//  QString qval(keyrep.c_str());
//  settings.setValue(qkeyrep, qval);
//  settings.sync();
}

void App::unsetPreference(const std::string &key) {
  (void) key;
//  QSettings settings;
//  std::string keyrep(getKeyRepr(key));
//  QString qkeyrep(keyrep.c_str());
//  settings.beginGroup(keyrep);
//  if ((settings.childGroups.length() != 0) || (settings.childKeys().length() != 0)) {
//    settings.setValue("", "");
//  } else {
//    settings.remove("");
//  }
//  settings.endGroup();
//  settings.sync();
}

void App::printPreference(const std::string &key) const {
  (void) key;
//  QSettings settings;
//  std::string keyrep(getKeyRepr(key));
//  QString qkeyrep(keyrep.c_str());
//  QString result = "undefined";
//  if (settings.contains(qkeyrep)) {
//    result = settings.value(qkeyrep, QString("undefined")).toString();
//  }
// std::cout << result << std::endl;
}

void App::printAllPreferences() const {
//  QSettings settings;
//  QStringList keys = settings.allKeys();
//  for ( QStringList::const_iterator it = keys.begin(); it != keys.end(); ++it ){
//    QString qkeystr = *it;
//    QString qvalstr = settings.value(qkeystr).toString();
//
//    if ( ! qvalstr.isEmpty() ){
//      std::string key=getKeyName(convert(qkeystr));
//      std::cout << key << "=" << qvalstr << std::endl;
//    }
//  }
}

void
App::setLogLevel(const std::string& logger, const std::string& level)
{
  diagnostic::LoggerPtr loggerptr = ((logger=="")?(diagnostic::Logger::getRootLogger()):(diagnostic::Logger::getLogger(logger)));
  std::string lowercaselevel(level);
  for ( size_t i = 0; i < lowercaselevel.size(); i++ ){
    lowercaselevel[i] = std::tolower(lowercaselevel[i]);
  }

//  if ( lowercaselevel == "all" ){
//    loggerptr->SetLevel(diagnostic::LogLevel::All);
//  }else
  if ( lowercaselevel == "trace" ){
    loggerptr->SetLevel(diagnostic::LogLevel::Trace);
  }else if ( lowercaselevel == "debug" ){
    loggerptr->SetLevel(diagnostic::LogLevel::Debug);
  }else if ( lowercaselevel == "info" ){
    loggerptr->SetLevel(diagnostic::LogLevel::Info);
  }else if ( lowercaselevel == "warn" ){
    loggerptr->SetLevel(diagnostic::LogLevel::Warn);
  }else if ( lowercaselevel == "error" ){
    loggerptr->SetLevel(diagnostic::LogLevel::Error);
  }else if ( lowercaselevel == "fatal" ){
    loggerptr->SetLevel(diagnostic::LogLevel::Fatal);
//  }else if ( (lowercaselevel == "off")  || (lowercaselevel == "none") ){
//    loggerptr->SetLevel(diagnostic::LogLevel::Off);
  }else{
    LOG_FATAL(s_logger_, "Unrecognized logging level: \"" << level << "\".");
    std::exit(1);
  }
}

App* App::s_instance_ = nullptr;

auto App::s_logger_ = diagnostic::Logger::getLogger("myApp");

}