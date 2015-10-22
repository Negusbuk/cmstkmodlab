#ifndef __VLSTEPEXPRESS_H
#define __VLSTEPEXPRESS_H

#include <string>
#include <vector>

typedef const char* ioport_t;

class VLStepExpress {

 public:

  enum Axis {
    X = 0,
    Y = 1,
    Z = 2,
    A = 3
  };

  enum Dimension {
    MICROSTEPS = 0,
    MICROMETER = 1,
    MILLIMETER = 2,
    DEGREE     = 3,
    ROTATIONS  = 4
  };

  enum AxisStatus {
    AXISREADY               = 0,
    AXISMOVING              = 1,
    AXISJOYSTICK            = 2,
    AXISINCONTROL           = 3,
    AXISLIMITSWITCHTRIPPED  = 4,
    AXISDISABLED            = 5,
    AXISSTATEUNKNOWN        = 99
  };

  VLStepExpress( const ioport_t );
  virtual ~VLStepExpress();

 public:

  virtual bool DeviceAvailable() const = 0;

  virtual void GetAutoStatus(int & value) = 0;
  virtual void SetAutoStatus(int value) = 0;

  virtual void GetAxisStatus(std::vector<int> & values) = 0;

  virtual void GetAxisEnabled(std::vector<int> & values) = 0;
  virtual void GetAxisEnabled(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetAxisEnabled(std::vector<int> & values) = 0;
  virtual void SetAxisEnabled(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetAxisDirection(std::vector<int> & values) = 0;
  virtual void GetAxisDirection(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetAxisDirection(std::vector<int> & values) = 0;
  virtual void SetAxisDirection(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetDimension(std::vector<int> & values) = 0;
  virtual void GetDimension(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetDimension(std::vector<int> & values) = 0;
  virtual void SetDimension(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetPowerAmplifierStatus(std::vector<int> & values) = 0;
  virtual void GetPowerAmplifierStatus(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetPowerAmplifierStatus(std::vector<int> & values) = 0;
  virtual void SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetPosition(std::vector<double> & values) = 0;
  virtual void GetPosition(VLStepExpress::Axis axis, double & value) = 0;
  virtual void SetPosition(std::vector<double> & values) = 0;
  virtual void SetPosition(VLStepExpress::Axis axis, double value) = 0;

  virtual void MoveAbsolute(std::vector<double> & values) = 0;
  virtual void MoveAbsolute(double x, double y, double z, double a) = 0;
  virtual void MoveAbsolute(VLStepExpress::Axis axis, double value) = 0;

  virtual void MoveRelative(std::vector<double> & values) = 0;
  virtual void MoveRelative(double x, double y, double z, double a) = 0;
  virtual void MoveRelative(VLStepExpress::Axis axis, double value) = 0;
  virtual void MoveRelative() = 0;

  virtual void Reset() = 0;
  virtual void ConfirmErrorRectification() = 0;
  virtual void ValidConfig() = 0;
  virtual void ValidParameter() = 0;

  // low level methods
  virtual void SendCommand(const std::string &) = 0;
  virtual void ReceiveString(std::string &) = 0;

 protected:

  char GetAxisName(VLStepExpress::Axis axis);
};

#endif
