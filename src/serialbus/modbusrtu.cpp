#include "modbusrtu.h"

#include <iostream>
#include <memory>
#include <qvariant.h>
#include <stdexcept>
#include "modbusrtucontext.h"
#include "../devices/deviceinter.h"
#include "../logger.h"
#include "../services/uploadservice.h"
#include "../devices/utils.h"
#include "qmap.h"
#include "qobject.h"
#include "qobjectdefs.h"

ModbusRtuManager::ModbusRtuManager() {}

//v1
void ModbusRtuManager::init() {
    //读取本地配置文件
    cfg::Config config = cfg::readConfig();
//    if (!m_modbusRtus.empty()) {
//        m_modbusRtus.clear();
//    }

    for (const auto &s: config.getServer()) {
        try {
            //如果不是当前处理的类型就跳过
            if (s.getServerTypeid() != IOType::ModbusRtu) continue;
#ifdef Q_OS_WIN
            if (s.getOs() != Os::Window) continue;

            setModbusParams(s);
            startRequest(s);
#endif

#ifdef Q_OS_LINUX
            if(s.getOs()!=Os::Linux) continue;
            //FIXME: 支持linux系统
            //Logger::logger->error("功能还没有实现");
            setModbusParams(s);
            startRequest(s);
#endif
        }
        catch (std::runtime_error &e) {
            // 日志记录
            Logger::logger->error(s.getPortName() + ":" + e.what());
        }
    }
}

void ModbusRtuManager::addPreHandler(const std::string &name, HandlerPtr f) {
    if (f == nullptr) return;
    //if(m_preHandlers[name].contains(f)) return;
    m_preHandlers[name].append(f);
}

void ModbusRtuManager::addGlobalPreHandler(HandlerPtr f) {
    if (f == nullptr) return;
    //if(m_preHandlers[name].contains(f)) return;
    m_globalPreHandlers.append(f);
}

void ModbusRtuManager::addGlobalEndHandler(HandlerPtr f) {
    if (f == nullptr) return;
    //if(m_preHandlers[name].contains(f)) return;
    m_globalEndHandlers.append(f);
}

void ModbusRtuManager::addEndHandler(const std::string &name, HandlerPtr f) {
    if (f == nullptr) return;
    //if(m_endHandlers[name].contains(f)) return;
    m_endHandlers[name].append(f);
}

void ModbusRtuManager::addErrorHandler(ModbusRtuManager::HandlerPtr f) {
    if(f == nullptr) return;
    m_errorHandlers.append(f);
}

void ModbusRtuManager::startRequest(const cfg::Server &s) {
    auto timer = QSharedPointer<QTimer>::create();
    timer->setInterval(s.getIntervalMs());
    QObject::connect(timer.get(), &QTimer::timeout, timer.get(), [this,s]() {
        for (const auto &d: s.getRequestParas()) {
            //ModbusRtuContext context{s,d,m_modbusRtus.value(s.getPortName())};
            auto rtu = m_modbusRtus.value(s.getPortName()).modbusRtu;
            auto context = QSharedPointer<ModbusRtuContext>::create(s,d,rtu); //std::make_shared<ModbusRtuContext>(s, d, rtu);
            std::string className = d.getClassName();
            if (className.empty()) continue;
            //FIXME: 只有创建 没有delete,内存泄漏!
            // - 先global 在个人
            // ok
            int address = Utils::checkHex(d.getAddress()); //请求设备的地址1-255
            DeviceInterface *deviceInter;
            auto isExisted = m_modbusRtus.value(s.getPortName()).deviceInfo.value(address).device;
            if (!isExisted) {
                // 使用工厂模式呢?
                DeviceInterface *device = static_cast<DeviceInterface *>(QMetaType::fromName(
                        className).create()); // 反射返回需要的实例
                QSharedPointer<DeviceInterface> sptr;
                sptr.reset(device);
                m_modbusRtus[s.getPortName()].deviceInfo[address].device = sptr;
                int size = m_modbusRtus.value(s.getPortName()).deviceInfo.value(address).size;
                device->setSize(size);
                //获取地址对应的命名
                auto f = m_modbusRtus.value(s.getPortName()).deviceInfo.value(address).names;
                if(f.isEmpty()) continue;
                device->setNameOfAddress(f);
                deviceInter = device;
                // 中间件
                //FIXME 下面捕获的槽函数 可以有点问题
                // - 第一次请求的时候这个槽函数还没连接
                // - OK

                //中间件
                if (!m_preHandlers.value(context->requestParam.getDeviceName()).empty()) {
                    auto list = m_preHandlers.value(context->requestParam.getDeviceName());
                    QObject::connect(deviceInter, &DeviceInterface::startProcessingData,
                                     [list](QSharedPointer<ModbusRtuContext> context) {
                                         for (int var = 0; var < list.size(); ++var) {
                                             list.at(var)(context); //调用函数 void(*)(std::shared_ptr<ModbusRtuContext>);
                                         }
                                     });
                }

                if (!m_endHandlers.value(context->requestParam.getDeviceName()).empty()) {
                    auto list = m_endHandlers.value(context->requestParam.getDeviceName());
                    QObject::connect(deviceInter, &DeviceInterface::endProcessingData,
                                     [list](QSharedPointer<ModbusRtuContext> context) {
                                         for (int var = 0; var < list.size(); ++var) {
                                             list.at(var)(context); //调用函数 void(*)(std::shared_ptr<ModbusRtuContext>);
                                         }
                                     });
                }
                //适用于所有的设备数据处理开始阶段
                QObject::connect(deviceInter, &DeviceInterface::startProcessingData,
                                 [&list = m_globalPreHandlers](QSharedPointer<ModbusRtuContext> context) {
                                     //std::cout<<context.get()->requestParam.getStartAddress()<<std::endl;
                                     for (int var = 0; var < list.size(); ++var) {
                                         list.at(var)(context); //调用函数 void(*)(std::shared_ptr<ModbusRtuContext>);
                                     }
                                 });

                //适用于所有的设备数据处理结束阶段
                QObject::connect(deviceInter, &DeviceInterface::endProcessingData,
                                 [&list = m_globalEndHandlers](QSharedPointer<ModbusRtuContext> context) {
                                     for (int var = 0; var < list.size(); ++var) {
                                         list.at(var)(context); //调用函数 void(*)(std::shared_ptr<ModbusRtuContext>);
                                     }
                                 });
                //错误的处理
                QObject::connect(deviceInter, &DeviceInterface::handleError,
                                 [&list = m_errorHandlers](QSharedPointer<ModbusRtuContext> context) {
                                     for (int var = 0; var < list.size(); ++var) {
                                         list.at(var)(context); //调用函数 void(*)(std::shared_ptr<ModbusRtuContext>);
                                     }
                                 });

                //----中间件end

                QObject::connect(deviceInter, &DeviceInterface::upload, [](const ModbusModel &model) {
                    //FIXME 当前的只有一个上传的函数
                    // - 同一个品类上传的函数应该是没啥区别的
                    // - 如果还有其他的如can啊之类的应该也是它对应的上传函数
                    auto instance = UploadServiceInstance::getInstance();
                    instance->serialUploadNetwork(model);
                });
            } else {
                deviceInter = m_modbusRtus.value(s.getPortName()).deviceInfo.value(address).device.get();
            }
            //发送数据
            deviceInter->sendRequest(context);
        }
    }, Qt::QueuedConnection);
    timer->start();
    m_modbusRtus[s.getPortName()].m_timers = timer;
}

