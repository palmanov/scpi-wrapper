#ifndef SCPI_CONTROLLER_H
#define SCPI_CONTROLLER_H

#include <QObject>
#include <QThread>

#include "scpi_command.h"

#include "externalrequestreceiver.h"
#include "device.h"
#include "monitor.h"

namespace scpi {

/**
 * @brief Класс "Контроллер"
 *
 * Используется для организации связи между объектами
 */
class Controller : public QObject
{
  Q_OBJECT
 public:
  /**
   * @brief Конструктор с параметрами
   *
   * @param interval  Интервал опроса телеметрии
   * @param parent    Объект-родитель
   */
  explicit Controller(int interval = 1000,
                      QObject* parent = nullptr);

 public slots:
  /**
   * @brief Метод инициализации компонентов
   */
  void init();
  /**
   * @brief Метод передачи на устройство SCPI команды
   * @param command SCPI-команда
   */
  void executeCommand(const scpi::ScpiCommand& command);
  /**
   * @brief Слот обработки запроса с внешнего API
   *
   * Принимает внешний запрос на обработку
   * в строковом формате JSON и отправляет на устройство
   * по протоколу SCPI
   *
   * @param external_request Запрос с внешнего API
   */
  void receiveExternalRequest(const QByteArray& external_request);

 private slots:
  /**
   * @brief Слот обработки телеметрии
   *
   * @param message Телеметрия
   */
  void receiveMeasureFromDevice(const QString& message);
  /**
   * @brief Слот для обработки ответа на запрос
   *
   * @param message Ответ
   */
  void receiveResponseFromDevice(const QString& message);

 signals:
  /**
   * @brief Сигнал отправки команды на устройство
   * @param command SCPI-команда
   */
  void sendToDevice(const QByteArray& command);
  /**
   * @brief Сигнал отправки телеметрии/ответа
   * в строковом формате JSON
   * @param Телеметрия/ответ
   */
  void sendToExternalServer(const QByteArray& json);

 private:
  //! Интервал опроса телеметрии
  int monitoring_interval_;
  //! Внешнее API
  std::unique_ptr<ExternalRequestReceiver> receiver_external_request_;
  //! Устройство
  std::unique_ptr<Device> scpi_device_;
  //! Мониторинг телеметрии
  std::unique_ptr<Monitor> monitor_;
  //! Поток для монитора
  QThread monitor_thread_;
};

} // namespace scpi

#endif // SCPI_CONTROLLER_H
