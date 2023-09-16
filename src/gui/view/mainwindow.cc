
#include "mainwindow.h"

#include <QStandardItemModel>
#include <QTextStream>
#include <QTimer>
#include <utility>
#include <QtCore/qfile.h>
#include <QtCore/qdir.h>
#include <QFileDialog>
#include <QMessageBox>

#include "./ui_mainwindow.h"
#include "../controller/main_controller.h"
#include "metric/metric_critical_value.h"

MainWindow::MainWindow(std::string &agents_folder, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) , log_file_(nullptr) {
  ui->setupUi(this);

  connect(ui->actionAdd_Agent,
          SIGNAL(triggered()),
          this,
          SLOT(AddAgentAction()));

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
    if (log_file_path_.isEmpty())
      return;
    log_file_ = new QFile(log_file_path_);
    if (!log_file_->open(QIODevice::ReadOnly | QIODevice::Text)) {
      log_file_ = nullptr;
      return;
    }
  }

  log_file_->seek(last_read_pos_);

  QTextStream in = QTextStream(log_file_);
  QString new_text = in.readAll();

  if (!ui->monitor_log_view->textCursor().atEnd()) {
    ui->monitor_log_view->moveCursor(QTextCursor::End);
  }
  ui->monitor_log_view->insertPlainText(new_text);

  last_read_pos_ += new_text.size();
  if (!ui->monitor_log_view->textCursor().atEnd()) {
    ui->monitor_log_view->moveCursor(QTextCursor::End);
  }
}

void MainWindow::on_agent_list_widget_itemClicked(QListWidgetItem *item) {
  if (item != nullptr) {
    ui->metric_list_widget->clear();
    ClearAgentDetails();
    ClearMetricDetails();
    try {
      auto agent = agents_.at(ui->agent_list_widget->row(item));
      auto metrics = agent->GetMetrics();
      for (auto &metric: *metrics) {
        ui->metric_list_widget->addItem(QString::fromStdString(metric->GetName()));
      }
      if (agent) {
        ShowAgentDetails(agent);
      }
    } catch (std::out_of_range &e) {
      // ignore
    }
  }
}

void MainWindow::on_metric_list_widget_itemClicked(QListWidgetItem *item) {
  QListWidgetItem *agent_item = ui->agent_list_widget->currentItem();
  try {
    auto agent = agents_.at(ui->agent_list_widget->row(agent_item));
    auto metrics_list = agent->GetMetrics();
    auto metric = metrics_list->at(ui->metric_list_widget->row(item));
    if (agent != nullptr && metric != nullptr) {
      ShowMetricDetails(metric);
    }
  } catch (std::out_of_range &e) {
    // ignore
  }
}

void MainWindow::AddAgent(std::shared_ptr<s21::AgentBundle> &agent) {
  ui->agent_list_widget->addItem(agent->GetAgentName().c_str());
  agents_.emplace_back(agent);
}

void MainWindow::RemoveAgent(std::shared_ptr<s21::AgentBundle> &agent) {
  if (ui->agent_list_widget->currentItem()) {
    if (ui->agent_list_widget->currentItem()->text()
        == QString::fromStdString(agent->GetAgentName())) {
      ui->metric_list_widget->clear();
      ClearAgentDetails();
      ClearMetricDetails();
    }
  }
  QListWidgetItem *agent_item = ui->agent_list_widget->findItems(
      QString::fromStdString(agent->GetAgentName()), Qt::MatchCaseSensitive).at(
      0);
  delete ui->agent_list_widget->takeItem(ui->agent_list_widget->row(agent_item));
  agents_.erase(std::remove(agents_.begin(), agents_.end(), agent),
                agents_.end());
}

void MainWindow::UpdateAgent(std::shared_ptr<s21::AgentBundle> &updated) {
  (void) updated;
  ui->agent_list_widget->clear();
  for (auto &agent: agents_) {
    ui->agent_list_widget->addItem(agent->GetAgentName().c_str());
  }
}

void MainWindow::OnCriticalValueReached(s21::MetricEvent event) {
  (void) event;
}

void MainWindow::InsertLogLine(const std::string &line) {
  (void) line;
}

void MainWindow::SetLogFile(const std::string &log_file) {
  QString app_dir = QCoreApplication::applicationDirPath();

  log_file_path_ = QDir::toNativeSeparators(
      app_dir + QDir::separator() + QString::fromStdString(log_file));
  log_file_path_ = QDir::cleanPath(log_file_path_);
  log_file_ = new QFile(log_file_path_);
  if (!log_file_->open(QIODevice::ReadOnly | QIODevice::Text))
    log_file_ = nullptr;
}

void MainWindow::SetAgentsPath(const std::string &agents_path) {
  QString app_dir = QCoreApplication::applicationDirPath();

  agents_folder_ = QDir::toNativeSeparators(
      app_dir + QDir::separator() + QString::fromStdString(agents_path));
  agents_folder_ = QDir::cleanPath(agents_folder_);
}

