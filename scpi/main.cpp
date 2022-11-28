#include <QCoreApplication>
#include <QCommandLineParser>

#include <QThread>
#include <QDir>

#include <cstdio>
#include <iostream>

#include "controller.h"
#include "signalgenerator.h"

bool isChannelValid(int channel);

int main(int argc, char *argv[])
{
  using namespace scpi;

  QCoreApplication a(argc, argv);
  QCoreApplication::setApplicationName("SCPI Wrapper");
  QCoreApplication::setApplicationVersion("0.1");

  // Парсер для аргументов командной строки
  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption interval_option(QStringList() << "i" << "interval",
                                     QCoreApplication::translate("main", "Интервал опроса телеметрии <ms>."),
                                     QCoreApplication::translate("main", "ms"));
  parser.addOption(interval_option);

  parser.process(a);

  // Зададим интервал опроса телеметрии
  int interval = 1000;
  if (parser.isSet(interval_option)) {
    interval = parser.value(interval_option).toInt();
  }

  // Создание директории для лог-файлов
  QDir(a.applicationDirPath()).mkdir("log");

  // Эмиттер сигналов для отправки команд, внесенные в командную
  // строку, в контроллер
  SignalGenerator siggen;

  QThread thread;
  Controller controller(interval);

  // Отправим контроллер в другой поток
  controller.moveToThread(&thread);

  qRegisterMetaType<scpi::ScpiCommand>("scpi::ScpiCommand");
  QObject::connect(&siggen, &SignalGenerator::sendCommand,
                   &controller, &Controller::executeCommand);

  QObject::connect(&thread, &QThread::started,
                   &controller, &Controller::init);

  thread.start();

  // Цикл для приема команд и их аргументов
  while (1) {
    std::string command, args;
    std::cin >> command >> args;

    if (command == "on") {
      int channel = -1;
      double current = 0.0;
      double voltage = 0.0;

      if ((std::sscanf(args.c_str(), "%i,%lf,%lf", &channel, &current, &voltage) == 3)
          && isChannelValid(channel)) {
        ScpiCommand command;

        command.command_name_ = "power_on";
        command.parameter_.insert(command.parameter_.begin(), { channel, current, voltage });

        siggen.setCommand(command);
      }
    } else if (command == "off") {
      int channel = -1;

      if ((std::sscanf(args.c_str(), "%i", &channel) == 1)
          && isChannelValid(channel)) {
        ScpiCommand command;

        command.command_name_ = "power_off";
        command.parameter_.push_back(channel);

        siggen.setCommand(command);
      }
    } else if (command == "state") {
      int channel = -1;

      if ((std::sscanf(args.c_str(), "%i", &channel) == 1)
          && isChannelValid(channel)) {
        ScpiCommand command;

        command.command_name_ = "state";
        command.parameter_.push_back(channel);

        siggen.setCommand(command);
      }
    }
  }

  return a.exec();
}

bool isChannelValid(int channel)
{
  if (channel < 1 || channel > 4) {
    std::cout << "Некорректный номер канала!\nЗначение должно быть 1..4\n";
    return false;
  }

  return true;
}
