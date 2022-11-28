#ifndef SCPI_SCPI_H
#define SCPI_SCPI_H

#include <QJsonObject>

#include <optional>

namespace scpi {

/**
 * @brief Структура "SCPI команда"
 */
struct ScpiCommand
{
  //! Имя команды
  QString command_name_;
  //! Параметры
  std::vector<std::variant<int, double>> parameter_;

  /**
   * @brief Функция конвертации JSON-запроса в SCPI
   *
   * @param JSON-объект
   *
   * @return SCPI-команда
   */
  static std::optional<ScpiCommand> fromJson(const QJsonObject& json);
  /**
   * @brief Функция подготовки к отправке SCPI-команды
   *
   * @param command SCPI-команда
   *
   * @return Данные для отправки в сокет
   */
  static QByteArray toByteArray(const ScpiCommand& command);
};

} // namespace scpi

#endif // SCPI_SCPI_H
