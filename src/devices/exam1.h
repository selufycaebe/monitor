#ifndef EXAM1_H
#define EXAM1_H

#include "deviceinter.h"
class Exam1 : public DeviceInterface
{
    Q_OBJECT
public:
    Exam1();
    // DeviceInterface interface
public:
    void sendRequest(const ModbusRtuContext &unit) override;

public slots:
    void onReadyRead(const ModbusRtuContext &c);
    void onError(QModbusDevice::Error e);
};

#endif // EXAM1_H
