#ifndef DEVICEINTER_H
#define DEVICEINTER_H

#include <string>
#include <QModbusDataUnit>
#include <QMap>

#include "../services/modbusmodel.h"
#include "../serialbus/modbusrtucontext.h"
class DeviceInterface : public QObject
{
    Q_OBJECT
public:
    DeviceInterface(const std::string& name): deviceName{name} {};
    virtual void sendRequest(const ModbusRtuContext& unit)=0;
    std::string deviceName="";
signals:
    void upload(const ModbusModel &model);
};



#endif // DEVICEINTER_H
