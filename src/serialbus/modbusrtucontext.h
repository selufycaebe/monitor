#ifndef MODBUSRTUCONTEXT_H
#define MODBUSRTUCONTEXT_H

#include "../config/config.h"
#include <QModbusRequest>
#include <QModbusRtuSerialClient>
#include <any>
#include <vector>

struct Extension {
    std::string name;
    std::any value;
    bool isNull = false;
    bool operator==(const std::string &name) const {
        return this->name == name;
    }
};

class ModbusRtuContext {
public:
    ModbusRtuContext(const cfg::Server &s, const cfg::RequestPara &p, std::shared_ptr<QModbusRtuSerialClient> client);

    int64_t os;
    int64_t serverTypeId;
    std::string portName;
    int64_t baudRate;
    int64_t databits;
    int64_t stopbits;
    int64_t parity;
    int64_t flowControl;
    bool isOnline = false;
    cfg::RequestPara requestParam; //需要获取原始数据的时候可以从当前变量中取
    std::shared_ptr<QModbusRtuSerialClient> client; //当前设置所对应的串口指针
    QModbusRequest request;//由requestParam计算
    //添加一些自定义的属性
    std::any get(const std::string &keyName) const;
    std::any get(int index) const;
    void set(const std::string &keyName, const std::any &value=0);

private:
    std::vector<Extension> m_externValue;
};

#endif // MODBUSRTUCONTEXT_H
