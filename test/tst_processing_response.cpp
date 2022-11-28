#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace testing;

#include "controller.h"

using namespace scpi;

TEST(ProcessingResponse_test, power_on_test)
{
  const QString response("000000000\t1");

  Controller controller;
  QSignalSpy spy(&controller, &Controller::sendToExternalServer);

  controller.receiveResponseFromDevice(response.toUtf8());
  EXPECT_EQ(spy.count(), 1);

  QList<QVariant> arguments = spy.takeFirst();

  QJsonParseError parse_error;
  QJsonDocument json_doc = QJsonDocument::fromJson(arguments.at(0).toByteArray(), &parse_error);
  EXPECT_EQ(parse_error.error, QJsonParseError::ParseError::NoError);
  EXPECT_EQ(json_doc.object().contains("answer"), true);
  EXPECT_EQ(json_doc.object().value("answer"), "1");
}

TEST(ProcessingResponse_test, power_off_test)
{
  const QString response("000000000\t0");

  Controller controller;
  QSignalSpy spy(&controller, &Controller::sendToExternalServer);

  controller.receiveResponseFromDevice(response.toUtf8());
  EXPECT_EQ(spy.count(), 1);

  QList<QVariant> arguments = spy.takeFirst();

  QJsonParseError parse_error;
  QJsonDocument json_doc = QJsonDocument::fromJson(arguments.at(0).toByteArray(), &parse_error);
  EXPECT_EQ(parse_error.error, QJsonParseError::ParseError::NoError);
  EXPECT_EQ(json_doc.object().contains("answer"), true);
  EXPECT_EQ(json_doc.object().value("answer"), "0");
}

TEST(ProcessingResponse_test, measure_test)
{
  const QString request("000000000\tCH3:1,2.0,12.0,24.0");

  Controller controller;
  QSignalSpy spy(&controller, &Controller::sendToExternalServer);

  controller.receiveMeasureFromDevice(request.toUtf8());
  EXPECT_EQ(spy.count(), 1);

  QList<QVariant> arguments = spy.takeFirst();

  QJsonParseError parse_error;
  QJsonDocument json_doc = QJsonDocument::fromJson(arguments.at(0).toByteArray(), &parse_error);
  EXPECT_EQ(parse_error.error, QJsonParseError::ParseError::NoError);

  EXPECT_EQ(json_doc.array().count(), 1);

  auto json_obj = json_doc.array().at(0).toObject();

  EXPECT_EQ(json_obj.contains("channel"), true);
  EXPECT_EQ(json_obj.value("channel").toInt(), 3);

  EXPECT_EQ(json_obj.contains("state"), true);
  EXPECT_EQ(json_obj.value("state").toInt(), 1);

  EXPECT_EQ(json_obj.contains("current"), true);
  EXPECT_EQ(json_obj.value("current").toDouble(), 2.0);

  EXPECT_EQ(json_obj.contains("voltage"), true);
  EXPECT_EQ(json_obj.value("voltage").toDouble(), 12.0);

  EXPECT_EQ(json_obj.contains("power"), true);
  EXPECT_EQ(json_obj.value("power").toDouble(), 24.0);
}
