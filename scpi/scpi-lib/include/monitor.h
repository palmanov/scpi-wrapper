#ifndef SCPI_MONITOR_H
#define SCPI_MONITOR_H

#include <QObject>
#include <QTimer>
#include <QFile>

#include <memory>

namespace scpi {

/**
 * @brief Класс "Мониторинг"
 *
 * Класс используется для мониторинга
 * и записи в лог-файл телеметрии устройства
 */
class Monitor : public QObject
{
  Q_OBJECT
 public:
  /**
   * @brief Конструктор с параметрами
   *
   * @param log_dir  Путь до лог-файла
   * @param interval  Интервал опроса телеметрии
   * @param parent    Объект-родитель
   */
  explicit Monitor(const QString& log_dir,
                   int interval = 1000,
                   QObject* parent = nullptr);

  /**
   * @brief Деструктор
   */
  ~Monitor();

 public slots:
  /**
   * @brief Метод инициализации мониторинга телеметрии устройства
   */
  void init();

 signals:
  /**
   * @brief Сигнал отправки запроса телеметрии
   */
  void measureRequest();

 public slots:
  /**
   * @brief Слот записи полученной телеметрии в лог-файл
   * @param Телеметрия
   */
  void writeToLogFile(const QString& message);

 private:
  //! Путь до лог-файла
  QString log_dir_;
  //! Интервал опроса
  int interval_;
  //! Таймер
  std::unique_ptr<QTimer> timer_;
  //! Лог-файл
  std::unique_ptr<QFile> log_file_;
};

} // namespace scpi

#endif // SCPI_MONITOR_H
