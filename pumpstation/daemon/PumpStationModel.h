#ifndef PUMPSTATIONMODEL_H
#define PUMPSTATIONMODEL_H

#include <array>

#include <QTimer>

#include <ConradModel.h>
#include <LeyboldGraphixThreeModel.h>

class PumpStationModel : public QObject
{

  Q_OBJECT
public:

  explicit PumpStationModel(ConradModel* conradModel,
                            LeyboldGraphixThreeModel* leyboldModel,
                            double updateInterval = 5,
                            QObject *parent = 0);

  const State& getSwitchState(int channel) const;

  double getPressure(int channel) const;

public slots:

  void setSwitchEnabled(int, bool);

protected slots:

  void updateInformation();
  void updateConrad();

protected:

  ConradModel* conradModel_;
  LeyboldGraphixThreeModel* leyboldModel_;

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  std::array<State,5> switchState_;
  std::array<double,3> pressure_;

signals:

  void switchStateChanged(int,State);
  void pressureChanged(int,double);
};

#endif // PUMPSTATIONMODEL_H