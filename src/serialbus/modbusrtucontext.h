#ifndef MODBUSRTUCONTEXT_H
#define MODBUSRTUCONTEXT_H

#include "../config/config.h"
#include <QModbusRequest>
#include <QModbusRtuSerialClient>

struct ModbusRtuContext
{
public:
    ModbusRtuContext(const Cfg::Server& s,const  Cfg::RequestPara& p,std::shared_ptr<QModbusRtuSerialClient> client);

    int64_t os;
    int64_t serverTypeId;
    std::string portName;
    int64_t baudRate;
    int64_t databits;
    int64_t stopbits;
    int64_t parity;
    int64_t flowControl;
    Cfg::RequestPara requestParam; //需要获取原始数据的时候可以从当前变量中取
    std::shared_ptr<QModbusRtuSerialClient> client; //当前设置所对应的串口指针
    QModbusRequest request;//由requestParam计算
};

#endif // MODBUSRTUCONTEXT_H
