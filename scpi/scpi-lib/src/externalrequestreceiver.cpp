#include "externalrequestreceiver.h"

#include <QHostAddress>
#include <QDebug>

namespace scpi {

ExternalRequestReceiver::ExternalRequestReceiver(quint16 port, QObject* parent)
    : QObject{parent},
      port_(port) {}

bool ExternalRequestReceiver::init() {
  socket_ = std::make_unique<QTcpSocket>();
  socket_->connectToHost(QHostAddress::LocalHost, port_);

  if (!socket_->waitForConnected()) {
    qDebug() << "Socket is not connected!";
    return false;
  }

  QObject::connect(socket_.get(), &QAbstractSocket::readyRead,
                   this, &ExternalRequestReceiver::readyReadSlot);

  return true;
}

void ExternalRequestReceiver::sendMessageToExternalServer(const QByteArray& response)
{
  if (socket_ && socket_->isOpen())
    socket_->write(response);
}

void ExternalRequestReceiver::readyReadSlot() {
  if (socket_ && socket_->isOpen())
    emit sendReceivedRequest(socket_->readAll());
}

} // namespace scpi
