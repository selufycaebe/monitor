#ifndef EXAM1_H
#define EXAM1_H

#include "deviceinter.h"
#include "QMetaObject"
class Exam1 : public DeviceInterface
{
public:
    Q_INVOKABLE Exam1();
public slots:
    std::optional<ModbusModel> onReadyRead(QSharedPointer<ModbusRtuContext> context) override;
};

#endif // EXAM1_H
