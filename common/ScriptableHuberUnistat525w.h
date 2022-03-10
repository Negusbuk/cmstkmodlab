/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef _SCRIPTABLEHUBERUNISTAT525W_H_
#define _SCRIPTABLEHUBERUNISTAT525W_H_

#include <QObject>
#include <QMutex>
#include <QJSValue>

#include <VScriptableDevice.h>

#include <HuberUnistat525wModel.h>

class ScriptableHuberUnistat525w : public VScriptableDevice
{
  Q_OBJECT
public:
  explicit ScriptableHuberUnistat525w(HuberUnistat525wModel* model,
                                      QObject *parent = 0);

  void abort();

public slots:

  void setTemperatureSetPoint(double temperature);
  QJSValue getTemperatureSetPoint();

  void switchTemperatureControlOn();
  void switchTemperatureControlOff();
  QJSValue isTemperatureControlOn();

  void switchCirculatorOn();
  void switchCirculatorOff();
  QJSValue isCirculatorOn();

  QJSValue getInternalTemperature();
  QJSValue getProcessTemperature();
  QJSValue getReturnTemperature();

  QJSValue getPumpPressure();
  QJSValue getPower();

  QJSValue getCoolingWaterInletTemperature();
  QJSValue getCoolingWaterOutletTemperature();

  QJSValue getControlMode();
  void setControlMode(bool process);

  QJSValue getAutoPID();
  void setAutoPID(bool autoPID);

  QJSValue getKpInternal();
  void setKpInternal(int Kp);
  QJSValue getTnInternal();
  void setTnInternal(double Tn);
  QJSValue getTvInternal();
  void setTvInternal(double Tv);

  QJSValue getKpProcess();
  void setKpProcess(int Kp);
  QJSValue getTnProcess();
  void setTnProcess(double Tn);
  QJSValue getTvProcess();
  void setTvProcess(double Tv);

  void waitForInternalTemperatureAbove(float temperature,
				       int timeout);
  void waitForInternalTemperatureBelow(float temperature,
				       int timeout);
  void waitForStableInternalTemperature(float deltaT,
					int delay,
					int timeout);

  void waitForProcessTemperatureAbove(float temperature,
				      int timeout);
  void waitForProcessTemperatureBelow(float temperature,
				      int timeout);
  void waitForStableProcessTemperature(float deltaT,
				       int delay,
				       int timeout);

protected:

  HuberUnistat525wModel* model_;

  bool abortRequested_;

  // For thread safety
  QMutex mutex_;
};

#endif // _SCRIPTABLEHUBERUNISTAT525W_H_
