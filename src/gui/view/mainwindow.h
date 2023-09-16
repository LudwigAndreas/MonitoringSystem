#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/qfile.h>

#include <QListWidgetItem>
#include <QMainWindow>

#include "core/metric/metric_event.h"

namespace s21 {
class MainController;
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(std::string &agents_folder, QWidget *parent = nullptr);
  ~MainWindow();

  void AddAgent(std::shared_ptr<s21::AgentBundle> &agent);
  void RemoveAgent(std::shared_ptr<s21::AgentBundle> &agent);
  void UpdateAgent(std::shared_ptr<s21::AgentBundle> &agent);
  void OnCriticalValueReached(s21::MetricEvent event);
  void InsertLogLine(const std::string &line);
  void SetLogFile(const std::string &log_file);
  void SetAgentsPath(const std::string &agents_path);
  void SetController(std::shared_ptr<s21::MainController> &controller);

 private slots:

  void on_agent_list_widget_itemClicked(QListWidgetItem *item);
  void on_metric_list_widget_itemClicked(QListWidgetItem *item);
  void update_monitor_log_view();
  void AddAgentAction();
  void on_delete_agent_button_clicked();
  void on_add_agent_button_clicked();
  void on_pushButton_clicked();
  void UpdateAgentUptime();

  void on_save_email_push_button_clicked();

  void on_save_telegram_push_button_clicked();

  void on_main_tab_widget_currentChanged(int index);

 private:
  void closeEvent(QCloseEvent *event) override;
  void ShowAgentDetails(std::shared_ptr<s21::AgentBundle> &agent);
  void ShowMetricDetails(std::shared_ptr<s21::ConfiguredMetric> &metric);
  void ClearAgentDetails();
  void ClearMetricDetails();
  bool CopyDirecory(const QString &from, const QString &to);

  Ui::MainWindow *ui;
  QTimer *log_timer_;
  QTimer *uptime_timer_;
  qint64 last_read_pos_ = 0;
  QString log_file_path_;
  std::vector<std::shared_ptr<s21::AgentBundle>> agents_;
  QFile *log_file_;
  std::shared_ptr<s21::MainController> controller_;
  QString agents_folder_;
};
#endif  // MAINWINDOW_H
