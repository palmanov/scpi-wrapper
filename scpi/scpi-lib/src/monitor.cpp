#include "monitor.h"

#include <QDateTime>
#include <QDebug>

namespace scpi {

Monitor::Monitor(const QString& log_dir,
                 int interval,
                 QObject* parent)
    : QObject{parent},
      log_dir_(log_dir),
      interval_(interval) {}

Monitor::~Monitor()
{
  timer_->stop();

  if (log_file_->isOpen()) {
    log_file_->close();
  }
}

void Monitor::init()
{
  if (!log_dir_.isEmpty()) {
    QString start_time = QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss.zzz");
    log_file_ = std::make_unique<QFile>(log_dir_ + '/' + start_time + ".log");
    if (!log_file_->open(QIODevice::WriteOnly)) {
      qWarning() << "Лог-файл не был открыт!";
    }
  }

  timer_ = std::make_unique<QTimer>();
  timer_->setInterval(interval_);

  QObject::connect(timer_.get(), &QTimer::timeout,
                   this, &Monitor::measureRequest);

  timer_->start();
}

void Monitor::writeToLogFile(const QString& message)
{
  if (!log_file_->isOpen())
    return;

  QTextStream out(log_file_.get());
  out << message;
}

} // namespace scpi
