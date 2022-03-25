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

#include <iostream>
#include <cmath>

#include <QApplication>
#include <QDateTime>
#include <QXmlStreamWriter>

#include <nqlogger.h>
#include <MattermostBot.h>

#include <ApplicationConfig.h>

#include "Thermo2TwoSThermalTestStructureModel.h"

Thermo2TwoSThermalTestStructureModel::Thermo2TwoSThermalTestStructureModel(KeithleyDAQ6510Model* keithleyModel,
		QObject * /* parent */)
 : QObject(),
   keithleyModel_(keithleyModel)
{
  ApplicationConfig* config = ApplicationConfig::instance();

  mattermostStatus_ = config->getValue<int>("2STTSMattermostStatus");
  mattermostInterval_ = config->getValue<int>("2STTSMattermostInterval");

  /*
  mattermostStatusTime_ = QDateTime::currentDateTime();
  QTime time = mattermostStatusTime_.time();
  time = time.addMSecs(-time.msec());
  time = time.addSecs(-time.second());
  time = time.addSecs(mattermostInterval_*60);
  mattermostStatusTime_.setTime(time);

  resistance_ = config->getValue<double>("ThroughPlaneResistance");
  cableResistance_ = config->getValue<double>("ThroughPlaneCableResistance");
  kBlock_ = config->getValue<double>("ThroughPlaneKBlock");
  ABlock_ = config->getValue<double>("ThroughPlaneABlock");
  nge103BChannel_ = config->getValue<unsigned int>("ThroughPlaneNGE103BChannel");
  keithleyTopSensors_ = config->getValueArray<unsigned int,6>("ThroughPlaneKeithleyTopSensors");
  keithleyTopPositions_ = config->getValueArray<double,6>("ThroughPlaneKeithleyTopPositions");
  keithleyTopCor0_ = config->getValueArray<double,6>("ThroughPlaneKeithleyTopCor0");
  keithleyTopCor1_ = config->getValueArray<double,6>("ThroughPlaneKeithleyTopCor1");
  keithleyTopCor2_ = config->getValueArray<double,6>("ThroughPlaneKeithleyTopCor2");
  keithleyTopCor3_ = config->getValueArray<double,6>("ThroughPlaneKeithleyTopCor3");
  keithleyBottomSensors_ = config->getValueArray<unsigned int,6>("ThroughPlaneKeithleyBottomSensors");
  keithleyBottomPositions_ = config->getValueArray<double,6>("ThroughPlaneKeithleyBottomPositions");
  keithleyBottomCor0_ = config->getValueArray<double,6>("ThroughPlaneKeithleyBottomCor0");
  keithleyBottomCor1_ = config->getValueArray<double,6>("ThroughPlaneKeithleyBottomCor1");
  keithleyBottomCor2_ = config->getValueArray<double,6>("ThroughPlaneKeithleyBottomCor2");
  keithleyBottomCor3_ = config->getValueArray<double,6>("ThroughPlaneKeithleyBottomCor3");
  keithleyAmbientSensor_ = config->getValue<double>("KeithleyAmbientSensor", 0);

  std::string sensorType;

  sensorType = config->getValue("ThroughPlaneKeithleyTopSensorTypes");
  if (sensorType=="4WirePT100") {
    keithleyTopSensorTypes_ = VKeithleyDAQ6510::FourWireRTD_PT100;
  } else if (sensorType=="Therm10k") {
    keithleyTopSensorTypes_ = VKeithleyDAQ6510::Thermistor_10000;
  } else {
    keithleyTopSensorTypes_ = VKeithleyDAQ6510::FourWireRTD_PT100;
  }

  sensorType = config->getValue("ThroughPlaneKeithleyBottomSensorTypes");
  if (sensorType=="4WirePT100") {
    keithleyBottomSensorTypes_ = VKeithleyDAQ6510::FourWireRTD_PT100;
  } else if (sensorType=="Therm10k") {
    keithleyBottomSensorTypes_ = VKeithleyDAQ6510::Thermistor_10000;
  } else {
    keithleyBottomSensorTypes_ = VKeithleyDAQ6510::FourWireRTD_PT100;
  }

  if (keithleyAmbientSensor_!=0) {
    sensorType = config->getValue("KeithleyAmbientSensorType");
    if (sensorType=="4WirePT100") {
      keithleyAmbientSensorType_ = VKeithleyDAQ6510::FourWireRTD_PT100;
    }else if (sensorType=="Therm10k") {
      keithleyAmbientSensorType_ = VKeithleyDAQ6510::Thermistor_10000;
    } else {
      keithleyAmbientSensorType_ = VKeithleyDAQ6510::FourWireRTD_PT100;
    }
  }

  bool throughPlaneAutoConfig = config->getValue<int>("ThroughPlaneAutoConfig");
  if (throughPlaneAutoConfig) {
    for (unsigned int i=0;i<6;++i) {
      keithleyModel_->setSensorMode(keithleyTopSensors_[i], keithleyTopSensorTypes_);
      keithleyModel_->setSensorEnabled(keithleyTopSensors_[i], true);

      keithleyModel_->setSensorMode(keithleyBottomSensors_[i], keithleyBottomSensorTypes_);
      keithleyModel_->setSensorEnabled(keithleyBottomSensors_[i], true);
    }

    if (keithleyAmbientSensor_!=0) {
      keithleyModel_->setSensorMode(keithleyAmbientSensor_, keithleyAmbientSensorType_);
      keithleyModel_->setSensorEnabled(keithleyAmbientSensor_, true);
    }
  }

  keithleyState_ = false;
  for (unsigned int c=0;c<6;++c) {
    keithleyTopSensorStates_[c] = false;
    keithleyBottomSensorStates_[c] = false;
    keithleyTopTemperatures_[c] = 0;
    keithleyBottomTemperatures_[c] = 0;

    // minimum Keithley readout frequency is 1/10s
    // storage for at least 1800 s
    keithleyTopTemperatureHistory_[c] = HistoryFifo<double>(1800/10);
    keithleyBottomTemperatureHistory_[c] = HistoryFifo<double>(1800/10);
  }
  */

  connect(keithleyModel_, SIGNAL(informationChanged()),
          this, SLOT(keithleyInfoChanged()));

  mattermostTimer_ = new QTimer(this);
  connect(mattermostTimer_, SIGNAL(timeout()),
	  this, SLOT(sendMattermostStatus()));
  mattermostTimer_->start(60*1000);

  connect(config, SIGNAL(valueChanged()),
          this, SLOT(configurationChanged()));
}

