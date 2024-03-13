#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include <QtSerialBus/QModbusRtuSerialClient>
#include "../config/config.h"
class ModbusRtuManager
{
public:
    ModbusRtuManager();
    Q_DISABLE_COPY(ModbusRtuManager);

    void init();
    void setModbusParams(const Cfg::Server &s);
private:
    QList<std::shared_ptr<QModbusRtuSerialClient>> m_modbusRtus;
};

#endif // MODBUSRTU_H
