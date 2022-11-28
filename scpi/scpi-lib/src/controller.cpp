#include "controller.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <memory>

#include "scpi_command.h"

namespace scpi {

Controller::Controller(int interval, QObject *parent)
    : QObject{parent},
      monitoring_interval_(interval) {}

void Controller::init()
{
  receiver_external_request_ = std::make_unique<ExternalRequestReceiver>(8080);
  if (!receiver_external_request_->init()) {
    qCritical() << "";
  }

  scpi_device_ = std::make_unique<Device>(QHostAddress::LocalHost, 5025);
  if (!scpi_device_->init()) {
    qCritical() << "";
  }

  monitor_ = std::make_unique<Monitor>(QCoreApplication::applicationDirPath() + "/log", monitoring_interval_);
  monitor_->moveToThread(&monitor_thread_);

  QObject::connect(receiver_external_request_.get(), &ExternalRequestReceiver::sendReceivedRequest,
                   this, &Controller::receiveExternalRequest);

  QObject::connect(this, &Controller::sendToDevice,
                   scpi_device_.get(), &Device::sendCommandToDevice);

  QObject::connect(scpi_device_.get(), &Device::sendResponseFromDevice,
                   this, &Controller::receiveResponseFromDevice);

  QObject::connect(scpi_device_.get(), &Device::sendMeasureFromDevice,
                   this, &Controller::receiveMeasureFromDevice);

  QObject::connect(this, &Controller::sendToExternalServer,
                   receiver_external_request_.get(), &ExternalRequestReceiver::sendMessageToExternalServer);

  QObject::connect(&monitor_thread_, &QThread::started,
                   monitor_.get(), &Monitor::init);

  QObject::connect(monitor_.get(), &Monitor::measureRequest,
                   scpi_device_.get(), &Device::getMeasureSlot);

  QObject::connect(scpi_device_.get(), &Device::sendToLogFile,
                   monitor_.get(), &Monitor::writeToLogFile);

  monitor_thread_.start();
}

void Controller::executeCommand(const ScpiCommand& command)
{
  emit sendToDevice(ScpiCommand::toByteArray(command));
}

void Controller::receiveExternalRequest(const QByteArray& external_request)
{
  QJsonParseError parse_error;
  QJsonDocument json_doc = QJsonDocument::fromJson(external_request, &parse_error);

  if (parse_error.error != QJsonParseError::ParseError::NoError) {
    qDebug() << parse_error.errorString();
    return;
  }

  auto command = ScpiCommand::fromJson(json_doc.object());
  if (!command) {
    qDebug() << "Command is not valid!";
    return;
  }

  emit sendToDevice(ScpiCommand::toByteArray(command.value()));
}

void Controller::receiveMeasureFromDevice(const QString& message)
{
  QJsonDocument json_doc;
  QJsonArray arr;

  int index = 0;
  for (auto& data : message.splitRef('\n')) {
    auto time_and_data = data.split('\t');
    if (time_and_data.count() == 2) {
      auto timestamp = time_and_data.at(0).toLongLong();
      auto channel_and_msg = time_and_data.at(1).split(':');
      if (channel_and_msg.count() == 2) {
        auto channel = channel_and_msg.at(0).right(1).toInt();
        auto values = channel_and_msg.at(1).split(',');
        if (values.count() == 4) {
          QJsonObject measure;

          measure.insert("channel", channel);
          measure.insert("timestamp", timestamp);
          measure.insert("state", values.at(0).toInt());
          measure.insert("current", values.at(1).toDouble());
          measure.insert("voltage", values.at(2).toDouble());
          measure.insert("power", values.at(3).toDouble());

          arr.insert(index, measure);
        }
      }
    }

    ++index;
  }

  if (!arr.isEmpty()) {
    json_doc.setArray(arr);

    emit sendToExternalServer(json_doc.toJson(QJsonDocument::Compact));
  }
}

void Controller::receiveResponseFromDevice(const QString& message)
{
  const auto& fields = message.splitRef('\t');
  if (fields.count() == 2) {
    QJsonDocument json_doc;
    QJsonObject response;

    response.insert("timestamp", fields.at(0).toLongLong());
    response.insert("answer", fields.at(1).toString());

    json_doc.setObject(response);

    emit sendToExternalServer(json_doc.toJson(QJsonDocument::Compact));
  }
}

} // namespace scpi
