#ifndef SCPI_DEVICE_H
#define SCPI_DEVICE_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>

#include <memory>

namespace scpi {

/**
 * @brief Класс "Устройство"
 *
 * Используется для передачи и получения информации с устройства
 */
class Device : public QObject
{
  Q_OBJECT
 public:
  /**
   * @brief Конструктор с параметрами
   *
   * @param ip_address  IP-адрес
   * @param port        Порт
   * @param parent      Объект-родитель
   */
  explicit Device(QHostAddress ip_address,
                  quint16 port = 5025,
                  QObject* parent = nullptr);

 public slots:
  /**
   * @brief Метод инициализации соединения с устройством
   * @return
   */
  bool init();
  /**
   * @brief Слот для отправки команды на устройство
   * @param Команда
   */
  void sendCommandToDevice(const QByteArray& command);
  /**
   * @brief Слот для получения телеметрия от устройства
   */
  void getMeasureSlot();

 signals:
  /**
   * @brief Сигнал отправки полученного ответа от устройства
   * @param Ответ
   */
  void sendResponseFromDevice(const QString& answer);
  /**
   * @brief Сигнал отправки полученной телеметрии от устройства
   * @param Телеметрия
   */
  void sendMeasureFromDevice(const QString& measure);
  /**
   * @brief Сигнал отправки полученной телеметрии на запись в лог-файл
   * @param Телеметрия
   */
  void sendToLogFile(const QString& response);

 private:
  //! IP-адрес устройства
  QHostAddress ip_address_;
  //! Номер порта
  quint16 port_;
  //! TCP-сокет
  std::unique_ptr<QTcpSocket> socket_;
};

} // namespace scpi

#endif // SCPI_DEVICE_H
