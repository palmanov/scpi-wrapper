#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QSignalSpy>

using namespace testing;

#include "controller.h"

using namespace scpi;

TEST(ExternalAPI_test, power_on_test)
{
  const QString request("{\"command\":\"power_on\",\"parameter\":{\"channel\":1,\"current\":0.1,\"voltage\":12.0}}");

  Controller controller;
  QSignalSpy spy(&controller, &Controller::sendToDevice);

  controller.receiveExternalRequest(request.toUtf8());
  EXPECT_EQ(spy.count(), 1);

  QList<QVariant> arguments = spy.takeFirst();

  const auto expected_answer = QByteArray(":SOURce1:CURRent 0.1\n:SOURce1:VOLTage 12\n:SOURce1:STATe ON\n:OUTPut1:STATe?\n");
  EXPECT_EQ(arguments.at(0).toByteArray(), expected_answer);
}

TEST(ExternalAPI_test, power_off_test)
{
  const QString request("{\"command\":\"power_off\",\"parameter\":{\"channel\":1}}");

  Controller controller;
  QSignalSpy spy(&controller, &Controller::sendToDevice);

  controller.receiveExternalRequest(request.toUtf8());
  EXPECT_EQ(spy.count(), 1);

  QList<QVariant> arguments = spy.takeFirst();

  const auto expected_answer = QByteArray(":SOURce1:STATe OFF\n:OUTPut1:STATe?\n");
  EXPECT_EQ(arguments.at(0).toByteArray(), expected_answer);
}

TEST(ExternalAPI_test, state_test)
{
  const QString request("{\"command\":\"state\",\"parameter\":{\"channel\":1}}");

  Controller controller;
  QSignalSpy spy(&controller, &Controller::sendToDevice);

  controller.receiveExternalRequest(request.toUtf8());
  EXPECT_EQ(spy.count(), 1);

  QList<QVariant> arguments = spy.takeFirst();

  const auto expected_answer = QByteArray(":MEASure1:ALL?\n");
  EXPECT_EQ(arguments.at(0).toByteArray(), expected_answer);
}
