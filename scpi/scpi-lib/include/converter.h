#ifndef SCPI_CONVERTER_H
#define SCPI_CONVERTER_H

#include <QObject>

#include <QJsonObject>

namespace scpi {

/**
 * @brief The Converter class
 */
class Converter : public QObject
{
  Q_OBJECT
 public:
  /**
   * @brief Converter
   * @param parent
   */
  explicit Converter(QObject* parent = nullptr);

 signals:
  /**
   * @brief
   */
  void sendScpi();
  /**
   * @brief sendJson
   */
  void sendJson();


};

} // namespace scpi

#endif // SCPI_CONVERTER_H
