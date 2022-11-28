#ifndef SCPI_SIGNALGENERATOR_H
#define SCPI_SIGNALGENERATOR_H

#include <QObject>

#include "scpi_command.h"

namespace scpi {

class SignalGenerator : public QObject
{
  Q_OBJECT
 public:
  explicit SignalGenerator(QObject *parent = nullptr)
      : QObject(parent) {}

 signals:
  void sendCommand(const scpi::ScpiCommand& command);

 public slots:
  void setCommand(const scpi::ScpiCommand& command) {
    emit sendCommand(command);
  }

};

} // namespace scpi

#endif // SCPI_SIGNALGENERATOR_H
