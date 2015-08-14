#ifndef ASSEMBLYVUEYECAMERA_H
#define ASSEMBLYVUEYECAMERA_H

#include <QObject>

class AssemblyVUEyeCamera : public QObject
{
    Q_OBJECT
public:
    explicit AssemblyVUEyeCamera(QObject *parent);
    ~AssemblyVUEyeCamera();

    void setCameraID(unsigned int id) { cameraID_ = id; }
    void setDeviceID(unsigned int id) { deviceID_ = id; }
    void setSensorID(unsigned int id) { sensorID_ = id; }
    void setSerialNumber(unsigned long serNo) { serialNumber_ = serNo; }
    void setModelName(const QString& name) { modelName_ = name; }
    void setFullModelName(const QString& name) { fullModelName_ = name; }
    void setStatus(unsigned int status) { status_ = status; }

    void setID(const QString& id) { id_ = id; }
    void setVersion(const QString& version) { version_ = version; }
    void setDate(const QString& date) { date_ = date; }

    void setSensorName(const QString& name) { sensorName_ = name; }
    void setColorMode(unsigned int colorMode) { colorMode_ = colorMode; }
    void setMaxWidth(unsigned int maxWidth) { maxWidth_ = maxWidth; }
    void setMaxHeight(unsigned int maxHeight) { maxHeight_ = maxHeight; }
    void setMasterGain(bool masterGain) { masterGain_ = masterGain; }
    void setRedGain(bool gain) { redGain_ = gain; }
    void setGreenGain(bool gain) { greenGain_ = gain; }
    void setBlueGain(bool gain) { blueGain_ = gain; }
    void setGlobalShutter(bool globalShutter) { globalShutter_ = globalShutter; }
    void setPixelSize(unsigned int pixelSize) { pixelSize_ = pixelSize; }

    unsigned int getCameraID() const { return cameraID_; }
    unsigned int getDeviceID() const { return deviceID_; }
    unsigned int getSensorID() const { return sensorID_; }
    unsigned long getSerialNumber() const { return serialNumber_; }
    const QString& getModelName() const { return modelName_; }
    const QString& getFullModelName() const { return fullModelName_; }
    unsigned int getStatus() const { return status_; }

    const QString& getID() const { return id_; }
    const QString& getVersion() const { return version_; }
    const QString& getDate() const { return date_; }

    const QString& getSensorName() const { return sensorName_; }
    unsigned int getColorMode() const { return colorMode_; }
    unsigned int getMaxWidth() const { return maxWidth_; }
    unsigned int getMaxHeight() const { return maxHeight_; }
    bool getMasterGain() const { return masterGain_; }
    bool getRedGain() const { return redGain_; }
    bool getGreenGain() const { return greenGain_; }
    bool getBlueGain() const { return blueGain_; }
    bool getGlobalShutter() const { return globalShutter_; }
    unsigned int getPixelSize() const { return pixelSize_; }

    virtual void updateInformation() = 0;
    virtual bool isAvailable() const = 0;

    virtual bool initialize() = 0;
    virtual bool exit() = 0;

public slots:

    virtual void aquireImage() = 0;

protected slots:

protected:

    unsigned int cameraID_;
    unsigned int deviceID_;
    unsigned int sensorID_;
    unsigned long serialNumber_;
    QString modelName_;

    QString fullModelName_;
    unsigned int status_;

    QString id_;
    QString version_;
    QString date_;

    QString sensorName_;
    unsigned int colorMode_;
    unsigned int maxWidth_;
    unsigned int maxHeight_;
    bool masterGain_;
    bool redGain_;
    bool greenGain_;
    bool blueGain_;
    bool globalShutter_;
    unsigned int pixelSize_;

signals:

private:

};

#endif // ASSEMBLYVUEYECAMERA_H