void MainWindow::UpdateAgentUptime() {
  if (ui->agent_list_widget->currentItem() != nullptr
      && ui->uptime_value_label->isEnabled()) {
    try {
      auto agent = agents_.at(ui->agent_list_widget->currentRow());
      std::chrono::system_clock::time_point
          currentTime = std::chrono::system_clock::now();
      std::chrono::duration<double>
          elapsedSeconds = currentTime - agent->GetUptimePoint();

      long hours =
          std::chrono::duration_cast<std::chrono::hours>(elapsedSeconds).count();
      long minutes =
          std::chrono::duration_cast<std::chrono::minutes>(elapsedSeconds).count()
              % 60;
      long seconds =
          std::chrono::duration_cast<std::chrono::seconds>(elapsedSeconds).count()
              % 60;

      ui->uptime_value_label->setText(QString("%1:%2:%3").arg(hours).arg(minutes).arg(
          seconds));
    } catch (std::out_of_range &e) {
      // ignore
    }
  }

}

void MainWindow::ShowAgentDetails(std::shared_ptr<s21::AgentBundle> &agent) {
  ui->agent_name_label_value->setEnabled(true);
  ui->agent_name_label_value->setText(QString::fromStdString(agent->GetAgentName()));
  ui->delete_agent_button->setEnabled(true);
//  ui->add_agent_button->setEnabled(true);
  ui->agent_enabled_checkbox->setEnabled(true);
  ui->agent_enabled_checkbox->setChecked(agent->IsEnabled());
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
  QString critical_value =
      QString::fromStdString(metric->GetCriticalValue().GetConditionOperator());
  critical_value.append(QString::number(metric->GetCriticalValue().GetCriticalValue()));
  ui->critical_value_line_edit->setText(critical_value);
}

void MainWindow::ClearAgentDetails() {
  ui->agent_name_label_value->clear();
  ui->agent_name_label_value->setDisabled(true);
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
  QString agent_folder_path = QFileDialog::getExistingDirectory(this,
                                                                "Select Agent Folder",
                                                                QDir::homePath());

  if (!agent_folder_path.isEmpty()) {
    QString target_folder = agents_folder_;
    QDir target_dir(target_folder);

    if (!target_dir.exists()) {
      if (!target_dir.mkpath(target_folder)) {
        QMessageBox::critical(this, "Error", "Failed to create target folder.");
        return;
      }
    }

    QString agent_folder_name = QFileInfo(agent_folder_path).fileName();
    QString destination_path = target_folder + "/" + agent_folder_name;

    if (QFile::copy(agent_folder_path, destination_path)) {
      QMessageBox::information(this,
                               "Success",
                               "Agent folder added successfully.");
    } else {
      QMessageBox::critical(this, "Error", "Failed to add agent folder.");
    }
  }

}

void MainWindow::on_add_agent_button_clicked() {
  AddAgentAction();
}

void MainWindow::on_delete_agent_button_clicked() {
  if (ui->agent_list_widget->currentItem()) {
    QMessageBox::StandardButton reply =
        QMessageBox::warning(this,
                             "Monitoring System",
                             "Do you really want to delete this agent? This action cannot be undone."
                             "The agent folder will be deleted. This may take up to a few minutes.",
                             QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
      return;
    }
    try {
      auto agent = agents_.at(ui->agent_list_widget->currentRow());
      controller_->DeleteAgent(agent);
    } catch (std::out_of_range &e) {
      return;
    }
  }
}

void MainWindow::on_pushButton_clicked() {
  if (ui->agent_list_widget->currentItem()) {
    try {
      s21::AgentBundlePtr
          agent = agents_.at(ui->agent_list_widget->currentRow());
      agent->SetName(ui->agent_name_label_value->text().toStdString());
      agent->SetType(ui->agent_type_value_line_edit->text().toStdString());
      agent->SetIsEnabled(ui->agent_enabled_checkbox->isChecked());
      for (auto &metric: *agent->GetMetrics()) {
        if (ui->metric_list_widget->currentItem() && metric->GetName()
            == ui->metric_name_value_label->text().toStdString()) {
          metric->SetUpdateTime(ui->update_time_value_spin_box->value());
          metric->SetCriticalValue(s21::MetricCriticalValue(ui->critical_value_line_edit->text().toStdString()));
        }
      }
      controller_->SaveConfiguration(agents_.at(ui->agent_list_widget->currentRow()));
    } catch (std::out_of_range &e) {
      return;
    }
  }
}


void MainWindow::on_save_email_push_button_clicked() {
  controller_->SetEmailSender(ui->email_address_value->text().toStdString());
}


void MainWindow::on_save_telegram_push_button_clicked() {
  controller_->SetTelegramSender(ui->telegram_username_value->text().toStdString());
}


void MainWindow::on_main_tab_widget_currentChanged(int index) {
  if (index == 3) {
    ui->telegram_username_value->setText(QString::fromStdString(controller_->GetTelegramSender()));
    ui->email_address_value->setText(QString::fromStdString(controller_->GetEmailSender()));
  }
}

