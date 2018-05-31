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

#ifndef ASSEMBLYPARAMETERSVIEW_H
#define ASSEMBLYPARAMETERSVIEW_H

#include <LStepExpressMotionManager.h>
#include <AssemblyMultiPickupTesterWidget.h>
#include <AssemblyPositionsRegistryWidget.h>

#include <QWidget>
#include <QPushButton>

class AssemblyParametersView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyParametersView(QWidget* parent=nullptr);
  virtual ~AssemblyParametersView() {}

 protected:

  QPushButton* paramIO_button_read_;
  QPushButton* paramIO_button_write_;

 public slots:

  void  read_parameters();
  void write_parameters();

 signals:

  void read_from_file_request(const QString&);
  void  write_to_file_request(const QString&);
};
// ===========================================================================

#endif // ASSEMBLYPARAMETERSVIEW_H
