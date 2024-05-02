#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include <QtSerialBus/QModbusRtuSerialClient>
#include <QTimer>
#include <functional>
#include <map>
#include <string>
#include "../config/config.h"
#include "qmap.h"
#include "qmodbusrtuserialclient.h"

class DeviceInterface;
class ModbusRtuContext;

class ModbusRtuManager
{
    using HandlerPtr = std::function<void(QSharedPointer<ModbusRtuContext>)>;
public:
    struct DeviceInfo {
        int size;
        QSharedPointer<DeviceInterface> device;
        QMap<int,std::string> names;
    };

    struct Common{
        QMap<int,DeviceInfo> deviceInfo; //这里的int 是具体设备的地址 1-255
        QSharedPointer<QTimer> m_timers;
        QSharedPointer<QModbusRtuSerialClient> modbusRtu;
    };
public:
    ModbusRtuManager();
    virtual  ~ModbusRtuManager() =default;
    Q_DISABLE_COPY(ModbusRtuManager);
    void init();

    void addPreHandler(const std::string&name, HandlerPtr f);
    void addEndHandler(const std::string&name, HandlerPtr f);
    void addGlobalPreHandler(HandlerPtr f);
    void addGlobalEndHandler(HandlerPtr f);
    void addErrorHandler(HandlerPtr f);
    void setSizeAndName(const std::string& portName,int address,int size,const QMap<int,std::string>& names);
    void broadcast() const;
private:
    void setModbusParams(const cfg::Server &s);
    void startRequest(const cfg::Server& s);
private:
    QList<HandlerPtr> m_globalPreHandlers;
    QList<HandlerPtr> m_globalEndHandlers;
    QList<HandlerPtr> m_errorHandlers;
    QMap<std::string,QList<HandlerPtr>> m_preHandlers;
    QMap<std::string,QList<HandlerPtr>> m_endHandlers;
    //一个端口对应 一个设备地址/设备类的 map, 一个设备地址需要包含多个寄存器地址的请求
    //QMap<std::string,QMap<int,DeviceInterface*>> m_devices;
    //QMap<std::string,std::shared_ptr<QModbusRtuSerialClient>> m_modbusRtus;
    //QMap<std::string,std::shared_ptr<QTimer>> m_timers;
    QMap<std::string,Common> m_modbusRtus;
};

#endif // MODBUSRTU_H
