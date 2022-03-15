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

#include <chrono>
#include <thread>

#include <QDebug>

#include <nqlogger.h>

#include "Thermo2ScriptableGlobals.h"

#include "ScriptableLeyboldGraphixOne.h"
#include "ScriptableHuberUnistat525w.h"
#include "ScriptableMarta.h"
#include "ScriptableRohdeSchwarzNGE103B.h"
#include "ScriptableKeithleyDAQ6510.h"
#include "ScriptableThermo2ThroughPlane.h"

#include "Thermo2ScriptThread.h"

Thermo2ScriptThread::Thermo2ScriptThread(Thermo2ScriptModel* scriptModel,
					 LeyboldGraphixOneModel* leyboldModel,
					 HuberUnistat525wModel* huberModel,
					 MartaModel* martaModel,
					 RohdeSchwarzNGE103BModel* nge103BModel,
					 KeithleyDAQ6510Model* keithleyModel,
					 Thermo2ThroughPlaneModel* t2tpModel,
					 QObject *parent)
  : QThread(parent),
    scriptModel_(scriptModel),
    leyboldModel_(leyboldModel),
    huberModel_(huberModel),
    martaModel_(martaModel),
    nge103BModel_(nge103BModel),
    keithleyModel_(keithleyModel),
    t2tpModel_(t2tpModel)
{

}

void Thermo2ScriptThread::executeScript(const QString & script)
{
  script_ = script;

  engine_ = new QJSEngine();

  globalsObj_ = new Thermo2ScriptableGlobals(scriptModel_, this);
  QJSValue globalsValue = engine_->newQObject(globalsObj_);
  engine_->globalObject().setProperty("thermo", globalsValue);

  if (leyboldModel_) {
    leyboldObj_ = new ScriptableLeyboldGraphixOne(leyboldModel_, this);
    QJSValue leyboldValue = engine_->newQObject(leyboldObj_);
    engine_->globalObject().setProperty("leybold", leyboldValue);
  }

  if (huberModel_) {
    huberObj_ = new ScriptableHuberUnistat525w(huberModel_, this);
    QJSValue huberValue = engine_->newQObject(huberObj_);
    engine_->globalObject().setProperty("huber", huberValue);
  }

  if (martaModel_) {
    martaObj_ = new ScriptableMarta(martaModel_, this);
    QJSValue martaValue = engine_->newQObject(martaObj_);
    engine_->globalObject().setProperty("marta", martaValue);
  }

  ScriptableRohdeSchwarzNGE103B *nge103BObj = new ScriptableRohdeSchwarzNGE103B(nge103BModel_, this);
  QJSValue nge103BValue = engine_->newQObject(nge103BObj);
  engine_->globalObject().setProperty("nge103b", nge103BValue);

  keithleyObj_ = new ScriptableKeithleyDAQ6510(keithleyModel_, this);
  QJSValue keithleyValue = engine_->newQObject(keithleyObj_);
  engine_->globalObject().setProperty("keithley", keithleyValue);

  t2tpObj_ = new ScriptableThermo2ThroughPlane(t2tpModel_, this);
  QJSValue t2tpValue = engine_->newQObject(t2tpObj_);
  engine_->globalObject().setProperty("t2tp", t2tpValue);

  start();
}

void Thermo2ScriptThread::abortScript()
{
  NQLogMessage("Thermo2ScriptThread") << "abortScript()";
  if (engine_) {
    NQLogMessage("Thermo2ScriptThread") << "interrupted " << (int)engine_->isInterrupted();
    engine_->setInterrupted(true);
    
    globalsObj_->abort();
    if (leyboldModel_) leyboldObj_->abort();
    if (huberModel_) huberObj_->abort();
    if (martaModel_) martaObj_->abort();
    keithleyObj_->abort();
    t2tpObj_->abort();
  }
}

void Thermo2ScriptThread::run()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  QJSValue fun = engine_->evaluate(script_);
  delete engine_;
  engine_ = 0;
}
