#include "modbusrtucontext.h"
#include <QModbusRtuSerialClient>
#include <QModbusReply>
#include <algorithm>
#include "../logger.h"
#include "../devices/utils.h"

ModbusRtuContext::ModbusRtuContext(const cfg::Server &s, const cfg::RequestPara &p,
                                   QSharedPointer<QModbusRtuSerialClient> client) {
    os = s.getOs();
    serverTypeId = s.getServerTypeid();
    portName = s.getPortName();
    baudRate = s.getBaudRate();
    databits = s.getDataBits();
    stopbits = s.getStopBits();
    parity = s.getParity();
    flowControl = s.getFlowControl();
    requestParam = p;
    this->client = client;

    QModbusRequest request{(QModbusRequest::FunctionCode) Utils::checkHex(p.getFuncCode()),
                           (quint16) Utils::checkHex(p.getStartAddress()),
                           (quint16) Utils::checkHex(p.getCountOrData())};
    request.setFunctionCode((QModbusRequest::FunctionCode) Utils::checkHex(p.getFuncCode()));
    quint16 startAddress = Utils::checkHex(p.getStartAddress());
    if (startAddress < 0 || startAddress > 65535) {
        Logger::logger->warn(p.getDeviceName() + "modbusRtu startAddress溢出");
    }
    quint16 countOrData = Utils::checkHex(p.getCountOrData());
    if (countOrData < 0 || countOrData > 65535) {
        Logger::logger->warn(p.getDeviceName() + "modbusRtu countOrData溢出");
    }
    if (!request.isValid()) {
        Logger::logger->warn(p.getDeviceName() + "modbusRtu QModbusRequest isVaild()返回false");
    }
    this->request = request;
}

/**
 * @brief ModbusRtuContext::get
 * @param keyName 键值名
 * @return 返回std::any空,或者带有值
 * @note 使用前 使用has_value()判断值是否存在
 */
std::any ModbusRtuContext::get(const std::string &keyName) const {
    auto it=std::find(m_externValue.begin(), m_externValue.end(),keyName);
    if(it==m_externValue.end()) {
        Logger::logger->warn("the key of "+keyName+"don't exist");
        return {};
    }
    return it->value;
}

std::any ModbusRtuContext::get(int index) const {
    return m_externValue.at(index).value;
}

void ModbusRtuContext::set(const std::string &keyName, const std::any &value)
{
    auto it=std::find(m_externValue.begin(), m_externValue.end(),keyName);
    if(it == m_externValue.end()) {
        Logger::logger->warn("the key of "+keyName+"has exist,the value will cover the old value");
        Extension e;
        e.name = keyName;
        e.value = value;
        m_externValue.push_back(e);
        return;
    }
    it->value=value;
}



