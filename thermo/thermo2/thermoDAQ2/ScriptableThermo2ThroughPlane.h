/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef SCRIPTABLETHERMO2THROUGHPLANE_H
#define SCRIPTABLETHERMO2THROUGHPLANE_H

#include <QObject>
#include <QMutex>
#include <QJSValue>

#include <Thermo2ThroughPlaneModel.h>

class ScriptableThermo2ThroughPlane : public QObject
{
  Q_OBJECT
public:

  explicit ScriptableThermo2ThroughPlane(Thermo2ThroughPlaneModel* model,
      QObject *parent = 0);

  void abort();

public slots:

  QJSValue getSinkTemperature();
  void setSinkTemperature(double temperature);

  QJSValue getSourcePower();
  void setSourcePower(double power);

  QJSValue getTopSensor(unsigned int position);
  QJSValue getBottomSensor(unsigned int position);

  QJSValue getTopTemperature(unsigned int position);
  QJSValue getBottomTemperature(unsigned int position);

  QJSValue getGradientTop();
  QJSValue getPowerTop();
  QJSValue getSampleTemperatureTop();
  QJSValue getSampleTemperatureMiddle();
  QJSValue getSampleTemperatureBottom();
  QJSValue getGradientBottom();
  QJSValue getPowerBottom();

  void waitForStableSampleTemperature(float FOMlimit, int delay, int timeout);

signals:

  void changeSinkTemperature(double);
  void changeSourcePower(double);

protected:

  Thermo2ThroughPlaneModel* model_;

  bool abortRequested_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLETHERMO2THROUGHPLANE_H
