#include "device.h"

#include <QDateTime>

#include "scpi_command.h"

namespace scpi {

Device::Device(QHostAddress ip_address,
               quint16 port,
               QObject* parent)
    : QObject{parent},
      ip_address_(ip_address),
      port_(port) {}

bool Device::init()
{
  socket_ = std::make_unique<QTcpSocket>();
  socket_->connectToHost(ip_address_, port_);

  if (!socket_->waitForConnected()) {
    qCritical() << "Socket is not connected!";
    return false;
  }

  return true;
}

void Device::sendCommandToDevice(const QByteArray& command)
{
  const auto timestamp = QDateTime::currentMSecsSinceEpoch();

  socket_->write(command);

  if (socket_->waitForReadyRead()) {
    QString response = QString("%1\t%2").arg(QString::number(timestamp),
                                             QString(socket_->readAll()));

    emit sendResponseFromDevice(response);
  }
}

void Device::getMeasureSlot()
{
  if (!socket_ || !socket_->isOpen())
    return;

  QString log_data;
  for (int i = 1; i <= 4; i++) {
    ScpiCommand cmd;
    cmd.command_name_ = "state";
    cmd.parameter_.push_back(i);

    socket_->write(ScpiCommand::toByteArray(cmd));

    if (socket_->waitForReadyRead()) {
      const auto response = QString(socket_->readAll());
      log_data += QString("%1\tCH%2:%3").arg(QString::number(QDateTime::currentMSecsSinceEpoch()),
                                             QString::number(i),
                                             response);
    }
  }

  emit sendToLogFile(log_data);
  emit sendMeasureFromDevice(log_data);
}

} // namespace scpi
