
#include "mainwindow.h"

#include <QStandardItemModel>
#include <QTimer>
#include <utility>
#include <QtCore/qfile.h>
#include <QtCore/qdir.h>
#include <QFileDialog>
#include <QMessageBox>

#include "./ui_mainwindow.h"
#include "../controller/MainController.h"

MainWindow::MainWindow(std::string &agents_folder, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connect(ui->actionAdd_Agent, SIGNAL(triggered()), this, SLOT(AddAgentAction()));

  log_timer_ = new QTimer(this);
  uptime_timer_ = new QTimer(this);
  connect(log_timer_, SIGNAL(timeout()), this, SLOT(update_monitor_log_view()));
  connect(uptime_timer_, SIGNAL(timeout()), this, SLOT(UpdateAgentUptime()));
  log_timer_->start(3000);
  uptime_timer_->start(1000);
  last_read_pos_ = 0;

  ClearMetricDetails();
  ClearAgentDetails();
  SetAgentsPath(agents_folder);
}

MainWindow::~MainWindow() {
  if (log_file_ && log_file_->isOpen())
    log_file_->close();
  delete log_file_;
  delete log_timer_;
  delete ui;
}

void MainWindow::update_monitor_log_view() {
  if (!log_file_ || !log_file_->isOpen()) {
    return;
  }

  log_file_->seek(last_read_pos_);

  QTextStream in(log_file_);
  QString new_text = in.readAll();

  ui->monitor_log_view->moveCursor(QTextCursor::End);
  ui->monitor_log_view->insertPlainText(new_text);

  last_read_pos_ += new_text.size();
  ui->monitor_log_view->moveCursor(QTextCursor::End);
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
  if (ui->agent_list_widget->currentItem()) {
    if (ui->agent_list_widget->currentItem()->text() == QString::fromStdString(agent->GetAgentName())) {
      ui->metric_list_widget->clear();
      ClearAgentDetails();
      ClearMetricDetails();
    }
  }
  QListWidgetItem *agent_item = ui->agent_list_widget->findItems(
      QString::fromStdString(agent->GetAgentName()), Qt::MatchCaseSensitive).at(0);
  delete ui->agent_list_widget->takeItem(ui->agent_list_widget->row(agent_item));
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

void MainWindow::SetAgentsPath(const std::string &agents_path) {
  QString app_dir = QCoreApplication::applicationDirPath();

  agents_folder_ = QDir::toNativeSeparators(app_dir + QDir::separator() + QString::fromStdString(agents_path));
  agents_folder_ = QDir::cleanPath(agents_folder_);
}

void MainWindow::UpdateAgentUptime() {
  if (ui->agent_list_widget->currentItem() != nullptr && ui->uptime_value_label->isEnabled()) {
    auto agent = agents_.find(ui->agent_list_widget->currentItem()->text());
    if (agent != agents_.end()) {
      std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsedSeconds = currentTime - agent->second->GetUptimePoint();

      long hours = std::chrono::duration_cast<std::chrono::hours>(elapsedSeconds).count();
      long minutes = std::chrono::duration_cast<std::chrono::minutes>(elapsedSeconds).count() % 60;
      long seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsedSeconds).count() % 60;

      ui->uptime_value_label->setText(QString("%1:%2:%3").arg(hours).arg(minutes).arg(seconds));
    }
  }

}

void MainWindow::ShowAgentDetails(std::shared_ptr<s21::AgentBundle> &agent) {
  ui->delete_agent_button->setEnabled(true);
//  ui->add_agent_button->setEnabled(true);
  ui->agent_enabled_checkbox->setEnabled(true);
  ui->type_label->setEnabled(true);
  ui->agent_type_value_line_edit->setEnabled(true);
  ui->enabled_label->setEnabled(true);
  ui->num_of_metrics_label->setEnabled(true);
  ui->num_of_metrics_value_label->setEnabled(true);
  ui->uptime_label->setEnabled(true);
  ui->uptime_value_label->setEnabled(true);
  ui->num_of_metrics_value_label->setText(QString::number(agent->GetMetrics()->size()));
  ui->uptime_value_label->setEnabled(true);
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
  ui->delete_agent_button->setDisabled(true);
  ui->uptime_value_label->setDisabled(true);
  ui->uptime_value_label->setText("00:00:00");
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

void MainWindow::AddAgentAction() {
  QString agent_folder_path = QFileDialog::getExistingDirectory(this, "Select Agent Folder", QDir::homePath());

  if (!agent_folder_path.isEmpty())
  {
    QString target_folder = agents_folder_;
    QDir target_dir(target_folder);

    if (!target_dir.exists())
    {
      if (!target_dir.mkpath(target_folder))
      {
        QMessageBox::critical(this, "Error", "Failed to create target folder.");
        return;
      }
    }

    QString agent_folder_name = QFileInfo(agent_folder_path).fileName();
    QString destination_path = target_folder + "/" + agent_folder_name;

    if (QFile::copy(agent_folder_path, destination_path))
    {
      QMessageBox::information(this, "Success", "Agent folder added successfully.");
    }
    else
    {
      QMessageBox::critical(this, "Error", "Failed to add agent folder.");
    }
  }

}

void MainWindow::on_add_agent_button_clicked() {
  AddAgentAction();
}


void MainWindow::on_delete_agent_button_clicked(){
  if (ui->agent_list_widget->currentItem()) {
    QMessageBox::information(this, "Warn", "Can not delete agent right now cause agent don't contain path to agent.");
  }
}

