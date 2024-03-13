#include "modbusrtu.h"

#include <stdexcept>
#include <QVariant>
#include <QSerialPort>

#include "../enums.h"
#include "modbusrtucontext.h"
#include "../devices/deviceinter.h"
#include "../logger.h"
#include "../services/uploadservice.h"

ModbusRtuManager::ModbusRtuManager()
{

}

void ModbusRtuManager::init() {
    //读取本地配置文件
    Cfg::Config config = Cfg::init();
    if(!m_modbusRtus.empty()) {
        m_modbusRtus.clear();
    }

    //v1
    if(config.getVersion()==1) {
        for(const auto&s:config.getServer()) {
            try{
                setModbusParams(s);
            }
            catch (std::runtime_error e) {
                //TODO 日志记录
                Logger::logger->error(e.what());
            }
        }
    }
}

void ModbusRtuManager::setModbusParams(const Cfg::Server& s)
{
    if(s.getServerTypeid() != IOType::ModbusRtu) return;
    auto log = Logger::logger;
    QModbusRtuSerialClient *client = new QModbusRtuSerialClient();
    client->setConnectionParameter(QModbusDevice::SerialPortNameParameter,QString::fromStdString(s.getPortName()));
    QList<int> options = {1200,2400,4800,9600,19200,38400,57600,115200}; //具体对应关系查看 QSerialPort::BaudRate
    if(options.contains(s.getBaudRate())) {
        client->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,s.getBaudRate());
    }else {
        log->warn(s.getPortName()+"serialbus baudrate参数设置错误");
        throw std::runtime_error("serialbus baudrate参数设置错误");
    }
    options = {5,6,7,8}; //具体对应关系查看 QSerialPort::DataBits
    if(options.contains(s.getDataBits())) {
        client->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,s.getDataBits());
    }else {
        log->warn(s.getPortName()+"serialbus databits参数设置错误");
        throw std::runtime_error("serialbus databits参数设置错误");
    }
    options = {1,3,2}; //具体对应关系查看 QSerialPort::StopBits 文档
    if(options.contains(s.getStopBits())) {
        client->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,s.getStopBits());
    }else {
        log->warn(s.getPortName()+"serialbus stopbits参数设置错误");
        throw std::runtime_error("serialbus stopbits参数设置错误");
    }
    options = {0,2,3,4,5}; //QSerialPort::Parity
    if(options.contains(s.getParity())) {
        client->setConnectionParameter(QModbusDevice::SerialParityParameter,s.getParity());
    }else {
        log->warn(s.getPortName()+"serialbus parity参数设置错误");
        throw std::runtime_error("serialbus parity参数设置错误");
    }
    auto isOpen = client->connectDevice();
    if(!isOpen) {
        //log->warn(s.getPortname()+"串口打开失败");
        throw std::runtime_error(s.getPortName()+"串口打开失败");
    }

    //添加所有初始化的实例指针
    std::shared_ptr<QModbusRtuSerialClient> c;
    c.reset(client);
    m_modbusRtus.push_back(c);
    for(const auto& d:s.getRequestParas()) {
        ModbusRtuContext context{s,d,m_modbusRtus.value(m_modbusRtus.size()-1)};
        std::string className = d.getClassName();
        if(className.empty()) continue;
        DeviceInterface *deviceInter = static_cast<DeviceInterface*>(QMetaType::fromName(className).create()); // 反射返回需要的实例
        deviceInter->sendRequest(context);
        QObject::connect(deviceInter,&DeviceInterface::upload,[](const ModbusModel&model){
            qDebug()<<"uploading start";
            auto instance = UploadServiceInstance::getInstance();
            instance->uploadNetwork1(model);
        });
    }
}