void Thermo2TwoSThermalTestStructureModel::configurationChanged()
{
  QMutexLocker locker(&mutex_);

  ApplicationConfig* config = ApplicationConfig::instance();

  /*
  resistance_ = config->getValue<double>("ThroughPlaneResistance");
  cableResistance_ = config->getValue<double>("ThroughPlaneCableResistance");
  kBlock_ = config->getValue<double>("ThroughPlaneKBlock");
  ABlock_ = config->getValue<double>("ThroughPlaneABlock");
  keithleyTopPositions_ = config->getValueArray<double,6>("ThroughPlaneKeithleyTopPositions");
  keithleyTopCor0_ = config->getValueArray<double,6>("ThroughPlaneKeithleyTopCor0");
  keithleyTopCor1_ = config->getValueArray<double,6>("ThroughPlaneKeithleyTopCor1");
  keithleyTopCor2_ = config->getValueArray<double,6>("ThroughPlaneKeithleyTopCor2");
  keithleyTopCor3_ = config->getValueArray<double,6>("ThroughPlaneKeithleyTopCor3");
  keithleyBottomPositions_ = config->getValueArray<double,6>("ThroughPlaneKeithleyBottomPositions");
  keithleyBottomCor0_ = config->getValueArray<double,6>("ThroughPlaneKeithleyBottomCor0");
  keithleyBottomCor1_ = config->getValueArray<double,6>("ThroughPlaneKeithleyBottomCor1");
  keithleyBottomCor2_ = config->getValueArray<double,6>("ThroughPlaneKeithleyBottomCor2");
  keithleyBottomCor3_ = config->getValueArray<double,6>("ThroughPlaneKeithleyBottomCor3");
  */
}

void Thermo2TwoSThermalTestStructureModel::keithleyInfoChanged()
{
  NQLogDebug("Thermo2TwoSThermalTestStructureModel") << "keithleyInfoChanged()";

  QMutexLocker locker(&mutex_);

  bool changed = false;

  /*
  unsigned int countTop = 0;
  unsigned int countBottom = 0;
  double temp, tcor;
  changed |= updateIfChanged<bool>(keithleyState_, keithleyModel_->getDeviceState()==READY ? true : false);
  for (unsigned int i=0;i<6;++i) {
    changed |= updateIfChanged<bool>(keithleyTopSensorStates_[i],
        keithleyModel_->getSensorState(keithleyTopSensors_[i])==READY ? true : false);

    temp = keithleyModel_->getTemperature(keithleyTopSensors_[i]);
    tcor =  keithleyTopCor0_[i];
    tcor += keithleyTopCor1_[i] * temp;
    tcor += keithleyTopCor2_[i] * temp * temp;
    tcor += keithleyTopCor3_[i] * temp * temp * temp;
    changed |= updateIfChanged<double>(keithleyTopTemperatures_[i], tcor);
    if (keithleyTopSensorStates_[i]) countTop++;

    changed |= updateIfChanged<bool>(keithleyBottomSensorStates_[i],
        keithleyModel_->getSensorState(keithleyBottomSensors_[i])==READY ? true : false);

    temp = keithleyModel_->getTemperature(keithleyBottomSensors_[i]);
    tcor =  keithleyBottomCor0_[i];
    tcor += keithleyBottomCor1_[i] * temp;
    tcor += keithleyBottomCor2_[i] * temp * temp;
    tcor += keithleyBottomCor3_[i] * temp * temp * temp;
    changed |= updateIfChanged<double>(keithleyBottomTemperatures_[i], tcor);
    if (keithleyBottomSensorStates_[i]) countBottom++;
  }
  */

  if (changed) {
    emit informationChanged();
  }
}

void Thermo2TwoSThermalTestStructureModel::sendMattermostStatus()
{
  if (!mattermostStatus_) return;

  auto currentDateTime = QDateTime::currentDateTime();
  if (currentDateTime<mattermostStatusTime_) return;

  NQLogDebug("Thermo2TwoSThermalTestStructureModel") << "sendMattermostStatus()";

  QString msg;

  msg += "#### 2S Thermal Test Structure Status (";
  msg += mattermostStatusTime_.toString("dd.MM.yyyy hh:mm");
  msg += ")\n";

  MattermostBot bot;
  bot.postMessage(msg);

  mattermostStatusTime_ = mattermostStatusTime_.addSecs(mattermostInterval_*60);
}

void Thermo2TwoSThermalTestStructureModel::statusMessage(const QString & text)
{
  emit message(text);
}