void ModbusRtuManager::setModbusParams(const cfg::Server &s) {
    if (s.getServerTypeid() != IOType::ModbusRtu) return;

    auto log = Logger::logger;
    QModbusRtuSerialClient *client = new QModbusRtuSerialClient();
    client->setConnectionParameter(QModbusDevice::SerialPortNameParameter, QString::fromStdString(s.getPortName()));
    QList<int> options = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200}; //具体对应关系查看 QSerialPort::BaudRate
    if (options.contains(s.getBaudRate())) {
        client->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QVariant::fromValue(s.getBaudRate()));
    } else {
        log->warn(s.getPortName()+"serialbus baudrate参数设置错误");
        throw std::runtime_error("serialbus baudrate参数设置错误");
    }
    options = {5, 6, 7, 8}; //具体对应关系查看 QSerialPort::DataBits
    if (options.contains(s.getDataBits())) {
        client->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QVariant::fromValue(s.getDataBits()));
    } else {
        log->warn(s.getPortName()+"serialbus databits参数设置错误");
        throw std::runtime_error("serialbus databits参数设置错误");
    }
    options = {1, 3, 2}; //具体对应关系查看 QSerialPort::StopBits 文档
    if (options.contains(s.getStopBits())) {
        client->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QVariant::fromValue(s.getStopBits()));
    } else {
        log->warn(s.getPortName()+"serialbus stopbits参数设置错误");
        throw std::runtime_error("serialbus stopbits参数设置错误");
    }
    options = {0, 2, 3, 4, 5}; //QSerialPort::Parity
    if (options.contains(s.getParity())) {
        client->setConnectionParameter(QModbusDevice::SerialParityParameter, QVariant::fromValue(s.getParity()));
    } else {
        log->warn(s.getPortName()+"serialbus parity参数设置错误");
        throw std::runtime_error("serialbus parity参数设置错误");
    }
    auto isOpen = client->connectDevice();
    if (!isOpen) {
        log->warn("{}串口打开失败",s.getPortName());
        throw std::runtime_error(s.getPortName() + "串口打开失败");
    }
    //1秒超时
    client->setTimeout(1000);
    client->setNumberOfRetries(3);
    //添加所有初始化的实例指针
    QSharedPointer<QModbusRtuSerialClient> c{client};
    //m_modbusRtus[s.getPortName()] = c;
    m_modbusRtus[s.getPortName()].modbusRtu = c;
}

void ModbusRtuManager::setSizeAndName(const std::string& portName,int address,int size,const QMap<int,std::string>& names){
    //在调用init函数之后,整个modbusRtus还缺少的变量就是size和地址对应的名称了
    //if(!m_modbusRtus.contains(portName)) return;
    m_modbusRtus[portName].deviceInfo[address].size =size;
    m_modbusRtus[portName].deviceInfo[address].names = names;
}

void ModbusRtuManager::broadcast() const {

}
