#include "deviceinter.h"
#include "fmt/core.h"
#include "qmap.h"
#include "spdlog/common.h"
#include "utils.h"
#include "../logger.h"
#include "../serialbus/middleware.h"
#include <iostream>
#include <memory>

DeviceInterface::DeviceInterface(const std::string &name)
        : deviceName{name} {}

void DeviceInterface::sendRequest(std::shared_ptr<ModbusRtuContext> context) {

    auto logger = Logger::logger;
    //发送请求 处理回复
    auto client = context->client;
    int address = Utils::checkHex(context->requestParam.getAddress());
    auto *reply = client->sendRawRequest(context->request, address);

    std::string deviceName = context->requestParam.getDeviceName();
    if (reply == nullptr) {
        logger->error(deviceName + "reply指针为空");
        return;
    }
    QObject::connect(reply, &QModbusReply::errorOccurred, this, [context, deviceName, this](QModbusDevice::Error e) {
        //处理错误
        //加上下面的信号 可以解决在设备没有连接成功的情况下,error的报错内容是 xxxx [common] [error] xxx
        //否则就是正常的 [global] [error] xxx
        onError(context, e);
    }, Qt::SingleShotConnection);
    QObject::connect(reply, &QModbusReply::finished, this, [context, this]() {
        emit startProcessingData(context);
        auto md = onReadyRead(context);
        if (!md.has_value()) return;
        context->isOnline = true;
        emit endProcessingData(context);
        ModbusModel m = md.value();
        //setNameOfAddress(m);
        emit upload(m);
    }, Qt::SingleShotConnection);
}

void DeviceInterface::onError(std::shared_ptr<ModbusRtuContext> context, QModbusDevice::Error e) {
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    printCustom(spdlog::level::err, reply->errorString().toStdString(), context);
    emit handleError(context);
    reply->deleteLater();
}

void DeviceInterface::setSize(int size) {
    m_size = size;
}

void DeviceInterface::setNameOfAddress(const QMap<int, std::string> &names) {
    m_NameOfAddress = names;
}


void DeviceInterface::printCustom(spdlog::level::level_enum level,const std::string& information,std::shared_ptr<ModbusRtuContext> context) const {
    auto logger =  Logger::logger;
    std::string pattern = fmt::format("[%Y-%m-%d %H:%M:%S.%e] [{}] [modbus] [{}] [%^%l%$] %v",
                                      context->portName,context->requestParam.getDeviceName());
    logger->set_pattern(pattern);
    logger->log(level,information);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [global] [%^%l%$] %v");
}
