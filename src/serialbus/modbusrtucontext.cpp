#include "modbusrtucontext.h"
#include <QModbusRtuSerialClient>
#include <QModbusReply>
#include "../logger.h"
#include "../devices/utils.h"

ModbusRtuContext::ModbusRtuContext(const Cfg::Server &s, const Cfg::RequestPara &p, std::shared_ptr<QModbusRtuSerialClient> client)
{
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

    QModbusRequest request{(QModbusRequest::FunctionCode) Utils::checkHex(p.getFuncCode()),(quint16)Utils::checkHex(p.getStartAddress()),(quint16)Utils::checkHex(p.getCountOrData())};
    request.setFunctionCode((QModbusRequest::FunctionCode) Utils::checkHex(p.getFuncCode()));
    quint16 startAddress = Utils::checkHex(p.getStartAddress());
    if(startAddress<0 || startAddress > 65535) {
        Logger::logger->warn(p.getDeviceName()+"modbusRtu startAddress溢出");
    }
    quint16 countOrData = Utils::checkHex(p.getCountOrData());
    if(countOrData<0 || countOrData>65535) {
        Logger::logger->warn(p.getDeviceName() + "modbusRtu countOrData溢出");
    }
    if(!request.isValid()) {
        Logger::logger->warn(p.getDeviceName() + "modbusRtu QModbusRequest isVaild()返回false");
    }
    this->request = request;
}


