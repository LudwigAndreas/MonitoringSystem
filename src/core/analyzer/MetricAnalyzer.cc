//
// Created by Ludwig Andreas on 24.08.2023.
//

#include "MetricAnalyzer.h"

#include <utility>

namespace s21 {

MetricAnalyzer::MetricAnalyzer() : last_log_day_(-1) {

}

MetricAnalyzer::MetricAnalyzer(diagnostic::LoggerPtr logger) : MetricAnalyzer(
    "./logs/",
    std::move(logger)) {}

MetricAnalyzer::MetricAnalyzer(std::string metric_output_dir,
                               diagnostic::LoggerPtr logger) {
  log_dir_ = std::move(metric_output_dir);
  logger_ = std::move(logger);
  logger_->SetLevel(diagnostic::LogLevel::Trace);
  auto now = std::chrono::system_clock::now();
  auto now_time_t = std::chrono::system_clock::to_time_t(now);
  last_log_day_ = -1;
  CheckForNewDay(now_time_t);
}

MetricAnalyzer::~MetricAnalyzer() {

}

void MetricAnalyzer::Log(const MetricEvent& event) {
  CheckForNewDay(event.GetTimestamp());
  if (!log_file_.is_open()) {
    OpenLogFile(event.GetTimestamp());
  }

  if (event.GetMetric()->GetCriticalValue().IsCritical(event.GetValue())) {
    NotifyCriticalValueReached(event);
  }

  std::string
      out = event.GetMetric()->GetName() + " : " + event.GetValue() + " | ";
  LOG_FATAL(logger_, out);
}

void MetricAnalyzer::OpenLogFile(time_t timestamp) {
  if (!fs::is_directory(log_dir_)) {
    fs::create_directory(log_dir_);
  }
  logger_->ClearOutputStream();
  auto local_time = std::localtime(&timestamp);
  std::ostringstream filename;
  filename << log_dir_ << "/" << std::put_time(local_time, "%m_%d_%Y")
           << ".log";
  current_log_file_ = filename.str();
  log_file_.open(current_log_file_, std::ios_base::app);
  logger_->AddOutputStream(log_file_, false);
}

void MetricAnalyzer::CheckForNewDay(time_t timestamp) {
  auto local_time = std::localtime(&timestamp);

  if (local_time->tm_mday != last_log_day_) {
    OpenLogFile(timestamp);
  }

  last_log_day_ = local_time->tm_mday;
}

void MetricAnalyzer::Subscribe(IMetricNotifier *notifier) {
  notifiers_.emplace_back(notifier);
}

void MetricAnalyzer::Unsubscribe(IMetricNotifier *notifier) {
  notifiers_.erase(std::remove(notifiers_.begin(), notifiers_.end(),
                               notifier), notifiers_.end());
}

void MetricAnalyzer::NotifyCriticalValueReached(const MetricEvent& event) {
  for (auto notifier: notifiers_) {
    notifier->OnCriticalValueReached(event);
  }
}

}