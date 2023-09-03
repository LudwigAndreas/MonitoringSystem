
#include "mainwindow.h"

#include <QStandardItemModel>
#include <QTimer>
#include <utility>
#include <QtCore/qfile.h>
#include <QtCore/qdir.h>

#include "./ui_mainwindow.h"
#include "../controller/MainController.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  log_timer_ = new QTimer(this);
  connect(log_timer_, SIGNAL(timeout()), this, SLOT(update_monitor_log_view()));
  log_timer_->start(3000);
  last_read_pos_ = 0;
  update_monitor_log_view();
  ClearMetricDetails();
  ClearAgentDetails();
}

MainWindow::~MainWindow() {
  log_file_->close();
  delete log_file_;
  delete log_timer_;
  delete ui;
}

void MainWindow::update_monitor_log_view() {
  if (!log_file_) {
    return;
  }

  log_file_->seek(last_read_pos_);

  QTextStream in(log_file_);
  QString new_text = in.readAll();

  ui->monitor_log_view->moveCursor(QTextCursor::End);
  ui->monitor_log_view->insertPlainText(new_text);

  last_read_pos_ += new_text.size();
}

void MainWindow::on_agent_list_widget_itemClicked(QListWidgetItem *item) {
  if (item != nullptr) {
    ui->metric_list_widget->clear();
    ClearAgentDetails();
    ClearMetricDetails();
    auto agent = agents_.find(item->text());
    if (agent != agents_.end()) {
      auto metrics = agent->second->GetMetrics();
      for (auto &metric: *metrics) {
        ui->metric_list_widget->addItem(QString::fromStdString(metric->GetName()));
      }
    }
    ShowAgentDetails(agent->second);
  }
}

void MainWindow::on_metric_list_widget_itemClicked(QListWidgetItem *item) {
  QListWidgetItem *agent_item = ui->agent_list_widget->currentItem();
  auto agent = agents_.find(agent_item->text());
  auto metrics_list = agent->second->GetMetrics();
  auto metric = metrics_list->at(ui->metric_list_widget->row(item));
  ShowMetricDetails(metric);
}

void MainWindow::AddAgent(std::shared_ptr<s21::AgentBundle> &agent) {
  QString name = QString::fromStdString(agent->GetAgentName());
  ui->agent_list_widget->addItem(name);
  agents_.insert({name, agent});
}

void MainWindow::RemoveAgent(std::shared_ptr<s21::AgentBundle> &agent) {
  ui->agent_list_widget->removeItemWidget(ui->agent_list_widget->findItems(
      QString::fromStdString(agent->GetAgentName()),
      Qt::MatchExactly).at(0));
  agents_.erase(QString::fromStdString(agent->GetAgentName()));
}

void MainWindow::UpdateAgent(std::shared_ptr<s21::AgentBundle> &agent) {
  (void) agent;
}

void MainWindow::OnCriticalValueReached(s21::MetricEvent event) {
  (void) event;
}

void MainWindow::InsertLogLine(const std::string &line) {
  (void) line;
}

void MainWindow::SetLogFile(const std::string &log_file) {
  QString app_dir = QCoreApplication::applicationDirPath();

  log_file_path_ = QDir::toNativeSeparators(app_dir + QDir::separator() + QString::fromStdString(log_file));
  log_file_path_ = QDir::cleanPath(log_file_path_);
  log_file_ = new QFile(log_file_path_);
  if (!log_file_->open(QIODevice::ReadOnly | QIODevice::Text))
    log_file_ = nullptr;
}

void MainWindow::ShowAgentDetails(std::shared_ptr<s21::AgentBundle> &agent) {
  ui->agent_enabled_checkbox->setEnabled(true);
  ui->type_label->setEnabled(true);
  ui->agent_type_value_line_edit->setEnabled(true);
  ui->enabled_label->setEnabled(true);
  ui->num_of_metrics_label->setEnabled(true);
  ui->num_of_metrics_value_label->setEnabled(true);
  ui->uptime_label->setEnabled(true);
  ui->uptime_value_label->setEnabled(true);
  ui->num_of_metrics_value_label->setText(QString::number(agent->GetMetrics()->size()));
//TODO  ui->uptime_value_label->setText(QString::number(agent->GetUptime()));
  ui->agent_type_value_line_edit->setText(QString::fromStdString(agent->GetAgentType()));
}

void MainWindow::ShowMetricDetails(std::shared_ptr<s21::ConfiguredMetric> &metric) {
  ui->critical_value_line_edit->setEnabled(true);
  ui->metric_critical_label->setEnabled(true);
  ui->metric_name_label->setEnabled(true);
  ui->metric_name_value_label->setEnabled(true);
  ui->update_time_value_spin_box->setEnabled(true);
  ui->metric_update_time_label->setEnabled(true);
  ui->update_time_value_spin_box->setValue(metric->GetUpdateTime());
  ui->metric_name_value_label->setText(QString::fromStdString(metric->GetName()));
  QString critical_value = QString::fromStdString(metric->GetCriticalValue().GetConditionOperator());
  critical_value.append(QString::number(metric->GetCriticalValue().GetCriticalValue()));
  ui->critical_value_line_edit->setText(critical_value);
}

void MainWindow::ClearAgentDetails() {
  ui->agent_enabled_checkbox->setDisabled(true);
  ui->type_label->setDisabled(true);
  ui->agent_type_value_line_edit->clear();
  ui->agent_type_value_line_edit->setDisabled(true);
  ui->enabled_label->setDisabled(true);
  ui->num_of_metrics_label->setDisabled(true);
  ui->num_of_metrics_value_label->clear();
  ui->num_of_metrics_value_label->setDisabled(true);
  ui->uptime_label->setDisabled(true);
  ui->uptime_value_label->clear();
  ui->uptime_value_label->setDisabled(true);
}

void MainWindow::ClearMetricDetails() {
  ui->critical_value_line_edit->clear();
  ui->critical_value_line_edit->setDisabled(true);
  ui->metric_critical_label->setDisabled(true);
  ui->metric_name_label->setDisabled(true);
  ui->metric_name_value_label->clear();
  ui->metric_name_value_label->setDisabled(true);
  ui->update_time_value_spin_box->cleanText();
  ui->update_time_value_spin_box->setDisabled(true);
  ui->metric_update_time_label->setDisabled(true);
}
void MainWindow::closeEvent(QCloseEvent *event) {
  QWidget::closeEvent(event);
  controller_->Quit();
}

void MainWindow::SetController(std::shared_ptr<s21::MainController> &controller) {
  controller_ = controller;
}
