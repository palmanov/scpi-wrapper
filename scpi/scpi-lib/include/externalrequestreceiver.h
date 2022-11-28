#ifndef SCPI_EXTERNALREQUESTRECEIVER_H
#define SCPI_EXTERNALREQUESTRECEIVER_H

#include <QObject>
#include <QTcpSocket>

#include <memory>

namespace scpi {

/**
 * @brief Класс для приема внешних запросов
 */
class ExternalRequestReceiver : public QObject
{
  Q_OBJECT
 public:
  /**
   * @brief Конструктор с параметрами
   * @param port    Порт хоста
   * @param parent  Объект-родитель
   */
  explicit ExternalRequestReceiver(quint16 port = 8080,
                                  QObject* parent = nullptr);

 public slots:
  /**
   * @brief Метод инициализации соединения с устройством
   * @return
   */
  bool init();
  /**
   * @brief Метод для отправки телеметрии и ответов на команды
   * @param response
   */
  void sendMessageToExternalServer(const QByteArray& response);

 signals:
  /**
   * @brief Сигнал отправки запроса на обработку
   * @param JSON-запрос
   */
  void sendReceivedRequest(const QByteArray& request);



 private slots:
  /**
   * @brief readyReadSlot
   */
  void readyReadSlot();

 private:
  //! Port
  quint16 port_;
  //! TCP-сокет
  std::unique_ptr<QTcpSocket> socket_;
};

} // namespace scpi

#endif // SCPI_EXTERNALREQUESTHANDLER_H
