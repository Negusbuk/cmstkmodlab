/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <QMutexLocker>

#include "ScriptableJulabo.h"

ScriptableJulabo::ScriptableJulabo(JulaboModel* julaboModel, QObject *parent) :
    QObject(parent),
    julaboModel_(julaboModel)
{
  connect(this, SIGNAL(changeP(double)),
          julaboModel_,SLOT(setProportionalValue(double)));
  connect(this, SIGNAL(changeTv(int)),
          julaboModel_,SLOT(setIntegralValue(int)));
  connect(this, SIGNAL(changeTd(int)),
          julaboModel_,SLOT(setDifferentialValue(int)));
  
  connect(this, SIGNAL(switchCirculator(bool)),
          julaboModel_, SLOT(setCirculatorEnabled(bool)));

  connect(this, SIGNAL(changeWorkingTemperature(double)),
          julaboModel_, SLOT(setWorkingTemperatureValue(double)));
}

void ScriptableJulabo::setPID( double p, int tv, int td ) {

  QMutexLocker locker(&mutex_);
  
  emit changeP(p);
  emit changeTv(tv);
  emit changeTd(td);
}

void ScriptableJulabo::enableCirculator() {

  QMutexLocker locker(&mutex_);
  emit switchCirculator( true );
}
void ScriptableJulabo::disableCirculator() {

  QMutexLocker locker(&mutex_);
  emit switchCirculator( false );
}

void ScriptableJulabo::setWorkingTemperature( double temperature ) {

  QMutexLocker locker(&mutex_);
  emit changeWorkingTemperature(temperature);
}

QJSValue ScriptableJulabo::circulator() {
  
  QMutexLocker locker(&mutex_);
  return QJSValue(julaboModel_->isCirculatorEnabled());
}

QJSValue ScriptableJulabo::workingTemperature() {
  
  QMutexLocker locker(&mutex_);
  return QJSValue(julaboModel_->getWorkingTemperatureParameter().getValue());
}

QJSValue ScriptableJulabo::bath() {
  
  QMutexLocker locker(&mutex_);
  return QJSValue(julaboModel_->getBathTemperature());
}

QJSValue ScriptableJulabo::safety() {
  
  QMutexLocker locker(&mutex_);
  return QJSValue(julaboModel_->getSafetySensorTemperature());
}
