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

#ifndef THERMAL2TWOSTHERMALTESTSTRUCTUREMODEL_H
#define THERMAL2TWOSTHERMALTESTSTRUCTUREMODEL_H

#include <vector>
#include <array>

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include <HistoryFifo.h>

#include "KeithleyDAQ6510Model.h"

class Thermo2TwoSThermalTestStructureModel : public QObject
{
  Q_OBJECT
public:

  explicit Thermo2TwoSThermalTestStructureModel(KeithleyDAQ6510Model* keithleyModel,
      QObject *parent = 0);

  void setMattermostStatus(bool value) { mattermostStatus_ = value; }
  bool getMattermostStatus() const { return mattermostStatus_; }

  /*
  unsigned int getKeithleyTopSensor(unsigned int position) const { return keithleyTopSensors_[position]; }
  unsigned int getKeithleyBottomSensor(unsigned int position) const { return keithleyBottomSensors_[position]; }
  unsigned int getKeithleyAmbientSensor() const { return keithleyAmbientSensor_; }
  bool getKeithleyState() const { return keithleyState_; }
  bool getKeithleyTopSensorState(unsigned int position) const { return keithleyTopSensorStates_[position]; }
  bool getKeithleyBottomSensorState(unsigned int position) const { return keithleyBottomSensorStates_[position]; }
  bool getKeithleyAmbientSensorState() const { return keithleyAmbientSensorState_; }
  double getKeithleyTopTemperature(unsigned int position) const { return keithleyTopTemperatures_[position]; }
  const HistoryFifo<double>& getKeithleyTopTemperatureHistory(unsigned int position) const { return keithleyTopTemperatureHistory_[position]; }
  double getKeithleyBottomTemperature(unsigned int position) const { return keithleyBottomTemperatures_[position]; }
  const HistoryFifo<double>& getKeithleyBottomTemperatureHistory(unsigned int position) const { return keithleyBottomTemperatureHistory_[position]; }
  double getKeithleyAmbientTemperature() const { return keithleyAmbientTemperature_; }
  */

  void statusMessage(const QString & text);

public slots:


protected slots:

  void configurationChanged();

  void keithleyInfoChanged();
  void sendMattermostStatus();

protected:

  KeithleyDAQ6510Model* keithleyModel_;

  QMutex mutex_;

  QDateTime currentTime_;

  template <typename T> bool updateIfChanged(T &variable, T newValue) {
    if (variable==newValue) return false;
    variable = newValue;
    return true;
  }

  bool mattermostStatus_;
  int mattermostInterval_;
  QTimer *mattermostTimer_;
  QDateTime mattermostStatusTime_;

  /*
  std::array<unsigned int,6> keithleyTopSensors_;
  VKeithleyDAQ6510::ChannelMode_t keithleyTopSensorTypes_;
  std::array<double,6> keithleyTopPositions_;
  std::array<double,6> keithleyTopCor0_;
  std::array<double,6> keithleyTopCor1_;
  std::array<double,6> keithleyTopCor2_;
  std::array<double,6> keithleyTopCor3_;
  std::array<unsigned int,6> keithleyBottomSensors_;
  VKeithleyDAQ6510::ChannelMode_t keithleyBottomSensorTypes_;
  std::array<double,6> keithleyBottomPositions_;
  std::array<double,6> keithleyBottomCor0_;
  std::array<double,6> keithleyBottomCor1_;
  std::array<double,6> keithleyBottomCor2_;
  std::array<double,6> keithleyBottomCor3_;
  unsigned int keithleyAmbientSensor_;
  VKeithleyDAQ6510::ChannelMode_t keithleyAmbientSensorType_;
  bool keithleyState_;
  std::array<bool,6> keithleyTopSensorStates_;
  std::array<bool,6> keithleyBottomSensorStates_;
  bool keithleyAmbientSensorState_;
  std::array<double,6> keithleyTopTemperatures_;
  std::array<HistoryFifo<double>,6> keithleyTopTemperatureHistory_;
  std::array<double,6> keithleyBottomTemperatures_;
  std::array<HistoryFifo<double>,6> keithleyBottomTemperatureHistory_;
  double keithleyAmbientTemperature_;
  */

signals:

  void informationChanged();
  void message(const QString & text);
  void log(const QString & text);
};

#endif // THERMAL2TWOSTHERMALTESTSTRUCTUREMODEL_H
