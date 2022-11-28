#include "scpi_command.h"

#include <QJsonArray>

namespace scpi {

std::optional<ScpiCommand> ScpiCommand::fromJson(const QJsonObject& json) {
  ScpiCommand scpi_data;

  if (!json.contains("command"))
    return {};

  scpi_data.command_name_ = json["command"].toString();
  auto parameter_object = json["parameter"].toObject();

  if (scpi_data.command_name_ == "power_on") {
    if (parameter_object.count() == 3
        && parameter_object.contains("channel")
        && parameter_object.contains("current")
        && parameter_object.contains("voltage")) {
      int channel = parameter_object["channel"].toInt();
      if (channel < 1 || channel > 4)
        return {};

      scpi_data.parameter_.push_back(channel);

      // Нужно добавить проверку на валидность значений
      scpi_data.parameter_.push_back(parameter_object["current"].toDouble());
      scpi_data.parameter_.push_back(parameter_object["voltage"].toDouble());
    } else {
      return {};
    }
  } else if (scpi_data.command_name_ == "power_off") {
    if (parameter_object.count() == 1
        && parameter_object.contains("channel")) {
      int channel = parameter_object["channel"].toInt();
      if (channel < 1 || channel > 4)
        return {};

      scpi_data.parameter_.push_back(channel);
    } else {
      return {};
    }
  } else if (scpi_data.command_name_ == "state") {
    auto parameter_object = json["parameter"].toObject();
    if (parameter_object.count() == 1
        && parameter_object.contains("channel")) {
      int channel = parameter_object["channel"].toInt();
      if (channel < 1 || channel > 4)
        return {};

      scpi_data.parameter_.push_back(channel);
    } else {
      return {};
    }
  }

  return scpi_data;
}

QByteArray ScpiCommand::toByteArray(const ScpiCommand& command)
{
  QString command_str;

  if (command.command_name_ == "power_on") {
    command_str += QString(":SOURce%1:CURRent %2\n")
                       .arg(std::get<int>(command.parameter_[0]))
                       .arg(std::get<double>(command.parameter_[1]));

    command_str += QString(":SOURce%1:VOLTage %2\n")
                       .arg(std::get<int>(command.parameter_[0]))
                       .arg(std::get<double>(command.parameter_[2]));

    command_str += QString(":SOURce%1:STATe ON\n")
                       .arg(std::get<int>(command.parameter_[0]));

    command_str += QString(":OUTPut%1:STATe?\n")
                       .arg(std::get<int>(command.parameter_[0]));
  } else if (command.command_name_ == "power_off") {
    command_str += QString(":SOURce%1:STATe OFF\n")
                       .arg(std::get<int>(command.parameter_[0]));

    command_str += QString(":OUTPut%1:STATe?\n")
                       .arg(std::get<int>(command.parameter_[0]));
  } else if (command.command_name_ == "state") {
    command_str += QString(":MEASure%1:ALL?\n")
                       .arg(std::get<int>(command.parameter_[0]));
  }

  return command_str.toUtf8();
}

} // namespace scpi
